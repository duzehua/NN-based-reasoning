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
// ==========================

// #include <omp.h>

/**************************网络层描述部分**************************/
int main()
{
    clock_t clock_start, clock_end;
    clock_start = clock();

    int i, j, k, h;
    // int tsn;
    // int chn;
    // int row;
    // int col;

    // 定义卷积步长
    CONVSTRIDE conv_stride;
    conv_stride.row = 1;  // 行步长
    conv_stride.col = 1;  // 列步长

    // 定义池化尺寸
    POOLSIZE pool_size;
    pool_size.row = 2;  // 池化核行数
    pool_size.col = 2;  // 池化核列数

    // 定义池化步长
    POOLSTRIDE pool_stride;
    pool_stride.row = 2;  // 行步长
    pool_stride.col = 2;  // 列步长

    // 通用张量卷积测试
    // 被卷积张量定义
    Tensor tensor4;
    int t2_len = 384;
    // DATA t2[2][3][8][8] = {{{{-1,-2,-3,-4,-51,-52,-53,-54},     {-5,-6,-7,-8,-51,-52,-53,-54},     {-9,-10,-11,-12,-51,-52,-53,-54},  {-13,-14,-15,-16,-51,-52,-53,-54},{-1,-2,-3,-4,-51,-52,-53,-54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                             {{17,18,19,20,51,52,53,54}, {21,22,23,24,51,52,53,54}, {25,26,27,28,51,52,53,54}, {29,30,31,32,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                             {{33,34,35,36,51,52,53,54}, {37,38,39,40,51,52,53,54}, {41,42,43,44,51,52,53,54}, {45,46,47,48,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}}},

    //                         {{{51,52,53,54,51,52,53,54},      {55,56,57,58,51,52,53,54},     {59,510,511,512,51,52,53,54},  {513,514,515,516,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                         {{517,518,519,520,51,52,53,54},  {521,522,523,524,51,52,53,54}, {525,526,527,528,51,52,53,54}, {529,530,531,532,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                         {{533,534,535,536,51,52,53,54},  {537,538,539,540,51,52,53,54}, {541,542,543,544,51,52,53,54}, {545,546,547,548,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}}}};
    
    DATA t2[2][3][9][9] = {{{{-1,-2,-3,-4,-51,-52,-53,-54, 5},     {-5,-6,-7,-8,-51,-52,-53,-5, 54},     {-9,-10,-11,-12,-51,-52,-53,-5, 54},  {-13,-14,-15,-16,-51,-52,-53,-5, 54},{-1,-2,-3,-4,-51,-52,-53,-5, 54},     {-5,-6,-7,-8,-51,-52,-53,-54, 5},     {-9,-10,-11,-12,-51,-52,-53,-54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}},
                                {{-1,-2,-3,-4,-51,-52,-53,-5, 54},     {-5,-6,-7,-8,-51,-52,-53,-5, 54},     {-9,-10,-11,-12,-51,-52,-53,-5, 54},  {-13,-14,-15,-16,-51,-52,-53,-5, 54},{-1,-2,-3,-4,-51,-52,-53,-5, 54},     {-5,-6,-7,-8,-51,-52,-53,-54, 5},     {-9,-10,-11,-12,-51,-52,-53,-54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}},
                                {{-1,-2,-3,-4,-51,-52,-53,-54, 5},     {-5,-6,-7,-8,-51,-52,-53,-54, 5},     {-9,-10,-11,-12,-51,-52,-53,-5, 54},  {-13,-14,-15,-16,-51,-52,-53,-5, 54},{-1,-2,-3,-4,-51,-52,-53,-54, 5},     {-5,-6,-7,-8,-51,-52,-53,-54, 5},     {-9,-10,-11,-12,-51,-52,-53,-54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}}},

                            {{{51,52,53,54,51,52,53,54, 5},      {55,56,57,58,51,52,53,54, 5},     {59,510,511,512,51,52,53,5, 54},  {513,514,515,516,51,52,53,5, 54},{1,2,3,4,51,52,53,5, 54},     {5,6,7,8,51,52,53,5, 54},     {9,10,11,12,51,52,53,54, 5},  {13,14,15,16,51,52,53,5, 54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}},
                            {{517,518,519,520,51,52,53,5, 54},  {521,522,523,524,51,52,53,5, 54}, {525,526,527,528,51,52,53,5, 54}, {529,530,531,532,51,52,53,5, 54},{1,2,3,4,51,52,53,5, 54},     {5,6,7,8,51,52,53,5, 54},     {9,10,11,12,51,52,53,54, 5},  {13,14,15,16,51,52,53,5, 54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}},
                            {{533,534,535,536,51,52,53,5, 54},  {537,538,539,540,51,52,53,54, 5}, {541,542,543,544,51,52,53,5, 54}, {545,546,547,548,51,52,53,5, 54},{1,2,3,4,51,52,53,5, 54},     {5,6,7,8,51,52,53,5, 54},     {9,10,11,12,51,52,53,54, 5},  {13,14,15,16,51,52,53,5, 54},  {-13,-14,-15,-16,-51,-52,-53,-54, 5}}}};
    // DATA t2[2][3][8][8] = {{{{-1,-2,-3,-4,-51,-52,-53,-54},     {-5,-6,-7,-8,-51,-52,-53,-54},     {-9,-10,-11,-12,-51,-52,-53,-54},  {-13,-14,-15,-16,-51,-52,-53,-54},{-1,-2,-3,-4,-51,-52,-53,-54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                             {{17,18,19,20,51,52,53,54}, {21,22,23,24,51,52,53,54}, {25,26,27,28,51,52,53,54}, {29,30,31,32,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                             {{33,34,35,36,51,52,53,54}, {37,38,39,40,51,52,53,54}, {41,42,43,44,51,52,53,54}, {45,46,47,48,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}}},

    //                         {{{51,52,53,54,51,52,53,54},      {55,56,57,58,51,52,53,54},     {59,510,511,512,51,52,53,54},  {513,514,515,516,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                         {{517,518,519,520,51,52,53,54},  {521,522,523,524,51,52,53,54}, {525,526,527,528,51,52,53,54}, {529,530,531,532,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}},
    //                         {{533,534,535,536,51,52,53,54},  {537,538,539,540,51,52,53,54}, {541,542,543,544,51,52,53,54}, {545,546,547,548,51,52,53,54},{1,2,3,4,51,52,53,54},     {5,6,7,8,51,52,53,54},     {9,10,11,12,51,52,53,54},  {13,14,15,16,51,52,53,54}}}};
    

    TENSOR4_SHAPE[0] = 2;
    TENSOR4_SHAPE[1] = 3;
    TENSOR4_SHAPE[2] = 9;
    TENSOR4_SHAPE[3] = 9;
    printf("被卷积张量\n");
    TensorInitial(&tensor4, TENSOR4_STR, TENSOR4_SHAPE);

    // // 卷积核定义
    // Tensor conv_kel;
    // TENSOR4_SHAPE[0] = 1;
    // TENSOR4_SHAPE[1] = 5;
    // TENSOR4_SHAPE[2] = 3;
    // TENSOR4_SHAPE[3] = 3;
    // printf("卷积核张量\n");
    // TensorInitial(&conv_kel, TENSOR4_STR, TENSOR4_SHAPE);
    // DATA t_ck[1][5][3][3] = {{
    //                         {{1,2,3},     {5,6,7},     {9,10,11}},
    //                         {{17,18,19}, {21,22,23}, {25,26,27}},
    //                         {{33,34,35}, {37,38,39}, {41,42,43}},
    //                         {{17,18,19}, {21,22,23}, {25,26,27}},
    //                         {{33,34,35}, {37,38,39}, {41,42,43}}
    //                         }};  
    // // DATA t_ck[1][15][1][1] = {{ {{1}}, {{2}}, {{3}}, {{4}}, {{5}}, {{6}}, {{7}}, {{8}}, {{9}}, {{0}}, {{1}}, {{2}}, {{3}}, {{4}}, {{5}} }};                           

    // Array bias;
    // ArrayInitial(&bias, 5);

    // Tensor conv_result;

    // for (int aaaa = 0; aaaa < 1; aaaa++)
    // {
    //     printf("%d, ", aaaa);
    //     TensorSet(&tensor4, &(t2[0][0][0][0]), 384);
    //     TensorSet(&conv_kel, &(t_ck[0][0][0][0]), 45);

    //     Conv2(&tensor4, &conv_kel, &conv_stride, &bias, VALID, &conv_result);

    //     for (i=0;i<conv_result.tsnum;i++)
    //     {
    //         printf("个数 %d ============================================\n", i);
    //         for (j=0;j<conv_result.chnum;j++)
    //         {
    //             printf("通道 %d ------------------------------------------\n", j);
    //             for (k = 0; k < conv_result.row; k++)
    //             {
    //                 for (h = 0; h < conv_result.col; h++)
    //                 {
    //                     printf("%f, ", conv_result.data[i][j][k][h]);
    //                 }
    //                 printf("\n");
    //             }
    //             printf("\n\n");
    //         }
    //         printf("\n\n\n");
    //     }
    // }

    // printf("激活处理\n");
    // RELU(&conv_result);

    // for (i=0;i<conv_result.tsnum;i++)
    // {
    //     printf("个数 %d ============================================\n", i);
    //     for (j=0;j<conv_result.chnum;j++)
    //     {
    //         printf("通道 %d ------------------------------------------\n", j);
    //         for (k = 0; k < conv_result.row; k++)
    //         {
    //             for (h = 0; h < conv_result.col; h++)
    //             {
    //                 printf("%f, ", conv_result.data[i][j][k][h]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n\n");
    //     }
    //     printf("\n\n\n");
    // }

    TensorSet(&tensor4, &(t2[0][0][0][0]), 486);
    // TensorSet(&tensor4, &(t2[0][0][0][0]), 384);
    // for (i=0;i<tensor4.tsnum;i++)
    // {
    //     printf("个 %d ============================================\n", i);
    //     for (j=0;j<tensor4.chnum;j++)
    //     {
    //         printf("通道 %d ------------------------------------------\n", j);
    //         for (k = 0; k < tensor4.row; k++)
    //         {
    //             for (h = 0; h < tensor4.col; h++)
    //             {
    //                 printf("%f, ", tensor4.data[i][j][k][h]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n\n");
    //     }
    //     printf("\n\n\n");
    // }

    printf("开始池化");
    Tensor pool_result;
    MaxPooling2D(&tensor4, &pool_size, &pool_stride, SAME, &pool_result);
    // MaxPooling2D(&tensor4, &pool_size, &pool_stride, SAME, &pool_result);

    for (i=0;i<pool_result.tsnum;i++)
    {
        printf("池化个 %d ============================================\n", i);
        for (j=0;j<pool_result.chnum;j++)
        {
            printf("池化通道 %d ------------------------------------------\n", j);
            for (k = 0; k < pool_result.row; k++)
            {
                for (h = 0; h < pool_result.col; h++)
                {
                    printf("%f, ", pool_result.data[i][j][k][h]);
                }
                printf("\n");
            }
            printf("\n\n");
        }
        printf("\n\n\n");
    }
    printf("结束池化");


    TensorFree(&tensor4);
    clock_end = clock();
    // printf("time = %f\n",(double)(clock_end - clock_start));
    printf("time = %f s\n",(double)(clock_end - clock_start)/CLK_TCK);

    //=====================================================================
    //=====================================================================

    // #pragma omp parallel for num_threads(6)
    // #pragma omp parallel 
    // for (int i = 0; i < 12; i++)  
    // {  
    //     printf("OpenMP Test, 线程编号为: %d\n", omp_get_thread_num());  
    // }


    // ----------------------------------------------------
    // // 2维张量测试
    // Tensor tensor2;
    // DATA m[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    // TENSOR2_SHAPE[0] = 4;
    // TENSOR2_SHAPE[1] = 4;
    // TensorInitial(&tensor2, TENSOR2_STR, TENSOR2_SHAPE);
    // TensorSet(&tensor2, &(m[0]), 16);
    // for (i=0;i<4;i++)
    // {
    //     for (j=0;j<4;j++)
    //     {
    //         printf("%f, ", tensor2.data[0][0][i][j]);
    //     }
    //     printf("\n");
    // }
    // TensorFree(&tensor2);

    // ----------------------------------------------------
    // // 3维张量测试
    // Tensor tensor3;
    // DATA t1[3][4][4] = {{{1,2,3,4},    {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                    {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                    {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}};
    // int t1_len = 48;
    // TENSOR3_SHAPE[0] = 3;
    // TENSOR3_SHAPE[1] = 4;
    // TENSOR3_SHAPE[2] = 4;
    // TensorInitial(&tensor3, TENSOR3_STR, TENSOR3_SHAPE);
    // TensorSet(&tensor3, &(t1[0][0][0]), 48);
    // for (i=0;i<3;i++)
    // {
    //     for (j=0;j<4;j++)
    //     {
    //         for (k = 0; k < 4; k++)
    //         {
    //             printf("%f, ", tensor3.data[0][i][j][k]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n\n");
    // }
    // TensorFree(&tensor3);

    // ----------------------------------------------------
    // 4维张量测试
    // Tensor tensor4;
    // DATA t2[2][3][4][4] = {{{{1,2,3,4},     {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                         {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                         {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}},

    //                       {{{51,52,53,54},      {55,56,57,58},     {59,510,511,512},  {513,514,515,516}},
    //                        {{517,518,519,520},  {521,522,523,524}, {525,526,527,528}, {529,530,531,532}},
    //                        {{533,534,535,536},  {537,538,539,540}, {541,542,543,544}, {545,546,547,548}}}};
    // int t2_len = 96;
    // TENSOR4_SHAPE[0] = 2;
    // TENSOR4_SHAPE[1] = 3;
    // TENSOR4_SHAPE[2] = 4;
    // TENSOR4_SHAPE[3] = 4;
    // TensorInitial(&tensor4, TENSOR4_STR, TENSOR4_SHAPE);
    // TensorSet(&tensor4, &(t2[0][0][0][0]), 96);
    // for (i=0;i<2;i++)
    // {
    //     for (j=0;j<3;j++)
    //     {
    //         for (k = 0; k < 4; k++)
    //         {
    //             for (h = 0; h < 4; h++)
    //             {
    //                 printf("%f, ", tensor4.data[i][j][k][h]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n\n");
    //     }
    //     printf("\n\n\n");
    // }
    // TensorFree(&tensor4);

    // Tensor tensor3;
    // TENSOR3_SHAPE[0] = 3;
    // TENSOR3_SHAPE[1] = 4;
    // TENSOR3_SHAPE[2] = 4;
    // TensorInitial(&tensor3, TENSOR3_STR, TENSOR3_SHAPE);

    // Tensor tensor4;
    // TENSOR4_SHAPE[0] = 3;
    // TENSOR4_SHAPE[1] = 4;
    // TENSOR4_SHAPE[2] = 4;
    // TENSOR4_SHAPE[3] = 4;
    // TensorInitial(&tensor4, TENSOR4_STR, TENSOR4_SHAPE);

    
    // ----------------------------------------------------
    // printf("矩阵测试\n");
    // DATA m[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    // int m_len = sizeof(m) / sizeof(m[0]);
    // row = 4;
    // col = 4;
    
    // Matrix con_1;
    // MatrixInitial(&con_1, row, col);
    // MatrixSet(&con_1, &(m[0]), m_len);

    // for (i=0;i<row;i++)
    // {
    //     for (j=0;j<col;j++)
    //     {
    //         printf("%f, ", con_1.data[i][j]);
    //     }
    //     printf("\n");
    // }

    // MatrixFree(&con_1);
    // printf("释放完成\n");

    // ----------------------------------------------------
    // printf("三维张量测试\n");
    // DATA t1[3][4][4] = {{{1,2,3,4},    {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                    {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                    {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}};
    // int t1_len = 48;
    // chn = 3;
    // row = 4;
    // col = 4;

    // Tensor3D con_2;
    // printf("定义完成\n");
    // Tensor3DInitial(&con_2, chn, row, col);
    // printf("初始化完成\n");
    // Tensor3DSet(&con_2, &(t1[0][0][0]), t1_len);

    // for (i=0;i<chn;i++)
    // {
    //     for (j=0;j<row;j++)
    //     {
    //         for (k=0;k<col;k++)
    //         {
    //         printf("%f, ", con_2.data[i][j][k]);
    //         }
    //         printf(" | ");
    //     }
    //     printf("\n\n");
    // }

    // Tensor3DFree(&con_2);
    // printf("释放完成\n");

    // ----------------------------------------------------
    // printf("四维张量测试\n");
    // DATA t2[2][3][4][4] = {{{{1,2,3,4},     {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                         {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                         {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}},

    //                       {{{51,52,53,54},      {55,56,57,58},     {59,510,511,512},  {513,514,515,516}},
    //                        {{517,518,519,520},  {521,522,523,524}, {525,526,527,528}, {529,530,531,532}},
    //                        {{533,534,535,536},  {537,538,539,540}, {541,542,543,544}, {545,546,547,548}}},
    //                    };
    // int t2_len = 96;
    // tsn = 2;
    // chn = 3;
    // row = 4;
    // col = 4;

    // Tensor4D con_3;
    // printf("定义完成\n");
    // Tensor4DInitial(&con_3, tsn, chn, row, col);
    // printf("初始化完成\n");
    // Tensor4DSet(&con_3, &(t2[0][0][0][0]), t2_len);

    // for (i=0;i<tsn;i++)
    // {
    //     for (j=0;j<chn;j++)
    //     {
    //         for (k=0;k<row;k++)
    //         {
    //             for (h=0;h<col;h++)
    //             {
    //                 printf("%f, ", con_3.data[i][j][k][h]);
    //             }
    //             printf(" | ");
    //         }
    //         printf(" ||\n ");
    //     }
    //     printf("\n\n");
    // }

    // Tensor4DFree(&con_3);
    // printf("释放完成\n");

    // ----------------------------------------------------
    // // 卷积操作
    // Matrix conved_mat;
    // Matrix conv_kel;

    // DATA m2[] = { 1, 2, 3, 4, 5,
    //               6, 7, 8, 9,10,
    //              11,12,13,14,15,
    //              16,17,18,19,20,
    //              21,22,23,24,25};
    // int m2_len = sizeof(m2) / sizeof(m2[0]);
    // row = 5;
    // col = 5;
    // MatrixInitial(&conved_mat, row, col);
    // MatrixSet(&conved_mat, &(m2[0]), m2_len);

    // DATA m3[] = {1,2,3,
    //              4,5,6,
    //              7,8,9};
    // int m3_len = sizeof(m3) / sizeof(m3[0]);
    // row = 3;
    // col = 3;  
    // MatrixInitial(&conv_kel, row, col);
    // MatrixSet(&conv_kel, &(m3[0]), m3_len);

    // // Conv2Matrix(conved_mat, conv_kel);
    // Matrix conv1_o;
    // // printf("矩阵定义\n");
    // Conv2Matrix(&conved_mat, &conv_kel, &conv1_o);

    // printf("矩阵计算完成\n");
    // // printf("结果矩阵尺寸[%d, %d]\n", conv1_o.row, conv1_o.col);
    // for (i = 0; i < conv1_o.row; i++)
    // {
    //     for (j = 0; j < conv1_o.col; j++)
    //     {
    //         printf("%f, ", conv1_o.data[i][j]);
    //     }
    //     printf("\n");
    // }
    
    // ----------------------------------------------------
    // // 3维张量卷积测试
    // Tensor3D conved_Tsr3D;
    // Matrix   conv_kel;
    // DATA t1[3][4][4] = {{{1,2,3,4},    {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                    {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                    {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}};

    // int t1_len = 48;
    // chn = 3;
    // row = 4;
    // col = 4;

    // Tensor3DInitial(&conved_Tsr3D, chn, row, col);
    // Tensor3DSet(&conved_Tsr3D, &(t1[0][0][0]), t1_len);

    // DATA m3[] = {1,2,3,
    //              4,5,6,
    //              7,8,9};
    // int m3_len = sizeof(m3) / sizeof(m3[0]);
    // row = 3;
    // col = 3;  
    // MatrixInitial(&conv_kel, row, col);
    // MatrixSet(&conv_kel, &(m3[0]), m3_len);

    // // Conv2Matrix(conved_mat, conv_kel);
    // Tensor3D conv2_o;
    // // printf("矩阵定义\n");
    // Conv2Tensor3D(&conved_Tsr3D, &conv_kel, &conv2_o);

    // printf("3维张量计算完成\n");
    // // printf("结果矩阵尺寸[%d, %d]\n", conv1_o.row, conv1_o.col);
    // for (i = 0; i < conv2_o.chnum; i++)
    // {
    //     for (j = 0; j < conv2_o.row; j++)
    //     {
    //         for (k = 0; k < conv2_o.col; k++)
    //         {
    //             printf("%f, ", conv2_o.data[i][j][k]);
    //         }
    //         printf("; ");
    //     }
    //     printf("\n");
    // }

    // ----------------------------------------------------
    // // 4维张量卷积测试
    // Tensor4D conved_Tsr4D;
    // Matrix   conv_kel;
    // DATA t2[2][3][4][4] = {{{{1,2,3,4},     {5,6,7,8},     {9,10,11,12},  {13,14,15,16}},
    //                         {{17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32}},
    //                         {{33,34,35,36}, {37,38,39,40}, {41,42,43,44}, {45,46,47,48}}},

    //                       {{{51,52,53,54},      {55,56,57,58},     {59,510,511,512},  {513,514,515,516}},
    //                        {{517,518,519,520},  {521,522,523,524}, {525,526,527,528}, {529,530,531,532}},
    //                        {{533,534,535,536},  {537,538,539,540}, {541,542,543,544}, {545,546,547,548}}},
    //                    };
    // int t2_len = 96;
    // tsn = 2;
    // chn = 3;
    // row = 4;
    // col = 4;

    // Tensor4DInitial(&conved_Tsr4D, tsn, chn, row, col);
    // Tensor4DSet(&conved_Tsr4D, &(t2[0][0][0][0]), t2_len);

    // DATA m3[] = {1,2,3,
    //              4,5,6,
    //              7,8,9};
    // int m3_len = sizeof(m3) / sizeof(m3[0]);
    // row = 3;
    // col = 3;  
    // MatrixInitial(&conv_kel, row, col);
    // MatrixSet(&conv_kel, &(m3[0]), m3_len);

    // // Conv2Matrix(conved_mat, conv_kel);
    // Tensor4D conv3_o;
    // // printf("矩阵定义\n");
    // Conv2Tensor4D(&conved_Tsr4D, &conv_kel, &conv3_o);

    // printf("3维张量计算完成\n");
    // // printf("结果矩阵尺寸[%d, %d]\n", conv1_o.row, conv1_o.col);
    // for (i = 0; i < conv3_o.tsnum; i++)
    // {
    //     for (j = 0; j < conv3_o.chnum; j++)
    //     {
    //         for (k = 0; k < conv3_o.row; k++)
    //         {
    //             for (h = 0; h < conv3_o.col; h++)
    //             {
    //                 printf("%f, ", conv3_o.data[i][j][k][h]);
    //             }
    //             printf("; ");
    //         }
    //         printf("\n");
    //     }
    //     printf("\n-----------------\n");
    // }


    return 0;
}