/**
* @file ConvOperation
* @ �������,�Ծ���\3ά����\4ά�����ľ��
* @details Ŀǰ���֧��2����ʽ�ľ��,
* @Zehua Du
* @date Mar. 22, 2022
* @version v1.0
* @par Copyright(c):
* @par History:
*version: author, date, desc\n
*/

#include <math.h>
//#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
// #include <omp.h>

#include "Tensor.h"
#include "GINFO_VAR.h"


/**
* �������
* ˵��: ���������ľ������.
* @param[in]  conved_Tsr  -> ����������ṹ��
* @param[in]  conv_kel    -> ����������ṹ��
* @param[in]  conv_stride -> ������㲽���ṹ��[conv_stride.row, conv_stride.col]
* @param[in]  conv_bias   -> �������ƫ��(Ĭ��0)
* @param[in]  conv_mode   -> ���ģʽ("FULL", "VALID", "SAME") https://zhuanlan.zhihu.com/p/62760780
* @param[in]  conv_result -> ������(����ǰ���趨�����)
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void Conv2(Tensor* conved_Tsr, Tensor* conv_kel, CONVSTRIDE* conv_stride, Array* conv_bias, char* conv_mode, Tensor* conv_result)
{
    // ��ÿ������, ��ÿ��ͨ���������, ��Ӧ�ȶ����������ͽ������, ��ÿ��ͨ����ֵ������, Ȼ�������֮��ֵ������
    int i, j, k, h;
    // int ci, cj, ck, ch; //�������ר������, ������Ż���

    int ck_row = conv_kel->row;
    int ck_col = conv_kel->col;

    // �޶�����˳ߴ�Ϊ����������һ��, ������ż������
    if (((ck_row % 2) == 0) && (ck_row != ck_col))
    {
        fprintf(stderr, "ERROR: Conv2: conv_kel => (shape[%d x %d] dimensions shall be odd and row == col)\n", ck_row, ck_col);
        exit(EXIT_FAILURE);
    }

    // �޶������ͨ����ƫ�ó���һ��
    if ((conv_result->chnum) != (conv_bias->len))
    {
        fprintf(stderr, "ERROR: Conv2: conv_kel->chnum len[%d] != conv_bias->len len[%d])\n", conv_result->chnum, conv_bias->len);
        exit(EXIT_FAILURE);
    }

    // ȷ��conved_Tsr���ذ��
    int cm_ext = (ck_row - 1) >> 1;
    // printf("��չ���%d\n", cm_ext);

    int ext_half_len; // ���ذ��
    // ���ݾ��ģʽȷ��extend_tensor�ߴ�
    // ����ģʽ����, �ο�: https://zhuanlan.zhihu.com/p/62760780
    if (strcmp(conv_mode, FULL) == 0)
    {
        // �������������ĳ���
        ext_half_len = (conv_kel->row - 1);
    }
    else if (strcmp(conv_mode, SAME) == 0)
    {
        ext_half_len = (conv_kel->row - 1) >> 1;
    }
    else if (strcmp(conv_mode, VALID) == 0)
    {
        ext_half_len = 0;
    }
    else
    {
        fprintf(stderr, "ERROR: Conv2: conv_mode=>[FULL, SAME, VALID]\n");
        exit(EXIT_FAILURE);
    }

    // printf("���ذ��: %d\n", ext_half_len);

    // ���� conved_Tsr �� extend_tensor ����ʵ������
    Tensor extend_tensor;
    TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR4_SHAPE[1] = conved_Tsr->chnum;
    TENSOR4_SHAPE[2] = (conved_Tsr->row) + (ext_half_len << 1);
    TENSOR4_SHAPE[3] = (conved_Tsr->col) + (ext_half_len << 1);
    // printf("���������� %d\n", ext_half_len<<1);
    // printf("��������\n");
    TensorInitial(&extend_tensor, TENSOR4_STR, TENSOR4_SHAPE);
    TensorPaddingZero(&extend_tensor);

    for (i = 0; i < (conved_Tsr->tsnum); i++)
    {
        for (j = 0; j < (conved_Tsr->chnum); j++)
        {
            for (k = 0; k < (conved_Tsr->row); k++)
            {
                for (h = 0; h < (conved_Tsr->col); h++)
                {
                    // printf("[%d][%d][%d][%d]\n", i, j, k + ext_half_len, h + ext_half_len);
                    extend_tensor.data[i][j][k + ext_half_len][h + ext_half_len] = conved_Tsr->data[i][j][k][h];
                }
            }
        }
    }

     //for (i=0;i<extend_tensor.tsnum;i++)
     //{
     //    printf("extend_tensor���� %d ============================================\n", i);
     //    for (j=0;j<extend_tensor.chnum;j++)
     //    {
     //        printf("ͨ�� %d ------------------------------------------\n", j);
     //        for (k = 0; k < extend_tensor.row; k++)
     //        {
     //            for (h = 0; h < extend_tensor.col; h++)
     //            {
     //                printf("%f, ", extend_tensor.data[i][j][k][h]);
     //            }
     //            printf("\n");
     //        }
     //        printf("\n\n");
     //    }
     //    printf("\n\n\n");
     //}

    // ����������
    // �ο�: https://oldtang.com/6775.html
    TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR4_SHAPE[1] = conv_kel->tsnum;   // ���������ͨ�������˸���һ��
    TENSOR4_SHAPE[2] = floor(((conved_Tsr->row) + (ext_half_len << 1) - ck_row) / conv_stride->row) + 1;  // �������������Ϊ conved_Tsr->row + ���ذ��*2
    TENSOR4_SHAPE[3] = floor(((conved_Tsr->col) + (ext_half_len << 1) - ck_row) / conv_stride->col) + 1;  // �������������Ϊ conved_Tsr->col + ���ذ��*2
    // printf("������\n");
    TensorInitial(conv_result, TENSOR4_STR, TENSOR4_SHAPE);
    TensorPaddingZero(conv_result);

    int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
    int ck_i, ck_j, ck_k, ck_h;  // conv_kel
    int rs_i, rs_j, rs_k, rs_h;  // conv_result
    int et_i, et_j, et_k, et_h;  // extend_mat

    /*
                                ��   ͨ��   ��    ��
    ������������� conved_Tsr    [cd_i][cd_j][cd_k][cd_h]
    ���������     conv_kel      [0   ][ck_j][ck_k][ck_h]
    ����������   conv_result   [rs_i][rs_j][rs_k][rs_h]
    ��չ�������   extend_tensor [et_i][et_j][et_k][et_h]
    */
    // �ο�: https://www.cnblogs.com/chumingqian/articles/11495364.html
    // ÿ�������ͨ����Ӧ1�����
    // TODO �ɲ��д���
    // TODO ��Ҫȷ�� conv_stride ����λ���Ƿ���ȷ
    // printf("%d\n", conv_stride->row);
    // #pragma omp parallel for num_threads(6)
    //DATA tmp = 0.0, tmp1 = 0.0;

    //printf("CONV2: \n");
    for (cd_i = 0; cd_i < conved_Tsr->tsnum; cd_i++)   // ����"�����"���� => ��Ӧʵ������ĸ���
    {
        //printf("level - 1\n");
        for (ck_j = 0; ck_j < conv_kel->tsnum; ck_j++) // ����"�����"���� => ��Ӧʵ�����ͨ���� m
        //for (ck_j = 0; ck_j < conv_kel->chnum; ck_j++) // ����"�����"ͨ�� => ��Ӧʵ�����ͨ����
        {
             //printf("level - 2\n");
            // ÿͨ��\��\��,��Ӧ�ĺ˴�С���ݽ��о��, Ȼ��ѭ���� + bias
            for (cd_k = 0; cd_k < conv_result->row; cd_k++)     // ����"����������"�� => ��Ӧʵ������� (+����) k
            {
                 //printf("level - 3\n");
                for (cd_h = 0; cd_h < conv_result->col; cd_h++) // ����"����������"�� => ��Ӧʵ������� (+����) j
                {
                     //printf("level - 4\n");
                    for (cd_j = 0; cd_j < conved_Tsr->chnum; cd_j++)     // ����"���������"ͨ�� n
                    {
                         //printf("level - 5\n");
                        // ��ǰ��Ϊ������, ��Ӧ�ĺ˴�С��������, ���
                        // += extend_tensor.data[cd_i][cd_j][�˷�Χ][�˷�Χ] .* conv_kel->data[0][ck_j][�˷�Χ][�˷�Χ]
                        for (ck_k = 0; ck_k < conv_kel->row; ck_k++)     // ����"�����"�� i
                        {
                            // #pragma omp parallel 
                            // printf("level - 6\n");
                            for (ck_h = 0; ck_h < conv_kel->col; ck_h++) // ����"�����"�� j
                            {
                                // printf("level - 7\n");
                                // printf("[%d][%d][%d][%d]\n", conved_Tsr->tsnum, conv_kel->chnum, conv_result->row, conv_result->col);
                                // printf("[%d][%d][%d][%d]\n", cd_i, ck_j, cd_k, cd_h);
                                // printf("[%d][%d][%d][%d] => %f\n", cd_i, cd_j, cd_k + ck_k, cd_h + ck_h, extend_tensor.data[cd_i][cd_j][cd_k + ck_k][cd_h + ck_h]);
                                conv_result->data[cd_i][ck_j][cd_k][cd_h] += (extend_tensor.data[cd_i][cd_j][cd_k * (conv_stride->row) + ck_k][cd_h * (conv_stride->col) + ck_h] * conv_kel->data[ck_j][cd_j][ck_k][ck_h]);
                                //printf("��������%f\n", conv_result->data[cd_i][ck_j][cd_k][cd_h]);
                            }
                        }
                    }
                    // TODO ��ƫ��, ��ȷ���Ƿ�������˾����һ��ƫ��
                    // �ο�: https://poloclub.github.io/cnn-explainer/
                    conv_result->data[cd_i][ck_j][cd_k][cd_h] += conv_bias->data[ck_j]; // 1�������ͨ����Ӧһ�����
                }
            }
        }

    }

    TensorFree(&extend_tensor);
}

