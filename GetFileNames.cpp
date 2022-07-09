#include <iostream>
#include <string>
#include <cstring>
#include <vector>
//ע�����ͷ�ļ���������linux���м����Դ���
#include <io.h>
#include <fstream>
#include <queue>
using namespace std;

void getAllFiles(string path, vector<string>& files)
{
	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {  //�Ƚ��ļ������Ƿ����ļ���
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
					//�ݹ�����
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);
					//filesNum++;
					//printf("\nfileNum: %d\n", filesNum);
				}
			}
			else {
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}

}

int getfileNumfromPath(string path)
{
	int fileNum = 0;

	std::vector<std::string> pathVec;
	std::queue<std::string> q;
	q.push(path);

	while (!q.empty())
	{
		std::string item = q.front(); q.pop();

		std::string path = item + "\\*";
		struct _finddata_t fileinfo;
		auto handle = _findfirst(path.c_str(), &fileinfo);
		if (handle == -1) continue;

		while (!_findnext(handle, &fileinfo))
		{
			if (fileinfo.attrib & _A_SUBDIR)
			{
				if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)continue;
				q.push(item + "\\" + fileinfo.name);
			}
			else
			{
				fileNum++;
				pathVec.push_back(item + "\\" + fileinfo.name);
			}
		}
		_findclose(handle);
	}

	return fileNum;
}


void getfileNamesfromPath(const char* path, string* allfilename)
{
	//��ȡȫ·��
	vector<string> fileNames;
	//string path(".\\layer_para"); 	//�Լ�ѡ��Ŀ¼����
	int filesNum = 0;
	getAllFiles(path, fileNames);
	//printf("fileNum: %d\n", fileNames.size());
	//string filep[11];
	int i = 0;
	for (const auto& ph : fileNames) {
		allfilename[i] = ph;
		i++;
		// std::cout << ph << "\n";
	}
	//for (i = 0; i < fileNames.size(); i++)
	//{
	//	std::cout << "filep: " << filep[i] << "\n";
	//}

	/**ֻ��ȡ�ļ���
	//struct _finddata_t fileinfo;
	//intptr_t hFile;
	//vector<string> fileNames;
	//// const char* localstorage = ".\\layer_para\\file.txt";
	//// ofstream file_out(localstorage);

	//// Find first .lib file in input directory
	//if ((hFile = _findfirst(".\\layer_para\\*.csv", &fileinfo)) == -1L)
	//	std::cout << "no files";
	//else
	//{
	//	do
	//	{
	//		fileNames.push_back(fileinfo.name); //cache file Name List
	//	} while (_findnext(hFile, &fileinfo) == 0);
	//	_findclose(hFile);
	//}
	////int k = fileNames.size();
	////string files[fileNames.size()];
	////Save fileNames to local
	//for (size_t i = 0; i < fileNames.size(); i++)
	//{
	//	allfilename[i] = fileNames[i].c_str();
	//	//cout << files[i] << endl;
	//}
	//// file_out.close();
	*/
}