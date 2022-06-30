#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* 张量初始化
* 说明: 每定义一个张量, 须执行初始化操作以获取指定大小的内存空间存储数据.
* @param[in]  tensor_data -> 张量结构体
* @param[in]  ts_type     -> 张量类型
                             TENSOR1_STR=>"array"
                             TENSOR2_STR=>"matrix"
                             TENSOR3_STR=>"tensor3d"
                             TENSOR4_STR=>"tensor4d"
* @param[in]  ts_shape    -> 张量尺寸
                             TENSOR1_SHAPE[1]={col}
                             TENSOR2_SHAPE[2]={row, col}
                             TENSOR3_SHAPE[3]={chnum, row, col}
                             TENSOR4_SHAPE[4]={tsnum, chnum, row, col}
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
* Data: Jun. 28, 2022; newly add: 插入一维张量; By: Zehua Du
*/
void TensorInitial(Tensor *tensor_data, char *ts_type, int *ts_shape)
// void TensorInitial(Tensor *tensor_data, int tsnum, int chnum, int row, int col)
{
    int i, j, k;
    
    if (strcmp(ts_type, TENSOR1_STR) == 0)
    {
        // 类型定义
        tensor_data->ts_type = (char*)malloc(5 * sizeof(char));
        strcpy(tensor_data->ts_type, TENSOR1_STR);

        // 尺寸定义
        tensor_data->tsnum = 1;
        tensor_data->chnum = 1;
        tensor_data->row   = 1;
        //printf("%d, %d\n", ts_shape[0], ts_shape[1]);
        tensor_data->col   = ts_shape[0];
        tensor_data->ts_shape = (int*)malloc(1 * sizeof(int));
        tensor_data->ts_shape[0] = ts_shape[0];
    }
    else if (strcmp(ts_type, TENSOR2_STR) == 0)
    {
        // 类型定义
        tensor_data->ts_type = (char*)malloc(6 * sizeof(char));
        strcpy(tensor_data->ts_type, TENSOR2_STR);

        // 尺寸定义
        tensor_data->tsnum = 1;
        tensor_data->chnum = 1;
        tensor_data->row   = ts_shape[0];
        tensor_data->col   = ts_shape[1];
        tensor_data->ts_shape = (int*)malloc(2 * sizeof(int));
        tensor_data->ts_shape[0] = ts_shape[0];
        tensor_data->ts_shape[1] = ts_shape[1];

        // debug
        // printf("类型 %s, ", tensor_data->ts_type);
        // printf("尺寸 [%d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1]);
    }
    // 3维张量
    else if (strcmp(ts_type, TENSOR3_STR) == 0)
    {
        tensor_data->ts_type = (char*)malloc(8 * sizeof(char));
        strcpy(tensor_data->ts_type, TENSOR3_STR);
        tensor_data->tsnum = 1;
        tensor_data->chnum = ts_shape[0];
        tensor_data->row   = ts_shape[1];
        tensor_data->col   = ts_shape[2];
        tensor_data->ts_shape = (int*)malloc(3 * sizeof(int));
        tensor_data->ts_shape[0] = ts_shape[0];
        tensor_data->ts_shape[1] = ts_shape[1];
        tensor_data->ts_shape[2] = ts_shape[2];

        // debug
        // printf("类型 %s\n", tensor_data->ts_type);
        // printf("尺寸 [%d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2]);
    }
    // 4维张量
    else if (strcmp(ts_type, TENSOR4_STR) == 0)
    {
        tensor_data->ts_type = (char*)malloc(8 * sizeof(char));
        strcpy(tensor_data->ts_type, TENSOR4_STR);
        tensor_data->tsnum = ts_shape[0];
        tensor_data->chnum = ts_shape[1];
        tensor_data->row   = ts_shape[2];
        tensor_data->col   = ts_shape[3];
        tensor_data->ts_shape = (int*)malloc(4 * sizeof(int));
        tensor_data->ts_shape[0] = ts_shape[0];
        tensor_data->ts_shape[1] = ts_shape[1];
        tensor_data->ts_shape[2] = ts_shape[2];
        tensor_data->ts_shape[3] = ts_shape[3];

        // debug
        // printf("类型 %s\n", tensor_data->ts_type);
        // printf("尺寸 [%d, %d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2], tensor_data->ts_shape[3]);
    }
    else
    {
        fprintf(stderr, "ERROR: TensorType => (%s not in [array, matrix, tensor3d, tensor4d])\n", ts_type);
        exit(EXIT_FAILURE);
    }

    // 初始化个数
    tensor_data->data = (DATA****)malloc(tensor_data->tsnum * sizeof(DATA***));
    // 初始化通道
    for (i = 0; i < tensor_data->tsnum; i++)
    {
        tensor_data->data[i] = (DATA***)malloc(tensor_data->chnum * sizeof(DATA**));
    }
    // 初始化单通道行
    for (i = 0; i < tensor_data->tsnum; i++)
    {
        for (j = 0; j < tensor_data->chnum; j++)
        {
            tensor_data->data[i][j] = (DATA**)malloc(tensor_data->row * sizeof(DATA*));
        }
    }
    // 初始化单通道列
    for (i = 0; i < tensor_data->tsnum; i++)
    {
        for (j = 0; j < tensor_data->chnum; j++)
        {
            for (k = 0; k < tensor_data->row; k++)
            {
                // printf("%d\n", tensor_data->col);
                tensor_data->data[i][j][k] = (DATA*)malloc(tensor_data->col * sizeof(DATA));
            }
        }
    }
}