//----------------------------------------------------------------------------

// /**
// * ������
// * ˵��: ���������ľ������.
// * @param[in]  conved_Matrix -> ���������ṹ��
// * @param[in]  conv_kel      -> ����˾���ṹ��
// * @param[in]  conv_result   -> ������(����ṹ�壬����ǰ���趨��)
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 22, 2022  
// */
// void Conv2Matrix(Matrix *conved_Matrix, Matrix *conv_kel, Matrix *conv_result)
// {
//     printf("debug 1\n");
//     // ��ÿ��ͨ���������, ��Ӧ�ȶ����������ͽ������, ��ÿ��ͨ����ֵ������, Ȼ�������֮��ֵ������
//     int i, j;
//     int ci, cj, ck, ch; //�������ר������, ������Ż���

//     int ck_row = conv_kel->row;
//     // int ck_col = conv_kel->col;

//     // �޶�����˳ߴ�Ϊ����, ������ż������
//     if ((ck_row % 2) == 0)
//     {
//         fprintf(stderr, "ERROR: conv_kel => (shape[%d x %d] dimensions shall be odd)\n", ck_row, ck_row);
//         exit(EXIT_FAILURE);
//     }

//     // ȷ��Matrix���ذ��
//     int cm_ext = (ck_row - 1) >> 1;
//     // printf("��չ���%d\n", cm_ext);

