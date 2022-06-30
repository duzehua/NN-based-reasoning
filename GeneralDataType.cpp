#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "GeneralDataType.h"
#include "GINFO_VAR.h"

/**
* 矩阵初始化
* 说明: 每定义一个矩阵, 须执行初始化操作以获取指定大小的内存空间存储数据.
* @param[in]  mat_data -> 矩阵结构体
* @param[in]  row      -> 矩阵行数
* @param[in]  col      -> 矩阵列数
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixInitial(Matrix *mat_data, int row, int col)
{
    // 初始化矩阵行
	mat_data->data = (DATA**)malloc(row * sizeof(DATA*));

    // 初始化矩阵列
    int i;
	for (i = 0; i < row; i++)
    {
        mat_data->data[i] = (DATA*)malloc(col*sizeof(DATA));
    }
		
	mat_data->row = row;
	mat_data->col = col;
}


/**
* 释放矩阵
* 说明: 每用完一个矩阵, 须执行释放操作来释放内存空间.
* @param[in]  mat_data -> 矩阵结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixFree(Matrix *mat_data)
{
    // 释放矩阵行
	int i;
	for (i = 0; i < (mat_data->row); i++)
    {
        if (mat_data->data[i] != NULL) 
        {
            free(mat_data->data[i]);
            mat_data->data[i] = NULL;
        }
    }


    // 释放矩阵
    if (mat_data->data != NULL) 
    {
        free(mat_data->data);
        mat_data->data = NULL;
    }
}


/**
* 矩阵赋值
* 说明: 用于矩阵初始化之后, 将数据赋值给矩阵.
* @param[in]  mat_data    -> 被赋值矩阵结构体
* @param[in]  mat_data_in -> 赋值数据(数组首位地址)
* @param[in]  mdi_len     -> 赋值数据长度
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixSet(Matrix *mat_data, DATA *mat_data_in, int mdi_len)
{
    // 赋值需保证赋值变量mat_data_in与被赋值变量mat_data长度一致
    int md_len = (mat_data->row) * (mat_data->col);
    if(md_len != mdi_len){
        fprintf(stderr, "ERROR: MatrixSet => (mat_data[%d] != mat_data_in[%d])\n", md_len, mdi_len);
        exit(EXIT_FAILURE);
    }

	int i, j; // 行, 列
	for (i = 0; i < (mat_data->row); i++)
    {
		for (j = 0; j < (mat_data->col); j++)
        {
            mat_data->data[i][j] = *(mat_data_in + i*(mat_data->col) + j); //mat_data_in[i][j];
        }
			
	}
}


/**
* 1维数组
* 说明: 每定义1维数组, 须执行初始化操作以获取指定大小的内存空间存储数据.
* @param[in]  arr_data -> 数组结构体
* @param[in]  len      -> 数组长度
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArrayInitial(Array *arr_data, int len)
{
    // 初始化
	arr_data->data = (DATA*)malloc(len * sizeof(DATA));
    // 初始化为0
    for (int i = 0; i < len; i++)
    {
        arr_data->data[i] = DATA_0;
    }
    
	arr_data->len = len;
}


/**
* 释放1维数组
* 说明: 每用完一个数组, 须执行释放操作来释放内存空间.
* @param[in]  arr_data -> 数组结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArrayFree(Array *arr_data)
{
    // 释放数组
    if (arr_data->data != NULL) 
    {
        free(arr_data->data);
        arr_data->data = NULL;
    }
}


/**
* 1维数组赋值
* 说明: 用于数组初始化之后, 将数据赋值给数组.
* @param[in]  arr_data    -> 被赋值矩阵结构体
* @param[in]  arr_data_in -> 赋值数据(数组首位地址)
* @param[in]  adi_len     -> 赋值数据长度
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArraySet(Array *arr_data, DATA *arr_data_in, int adi_len)
{
    // 赋值需保证赋值变量arr_data_in与被赋值变量arr_data长度一致
    if(arr_data->len != adi_len){
        fprintf(stderr, "ERROR: ArraySet => (arr_data[%d] != arr_data_in[%d])\n", arr_data->len, adi_len);
        exit(EXIT_FAILURE);
    }

	for (int i = 0; i < (arr_data->len); i++)
    {
        arr_data->data[i] = *(arr_data_in + i); //mat_data_in[i][j];
	}
}