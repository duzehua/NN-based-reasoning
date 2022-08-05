#include "mex.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* 
接口函数是C语言和matlab的接口，或者理解成协议，桥梁也可以，它接受4个参数：
nlhs是左手参数个数，
plhs是左手参数数组其内容为指针，该指针指向数据类型为mxArray的变量。
相对应的nrhs和prhs。

编写的**C-Mex**文件应该保存为.c文件,文件类型是**all file**，
尤其要强调的一点是，该文件的文件名要和函数的函数名一致，不然使用的时候会报**undefined function**的错误

}
*/

void mexFunction(int nlhs,       mxArray *plhs[], 
                 int nrhs, const mxArray *prhs[])
//                   左个数         左指针      右个数               右指针
//                   左 输出                    右 输入
{
	// 右代表输入，图片数据（1维数据）， 尺寸[1, 通道, 长, 宽]
	
	if (nrhs != 2)
	{
		mexErrMsgIdAndTxt("matlab_image_cmex:nrhs", "Two inputs are required.");
	}

	clock_t clock_start, clock_end;
	clock_start = clock();
	
	/* rename the pointers */
	#define IMG_DATA  prhs[0]
	#define IMG_SHAPE prhs[1]
	
	/* define pointers */
	float *img_data_ptr;
	int   *img_shape_ptr;
	
	/* obtain the pointers value */
	img_data_ptr  = (float *)mxGetData(IMG_DATA);  // 非double类型用mxGetData,图片拉伸成一维数组，需要还原
	img_shape_ptr =   (int *)mxGetData(IMG_SHAPE);  // 图片尺寸，可结合利用已有函数进行图片数据的还原
	
	// 保存成txt，通过matlab进行验证
	int len_data = img_shape_ptr[0] * img_shape_ptr[1] * img_shape_ptr[2] * img_shape_ptr[3];
    // for (int i = 0; i < 4; i++)
    // {
    //     mexPrintf("data shape[%d]: %d  ", i, img_shape_ptr[i]);
    // }
	FILE *p;
    p = fopen("./test.txt","w");
    // mexPrintf("\nRaw Data: \n");
    for (int i = 0; i < len_data; i++)
    {
        fprintf(p,"%f, ", img_data_ptr[i]);
        if((i+1) % 32 == 0) mexPrintf("\n");
        // mexPrintf("%f   ", img_data_ptr[i]);
    }
    fclose(p);
	
	// 后续补全模型的输出，通过plhs回传给matlab
	Tensor input_0;
	TENSORSHAPE input_0_shape[4] = { img_shape_ptr[0],img_shape_ptr[1],img_shape_ptr[2],img_shape_ptr[3] };
	TensorInitial(&input_0,TENSOR4_STR, input_0_shape);
	TensorSet(&input_0, img_data_ptr, len_data);


	//// 定义池化步长
	POOLSTRIDE pool_stride;
	pool_stride.row = 2;  // 行步长
	pool_stride.col = 2;  // 列步长
	//// 定义池化尺寸
	POOLSIZE pool_size;
	pool_size.row = 2;  // 池化核行数
	pool_size.col = 2;  // 池化核列数

	//const char* biasparm_pre_path = "./0711para/CPU/layer_para/bias";
	//const char* weightparm_pre_path = "./0711para/CPU/layer_para/weight";
	//const char* layeroutput_pre_path = "./0711para/CPU/layeroutput_data";
	const char* biasparm_pre_path = "./0711para/GPU/layer_para/bias";
	const char* weightparm_pre_path = "./0711para/GPU/layer_para/weight";
	const char* layeroutput_pre_path = "./0711para/GPU/layeroutput_data";

	//const char* pngfile = "./test_images/padpic_6_23534.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_3_57585.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_4_54348.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_2_7140.png";    //预测正确例子
	// const char* pngfile = "./test_images/padpic_2_11796.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_0_15066.png";    //预测正确例子
	//const char* pngfile = "./test_images/padpic_9_13429.png";    //预测错误例子1
	//const char* pngfile = "./test_images/padpic_7_39306.png";  //预测错误例子2

	// 使用string类型获取文件路径的方法
	// 先获取文件数目
	int bias_filesNum, weight_filesNum, layer_filesNum;
	bias_filesNum = getfileNumfromPath(biasparm_pre_path);
	weight_filesNum = getfileNumfromPath(weightparm_pre_path);
	layer_filesNum = getfileNumfromPath(layeroutput_pre_path);
	//printf("bias_filesNum: %d\n", bias_filesNum);
	// 申请string数组，sr表示string
	string* bias_filenames_sr = new string[bias_filesNum];
	string* weight_filenames_sr = new string[weight_filesNum];
	string* layer_filenames_sr = new string[layer_filesNum];
	//// 获得对应文件夹下的所有文件的全路径
	getfileNamesfromPath(biasparm_pre_path, bias_filenames_sr);
	getfileNamesfromPath(weightparm_pre_path, weight_filenames_sr);
	getfileNamesfromPath(layeroutput_pre_path, layer_filenames_sr);

	////将string转为const char* 传给参数加载函数，cr表示char
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
	//// 申请一个shape二维数组来放各层输出的shape，-1是因为实际只需要8个tensor而layoutput_data下有9个文件
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

	// Tensor input_0;

	// //使用decodepng.cpp进行png解析，会有xlocale运行错误
	// //getPixeldata(pngfile, &input_0);

	// //使用opencv库进行图片解析
	// getPixeldata_CV(pngfile, 0, &input_0);

    MinMaxNormalization(&input_0, 0, 255, 0, 1);  

	TensorTranspose2D(&weight_ts[2], &transpose_weight_ts[0]);  //权重矩阵要转置

	TensorTranspose2D(&weight_ts[3], &transpose_weight_ts[1]);  //权重矩阵要转置

	TensorTranspose2D(&weight_ts[4], &transpose_weight_ts[2]);  //权重矩阵要转置


	// 网络结构定义
	//// Conv-1, 卷积层1 k=5, o=6, s=1

	CONVSTRIDE conv_stride_1;
	conv_stride_1.row = 1;  // 行步长
	conv_stride_1.col = 1;  // 列步长

	Conv2(&input_0, &weight_ts[0], &conv_stride_1, &bias_ts[0], VALID, &layeroutput_ts[0]);
	//// Pool-1, 池化层1 k=2, Max, s=2
	POOLSIZE pool_size_2;
	pool_size_2.row = 2;
	pool_size_2.col = 2;
	POOLSTRIDE pool_stride_2;
	pool_stride_2.row = 2;
	pool_stride_2.col = 2;
	RELU(&layeroutput_ts[0]);

	MaxPooling2D(&layeroutput_ts[0], &pool_size_2, &pool_stride_2, VALID, &layeroutput_ts[1]);
	//// Conv-2, 卷积层2 k=5, o=6, s=1

	CONVSTRIDE conv_stride_3;
	conv_stride_3.row = 1;  // 行步长
	conv_stride_3.col = 1;  // 列步长

	Conv2(&layeroutput_ts[1], &weight_ts[1], &conv_stride_3, &bias_ts[1], VALID, &layeroutput_ts[2]);
	//// Pool-2, 池化层2, k=2, Max, s=2
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
		//printf("个数 %d ============================================\n", i);
		for (int j = 0; j < 1; j++)
		{
			//printf("通道 %d ------------------------------------------\n", j);
			for (int k = 0; k < 1; k++)
			{
				for (int h = 0; h < 10; h++)
				{
					mexPrintf("数字%d的概率:%f \n", h, layeroutput_ts[7].data[i][j][k][h]);
				}
				mexPrintf("\n");
			}
			//printf("\n\n");
		}
		// printf("\n\n\n");
	}



	// 空间释放
	delete[] bias_filenames_sr;
	delete[] weight_filenames_sr;
	delete[] layer_filenames_sr;

	delete[] bias_filepath_cr;
	delete[] weight_filepath_cr;
	delete[] layer_filepath_cr;

	for (int i = 0; i < layer_filesNum - 1; i++)
		delete[] layerout_ts_shape[i];
	delete[]layerout_ts_shape;

	for (int i = 0; i < layer_filesNum - 1; i++)  // -1是因为实际只需要8个tensor而layoutput_data下有9个文件
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
	mexPrintf("time = %f s\n", (double)(clock_end - clock_start) / CLK_TCK);

}