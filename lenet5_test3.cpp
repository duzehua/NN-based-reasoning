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
#include "GetFileNames.h"
//#include "load_data.h"
#include "decodepng.h"
#include "decodeImage_CV.h"

/**************************�������������**************************/
int main()
{
	clock_t clock_start, clock_end;
	clock_start = clock();


	//// ����ػ�����
	POOLSTRIDE pool_stride;
	pool_stride.row = 2;  // �в���
	pool_stride.col = 2;  // �в���
	//// ����ػ��ߴ�
	POOLSIZE pool_size;
	pool_size.row = 2;  // �ػ�������
	pool_size.col = 2;  // �ػ�������

	const char* biasparm_pre_path = "./layer_para/bias";
	const char* weightparm_pre_path = "./layer_para/weight";
	const char* layeroutput_pre_path = "./layeroutput_data";

	//const char* pngfile = "./test_images/padpic_6_23534.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_3_57585.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_4_54348.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_2_7140.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_2_11796.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_0_15066.png";    //Ԥ����ȷ����
	//const char* pngfile = "./test_images/padpic_9_13429.png";    //Ԥ���������1
	const char* pngfile = "./test_images/padpic_7_39306.png";  //Ԥ���������2

	// ʹ��string���ͻ�ȡ�ļ�·���ķ���
	// �Ȼ�ȡ�ļ���Ŀ
	int bias_filesNum, weight_filesNum, layer_filesNum;
	bias_filesNum = getfileNumfromPath(biasparm_pre_path);
	weight_filesNum = getfileNumfromPath(weightparm_pre_path);
	layer_filesNum = getfileNumfromPath(layeroutput_pre_path);
	//printf("bias_filesNum: %d\n", bias_filesNum);
	// ����string���飬sr��ʾstring
	string* bias_filenames_sr = new string[bias_filesNum];
	string* weight_filenames_sr = new string[weight_filesNum];
	string* layer_filenames_sr = new string[layer_filesNum];
	//// ��ö�Ӧ�ļ����µ������ļ���ȫ·��
	getfileNamesfromPath(biasparm_pre_path, bias_filenames_sr);
	getfileNamesfromPath(weightparm_pre_path, weight_filenames_sr);
	getfileNamesfromPath(layeroutput_pre_path, layer_filenames_sr);

	////��stringתΪconst char* �����������غ�����cr��ʾchar
	const char** bias_filepath_cr = new const char* [bias_filesNum];
	const char** weight_filepath_cr = new const char* [weight_filesNum];
	const char** layer_filepath_cr = new const char* [layer_filesNum];

	Tensor* bias_ts = new Tensor[bias_filesNum];
	Tensor* weight_ts = new Tensor[weight_filesNum];
	Tensor* layeroutput_ts = new Tensor[layer_filesNum];
	Tensor* transpose_weight_ts = new Tensor[3];

	for (int i = 0; i < bias_filesNum; i++)
	{
		bias_filepath_cr[i] = bias_filenames_sr[i].c_str();
		//printf("bias_filepath[%d]: %s\n", i, bias_filepath_cr[i]);
		LayerParamLoadFromCSV(bias_filepath_cr[i], &bias_ts[i]);
	}
	for (int j = 0; j < weight_filesNum; j++)
	{
		weight_filepath_cr[j] = weight_filenames_sr[j].c_str();
		LayerParamLoadFromCSV(weight_filepath_cr[j], &weight_ts[j]);
	}
	for (int k = 0; k < layer_filesNum; k++)
	{
		layer_filepath_cr[k] = layer_filenames_sr[k].c_str();
	}
	//// ����һ��shape��ά�������Ÿ��������shape��-1����Ϊʵ��ֻ��Ҫ8��tensor��layoutput_data����9���ļ�
	int** layerout_ts_shape = new int* [layer_filesNum - 1];
	for (int i = 0; i < layer_filesNum - 1; i++)
		layerout_ts_shape[i] = new int[4];

	for (int i = 0; i < layer_filesNum - 1; i++)
	{
		ExtractShapeFromCharForTensor(layer_filepath_cr[i], layerout_ts_shape[i]);
		//for (int j = 0; j < 4; j++)
		//{
		//	printf("layout_ts_shape[%d]:%d ", i, layerout_ts_shape[i][j]);
		//}
		//printf("\n");
		TensorInitial(&layeroutput_ts[i], TENSOR4_STR, layerout_ts_shape[i]);
	}

	Tensor input_0;

	//ʹ��decodepng.cpp����png����������xlocale���д���
	//getPixeldata(pngfile, &input_0);

	//ʹ��opencv�����ͼƬ����
	getPixeldata_CV(pngfile, 0, &input_0);

	MinMaxNormalization(&input_0, 0, 255, 0, 1);

	TensorTranspose2D(&weight_ts[2], &transpose_weight_ts[0]);  //Ȩ�ؾ���Ҫת��

	TensorTranspose2D(&weight_ts[3], &transpose_weight_ts[1]);  //Ȩ�ؾ���Ҫת��

	TensorTranspose2D(&weight_ts[4], &transpose_weight_ts[2]);  //Ȩ�ؾ���Ҫת��


	// ����ṹ����
	//// Conv-1, �����1 k=5, o=6, s=1

	CONVSTRIDE conv_stride_1;
	conv_stride_1.row = 1;  // �в���
	conv_stride_1.col = 1;  // �в���

	Conv2(&input_0, &weight_ts[0], &conv_stride_1, &bias_ts[0], VALID, &layeroutput_ts[0]);
	//// Pool-1, �ػ���1 k=2, Max, s=2
	POOLSIZE pool_size_2;
	pool_size_2.row = 2;
	pool_size_2.col = 2;
	POOLSTRIDE pool_stride_2;
	pool_stride_2.row = 2;
	pool_stride_2.col = 2;
	RELU(&layeroutput_ts[0]);

	MaxPooling2D(&layeroutput_ts[0], &pool_size_2, &pool_stride_2, VALID, &layeroutput_ts[1]);
	//// Conv-2, �����2 k=5, o=6, s=1

	CONVSTRIDE conv_stride_3;
	conv_stride_3.row = 1;  // �в���
	conv_stride_3.col = 1;  // �в���

	Conv2(&layeroutput_ts[1], &weight_ts[1], &conv_stride_3, &bias_ts[1], VALID, &layeroutput_ts[2]);
	//// Pool-2, �ػ���2, k=2, Max, s=2
	POOLSIZE pool_size_4;
	pool_size_4.row = 2;
	pool_size_4.col = 2;
	POOLSTRIDE pool_stride_4;
	pool_stride_4.row = 2;
	pool_stride_4.col = 2;
	RELU(&layeroutput_ts[2]);

	MaxPooling2D(&layeroutput_ts[2], &pool_size_4, &pool_stride_4, VALID, &layeroutput_ts[3]);
	//// Flatten

	TensorFlatten(&layeroutput_ts[3], &layeroutput_ts[4]);
	//// Full-1, o=400

	FullConnected(&layeroutput_ts[4], &transpose_weight_ts[0], &bias_ts[2], &layeroutput_ts[5]);
	//// Activation, Relu
	RELU(&layeroutput_ts[5]);
	//// Full-2, o=120

	FullConnected(&layeroutput_ts[5], &transpose_weight_ts[1], &bias_ts[3], &layeroutput_ts[6]);
	//// Activation, Relu
	RELU(&layeroutput_ts[6]);
	//// Full-3, o=84

	FullConnected(&layeroutput_ts[6], &transpose_weight_ts[2], &bias_ts[4], &layeroutput_ts[7]);
	//// Softmax
	SOFTMAX(&layeroutput_ts[7]);

	for (int i = 0; i < 1; i++)
	{
		//printf("���� %d ============================================\n", i);
		for (int j = 0; j < 1; j++)
		{
			//printf("ͨ�� %d ------------------------------------------\n", j);
			for (int k = 0; k < 1; k++)
			{
				for (int h = 0; h < 10; h++)
				{
					printf("����%d�ĸ���:%f \n", h, layeroutput_ts[7].data[i][j][k][h]);
				}
				printf("\n");
			}
			//printf("\n\n");
		}
		// printf("\n\n\n");
	}



	// �ռ��ͷ�
	delete[] bias_filenames_sr;
	delete[] weight_filenames_sr;
	delete[] layer_filenames_sr;

	delete[] bias_filepath_cr;
	delete[] weight_filepath_cr;
	delete[] layer_filepath_cr;

	for (int i = 0; i < layer_filesNum - 1; i++)
		delete[] layerout_ts_shape[i];
	delete[]layerout_ts_shape;

	for (int i = 0; i < layer_filesNum - 1; i++)  // -1����Ϊʵ��ֻ��Ҫ8��tensor��layoutput_data����9���ļ�
	{
		TensorFree(&layeroutput_ts[i]);
	}

	for (int i = 0; i < bias_filesNum; i++)
	{
		TensorFree(&bias_ts[i]);
	}

	for (int j = 0; j < weight_filesNum; j++)
	{
		TensorFree(&weight_ts[j]);
	}

	for (int k = 0; k < 3; k++)
	{
		TensorFree(&transpose_weight_ts[k]);
	}


	clock_end = clock();
	// printf("time = %f\n",(double)(clock_end - clock_start));
	printf("time = %f s\n", (double)(clock_end - clock_start) / CLK_TCK);
}
