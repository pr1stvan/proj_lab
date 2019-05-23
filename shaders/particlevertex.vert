#version 450 core

uniform float r;
uniform mat4 VP;			// View-Projection matrix in row-major format
uniform mat4 V;

uniform float Smoothness;
uniform float AmbientPower;

uniform vec3 Light1Position_worldspace;
uniform vec3 Light1Color;
uniform float Light1Power;
uniform float Light1SpecularPower;

uniform vec3 Light2Position_worldspace;
uniform vec3 Light2Color;
uniform float Light2Power;
uniform float Light2SpecularPower;
	
layout(location = 0) in vec3 vertexPosition_relativeToCenter;
layout(location = 1) in vec3 centerPosition_worldSpace;
layout(location = 3) in vec3 vertexNormal_worldSpace;
layout(location = 2) in vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 fragment_color;
	
void main() {
	vec3 vertexPosition_worldspace = r*vertexPosition_relativeToCenter + centerPosition_worldSpace;
	vec3 vertexPosition_cameraspace = ( V * vec4(vertexPosition_worldspace,1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 Light1Position_cameraspace = ( V * vec4(Light1Position_worldspace,1)).xyz;
	vec3 Light2Position_cameraspace = ( V * vec4(Light2Position_worldspace,1)).xyz;
		
	vec3 Normal_cameraspace = normalize(( V * vec4(vertexNormal_worldSpace,0)).xyz);
	// Material properties
	vec3 MaterialDiffuseColor = vertexColor;
	vec3 MaterialAmbientColor = vec3(AmbientPower,AmbientPower,AmbientPower) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(1.0,1.0,1.0);		
		
	// Distance to the light
	float light1distance = length( Light1Position_worldspace - vertexPosition_worldspace );
	float light2distance = length( Light2Position_worldspace - vertexPosition_worldspace );		

	//Normal in cameraspace, V do not translate, because h is 0
	vec3 n1 = Normal_cameraspace;
	// Direction of the light (from the vertex to the light)
	vec3 l1 = normalize( Light1Position_cameraspace + EyeDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta1 = clamp( dot( n1,l1 ), 0,1 );
		
	// Eye vector (towards the camera)
	vec3 E1 = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R1 = reflect(-l1,n1);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha1 = clamp( dot( E1,R1 ), 0,1 );		
	
	//Normal in cameraspace, V do not translate, because h is 0
	vec3 n2 = Normal_cameraspace;
	// Direction of the light (from the vertex to the light)
	vec3 l2 = normalize( Light2Position_cameraspace + EyeDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta2 = clamp( dot( n2,l2 ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E2 = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R2 = reflect(-l2,n2);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha2 = clamp( dot( E2,R2 ), 0,1 );		

	gl_Position = VP * vec4(vertexPosition_worldspace, 1); 		// transform to clipping space
			
	fragment_color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * Light1Color * Light1Power * cosTheta1 / (light1distance*light1distance) +
		MaterialDiffuseColor * Light2Color * Light2Power * cosTheta2 / (light2distance*light2distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * Light1Color * (Light1Power * Light1SpecularPower) * pow(cosAlpha1,Smoothness) / (light1distance*light1distance)+
		MaterialSpecularColor * Light2Color * (Light2Power * Light2SpecularPower) * pow(cosAlpha2,Smoothness) / (light2distance*light2distance);

}