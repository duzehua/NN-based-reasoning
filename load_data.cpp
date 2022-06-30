#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "load_data.h"
#include "GINFO_VAR.h"

/**
 * @brief 从csv文件加载权重数据到Tensor
 *
 * @param filename
 * @param tensor_data
 * @param ts_shape
 * @param ts_type
 */
void loadweight_csv2tensor(const char* filename, Tensor* tensor_data, int* ts_shape, char* ts_type)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed.\n");
        exit(EXIT_FAILURE);
    }
    char col_max[102400];    //csv文件数据最大列数，尽可能大（一个权重占22个字符左右，一个张量的所有权重放在一行）
    char* token;

    int tsnum, chnum, row, col;
    if (strcmp(ts_type, TENSOR2_STR) == 0)
    {
        tsnum = 1;    //tsnum
        chnum = 1;    //chnum
        row = ts_shape[2];    //row
        col = ts_shape[3];    //col
    }
    else if (strcmp(ts_type, TENSOR3_STR) == 0)
    {
        tsnum = 1;    //tsnum
        chnum = ts_shape[1];    //chnum
        row = ts_shape[2];    //row
        col = ts_shape[3];    //col
    }
    else if (strcmp(ts_type, TENSOR4_STR) == 0)
    {
        tsnum = ts_shape[0];    //tsnum
        chnum = ts_shape[1];    //chnum
        row = ts_shape[2];    //row
        col = ts_shape[3];    //col
    }
    else
    {
        fprintf(stderr, "ERROR: TensorType => (%s not in [matrix, tensor3d, tensor4d])\n", ts_type);
        exit(EXIT_FAILURE);
    }

    //将csV中权重数据加载到tensor
    int i, j, k, l;
    i = j = k = l = 0;
    while (fgets(col_max, 102400, fp) != NULL && i < tsnum)
    {
        token = strtok(col_max, ",");
        j = 0;
        while (token != NULL && j < chnum)
        {
            k = 0;
            while (token != NULL && k < row)
            {
                l = 0;
                while (token != NULL && l < col)
                {
                    //printf("Token: %s\n", token);
                    tensor_data->data[i][j][k][l] = strtod(token, NULL);
                    //printf("%d %d %d %d: %.19f\n", i, j, k, l, tensor_data->data[i][j][k][l]);
                    l++;
                    token = strtok(NULL, ",");
                }
                k++;
            }
            j++;
        }
        i++;
    }

    fclose(fp);

}

/**
 * @brief 从csv文件加载全连接层的权重到Matrix
 *
 * @param filename
 * @param matrix
 */
void loadFCweight_csv2Matrix(const char* filename, Matrix* mat_data)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed.\n");
        exit(EXIT_FAILURE);
    }

    //int row = mat_data->row;
    //int col = mat_data->col;
    int row = mat_data->col;  // transpose
    int col = mat_data->row;  //transpose
    char col_max[102400];
    char* token;
    int i, j;
    i = j = 0;
    while (fgets(col_max, 102400, fp) != NULL) {
        token = strtok(col_max, ",");
        while (token != NULL && i < row)
        {
            j = 0;
            while (token != NULL && j < col) {
                //printf("Token: %s\n", token);
                //mat_data->data[i][j] = strtod(token, NULL);
                mat_data->data[j][i] = strtod(token, NULL);  // 矩阵乘法，先对全连接的权重和偏置进行转置（reshape）
                //printf("%d %d : %.19f\n", i, j, mat_data->data[i][j]);
                j++;
                token = strtok(NULL, ",");
            }
            i++;
        }

    }
}


void loadFCbias_csv2Matrix(const char* filename, Matrix* mat_data)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed.\n");
        exit(EXIT_FAILURE);
    }

    int row = mat_data->row;
    int col = mat_data->col;
    char col_max[10240];
    char* token;
    int i, j;
    i = j = 0;
    while (fgets(col_max, 10240, fp) != NULL) {
        token = strtok(col_max, ",");
        while (token != NULL && i < row)
        {
            j = 0;
            while (token != NULL && j < col) {
                //printf("Token: %s\n", token);
                mat_data->data[i][j] = strtod(token, NULL);
                //printf("%d %d : %.19f\n", i, j, mat_data->data[i][j]);
                j++;
                token = strtok(NULL, ",");
            }
            i++;
        }

    }
}

/**
 * @brief 从csv文件加载偏置数据到Array
 *
 * @param filename
 * @param arr_data
 * @param len
 */
void loadbias_csv2array(const char* filename, Array* arr_data, int len)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed.\n");
        exit(EXIT_FAILURE);
    }
    char col_max[1000]; //偏置数据每个张量只有一个，一个只占一行
    char* token;

    int i = 0;
    while (fgets(col_max, 100, fp) != NULL)
    {
        token = strtok(col_max, ",");
        while (token != NULL && i < len)
        {
            //printf("Token: %s\n", token);
            arr_data->data[i] = strtod(token, NULL);
            //printf("%d : %.19f\n", i, arr_data->data[i]);
            token = strtok(NULL, ",");
        }
        i++;
    }
    fclose(fp);
}