/**
* 释放张量
* 说明: 每用完一个张量, 须执行释放操作来释放内存空间.
* @param[in]  tensor_data -> 张量结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void TensorFree(Tensor *tensor_data)
{
 	int i, j, k;   

    // 释放张量列
    for (i = 0; i < (tensor_data->tsnum); i++)
    {
        for (j = 0; j < (tensor_data->chnum); j++)
        {
            for (k = 0; k < (tensor_data->row); k++)
            {
                if (tensor_data->data[i][j][k] != NULL) 
                {
                    free((DATA ****)(tensor_data->data[i][j][k]));
                    tensor_data->data[i][j][k] = NULL;
                }
            }
        }
    }

    // 释放张量行
	for (i = 0; i < (tensor_data->tsnum); i++)
    {
        for (j = 0; j < (tensor_data->chnum); j++)
        {
            if (tensor_data->data[i][j] != NULL) 
            {
                free(*tensor_data->data[i][j]);
                tensor_data->data[i][j] = NULL;                
            }
        }
    }

    // 释放张量通道
    for (i = 0; i < (tensor_data->tsnum); i++)
    {
        if (tensor_data->data[i] != NULL) 
        {
            free(*tensor_data->data[i]);
            tensor_data->data[i] = NULL;            
        }
    }
    
    // 释放张量
    if (tensor_data->data[i] != NULL) 
    {
        free(*tensor_data->data);
        tensor_data->data = NULL;        
    }
}


/**
* 张量赋值
* 说明: 用于张量初始化之后, 将数据赋值给张量.
* @param[in]  Tensor         -> 被赋值张量结构体
* @param[in]  tensor_data_in -> 赋值数据(数组首位地址)
* @param[in]  ti_len         -> 赋值数据长度
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void TensorSet(Tensor *tensor_data, DATA *tensor_data_in, int ti_len)
{
    // 赋值需保证赋值变量tensor_data_in与被赋值变量tensor_data长度一致
    int m_len = (tensor_data->tsnum) * (tensor_data->chnum) * (tensor_data->row) * (tensor_data->col);
    if (m_len != ti_len)
    {
        fprintf(stderr, "ERROR: TensorSet => (tensor_data[%d] != tensor_data_in[%d])\n", m_len, ti_len);
        exit(EXIT_FAILURE);
    }

	int i, j, k, h;  // 个数, 通道, 行, 列
    int tsr_gain;    // 个数增益
    int chn_gain;    // 通道增益
    int row_gain;    // 行增益
	for (i = 0; i < (tensor_data->tsnum); i++)
    {
		for (j = 0; j < (tensor_data->chnum); j++)
        {
            for (k = 0; k < (tensor_data->row); k++)
            {
                for (h = 0; h < (tensor_data->col); h++)
                {
                    tsr_gain = i * (tensor_data->chnum) * (tensor_data->row) * (tensor_data->col);
                    chn_gain = j * (tensor_data->row) * (tensor_data->col);
                    row_gain = k * (tensor_data->col);
                    tensor_data->data[i][j][k][h] = *(tensor_data_in + tsr_gain + chn_gain + row_gain + h); //tensor_data_in[个数][通道][行][列];
                    // printf("%f\n", tensor_data->data[i][j][k][h]);
                }
            }
        }
	}
}


/**
* 张量值置零
* 说明: 用于张量初始化之后, 将张量赋值为0.
* @param[in]  tensor_data -> 被赋值张量结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void TensorPaddingZero(Tensor *tensor_data)
{
    int i, j, k, h;  // 个数, 通道, 行, 列
    for (i = 0; i < (tensor_data->tsnum); i++)
    {
		for (j = 0; j < (tensor_data->chnum); j++)
        {
            for (k = 0; k < (tensor_data->row); k++)
            {
                for (h = 0; h < (tensor_data->col); h++)
                {
                    tensor_data->data[i][j][k][h] = DATA_0;
                }
            }
        }
    }
}

/**
* 查看张量
* 说明: 查看张量的类型和形状.
* @param[in]  tensor_data -> 被查看的张量
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 29, 2022  
*/
void ViewTensor(Tensor *tensor_data)
{
    //
    printf("-------------------------------\n");
    printf("tensor type : %s\n", tensor_data->ts_type);
    printf("       shape: [%d, %d, %d, %d]\n", tensor_data->tsnum, tensor_data->chnum, tensor_data->row, tensor_data->col);
    printf("-------------------------------\n");
}

