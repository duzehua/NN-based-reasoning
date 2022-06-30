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
#include <malloc.h>

#include "GINFO.h"
#include "GINFO_VAR.h"
#include "Tensor.h"
#include "ConvOperation.h"
#include "ActivationFunction.h"
#include "PoolingOperation.h"
#include "FullyConnectedFunction.h"
#include "LayerParamLoad.h"
#include "StringOperation.h"
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
    //// 定义张量文件信息
    int epoch = 0;
    int step  = 0;
    int layer = 0;
    int tensor_info[4] = {32, 1, 3, 3};
    char parm_pre_path[] = "C:/ManQi/DataStore/Python/Mnist_Pytorch/layer_result/";

    Tensor input_0;

    // 网络结构定义
    //// Conv-1, 卷积层1 k=5, o=6, s=1
    Tensor conv_kel_1;
    Tensor bias_1;
    Tensor output_1;
    if (1)  // 卷积核 conv_kel_1 导入
    {
        char f_conv1_weight[] = "0_0_1_32_1_3_3_conv1.weight.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_conv1_weight);
        LayerParamLoadFromCSV(combined_parm_path, &conv_kel_1);
        ViewTensor(&conv_kel_1);
    }
    if (1)  // 偏置 bias_1 导入
    {
        char f_bias1_weight[] = "0_0_2_0_0_0_32_conv1.bias.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_bias1_weight);
        LayerParamLoadFromCSV(combined_parm_path, &bias_1);
        ViewTensor(&bias_1);
    }
    if (1)  // 卷积基本参数
    {
        CONVSTRIDE conv_stride_1;
        conv_stride_1.row = 1;  // 行步长
        conv_stride_1.col = 1;  // 列步长
    }
    // Conv2(&input_0, &conv_kel_1, &conv_stride_1, &bias_1, VALID, &output_1);
    // //// Activation, RELU
    // RELU(&output_1);
    //// Pool-1, 池化层1 k=2, Max, s=2
    // POOLSIZE pool_size_2;
    // pool_size_2.row = 2;
    // pool_size_2.col = 2;
    // POOLSTRIDE pool_stride_2;
    // pool_stride_2.row = 2;
    // pool_stride_2.col = 2;
    // Tensor output_2;
    // MaxPooling2D(&output_1, &pool_size_2, &pool_stride_2, VALID, &output_2);

    // //// Flatten
    // Tensor output_3;
    // TensorFlatten(&output_2, &output_3);
    //// Full-1, o=120
    Tensor weight_4;
    Tensor bias_4;
    Tensor output_4;
    if (1)  // 权重 weight_4 导入
    {
        char f_weight_4[] = "0_0_3_0_0_128_5408_fc1.weight.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_weight_4);
        LayerParamLoadFromCSV(combined_parm_path, &weight_4);
        ViewTensor(&weight_4);
    }
    if (1)  // 权重 bias_4 导入
    {
        char f_bias_4[] = "0_0_4_0_0_0_128_fc1.bias.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_bias_4);
        LayerParamLoadFromCSV(combined_parm_path, &bias_4);
        ViewTensor(&bias_4);
    }
    // FullConnected(&output_3, &weight_4, &bias_4, &output_4);
    // //// Activation, RELU
    // RELU(&output_4);

    //// Full-2, o=84
    Tensor weight_5;
    Tensor bias_5;
    Tensor output_5;
    if (1)  // 权重 weight_5 导入
    {
        char f_weight_5[] = "0_0_5_0_0_10_128_fc2.weight.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_weight_5);
        LayerParamLoadFromCSV(combined_parm_path, &weight_5);
        ViewTensor(&weight_5);
    }
    if (1)  // 权重 bias_5 导入
    {
        char f_bias_5[] = "0_0_6_0_0_0_10_fc2.bias.csv";
        char *combined_parm_path = OSPathJoin(parm_pre_path, f_bias_5);
        LayerParamLoadFromCSV(combined_parm_path, &bias_5);
        ViewTensor(&bias_5);
    }
    // FullConnected(&output_4, &weight_5, &bias_5, &output_5);
    // //// Softmax
    // SOFTMAX(&output_5);

    clock_end = clock();
    // printf("time = %f\n",(double)(clock_end - clock_start));
    printf("time = %f s\n",(double)(clock_end - clock_start)/CLK_TCK);
}
