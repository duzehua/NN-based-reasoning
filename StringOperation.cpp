#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "StringOperation.h"

/**
* �����ַ�������
* ˵��: �����ַ������ȣ����յ�ַ���ȼ���.
* @param[in]  Str          -> ��������ַ���
* @param[out] �ַ������ȣ�int
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
*/
int CalStrLen(const char* str)
{
	int count = 0;
	while (*str != '\0')
	{
		count++;
		str++;
	}
	return count;
}


// ·��ƴ��
char* OSPathJoin(const char* pre_path, char* after_path)
{
	int len_add = 0;
	int len_pre_path = CalStrLen(pre_path);
	// printf("%d\n", len_pre_path);
	int len_after_path = CalStrLen(after_path);
	// printf("%d\n", len_after_path);
	// printf("%d\n", len_pre_path);
	// printf("%c\n", pre_path[len_pre_path - 1]);

	// �ж����һλ�Ƿ����/��û�������
	if (pre_path[len_pre_path - 1] != '/')
	{
		len_add = 1;
	}
	int len_combined_path = len_pre_path + len_after_path + len_add;
	// printf("%d\n", len_combined_path);
	char* combined_path = (char*)malloc(len_combined_path);
	// combined_path = (char*)malloc((len_pre_path + len_after_path + len_add) * sizeof(char));

	int idx_char;
	for (idx_char = 0; idx_char < len_pre_path; idx_char++)
	{
		combined_path[idx_char] = pre_path[idx_char];
	}
	// strcpy(combined_path, pre_path);

	// printf("%d\n", idx_char);
	if (len_add)
	{
		combined_path[idx_char] = '/';
	}

	for (idx_char = 0; idx_char < len_after_path; idx_char++)
	{
		combined_path[idx_char + len_pre_path + len_add] = after_path[idx_char];
	}

	combined_path[idx_char + len_pre_path + len_add] = '\0';

	// printf("%s\n", combined_path);
	// len_combined_path = CalStrLen(combined_path);
	// printf("%d\n", len_combined_path);

	return combined_path;
}