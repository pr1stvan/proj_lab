#include "FrameLoader.h"

std::string FrameLoader::getFileNameWithoutNumberFromFolderPath(char *folderPath)
{
	//c stilusban Megkeresi a mappa nevet es beleteszi a filename stringbe, mivel a mappa neve lesz a fajl neve is
	int j;
	for (j = 0; folderPath[j] != 0; j++);
	bool hiba = false;
	while (folderPath[j] != '/' && !hiba)
	{
		j--;
		if (j < 0)
                        return std::string("no file");//Ha gond van, nem lesz betoltes
	}
	j++;
	char * fileName = folderPath + j;

	return std::string(fileName);
}

std::string FrameLoader::createFilePath(char *folderPath, std::string fileName, int fileNumber)
{
	char filePath[100];
	sprintf(filePath, "%s/%s_%04d.prt", folderPath, fileName.c_str(), fileNumber);

	return std::string(filePath);
}

int FrameLoader::getParticleCountPerFrame(int index)
{
	return particleCountPerFrame;
}
char* FrameLoader::getFileFolder()
{
	return file;
}
FrameLoader::~FrameLoader()
{

}