// -------------------------------------------------------------------------


// /**
// * 三维张量初始化
// * 说明: 每定义一个三维张量, 须执行初始化操作以获取指定大小的内存空间存储数据.
// * @param[in]  tensor3d_data -> 3维张量结构体
// * @param[in]  chnum         -> 3维张量通道数
// * @param[in]  row           -> 3维张量中单通道矩阵行数
// * @param[in]  col           -> 3维张量中单通道矩阵列数
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DInitial(Tensor3D *tensor3d_data, int chnum, int row, int col)
// {
//     int i, j;

//     // 初始化通道
// 	tensor3d_data->data = (DATA***)malloc(chnum * sizeof(DATA**));

//     // 初始化通道行
//     for (i = 0; i < row; i++)
//     {
//         tensor3d_data->data[i] = (DATA**)malloc(row * sizeof(DATA*));
//     }

//     // 初始化通道列
//     for (i = 0; i < chnum; i++)
//     {
//         for (j = 0; j < row; j++)
//         {
//             tensor3d_data->data[i][j] = (DATA*)malloc(col * sizeof(DATA));
//         }
//     }

//     tensor3d_data->chnum = chnum;
// 	tensor3d_data->row   = row;
// 	tensor3d_data->col   = col;
// }


// /**
// * 释放三维张量
// * 说明: 每用完一个三维张量, 须执行释放操作来释放内存空间.
// * @param[in]  tensor3d_data -> 三维张量结构体
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DFree(Tensor3D *tensor3d_data)
// {
//     // // 释放张量列
// 	int i, j;
// 	for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
//         for (j = 0; j < (tensor3d_data->row); j++)
//         {
//             // printf("释放一级");
//             if (tensor3d_data->data[i][j] != NULL) 
//             {
//                 free((DATA ***)(tensor3d_data->data[i][j]));
//                 tensor3d_data->data[i][j] = NULL;
// 			}
//         }
//     }

//     // 释放张量行
//     for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
//         if (tensor3d_data->data[i] != NULL) 
//         {
//             free(*tensor3d_data->data[i]);
//             tensor3d_data->data[i] = NULL;
//         }
//     }
    
//     // 释放张量
//     if (tensor3d_data->data != NULL) 
//     {
//         printf("非空");
//         free(*tensor3d_data->data);
//         tensor3d_data->data = NULL;
//     }
// }


// /**
// * 三维张量赋值
// * 说明: 用于三维初始化之后, 将数据赋值给三维张量.
// * @param[in]  tensor3d_data    -> 被赋值张量结构体
// * @param[in]  tensor3d_data_in -> 赋值数据(数组首位地址)
// * @param[in]  t3di_len         -> 赋值数据长度
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DSet(Tensor3D *tensor3d_data, DATA *tensor3d_data_in, int t3di_len)
// {
//     // 赋值需保证赋值变量tensor3d_data_in与被赋值变量tensor3d_data长度一致
//     int m3d_len = (tensor3d_data->chnum) * (tensor3d_data->row) * (tensor3d_data->col);
//     if (m3d_len != t3di_len)
//     {
//         fprintf(stderr, "ERROR: Tensor3DSet => (tensor3d_data[%d] != tensor3d_data_in[%d])\n", m3d_len, t3di_len);
//         exit(EXIT_FAILURE);
//     }

//     // printf("3维张量开始赋值1\n");
// 	int i, j, k;  // 通道, 行, 列
//     int chn_gain; // 通道增益
//     int row_gain; // 行增益
// 	for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
// 		for (j = 0; j < (tensor3d_data->row); j++)
//         {
//             for (k = 0; k < (tensor3d_data->col); k++)
//             {
//                 chn_gain = i * (tensor3d_data->row) * (tensor3d_data->col);
//                 row_gain = j * (tensor3d_data->col);
//                 // printf("3维张量开始赋值2\n");
//                 tensor3d_data->data[i][j][k] = *(tensor3d_data_in + chn_gain + row_gain + k); //tensor3d_data_in[通道][行][列];
//             }
//         }
// 	}
// }


