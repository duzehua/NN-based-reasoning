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
* �����ַ������ض��ַ�������
* ˵��: �����ַ������ض��ַ��ض��������ֵ�����.
* @param[in]  Str          -> ����ȡ���ֵ��ַ���
* @param[in]  idx_start    -> ��ʼ����(ȡ��)
* @param[in]  idx_end      -> ��ֹ����(ȡ��)
* @param[out] ������ȡ�����֣�int
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
	*(num_char + len) = '\0'; //����ַ������ַ���������
	int num_int = atoi(num_char);
	free(num_char);
	return num_int;
}


/**
* �����ַ������ض��ַ�������
* ˵��: �����ַ������ض��ַ��ض��������ֵ�����.
* @param[in]  Str          -> �����ҵ��ַ���
* @param[in]  Char         -> �����ҵ��ַ�
* @param[in]  idx_start    -> ��ʼ����
* @param[in]  idx_num      -> ��n�γ��ֵ�����
* @param[in]  descend_flag -> �Ƿ�����ҵı�־λ
* @param[out] ����ָ������
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
* Warning: δ����쳣�жϺʹ�������
*/
int FindCharIdxInStr(const char* Str, char Char, int idx_start, int idx_num, int descend_flag)
{
	const char* curt_num_addr;
	int   idx_curt_num;
	curt_num_addr = &Str[idx_start];
	// �����idx_num�����ų��ֵ�λ��
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
* ���ļ���������ȡ�����ĳߴ�
* ˵��: ���ļ���������ȡ�����ߴ磬�ļ����Ƹ�ʽΪ��epoch_step_index_��������_ͨ����_��_��_�����.��������.csv.
* @param[in]  f_path_name  -> ����ȡ������״��Ϣ���ļ�·������
* @param[in]  shape_tensor -> ��ȡ����״, ����\ͨ����\��\��
* @param[out] null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 28, 2022
*/
void ExtractShapeFromCharForTensor(const char* f_path_name, int* shape_tensor)
{
	// ����ļ���
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


// �����ļ�·��, ��ȡ��Ӧ������
void LayerParamLoadFromCSV(const char* f_path_name, Tensor* param_tensor)
{

	// ͨ���ļ�������ȡ��������״
	int shape_tensor[4] = { 0,0,0,0 };
	ExtractShapeFromCharForTensor(f_path_name, shape_tensor);
	//printf("%d, %d, %d, %d\n", shape_tensor[0], shape_tensor[1], shape_tensor[2], shape_tensor[3]);

	// ��ȡ�ļ��е�����
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

	// ��������
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

	// ������ֵ
	TensorSet(param_tensor, &(a_tensor_flatten.data[0]), sz);

	// clock_end = clock();
	// // printf("time = %f\n",(double)(clock_end - clock_start));
	// printf("time = %f s\n",(double)(clock_end - clock_start)/CLK_TCK);
	// 
	//free(f_path_name);
	//f_path_name = NULL;
}
