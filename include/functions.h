#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>

float rand_FloatRange(float a, float b);

std::vector<std::string> getPrtFiles(std::string directoryName);