//     // Matrix conv_result; // ������
//     // printf("���������ߴ�[%d, %d]\n", conved_Matrix->row, conved_Matrix->col);
//     MatrixInitial(conv_result, conved_Matrix->row, conved_Matrix->col);
//     // printf("�������ߴ�[%d, %d]\n", conv_result->row, conv_result->col);
//     for (i = 0; i < conv_result->row; i++)
//     {
//         for (j = 0; j < conv_result->col; j++)
//         {
//             conv_result->data[i][j] = 0;
//         }
//     }
//     // for (i = 0; i < conv_result->row; i++)
//     // {
//     //     for (j = 0; j < conv_result->col; j++)
//     //     {
//     //         printf("%f, ", conv_result->data[i][j]);
//     //     }
//     //     printf("\n");
//     // }

//     Matrix extend_mat;  // ���صľ���
//     MatrixInitial(&extend_mat, conved_Matrix->row + (cm_ext<<1), conved_Matrix->col + (cm_ext<<1));
//     // extend_mat��ʼ��Ϊ0����
//     for (i = 0; i < extend_mat.row; i++)
//     {
//         for (j = 0; j < extend_mat.col; j++)
//         {
//             extend_mat.data[i][j] = 0;
//         }
//     }

//     // conved_Matrix���Ƹ�extend_mat
//     for (i = 0; i < conved_Matrix->row; i++)
//     {
//         for (j = 0; j < conved_Matrix->col; j++)
//         {
//             extend_mat.data[i + cm_ext][j + cm_ext] = conved_Matrix->data[i][j];
//         }
//     }

