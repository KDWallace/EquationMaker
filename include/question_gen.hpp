#ifndef GENERATOR_HPP
//if statement for definition to ensure that this headers libraries and methods/functions are only used once.
//if previously defined, the contents will be ignored
#define GENERATOR_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <sstream>
#ifdef WIN32
#include <direct.h>
#define getcwd _getcwd
#define mkdir _mkdir

#else
#include <unistd.h>
#endif

//File handler
void readConfig(int*,int*,int*,std::string);
std::string getCurrentDir();
void writeConfig(std::string, int, int, int);
bool testFile(std::string,std::string,bool);

#endif

//NOTE:
//If for windows, compile with -DWIN32 flag
