#ifndef GET_FILENAMES_H
#define GET_FILENAMES_H

//#include <iostream>
#include <string>
#include <cstring>
#include <vector>
//ע�����ͷ�ļ���������linux���м����Դ���
//#include <io.h>
//#include <fstream>

using namespace std;

void getAllFiles(string path, vector<string>& files);

int getfileNumfromPath(string path);

void getfileNamesfromPath(const char* filepath, string* allfilename);

#endif