//     // �������
//     for (ci = 0; ci < conved_Matrix->row; ci++)
//     {
//         for (cj = 0; cj < conved_Matrix->col; cj++)
//         {
//             // printf("conved_Matrix:[%d, %d]\n ", i, j);
//             for (ck = ci; ck < (ci + conv_kel->row); ck++)
//             {
//                 for (ch = cj; ch < (cj + conv_kel->col); ch++)
//                 {
//                     // printf("conv_ext:[%d, %d], conv_kel:[%d, %d]; ", k, h, k - i, h - j);
//                     conv_result->data[ci][cj] += extend_mat.data[ck][ch] * conv_kel->data[ck - ci][ch - cj];
//                 }
//             // printf("\n");
//             }
//         }
//         // printf("-----------------\n");
//     }
//     // for (i = 0; i < conv_result->row; i++)
//     // {
//     //     for (j = 0; j < conv_result->col; j++)
//     //     {
//     //         printf("%f, ", conv_result->data[i][j]);
//     //     }
//     //     printf("\n");
//     // }
//     MatrixFree(&extend_mat);
// }


// /**
// * 3ά�������
// * ˵��: ���������ľ������.
// * @param[in]  conved_Tsr3D  -> �����3ά�����ṹ��
// * @param[in]  conv_kel      -> ����˾���ṹ��
// * @param[in]  conv_result   -> ������(3ά�����ṹ�壬����ǰ���趨��)
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 22, 2022  
// */
// void Conv2Tensor3D(Tensor3D *conved_Tsr3D, Matrix *conv_kel, Tensor3D *conv_result)
// {
//     // ��ÿ��ͨ���������, ��Ӧ�ȶ����������ͽ������, ��ÿ��ͨ����ֵ������, Ȼ�������֮��ֵ������
//     int i, j, k;
//     int ci, cj, ck, ch; //�������ר������, ������Ż���

//     int ck_row = conv_kel->row;
//     // int ck_col = conv_kel->col;

//     // �޶�����˳ߴ�Ϊ����, ������ż������
//     if ((ck_row % 2) == 0)
//     {
//         fprintf(stderr, "ERROR: conv_kel => (shape[%d x %d] dimensions shall be odd)\n", ck_row, ck_row);
//         exit(EXIT_FAILURE);
//     }

//     // ȷ��conved_Tsr3D���ذ��
//     int cm_ext = (ck_row - 1) >> 1;
//     // printf("��չ���%d\n", cm_ext);

//     // Tensor3D conv_result; // ������
//     Tensor3DInitial(conv_result, conved_Tsr3D->chnum, conved_Tsr3D->row, conved_Tsr3D->col);
//     for (i = 0; i < conv_result->chnum; i++)
//     {
//         for (j = 0; j < conv_result->row; j++)
//         {
//             for (k = 0; k < conv_result->col; k++)
//             {
//                 conv_result->data[i][j][k] = 0;
//             }
//         }
//     }

//     Matrix extend_mat;  // ���صľ���
//     MatrixInitial(&extend_mat, conved_Tsr3D->row + (cm_ext<<1), conved_Tsr3D->col + (cm_ext<<1));
//     // extend_mat��ʼ��Ϊ0����
//     for (i = 0; i < extend_mat.row; i++)
//     {
//         for (j = 0; j < extend_mat.col; j++)
//         {
//             extend_mat.data[i][j] = 0;
//         }
//     }

//     for (i = 0; i < conved_Tsr3D->chnum; i++)
//     {
//         // conved_Tsr3D��ǰͨ�������Ƹ�extend_mat
//         for (j = 0; j < conved_Tsr3D->row; j++)
//         {
//             for (k = 0; k < conved_Tsr3D->col; k++)
//             {
//                 extend_mat.data[j + cm_ext][k + cm_ext] = conved_Tsr3D->data[i][j][k];
//             }
//         }

//         // �������
//         for (ci = 0; ci < conv_result->row; ci++)
//         {
//             for (cj = 0; cj < conv_result->col; cj++)
//             {
//                 // printf("conv_result:[%d, %d]\n ", i, j);
//                 for (ck = ci; ck < (ci + conv_kel->row); ck++)
//                 {
//                     for (ch = cj; ch < (cj + conv_kel->col); ch++)
//                     {
//                         // printf("conv_ext:[%d, %d], conv_kel:[%d, %d]; ", k, h, k - i, h - j);
//                         conv_result->data[i][ci][cj] += extend_mat.data[ck][ch] * conv_kel->data[ck - ci][ch - cj];
//                     }
//                 // printf("\n");
//                 }
//                 printf("conv_result [%d, %d, %d]: %f\n", i, ci, cj, conv_result->data[i][ci][cj]);
//             }
//             // printf("-----------------\n");
//     }
//     }
//     MatrixFree(&extend_mat);
// }


