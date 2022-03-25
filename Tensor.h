#ifndef TENSOR_H
#define TENSOR_H

#include "GINFO.h"


/**
* @张量结构体定义
* @details 将矩阵\3维张量\4维张量全部定义成一种数据, 通过类型索引辨别
*/
typedef struct
{
    DATA ****data;     /*!<张量数据 [tsnum][chnum][row][col]*/
    char *ts_type;
    int *ts_shape;
    // ts_shape ts_shape_;
    int      tsnum;    /*!<张量个数>*/
    int      chnum;    /*!<张量通道>*/
    int      row;      /*!<张量行数>*/
    int      col;      /*!<张量列数>*/
    // int      i_type[3] = {1, 2, 3};                          /*!<张量类型索引数组*/
    // char     s_type[3] = {'Matrix', 'Tensor3D', 'Tensor4D'}; /*!<张量类型字符串数组*/
}Tensor;

/**
* @矩阵结构体定义
* @details 无
*/
typedef struct
{
    DATA **data; /*!<矩阵数据 [row][col]>*/
    int    row;  /*!<矩阵行数>*/
    int    col;  /*!<矩阵列数>*/
}Matrix;

/**
* @数组结构体定义
* @details 无
*/
typedef struct
{
    DATA *data;  /*!<数组数据 [idx]>*/
    int    len;  /*!<数组长度>*/
}Array;

// 通用张量部分
void TensorInitial(Tensor *tensor_data, char *ts_type, int *ts_shape);
void TensorFree(Tensor *tensor_data);
void TensorSet(Tensor *tensor_data, DATA *tensor_data_in, int ti_len);
void TensorPaddingZero(Tensor *tensor_data);

// 矩阵部分
void MatrixInitial(Matrix *mat_data, int row, int col);
void MatrixFree(Matrix *mat_data);
void MatrixSet(Matrix *mat_data, DATA *mat_data_in, int mdi_len);

// 数组部分
void ArrayInitial(Array *arr_data, int len);
void ArrayFree(Array *arr_data);
void ArraySet(Array *arr_data, DATA *arr_data_in, int adi_len);

//--------------------------------------------------------------------

// /**
// * @三维张量结构体定义
// * @details 在矩阵的基础上添加通道, 构成三维张量.
// */
// typedef struct
// {
//     DATA ***data;     /*!<张量数据 [chnum][row][col]>*/
//     int     chnum;    /*!<张量通道>*/
//     int     row;      /*!<张量行数>*/
//     int     col;      /*!<张量列数>*/
//     // char    s_type = 't3d';
// }Tensor3D;

// /**
// * @四维张量结构体定义
// * @details 在三维张量的基础上添加个数条件, 构成思维张量, 表示多个不同的三维张量.
// */
// typedef struct
// {
//     DATA ****data;     /*!<张量数据 [tsnum][chnum][row][col]>*/
//     int      tsnum;    /*!<张量个数>*/
//     int      chnum;    /*!<张量通道>*/
//     int      row;      /*!<张量行数>*/
//     int      col;      /*!<张量列数>*/
//     // char     s_type = 't4d';
// }Tensor4D;

// // 三维张量部分
// void Tensor3DInitial(Tensor3D *tensor3d_data, int chnum, int row, int col);
// void Tensor3DFree(Tensor3D *tensor3d_data);
// void Tensor3DSet(Tensor3D *tensor3d_data, DATA *tensor3d_data_in, int t3di_len);

// // 四维张量部分
// void Tensor4DInitial(Tensor4D *tensor4d_data, int tsnum, int chnum, int row, int col);
// void Tensor4DFree(Tensor4D *tensor4d_data);
// void Tensor4DSet(Tensor4D *tensor4d_data, DATA *tensor4d_data_in, int t4di_len);

#endif