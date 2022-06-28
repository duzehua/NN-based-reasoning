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

#define _CRT_SECURE_NO_WARNINGS

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
#include "NormalizationFunction.h"
#include "load_data.h"
#include "decodepng.h"

/**************************网络层描述部分**************************/
int main()
{
    clock_t clock_start, clock_end;
    clock_start = clock();

    // TENSORTYPE  TENSOR4_STR[9] = "tensor4d";
    // CONVMODE FULL[5]  = "FULL";
    // CONVMODE SAME[5]  = "SAME";
    // CONVMODE VALID[6] = "VALID";

    //// 定义池化步长
    POOLSTRIDE pool_stride;
    pool_stride.row = 2;  // 行步长
    pool_stride.col = 2;  // 列步长
    //// 定义池化尺寸
    POOLSIZE pool_size;
    pool_size.row = 2;  // 池化核行数
    pool_size.col = 2;  // 池化核列数

    const char* pngfile = "./test_images/padpic_4_8318.png";
    const char* conv_kel_1_file = "./csv/block_1.0.weight.csv";
    const char* conv_kel_3_file = "./csv/block_1.3.weight.csv";
    const char* weight_6_file = "./csv/block_2.0.weight.csv";
    const char* weight_7_file = "./csv/block_2.2.weight.csv";
    const char* weight_8_file = "./csv/block_2.4.weight.csv";
    const char* bias_1_file = "./csv/block_1.0.bias.csv";
    const char* bias_3_file = "./csv/block_1.3.bias.csv";
    const char* bias_6_file = "./csv/block_2.0.bias.csv";
    const char* bias_7_file = "./csv/block_2.2.bias.csv";
    const char* bias_8_file = "./csv/block_2.4.bias.csv";
    TENSORSHAPE input_0_shape[4] = { 1,1,32,32 };
    TENSORSHAPE conv_kel_1_shape[4] = { 6,1,5,5 };
    TENSORSHAPE output_1_shape[4] = { 1,6,28,28 };
    TENSORSHAPE output_2_shape[4] = { 1,6,14,14 };
    TENSORSHAPE conv_kel_3_shape[4] = { 16,6,5,5 };
    TENSORSHAPE output_3_shape[4] = { 1,16,10,10 };
    TENSORSHAPE output_4_shape[4] = { 1,16,5,5 };

    TENSORSHAPE output_5_shape[4] = { 1,1,1,400 };
    TENSORSHAPE output_6_shape[4] = { 1,1,1,120 };
    TENSORSHAPE output_7_shape[4] = { 1,1,1,84 };
    TENSORSHAPE output_8_shape[4] = { 1,1,1,10 };
    // TENSORSHAPE weight_6_shape[2] = {120,400};



    Tensor input_0;
    Tensor conv_kel_1;
    Array  bias_1;
    Tensor output_1;
    TensorInitial(&input_0, TENSOR4_STR, input_0_shape);
    TensorInitial(&conv_kel_1, TENSOR4_STR, conv_kel_1_shape);
    TensorInitial(&output_1, TENSOR4_STR, output_1_shape);
    ArrayInitial(&bias_1, 6);
    getPixeldata(pngfile, &input_0);
    MinMaxNormalization(&input_0, 0, 255, 0, 1);
    loadweight_csv2tensor(conv_kel_1_file, &conv_kel_1, conv_kel_1_shape, TENSOR4_STR);
    loadbias_csv2array(bias_1_file, &bias_1, 6);


    Tensor output_2;
    TensorInitial(&output_2, TENSOR4_STR, output_2_shape);

    Tensor conv_kel_3;
    TensorInitial(&conv_kel_3, TENSOR4_STR, conv_kel_3_shape);
    loadweight_csv2tensor(conv_kel_3_file, &conv_kel_3, conv_kel_3_shape, TENSOR4_STR);
    Array  bias_3;
    ArrayInitial(&bias_3, 16);
    loadbias_csv2array(bias_3_file, &bias_3, 16);
    Tensor output_3;
    TensorInitial(&output_3, TENSOR4_STR, output_3_shape);

    Tensor output_4;
    TensorInitial(&output_4, TENSOR4_STR, output_4_shape);

    Tensor output_5;
    TensorInitial(&output_5, TENSOR4_STR, output_5_shape);
    Matrix weight_6;
    MatrixInitial(&weight_6, 400, 120);  //transpose
    //MatrixInitial(&weight_6, 120, 400);
    loadFCweight_csv2Matrix(weight_6_file, &weight_6);
    Matrix  bias_6;
    MatrixInitial(&bias_6, 1, 120);  //transpose
    //MatrixInitial(&bias_6, 120, 1);
    loadFCweight_csv2Matrix(bias_6_file, &bias_6);
    Tensor output_6;
    TensorInitial(&output_6, TENSOR4_STR, output_6_shape);

    Matrix weight_7;
    MatrixInitial(&weight_7, 120, 84);  //transpose
    //MatrixInitial(&weight_7, 84, 120);
    loadFCweight_csv2Matrix(weight_7_file, &weight_7);
    Matrix  bias_7;
    MatrixInitial(&bias_7, 1, 84);  //transpose
    //MatrixInitial(&bias_7, 84, 1);
    loadFCweight_csv2Matrix(bias_7_file, &bias_7);

    Tensor output_7;
    TensorInitial(&output_7, TENSOR4_STR, output_7_shape);

    Matrix weight_8;
    MatrixInitial(&weight_8, 84, 10);  //transpose
    //MatrixInitial(&weight_8, 10, 84);
    loadFCweight_csv2Matrix(weight_8_file, &weight_8);
    Matrix  bias_8;
    MatrixInitial(&bias_8, 1, 10);  //transpose
    //MatrixInitial(&bias_8, 10, 1);
    loadFCweight_csv2Matrix(bias_8_file, &bias_8);
    Tensor output_8;
    TensorInitial(&output_8, TENSOR4_STR, output_8_shape);

    // 网络结构定义
    //// Conv-1, 卷积层1 k=5, o=6, s=1

    CONVSTRIDE conv_stride_1;
    conv_stride_1.row = 1;  // 行步长
    conv_stride_1.col = 1;  // 列步长

    Conv2(&input_0, &conv_kel_1, &conv_stride_1, &bias_1, VALID, &output_1);
    //// Pool-1, 池化层1 k=2, Max, s=2
    POOLSIZE pool_size_2;
    pool_size_2.row = 2;
    pool_size_2.col = 2;
    POOLSTRIDE pool_stride_2;
    pool_stride_2.row = 2;
    pool_stride_2.col = 2;

    MaxPooling2D(&output_1, &pool_size_2, &pool_stride_2, VALID, &output_2);
    //// Conv-2, 卷积层2 k=5, o=6, s=1

    CONVSTRIDE conv_stride_3;
    conv_stride_3.row = 1;  // 行步长
    conv_stride_3.col = 1;  // 列步长

    Conv2(&output_2, &conv_kel_3, &conv_stride_3, &bias_3, VALID, &output_3);
    //// Pool-2, 池化层2, k=2, Max, s=2
    POOLSIZE pool_size_4;
    pool_size_4.row = 2;
    pool_size_4.col = 2;
    POOLSTRIDE pool_stride_4;
    pool_stride_4.row = 2;
    pool_stride_4.col = 2;

    MaxPooling2D(&output_3, &pool_size_4, &pool_stride_4, VALID, &output_4);
    //// Flatten

    TensorFlatten(&output_4, &output_5);
    //// Full-1, o=400

    FullConnected(&output_5, &weight_6, &bias_6, &output_6);
    //// Activation, tanh
    //TANH(&output_6);
    RELU(&output_6);
    //// Full-2, o=120

    FullConnected(&output_6, &weight_7, &bias_7, &output_7);
    //// Activation, tanh
    //TANH(&output_7);
    RELU(&output_7);
    //// Full-3, o=84

    FullConnected(&output_7, &weight_8, &bias_8, &output_8);
    //// Softmax
    SOFTMAX(&output_8);

    for (int i = 0; i < 1; i++)
    {
        //printf("个数 %d ============================================\n", i);
        for (int j = 0; j < 1; j++)
        {
            //printf("通道 %d ------------------------------------------\n", j);
            for (int k = 0; k < 1; k++)
            {
                for (int h = 0; h < 10; h++)
                {
                    printf("数字%d的概率:%f \n", h, output_8.data[i][j][k][h]);
                }
                printf("\n");
            }
            //printf("\n\n");
        }
        // printf("\n\n\n");
    }

}
