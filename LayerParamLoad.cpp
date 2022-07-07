#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GINFO.h"
#include <malloc.h>
#include <time.h>
#include "Tensor.h"
#include "GINFO_VAR.h"
#include "StringOperation.h"
#include "GeneralDataType.h"
#include "DataDefinition.h"


/**
* 查找字符串中特定字符的索引
* 说明: 查找字符串中特定字符特定次数出现的索引.
* @param[in]  Str          -> 待提取数字的字符串
* @param[in]  idx_start    -> 起始索引(取到)
* @param[in]  idx_end      -> 终止索引(取到)
* @param[out] 返回提取的数字，int
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
*/
int ExtractNumFromChar(const char* Str, int idx_start, int idx_end)
{
	int Str_len = CalStrLen(Str);
	char* num_char = (char*)malloc(Str_len * sizeof(char));
	int len = idx_end - idx_start + 1;
	// printf("len = %d\n", len);
	memmove(num_char, Str + idx_start, len);
	*(num_char + len) = '\0'; //添加字符串，字符串结束符
	int num_int = atoi(num_char);
	free(num_char);
	return num_int;
}


/**
* 查找字符串中特定字符的索引
* 说明: 查找字符串中特定字符特定次数出现的索引.
* @param[in]  Str          -> 被查找的字符串
* @param[in]  Char         -> 待查找的字符
* @param[in]  idx_start    -> 起始索引
* @param[in]  idx_num      -> 第n次出现的索引
* @param[in]  descend_flag -> 是否倒序查找的标志位
* @param[out] 返回指定索引
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
* Warning: 未添加异常判断和处理！！！
*/
int FindCharIdxInStr(const char* Str, char Char, int idx_start, int idx_num, int descend_flag)
{
	const char* curt_num_addr;
	int   idx_curt_num;
	curt_num_addr = &Str[idx_start];
	// 计算第idx_num个符号出现的位置
	for (int i = 0; i < idx_num; i++)
	{
		if (descend_flag == 1)
		{
			curt_num_addr = strrchr(curt_num_addr, Char);
		}
		else
		{
			curt_num_addr = strchr(curt_num_addr, Char);
		}
		curt_num_addr++;
	}
	curt_num_addr--;
	//printf("%s\n", curt_num_addr);
	idx_curt_num = curt_num_addr - Str;
	//printf("%d -> %c\n", idx_curt_num, Str[idx_curt_num]); 
	return idx_curt_num;
}


/**
* 从文件名称中提取张量的尺寸
* 说明: 从文件名称中提取张量尺寸，文件名称格式为：epoch_step_index_张量个数_通道数_长_宽_层序号.张量类型.csv.
* @param[in]  f_path_name  -> 待提取张量形状信息的文件路径名称
* @param[in]  shape_tensor -> 提取的形状, 个数\通道数\长\宽
* @param[out] null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
*/
void ExtractShapeFromCharForTensor(const char* f_path_name, int* shape_tensor)
{
	// 拆分文件名
	int char_idx0, char_idx3, char_idx4, char_idx5, char_idx6, char_idx7;
	// int tsnum, chnum, row, col;
	char_idx0 = FindCharIdxInStr(f_path_name, '/', 0, 1, 1);
	char_idx3 = FindCharIdxInStr(f_path_name, '_', char_idx0, 3, 0);
	char_idx4 = FindCharIdxInStr(f_path_name, '_', char_idx0, 4, 0);
	char_idx5 = FindCharIdxInStr(f_path_name, '_', char_idx0, 5, 0);
	char_idx6 = FindCharIdxInStr(f_path_name, '_', char_idx0, 6, 0);
	char_idx7 = FindCharIdxInStr(f_path_name, '_', char_idx0, 7, 0);

	// 3-4 4-5 5-6 6-7
	shape_tensor[0] = ExtractNumFromChar(f_path_name, char_idx3 + 1, char_idx4 - 1);
	shape_tensor[1] = ExtractNumFromChar(f_path_name, char_idx4 + 1, char_idx5 - 1);
	shape_tensor[2] = ExtractNumFromChar(f_path_name, char_idx5 + 1, char_idx6 - 1);
	shape_tensor[3] = ExtractNumFromChar(f_path_name, char_idx6 + 1, char_idx7 - 1);
	//printf("%d, %d, %d, %d\n", tsnum, chnum, row, col);
}


// 输入文件路径, 获取对应的张量
void LayerParamLoadFromCSV(const char* f_path_name, Tensor* param_tensor)
{

	// 通过文件名，提取张量的形状
	int shape_tensor[4] = { 0,0,0,0 };
	ExtractShapeFromCharForTensor(f_path_name, shape_tensor);
	//printf("%d, %d, %d, %d\n", shape_tensor[0], shape_tensor[1], shape_tensor[2], shape_tensor[3]);

	// 提取文件中的数据
	FILE* fp = fopen(f_path_name, "r");

	if (fp == NULL) {
		perror("fopen");
		return;
	}

	size_t sz = 0;

	for (int c = getc(fp); c != EOF; c = getc(fp)) {
		if (c == ' ') sz++;
	}
	sz++;
	rewind(fp);

	// printf("%d\n", sz);
	double* d_tensor_flatten = (double*)malloc(sz * sizeof(double));

	for (size_t i = 0; i < sz; i++) {
		if (fscanf(fp, "%lf", &d_tensor_flatten[i]) != 1) {
			fclose(fp);
			fputs("I/O error, malformed input, or premature EOF\n", stderr);
			return;
		}
	}
	fclose(fp);

	// 量化过程
	Array a_tensor_flatten;
	QuantifiedDataType(d_tensor_flatten, &a_tensor_flatten, sz);

	if ((shape_tensor[0] + shape_tensor[1] + shape_tensor[2]) == 0)  // "array"
	{
		TENSOR1_SHAPE[0] = shape_tensor[3];
		TensorInitial(param_tensor, TENSOR1_STR, TENSOR1_SHAPE);
	}
	else if ((shape_tensor[0] + shape_tensor[1]) == 0)  // "matrix"
	{
		TENSOR2_SHAPE[0] = shape_tensor[2];
		TENSOR2_SHAPE[1] = shape_tensor[3];
		TensorInitial(param_tensor, TENSOR2_STR, TENSOR2_SHAPE);
	}
	else if (shape_tensor[0] == 0)  // "tensor3d"
	{
		TENSOR3_SHAPE[0] = shape_tensor[1];
		TENSOR3_SHAPE[1] = shape_tensor[2];
		TENSOR3_SHAPE[2] = shape_tensor[3];
		TensorInitial(param_tensor, TENSOR3_STR, TENSOR3_SHAPE);
	}
	else  // "tensor4d"
	{
		TENSOR4_SHAPE[0] = shape_tensor[0];
		TENSOR4_SHAPE[1] = shape_tensor[1];
		TENSOR4_SHAPE[2] = shape_tensor[2];
		TENSOR4_SHAPE[3] = shape_tensor[3];
		TensorInitial(param_tensor, TENSOR4_STR, TENSOR4_SHAPE);
	}

	// 张量赋值
	TensorSet(param_tensor, &(a_tensor_flatten.data[0]), sz);

	// clock_end = clock();
	// // printf("time = %f\n",(double)(clock_end - clock_start));
	// printf("time = %f s\n",(double)(clock_end - clock_start)/CLK_TCK);
	// 
	//free(f_path_name);
	//f_path_name = NULL;
}
