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
#include "DataDefinition.h"
#include "GeneralDataType.h"
#include "StringOperation.h"
#include "LayerParamLoad.h"
//#include "GetFileNames.h"
//#include "load_data.h"
#include "decodepng.h"

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

	const char* parm_pre_path = "./layer_para";
	char layerout_pre_path[] = "./layerout_data";

	const char* pngfile = "./test_images/padpic_6_23534.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_9_13429.png";    //预测错误例子1
	//const char* pngfile = "./test_images/padpic_7_39306.png";  //预测错误例子2

	// 使用string类型获取文件路径的方法
	//string filenames[10];
	//getfileNamesfromPath(parm_pre_path, filenames);
	////for (int i = 0; i < 10; i++)
	////{
	////	std::cout << "filenames: " << filenames[i] << "\n";
	////}
	//// 注意filenames的下标（与文件命名以及读取顺序相关）
	//const char* cp_conv_kel_1 = filenames[0].c_str();
	//const char* cp_conv_bias_1 = filenames[1].c_str();
	//const char* cp_conv_kel_3 = filenames[2].c_str();
	//const char* cp_conv_bias_3 = filenames[3].c_str();
	//const char* cp_fcweight_6 = filenames[4].c_str();
	//const char* cp_f_fcbias_6 = filenames[5].c_str();
	//const char* cp_fcweight_7 = filenames[6].c_str();
	//const char* cp_f_fcbias_7 = filenames[7].c_str();
	//const char* cp_fcweight_8 = filenames[8].c_str();
	//const char* cp_f_fcbias_8 = filenames[9].c_str();

	//printf("cp_conv_kel_1: %s \n ", cp_conv_kel_1);
	//printf("cp_conv_kel_3: %s \n", cp_conv_kel_3);
	//printf("cp_conv_bias_1: %s \n", cp_conv_bias_1);
	//printf("cp_conv_bias_3: %s \n", cp_conv_bias_3);
	//printf("cp_f_fcbias_8: %s \n", cp_f_fcbias_8);

	// 直接定义文件名称的方法（兼容性好）
	char f_conv_kel_1[] = "14_117_0_6_1_5_5_block_1.0.weight.csv";
	char f_conv_kel_3[] = "14_117_2_16_6_5_5_block_1.3.weight.csv";
	char f_conv_bias_1[] = "14_117_1_0_0_0_6_block_1.0.bias.csv";
	char f_conv_bias_3[] = "14_117_3_0_0_0_16_block_1.3.bias.csv";
	char f_fcweight_6[] = "14_117_4_0_0_120_400_block_2.0.weight.csv";
	char f_fcweight_7[] = "14_117_6_0_0_84_120_block_2.2.weight.csv";
	char f_fcweight_8[] = "14_117_8_0_0_10_84_block_2.4.weight.csv";
	char f_fcbias_6[] = "14_117_5_0_0_0_120_block_2.0.bias.csv";
	char f_fcbias_7[] = "14_117_7_0_0_0_84_block_2.2.bias.csv";
	char f_fcbias_8[] = "14_117_9_0_0_0_10_block_2.4.bias.csv";

	// combine path
	char* cp_conv_kel_1 = OSPathJoin(parm_pre_path, f_conv_kel_1);
	char* cp_conv_kel_3 = OSPathJoin(parm_pre_path, f_conv_kel_3);
	char* cp_conv_bias_1 = OSPathJoin(parm_pre_path, f_conv_bias_1);
	char* cp_conv_bias_3 = OSPathJoin(parm_pre_path, f_conv_bias_3);
	char* cp_fcweight_6 = OSPathJoin(parm_pre_path, f_fcweight_6);
	char* cp_fcweight_7 = OSPathJoin(parm_pre_path, f_fcweight_7);
	char* cp_fcweight_8 = OSPathJoin(parm_pre_path, f_fcweight_8);
	char* cp_f_fcbias_6 = OSPathJoin(parm_pre_path, f_fcbias_6);
	char* cp_f_fcbias_7 = OSPathJoin(parm_pre_path, f_fcbias_7);
	char* cp_f_fcbias_8 = OSPathJoin(parm_pre_path, f_fcbias_8);

	TENSORSHAPE input_0_shape[4] = { 1,1,32,32 };
	//TENSORSHAPE conv_kel_1_shape[4] = { 6,1,5,5 };
	TENSORSHAPE output_1_shape[4] = { 1,6,28,28 };
	TENSORSHAPE output_2_shape[4] = { 1,6,14,14 };
	//TENSORSHAPE conv_kel_3_shape[4] = { 16,6,5,5 };
	TENSORSHAPE output_3_shape[4] = { 1,16,10,10 };
	TENSORSHAPE output_4_shape[4] = { 1,16,5,5 };

	TENSORSHAPE output_5_shape[4] = { 1,1,1,400 };
	TENSORSHAPE output_6_shape[4] = { 1,1,1,120 };
	TENSORSHAPE output_7_shape[4] = { 1,1,1,84 };
	TENSORSHAPE output_8_shape[4] = { 1,1,1,10 };

	//TENSORSHAPE bias_1_shape[4] = { 1,1,1,6 };
	//TENSORSHAPE bias_3_shape[4] = { 1,1,1,16 };
	//TENSORSHAPE bias_6_shape[4] = { 1,1,1,120 };
	//TENSORSHAPE bias_7_shape[4] = { 1,1,1,84 };
	//TENSORSHAPE bias_8_shape[4] = { 1,1,1,10 };


	//TENSORSHAPE weight_6_shape[4] = { 1,1,120,400 };
	//TENSORSHAPE weight_7_shape[4] = { 1,1,84,120 };
	//TENSORSHAPE weight_8_shape[4] = { 1,1,10,84 };
	//TENSORSHAPE weight_6_transhape[4] = { 1,1,400,120 };
	//TENSORSHAPE weight_7_transhape[4] = { 1,1,120,84 };
	//TENSORSHAPE weight_8_transhape[4] = { 1,1,84,10 };





	Tensor input_0;
	Tensor output_1;
	Tensor output_2;
	Tensor output_3;
	Tensor output_4;
	Tensor output_5;
	Tensor output_6;
	Tensor output_7;
	Tensor output_8;

	Tensor conv_kel_1;
	Tensor conv_kel_3;
	Tensor conv_bias_1;
	Tensor conv_bias_3;
	Tensor fc_weight_6;
	Tensor fc_weight_6_trans;
	Tensor fc_bias_6;
	Tensor fc_weight_7;
	Tensor fc_weight_7_trans;
	Tensor fc_bias_7;
	Tensor fc_weight_8;
	Tensor fc_weight_8_trans;
	Tensor fc_bias_8;

	TensorInitial(&input_0, TENSOR4_STR, input_0_shape);
	getPixeldata(pngfile, &input_0);
	MinMaxNormalization(&input_0, 0, 255, 0, 1);

	TensorInitial(&output_1, TENSOR4_STR, output_1_shape);
	TensorInitial(&output_2, TENSOR4_STR, output_2_shape);
	TensorInitial(&output_3, TENSOR4_STR, output_3_shape);
	TensorInitial(&output_4, TENSOR4_STR, output_4_shape);
	TensorInitial(&output_5, TENSOR4_STR, output_5_shape);
	TensorInitial(&output_6, TENSOR4_STR, output_6_shape);
	TensorInitial(&output_7, TENSOR4_STR, output_7_shape);
	TensorInitial(&output_8, TENSOR4_STR, output_8_shape);

	LayerParamLoadFromCSV(cp_conv_kel_1, &conv_kel_1);
	LayerParamLoadFromCSV(cp_conv_kel_3, &conv_kel_3);
	LayerParamLoadFromCSV(cp_conv_bias_1, &conv_bias_1);
	LayerParamLoadFromCSV(cp_conv_bias_3, &conv_bias_3);

	LayerParamLoadFromCSV(cp_fcweight_6, &fc_weight_6);
	TensorTranspose2D(&fc_weight_6, &fc_weight_6_trans);  //权重矩阵要转置
	LayerParamLoadFromCSV(cp_fcweight_7, &fc_weight_7);
	TensorTranspose2D(&fc_weight_7, &fc_weight_7_trans);  //权重矩阵要转置
	LayerParamLoadFromCSV(cp_fcweight_8, &fc_weight_8);
	TensorTranspose2D(&fc_weight_8, &fc_weight_8_trans);  //权重矩阵要转置

	LayerParamLoadFromCSV(cp_f_fcbias_6, &fc_bias_6);
	LayerParamLoadFromCSV(cp_f_fcbias_7, &fc_bias_7);
	LayerParamLoadFromCSV(cp_f_fcbias_8, &fc_bias_8);


	// 网络结构定义
	//// Conv-1, 卷积层1 k=5, o=6, s=1

	CONVSTRIDE conv_stride_1;
	conv_stride_1.row = 1;  // 行步长
	conv_stride_1.col = 1;  // 列步长

	Conv2(&input_0, &conv_kel_1, &conv_stride_1, &conv_bias_1, VALID, &output_1);
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

	Conv2(&output_2, &conv_kel_3, &conv_stride_3, &conv_bias_3, VALID, &output_3);
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

	FullConnected(&output_5, &fc_weight_6_trans, &fc_bias_6, &output_6);
	//// Activation, tanh
	//TANH(&output_6);
	RELU(&output_6);
	//// Full-2, o=120

	FullConnected(&output_6, &fc_weight_7_trans, &fc_bias_7, &output_7);
	//// Activation, tanh
	//TANH(&output_7);
	RELU(&output_7);
	//// Full-3, o=84

	FullConnected(&output_7, &fc_weight_8_trans, &fc_bias_8, &output_8);
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

	TensorFree(&input_0);
	TensorFree(&output_1);
	TensorFree(&output_2);
	TensorFree(&output_3);
	TensorFree(&output_4);
	TensorFree(&output_5);
	TensorFree(&output_6);
	TensorFree(&output_7);
	TensorFree(&output_8);
	TensorFree(&conv_kel_1);
	TensorFree(&conv_kel_3);
	TensorFree(&fc_weight_6);
	TensorFree(&fc_weight_7);
	TensorFree(&fc_weight_8);
	TensorFree(&fc_weight_6_trans);
	TensorFree(&fc_weight_7_trans);
	TensorFree(&fc_weight_8_trans);
	TensorFree(&fc_bias_6);
	TensorFree(&fc_bias_7);
	TensorFree(&fc_bias_8);
	TensorFree(&conv_bias_1);
	TensorFree(&conv_bias_3);



	clock_end = clock();
	// printf("time = %f\n",(double)(clock_end - clock_start));
	printf("time = %f s\n", (double)(clock_end - clock_start) / CLK_TCK);
}
