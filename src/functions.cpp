#include "functions.h"

float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

std::vector<std::string> getPrtFiles(std::string directoryName)
{
    std::vector<std::string> list;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(directoryName.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL)
      {
          std::string fn = ent->d_name;
          if(fn.substr(fn.find_last_of(".") + 1) == "prt")
          {
              list.push_back(directoryName + '/' + fn);
          }
      }
      closedir (dir);
    }
    else
    {
      printf("Could not open the directory.\n");
    }
    std::sort(list.begin(), list.end());
    return list;
}
