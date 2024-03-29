#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* 使用OpenCV库进行图片解析
* 说明: 获得图片的像素数据矩阵
* @param[in]  path  -> 图片路径
* @param[in]  flag -> 灰度解析（flag=0）或者全彩解析（flag=1）
* @param[out] pixel_ts -> 像素数据Tensor
*/
void getPixeldata_CV(const char* path, int flag, Tensor* pixel_ts)
{
	cv::Mat image;
	image = cv::imread(path, flag); //flag=0, 读取灰度图片
	if (image.data == nullptr)//nullptr是c++11新出现的空指针常量
	{
		cerr << "图片文件不存在" << endl;
		return;
	}

	int tsnum, chnum, row, col;
	tsnum = 1;  // one tensor
	// flag不同，chnum不同
	if (flag == 0) chnum = 1;
	else if (flag == 1) chnum = 3;
	else chnum = 1;
	row = image.rows;
	col = image.cols;
	TENSORSHAPE datashape[4] = { tsnum, chnum, row, col };

	TensorInitial(pixel_ts, TENSOR4_STR, datashape);
	if (flag == 0)
	{
		for (int i1 = 0; i1 < tsnum; i1++)
		{
			for (int i2 = 0; i2 < chnum; i2++)
			{
				//std::cout << std::endl;
				//std::cout << std::endl;
				for (int i3 = 0; i3 < image.rows; i3++)
				{
					//std::cout << std::endl;
					for (int i4 = 0; i4 < image.cols; i4++)
					{
						pixel_ts->data[i1][i2][i3][i4] = image.at<uchar>(i3, i4);    // 使用at<uchar>(i,j)方法获得某个像素点数据
						//std::cout << std::setw(3) << pixel_ts->data[i1][i2][i3][i4] << " ";
					}
				}
			}
		}
	}
	// TODO: 多通道数据读取待确认
	else
	{
		for (int i1 = 0; i1 < tsnum; i1++)
		{
			for (int i2 = 0; i2 < chnum; i2++)
			{
				//std::cout << std::endl;
				//std::cout << std::endl;
				for (int i3 = 0; i3 < image.rows; i3++)
				{
					//std::cout << std::endl;
					for (int i4 = 0; i4 < image.cols; i4++)
					{
						pixel_ts->data[i1][i2][i3][i4] = image.at<cv::Vec3b>(i3, i4)[i2];  // 使用at<cv::Vec3b>(i,j)[k]方法获得全彩图像某个像素点数据（k表示R/G/B）
						//std::cout << std::setw(3) << pixel_ts->data[i1][i2][i3][i4] << " ";
					}
				}
			}
		}
	}

	image.release();
}


// 测试代码
//int main(int argc, char** argv) {
//	cv::Mat image;
//	int flag = 1;
//	image = cv::imread("./test_images/padpic_6_23534.png", flag);
//	if (image.data == nullptr)//nullptr是c++11新出现的空指针常量
//	{
//		cerr << "图片文件不存在" << endl;
//		return 0;
//	}
//	//else
//		//cout << image << endl;//你会发现图片就是一个矩阵
//
//	int tsnum, chnum, row, col;
//	tsnum = 1;  // one tensor
//	if (flag == 0) chnum = 1;
//	else chnum = 3;  // three channels
//	row = image.rows;
//	col = image.cols;
//
//	int datashape[4] = { tsnum,chnum,row,col };
//	Tensor data;
//	TensorInitial(&data, TENSOR4_STR, datashape);
//
//	//for (int i1 = 0; i1 < 1; i1++)
//	//{
//	//	for (int i2 = 0; i2 < 1; i2++)
//	//	{
//	//		std::cout << std::endl;
//	//		std::cout << std::endl;
//	//		for (int i3 = 0; i3 < image.rows; i3++)
//	//		{
//	//			std::cout << std::endl;
//	//			for (int i4 = 0; i4 < image.cols; i4++)
//	//			{
//	//				data.data[i1][i2][i3][i4] = image.at<uchar>(i3, i4);
//	//				std::cout << std::setw(3) << data.data[i1][i2][i3][i4] << " ";
//	//			}
//	//		}
//	//	}
//	//}
//
//	for (int i1 = 0; i1 < tsnum; i1++)
//	{
//		for (int i2 = 0; i2 < chnum; i2++)
//		{
//			std::cout << std::endl;
//			std::cout << std::endl;
//			for (int i3 = 0; i3 < image.rows; i3++)
//			{
//				std::cout << std::endl;
//				for (int i4 = 0; i4 < image.cols; i4++)
//				{
//					data.data[i1][i2][i3][i4] = image.at<cv::Vec3b>(i3, i4)[i2];
//					std::cout << std::setw(3) << data.data[i1][i2][i3][i4] << " ";
//				}
//			}
//		}
//	}
//
//	return 0;
//}