// /**
// * 4ά�������
// * ˵��: ���������ľ������.
// * @param[in]  conved_Tsr4D  -> �����4ά�����ṹ��
// * @param[in]  conv_kel      -> ����˾���ṹ��
// * @param[in]  conv_result   -> ������(4ά�����ṹ�壬����ǰ���趨��)
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 22, 2022  
// */
// void Conv2Tensor4D(Tensor4D *conved_Tsr4D, Matrix *conv_kel, Tensor4D *conv_result)
// {
//     // ��ÿ��3ά����, ��ÿ��ͨ���������, ��Ӧ�ȶ����������ͽ������, ��ÿ��ͨ����ֵ������, Ȼ�������֮��ֵ������
//     int i, j, k, h;
//     int ci, cj, ck, ch; //�������ר������, ������Ż���

//     int ck_row = conv_kel->row;
//     // int ck_col = conv_kel->col;

//     // �޶�����˳ߴ�Ϊ����, ������ż������
//     if ((ck_row % 2) == 0)
//     {
//         fprintf(stderr, "ERROR: conv_kel => (shape[%d x %d] dimensions shall be odd)\n", ck_row, ck_row);
//         exit(EXIT_FAILURE);
//     }

//     // ȷ��conved_Tsr4D���ذ��
//     int cm_ext = (ck_row - 1) >> 1;
//     // printf("��չ���%d\n", cm_ext);

//     // Tensor4D conv_result; // ������
//     Tensor4DInitial(conv_result, conved_Tsr4D->tsnum, conved_Tsr4D->chnum, conved_Tsr4D->row, conved_Tsr4D->col);
//     for (i = 0; i < conv_result->tsnum; i++)
//     {
//         for (j = 0; j < conv_result->chnum; j++)
//         {
//             for (k = 0; k < conv_result->row; k++)
//             {
//                 for (h = 0; h < conv_result->col; h++)
//                 {
//                     conv_result->data[i][j][k][h] = 0;
//                 }
//             }
//         }

//     }

//     Matrix extend_mat;  // ���صľ���
//     MatrixInitial(&extend_mat, conved_Tsr4D->row + (cm_ext<<1), conved_Tsr4D->col + (cm_ext<<1));
//     // extend_mat��ʼ��Ϊ0����
//     for (i = 0; i < extend_mat.row; i++)
//     {
//         for (j = 0; j < extend_mat.col; j++)
//         {
//             extend_mat.data[i][j] = 0;
//         }
//     }


//     for (i = 0; i < conved_Tsr4D->tsnum; i++)
//     {
//         for (j = 0; j < conved_Tsr4D->chnum; j++)
//         {
//             // conved_Tsr4D��ǰͨ�������Ƹ�extend_mat
//             for (k = 0; k < conved_Tsr4D->row; k++)
//             {
//                 for (h = 0; h < conved_Tsr4D->col; h++)
//                 {
//                     extend_mat.data[k + cm_ext][h + cm_ext] = conved_Tsr4D->data[i][j][k][h];
//                 }
//             }

//             // �������
//             for (ci = 0; ci < conv_result->row; ci++)
//             {
//                 for (cj = 0; cj < conv_result->col; cj++)
//                 {
//                     // printf("conved_Tsr3D:[%d, %d]\n ", i, j);
//                     for (ck = ci; ck < (ci + conv_kel->row); ck++)
//                     {
//                         for (ch = cj; ch < (cj + conv_kel->col); ch++)
//                         {
//                             // printf("conv_ext:[%d, %d], conv_kel:[%d, %d]; ", k, h, k - i, h - j);
//                             conv_result->data[i][j][ci][cj] += extend_mat.data[ck][ch] * conv_kel->data[ck - ci][ch - cj];
//                         }
//                     // printf("\n");
//                     }
//                 }
//                 // printf("-----------------\n");
//         }


//     }
//     }
//     MatrixFree(&extend_mat);
// }
