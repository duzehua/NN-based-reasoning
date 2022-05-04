/**
* @file
* @briefThis is a brief description.
* @details This is the detail description.
* @authorauthor
* @date date
* @version v1.0
* @par Copyright(c): abc corporation
* @par History:
*version: author, date, desc\n
*/ 

// #include <error.h>
#include <time.h>
#include <stdio.h>

#include "GINFO.h"
#include "GINFO_VAR.h"
#include "Tensor.h"
#include "ConvOperation.h"
#include "ActivationFunction.h"
#include "PoolingOperation.h"
#include "FullyConnectedFunction.h"

/**************************网络层描述部分**************************/
int main()
{
    clock_t clock_start, clock_end;
    clock_start = clock();

    //// 定义池化步长
    POOLSTRIDE pool_stride;
    pool_stride.row = 2;  // 行步长
    pool_stride.col = 2;  // 列步长
    //// 定义池化尺寸
    POOLSIZE pool_size;
    pool_size.row = 2;  // 池化核行数
    pool_size.col = 2;  // 池化核列数

    Tensor input_0;

    // 网络结构定义
    //// Conv-1, 卷积层1 k=5, o=6, s=1
    Tensor conv_kel_1;
    CONVSTRIDE conv_stride_1;
    conv_stride_1.row = 1;  // 行步长
    conv_stride_1.col = 1;  // 列步长
    Array bias_1;
    Tensor output_1;
    Conv2(&input_0, &conv_kel_1, &conv_stride_1, &bias_1, VALID, &output_1);
    //// Pool-1, 池化层1 k=2, Max, s=2
    POOLSIZE pool_size_2;
    pool_size_2.row = 2;
    pool_size_2.col = 2;
    POOLSTRIDE pool_stride_2;
    pool_stride_2.row = 2;
    pool_stride_2.col = 2;
    Tensor output_2;
    MaxPooling2D(&output_1, &pool_size_2, &pool_stride_2, VALID, &output_2);
    //// Conv-2, 卷积层2 k=5, o=6, s=1
    Tensor conv_kel_3;
    CONVSTRIDE conv_stride_3;
    conv_stride_3.row = 1;  // 行步长
    conv_stride_3.col = 1;  // 列步长
    Array bias_3;
    Tensor output_3;
    Conv2(&output_2, &conv_kel_3, &conv_stride_3, &bias_3, VALID, &output_3); 
    //// Pool-2, 池化层2, k=2, Max, s=2
    POOLSIZE pool_size_4;
    pool_size_4.row = 2;
    pool_size_4.col = 2;
    POOLSTRIDE pool_stride_4;
    pool_stride_4.row = 2;
    pool_stride_4.col = 2;
    Tensor output_4;
    MaxPooling2D(&output_3, &pool_size_4, &pool_stride_4, VALID, &output_4);
    //// Flatten
    Tensor output_5;
    TensorFlatten(&output_4, &output_5);
    //// Full-1, o=120
    Matrix weight_6;
    Matrix bias_6;
    Tensor output_6;
    FullConnected(&output_5, &weight_6, &bias_6, &output_6);
    //// Activation, tanh
    TANH(&output_6);
    //// Full-2, o=84
    Matrix weight_7;
    Matrix bias_7;
    Tensor output_7;
    FullConnected(&output_6, &weight_7, &bias_7, &output_7);
    //// Activation, tanh
    TANH(&output_7);
    //// Full-3, o=cls_num
    Matrix weight_8;
    Matrix bias_8;
    Tensor output_8;
    FullConnected(&output_7, &weight_8, &bias_8, &output_8);
    //// Softmax
    SOFTMAX(&output_8);
}
