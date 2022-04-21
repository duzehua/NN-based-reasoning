#include <math.h>
#include <string.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

// �ο� http://t.zoukankan.com/ssyfj-p-13964588.html
void MaxPooling2D(Tensor *conved_Tsr, POOLSIZE *pool_size, POOLSTRIDE *strides, char *pool_mode, Tensor *pool_result)
{
    // ��ÿ������, ��ÿ��ͨ�������ػ�, ��Ӧ�ȶ����������ͽ������, ��ÿ��ͨ����ֵ������, Ȼ�������֮��ֵ������
    int i, j, k, h;
    int ext_pool_flag = 0; // Ĭ�ϲ�ʹ����0����, ��SAMEģʽ��������ʱ, ��1���е�������

    // �ػ��˵ĳߴ�
    int ck_row = pool_size->row;
    int ck_col = pool_size->col;

    int pool_result_row; // �������������
    int pool_result_col; // �������������

    int ext_len_row = 0; // ����������0�п�
    int ext_len_col = 0; // ����������0�п�

    // ���ݳػ�ģʽȷ����������ĳߴ�, �Լ���0��λ��
    if (strcmp(pool_mode, VALID) == 0)
    {
        // TODO ��ȷ��
        // Validģʽ���Ჹ0, ��˿��ܻ�����޷����������������ݵ����
        pool_result_row = (((conved_Tsr->row) - ck_row) / (strides->row)) + 1;
        pool_result_col = (((conved_Tsr->col) - ck_col) / (strides->col)) + 1;
        // printf("�ػ�����ߴ� %d, %d\n", pool_result_row, pool_result_col);

    }
    else if (strcmp(pool_mode, SAME) == 0)
    {
        // TODO ��ȷ��
        // Sameģʽ�ڳ�������ʱ��Ჹ0
        pool_result_row = (((conved_Tsr->row) - ck_row - 1) / (strides->row) + 1) + 1;
        pool_result_col = (((conved_Tsr->col) - ck_col - 1) / (strides->col) + 1) + 1;
        // printf("%d, %d, %d\n", (conved_Tsr->row) - ck_row, ((conved_Tsr->row) - ck_row) / (strides->row), ((conved_Tsr->row) - ck_row) / (strides->row) + 1);
        // printf("�ػ�����ߴ� %d, %d\n", pool_result_row, pool_result_col);
        // pool_result_row = ceil(((conved_Tsr->row) - (pool_size->row) + 1) / (strides->row));
        // pool_result_col = ceil(((conved_Tsr->col) - (pool_size->col) + 1) / (strides->col));

        // ��0����, ��(��)���Ա���������
        int conved_Tsr_mod_stride_row = (((conved_Tsr->row) - ck_row) % (strides->row));
        if (conved_Tsr_mod_stride_row != 0) 
        {
            ext_len_row = (strides->row) - conved_Tsr_mod_stride_row;
            ext_pool_flag = 1;
        }
        int conved_Tsr_mod_stride_col = (((conved_Tsr->col) - ck_col) % (strides->col));
        if (conved_Tsr_mod_stride_col != 0) 
        {
            ext_len_col = (strides->col) - conved_Tsr_mod_stride_col;
            ext_pool_flag = 1;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: MaxPooling2D: pool_mode=>[SAME, VALID]\n");
        exit(EXIT_FAILURE);
    }

    // ���������ؾ������ѵ��
    if (ext_pool_flag == 0)
    {
        // �����������ĳߴ�
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = pool_result_row;  // �������������Ϊ conved_Tsr->row + ���ذ��*2
        TENSOR4_SHAPE[3] = pool_result_col;  // �������������Ϊ conved_Tsr->col + ���ذ��*2

        // printf("������\n");
        TensorInitial(pool_result, TENSOR4_STR, TENSOR4_SHAPE);
        // TensorPaddingZero(conv_result);

        // ���ػ��������
        int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
        int pl_k = 0, pl_h = 0;  // pool_result
        int pl_r, pl_c;  // pool operation  
        DATA pl_result_tmp = NInf;    
        // printf("init %f\n", pl_result_tmp);         

        // /*
        //                              ��    ͨ��  ��    ��
        // ���ػ��������� conved_Tsr    [cd_i][cd_j][cd_k][cd_h]
        // �ػ��������   pool_result   [cd_i][cd_j][pl_k][pl_h]
        // int temp=0;
        for (cd_i = 0; cd_i < conved_Tsr->tsnum; cd_i++)
        {
            // printf("num %d\n", cd_i);
            for (cd_j = 0; cd_j < conved_Tsr->chnum; cd_j++)
            {
                // printf("chnum %d\n", cd_j);
                // ������ǰͨ������
                // �Ӻ�ĩβ��ʼ����, ����Ϊ�ػ�����, ֱ���߽�
                pl_k = 0;
                for (cd_k = (ck_row-1); cd_k < (conved_Tsr->row); cd_k = (cd_k+(strides->row)))
                {
                    // printf("cd_k %d\n", cd_k);
                    pl_h = 0;
                    for (cd_h = (ck_col-1); cd_h < (conved_Tsr->col); cd_h = (cd_h+(strides->col)))
                    {
                        // printf("cd_h %d\n", cd_h);

                        pl_result_tmp = NInf;
                        // cd_k\cd_h Ϊ�ػ������½ǵ� ��\�� ����, ����\�����
                        for (pl_r = cd_k; pl_r > (cd_k-ck_row); pl_r--)
                        {
                            // printf("pl row %d, cd_k %d\n", pl_r, cd_k);
                            for (pl_c = cd_h; pl_c > (cd_h-ck_col); pl_c--)
                            {
                                // printf("pl col %d, cd_h %d\n", pl_c, cd_h);
                                // printf("curt %f\n", conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]);
                                if ((conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]) > pl_result_tmp)
                                {
                                    
                                    pl_result_tmp = (conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]);
                                    // printf("max %f\n", pl_result_tmp);
                                }
                            }
                        }
                        // printf("%f\n", pl_result_tmp);
                        // temp++;
                        // printf("%d\n", temp);
                        // printf("[cd_i %d][cd_j %d][pl_k %d][pl_h %d] %f\n", cd_i, cd_j, pl_k, pl_h, pl_result_tmp);
                        pool_result->data[cd_i][cd_j][pl_k][pl_h] = pl_result_tmp;
                        pl_h++;
                    }
                    pl_k++;
                }
            }
        }
    }
    else
    {
        //
        // printf("'????????????????????????'");
        // �����������ĳߴ�
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = pool_result_row; 
        TENSOR4_SHAPE[3] = pool_result_col;  
        // printf("������\n");
        TensorInitial(pool_result, TENSOR4_STR, TENSOR4_SHAPE);

        Tensor conved_Tsr_Tmp;
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = conved_Tsr->row + ext_len_row; 
        TENSOR4_SHAPE[3] = conved_Tsr->col + ext_len_col;  
        printf("%d, %d, %d, %d\n", TENSOR4_SHAPE[0], TENSOR4_SHAPE[1], TENSOR4_SHAPE[2], TENSOR4_SHAPE[3]);
        TensorInitial(&conved_Tsr_Tmp, TENSOR4_STR, TENSOR4_SHAPE);
        TensorPaddingZero(&conved_Tsr_Tmp);
        for (i = 0; i < (conved_Tsr->tsnum); i++)
        {
            for (j = 0; j < (conved_Tsr->chnum); j++)
            {
                for (k = 0; k < (conved_Tsr->row); k++)
                {
                    for (h = 0; h < (conved_Tsr->col); h++)
                    {
                        // printf("[%d][%d][%d][%d]\n", i, j, k + ext_half_len, h + ext_half_len);
                        conved_Tsr_Tmp.data[i][j][k][h] = conved_Tsr->data[i][j][k][h];
                    }
                }
            }
        }

        // for (i=0;i<conved_Tsr_Tmp.tsnum;i++)
        // {
        //     printf("�ػ��м�� %d ============================================\n", i);
        //     for (j=0;j<conved_Tsr_Tmp.chnum;j++)
        //     {
        //         printf("�ػ��м�ͨ�� %d ------------------------------------------\n", j);
        //         for (k = 0; k < conved_Tsr_Tmp.row; k++)
        //         {
        //             for (h = 0; h < conved_Tsr_Tmp.col; h++)
        //             {
        //                 printf("%f, ", conved_Tsr_Tmp.data[i][j][k][h]);
        //             }
        //             printf("\n");
        //         }
        //         printf("\n\n");
        //     }
        //     printf("\n\n\n");
        // }

        // ���ػ��������
        int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
        int pl_k = 0, pl_h = 0;  // pool_result
        int pl_r, pl_c;  // pool operation  
        DATA pl_result_tmp = NInf;    

        for (cd_i = 0; cd_i < conved_Tsr_Tmp.tsnum; cd_i++)
        {
            // printf("num %d\n", cd_i);
            for (cd_j = 0; cd_j < conved_Tsr_Tmp.chnum; cd_j++)
            {
                // printf("chnum %d\n", cd_j);
                // ������ǰͨ������
                // �Ӻ�ĩβ��ʼ����, ����Ϊ�ػ�����, ֱ���߽�
                pl_k = 0;
                for (cd_k = (ck_row-1); cd_k < (conved_Tsr_Tmp.row); cd_k = (cd_k+(strides->row)))
                {
                    // printf("cd_k %d\n", cd_k);
                    pl_h = 0;
                    for (cd_h = (ck_col-1); cd_h < (conved_Tsr_Tmp.col); cd_h = (cd_h+(strides->col)))
                    {
                        // printf("cd_h %d\n", cd_h);

                        pl_result_tmp = NInf;
                        // cd_k\cd_h Ϊ�ػ������½ǵ� ��\�� ����, ����\�����
                        for (pl_r = cd_k; pl_r > (cd_k-ck_row); pl_r--)
                        {
                            // printf("pl row %d, cd_k %d, cd_h %d, cd_h-ck_col %d\n", pl_r, cd_k, cd_h, (cd_h-ck_col));
                            for (pl_c = cd_h; pl_c > (cd_h-ck_col); pl_c--)
                            {
                            //     printf("pl col %d, cd_h %d\n", pl_c, cd_h);
                            //     printf("curt %f\n", conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]);
                                if ((conved_Tsr_Tmp.data[cd_i][cd_j][pl_r][pl_c]) > pl_result_tmp)
                                {
                                    
                                    pl_result_tmp = (conved_Tsr_Tmp.data[cd_i][cd_j][pl_r][pl_c]);
                                    // printf("max %f\n", pl_result_tmp);
                                }
                                // printf("%f\n", conved_Tsr_Tmp.data[cd_i][cd_j][pl_r][pl_c]);
                            }
                        }
                        // printf("%f\n", pl_result_tmp);
                        // temp++;
                        // printf("%d\n", temp);
                        // printf("[cd_i %d][cd_j %d][pl_k %d][pl_h %d] %f\n", cd_i, cd_j, pl_k, pl_h, pl_result_tmp);
                        pool_result->data[cd_i][cd_j][pl_k][pl_h] = pl_result_tmp;
                        // printf("%d, %d\n", pl_k, pl_h);
                        pl_h++;
                    }
                    pl_k++;
                }
            }
        }
        TensorFree(&conved_Tsr_Tmp);
    }
    
}