// /**
// * 四维张量初始化
// * 说明: 每定义一个四维张量, 须执行初始化操作以获取指定大小的内存空间存储数据.
// * @param[in] tensor4d_data -> 4维张量结构体
// * @param[in] tsnum         -> 4维张量中3维张量个数
// * @param[in] chnum         -> 4维张量中3维张量通道数
// * @param[in] row           -> 4维张量中, 3维张量单通道矩阵行数
// * @param[in] col           -> 4维张量中, 3维张量单通道矩阵列数
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DInitial(Tensor4D *tensor4d_data, int tsnum, int chnum, int row, int col)
// {
//     int i, j, k;

//     // 初始化个数
// 	tensor4d_data->data = (DATA****)malloc(tsnum * sizeof(DATA***));

//     // 初始化通道
//     for (i = 0; i < tsnum; i++)
//     {
//         tensor4d_data->data[i] = (DATA***)malloc(chnum * sizeof(DATA**));
//     }

//     // 初始化单通道行
//     for (i = 0; i < tsnum; i++)
//     {
//         for (j = 0; j < chnum; j++)
//         {
//             tensor4d_data->data[i][j] = (DATA**)malloc(row * sizeof(DATA*));
//         }
//     }

//     // 初始化单通道列
//     for (i = 0; i < tsnum; i++)
//     {
//         for (j = 0; j < chnum; j++)
//         {
//             for (k = 0; k < row; k++)
//             {
//                 tensor4d_data->data[i][j][k] = (DATA*)malloc(col * sizeof(DATA));
//             }
//         }
//     }

//     tensor4d_data->tsnum = tsnum;
//     tensor4d_data->chnum = chnum;
// 	tensor4d_data->row   = row;
// 	tensor4d_data->col   = col;
// }


// /**
// * 释放四维张量
// * 说明: 每用完一个四维张量, 须执行释放操作来释放内存空间.
// * @param[in]  tensor4d_data -> 四维张量结构体
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DFree(Tensor4D *tensor4d_data)
// {
//  	int i, j, k;   

//     // 释放张量列
//     for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             for (k = 0; k < (tensor4d_data->row); k++)
//             {
//                 if (tensor4d_data->data[i][j][k] != NULL) 
//                 {
//                     free((DATA ****)(tensor4d_data->data[i][j][k]));
//                     tensor4d_data->data[i][j][k] = NULL;
//                 }
//             }
//         }
//     }

//     // 释放张量行
// 	for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             if (tensor4d_data->data[i][j] != NULL) 
//             {
//                 free(*tensor4d_data->data[i][j]);
//                 tensor4d_data->data[i][j] = NULL;                
//             }
//         }
//     }

//     // 释放张量通道
//     for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         if (tensor4d_data->data[i] != NULL) 
//         {
//             free(*tensor4d_data->data[i]);
//             tensor4d_data->data[i] = NULL;            
//         }
//     }
    
//     // 释放张量
//     if (tensor4d_data->data[i] != NULL) 
//     {
//         free(*tensor4d_data->data);
//         tensor4d_data->data = NULL;        
//     }
// }


// /**
// * 四维张量赋值
// * 说明: 用于四维初始化之后, 将数据赋值给四维张量.
// * @param[in]  tensor4d_data    -> 被赋值张量结构体
// * @param[in]  tensor4d_data_in -> 赋值数据(数组首位地址)
// * @param[in]  t4di_len         -> 赋值数据长度
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DSet(Tensor4D *tensor4d_data, DATA *tensor4d_data_in, int t4di_len)
// {
//     // 赋值需保证赋值变量tensor3d_data_in与被赋值变量tensor3d_data长度一致
//     int m4d_len = (tensor4d_data->tsnum) * (tensor4d_data->chnum) * (tensor4d_data->row) * (tensor4d_data->col);
//     if (m4d_len != t4di_len)
//     {
//         fprintf(stderr, "ERROR: Tensor4DSet => (tensor4d_data[%d] != tensor3d_data_in[%d])\n", m4d_len, t4di_len);
//         exit(EXIT_FAILURE);
//     }

// 	int i, j, k, h;  // 个数, 通道, 行, 列
//     int tsr_gain;    // 个数增益
//     int chn_gain;    // 通道增益
//     int row_gain;    // 行增益
// 	for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
// 		for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             for (k = 0; k < (tensor4d_data->row); k++)
//             {
//                 for (h = 0; h < (tensor4d_data->col); h++)
//                 {
//                     tsr_gain = i * (tensor4d_data->chnum) * (tensor4d_data->row) * (tensor4d_data->col);
//                     chn_gain = j * (tensor4d_data->row) * (tensor4d_data->col);
//                     row_gain = k * (tensor4d_data->col);
//                     tensor4d_data->data[i][j][k][h] = *(tensor4d_data_in + tsr_gain + chn_gain + row_gain + h); //tensor4d_data_in[个数][通道][行][列];
//                 }
//             }
//         }
// 	}
// }