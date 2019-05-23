cmake_minimum_required (VERSION 3.5)
project (project_laboratory)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

if(MSVC) # Check if we are using the Visual Studio compiler
    add_definitions(-DVISUAL_S)
  else()
    
endif()

#AntTweakBar doesn't compile on windows with shared lib
add_definitions(-DTW_STATIC)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Whether to build a shared library" FORCE)
set(ATB_GUIRENDERER_USE_D3D11 OFF CACHE BOOL "Enable Direct3D 11 rendering code" FORCE)
set(ATB_GUIRENDERER_USE_D3D10 OFF CACHE BOOL "Enable Direct3D 10 rendering code" FORCE)
set(ATB_GUIRENDERER_USE_D3D9 OFF CACHE BOOL "Enable Direct3D 9 rendering code" FORCE)
set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

# GLFW options
#
option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)

add_subdirectory (dependencies/AntTweakBar)
add_subdirectory (dependencies/glfw)
add_subdirectory (dependencies/partioPRTForWindows)


set (EXTRA_LIBS ${EXTRA_LIBS} AntTweakBar partioPRTForWindows glfw)

add_executable(project_laboratory
    src/Array3d.cpp
    src/Camera.cpp
    src/FrameLoader.cpp
    src/LoadBeforeFrameLoader.cpp 
    src/LoadRuntimeFrameLoader.cpp
    src/functions.cpp
    src/glad.cpp
    src/main.cpp
    src/Particle.cpp
    src/ParticleSystem.cpp
    src/Sphere.cpp
    src/LightSource.cpp
    src/ViewOpenGL.cpp
    src/Model.cpp
    src/ParticleArrayLoader.cpp
    src/ShaderLoader.cpp
    src/ProjLabShaderProgramProvider.cpp
    include/Array3d.h
    include/Camera.h
    include/FrameLoader.h
    include/functions.h
    include/header.h
    include/LightSource.h
    include/LoadBeforeFrameLoader.h
    include/LoadRuntimeFrameLoader.h
    include/Model.h
    include/MyAntTweakBar.h
    include/Particle.h
    include/ParticleArrayLoader.h
    include/ParticleSystem.h
    include/Sphere.h
    include/ViewOpenGL.h
    include/ShaderLoader.h
    include/IShaderProgramProvider.h
    include/ProjLabShaderProgramProvider.h
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_include_directories(${PROJECT_NAME} PRIVATE dependencies/glm/glm)
target_include_directories(${PROJECT_NAME} PRIVATE dependencies/glfw/include/GLFW)
target_include_directories(${PROJECT_NAME} PRIVATE dependencies/AntTweakBar/include)
target_include_directories(${PROJECT_NAME} PRIVATE dependencies/partioPRTForWindows/include)

target_link_libraries(project_laboratory ${EXTRA_LIBS})

add_custom_command(TARGET project_laboratory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/simulation
        $<TARGET_FILE_DIR:project_laboratory>/simulation)
		
add_custom_command(TARGET project_laboratory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:project_laboratory>/shaders)
		
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/simulation DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )	
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
		
		
		
		
		
		

