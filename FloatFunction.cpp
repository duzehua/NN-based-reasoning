#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>


unsigned int bitRep(float x) {  // ���ص����޷��ŵ���������float2BiasExp��ϵõ���ƫָ��
	union {
		float fl;
		unsigned int i;
	} flint;
	flint.fl = x;
	return flint.i;
}

int float2BiasExp(float x) { // returns the unbiased exponent of x
	// Biased exponent is bits 30-23 of the bit rep of x
	int mask = ((1 << 8) - 1) << 23;
	return (bitRep(x) & mask) >> 23; // *Biased exponent
}

int isNormal(float x) { // Returns 1 if x is normal, 0 if x is denormal
	return 0 != float2BiasExp(x);
}


// windows��longΪ32λ��long longΪ64λ����������x��С�����ֳ���2^23
long long float2Frac(float x) { // returns the fractional part of x times 2^23
	// Special cases
	if (isinf(x)) return 0;
	if (isnan(x)) return 1;
	// return the rightmost 23 bits of the bit representation of x
	unsigned int xi = bitRep(x);
	long long frac = xi & ((1 << 23) - 1); // May be denormal!
	if (isNormal(x) == 0) return frac; // denormal... no 1.
	return frac | (1 << 23); // Or in the 1. in front of the decimal!����񻯣�
}

int float2Exp(float x) { // returns the unbiased exponent of x
	if (isNormal(x)) return float2BiasExp(x) - 127;
	return -126;
}

int float2Sign(float x) { //returns the sign bit of x (1 if negative, 0 if positive)
	return (0 != (bitRep(x) & (1 << 31)));
}

float makeFloat(int sign, int exp, int frac) { // returns the floating point representation
	union {
		float fl;
		unsigned int i;
	} flint;
	// Assumes exp=-127 and frac already shifted for denormals
	flint.i = 0;
	assert(sign == 0 || sign == 1);
	assert(exp > -128 && exp < 129);
	assert(frac < (1 << 24)); // frac fits in 24 bits?
	flint.i |= (sign << 31); // Or in sign bit
	// printf("sign, exp, frac: %d, %d, %d\n", sign, exp, frac);
	// printf("flint.i: %d\n", flint.i);
	if (frac < (1 << 23)) { // Is it a normal number?
		exp = -127; // sub-normal special case for exponent
	}
	else { // Remove the 1. from the fraction!
		frac &= ~(1 << 23); // Turn off 1.
	}
	// printf("flint.i: %d\n", flint.i);
	flint.i |= (exp + 127) << 23; // Or in exponent
	// printf("flint.i: %d\n", flint.i);
	flint.i |= frac; // Or in fraction
	// printf("flint.i: %x\n", flint.i);
	return flint.fl;
}


void floatPrint(char const* pref, float a) {
	int norm = isNormal(a);
	int frac = float2Frac(a);
	int exp = float2Exp(a);
	if (isNormal(a)) {
		frac &= ~(1 << 23); // Turn of 1. for print
	}
	else exp = -126;
	printf("%s %g = -1^%d x %d.%06x x 2^%d\n", pref, a,
		float2Sign(a), norm, frac, exp);
}

float floatProd(float a, float b) {
	/* Your code to compute the product of a and b without doing a*b goes here */
	int sign_bit_value;
	// int sign_bit_value_b;
	int exponent_product;
	unsigned long long fractional_product;
	float result;
	if (float2Sign(a) != float2Sign(b)) { sign_bit_value = 1; }
	else { sign_bit_value = 0; }
	// printf("sign: %d\n", sign_bit_value);
	exponent_product = float2Exp(a) + float2Exp(b);
	// printf("float2Frac(a), float2Frac(b): %x, %x\n", float2Frac(a), float2Frac(b));
	fractional_product = ((unsigned long long)float2Frac(a)) * ((unsigned long long)float2Frac(b));
	// printf("fractional_product: %x\n", fractional_product);
	fractional_product = fractional_product >> 23;
	// printf("fractional_product: %lld\n", fractional_product);
	// printf("exponent_product: %d\n", exponent_product);
	// ��ֹ��������й��
	if (fractional_product > pow(2, 24)) {
		fractional_product = fractional_product >> 1;
		exponent_product++;
	}
	if (exponent_product > 126) {
		exponent_product = 127;
		fractional_product = 0.0;
	}
	if (exponent_product < -126) {
		return 0.0;
	}
	// printf("exponent_product: %d\n", exponent_product);

	result = makeFloat(sign_bit_value, exponent_product, fractional_product);
	if (isnan(a) || isnan(b)) {
		return NAN;
	}
	if (isinf(a) || isinf(b)) {
		if (a != 0.0 || b != 0.0) {
			if (float2Sign(a) != float2Sign(b)) {
				return -INFINITY;
			}
			else {
				return INFINITY;
			}
		}
		if (a == 0.0 || b == 0.0) {
			return NAN;
		}
	}
	if (result > 0 && result < 1) {
		while (exponent_product > -126) {
			exponent_product = exponent_product - 1;
			fractional_product = fractional_product << 1;
		}
		while (exponent_product < -126) {
			exponent_product = exponent_product + 1;
			fractional_product = fractional_product >> 1;
		}

	}
	// printf("result sign: %d\n", float2Sign(result));
	return result;

}

float floatAdd(float a, float b)
{
	int sign_bit_value;
	int exponent_add;
	int deltaE;
	float tmp;

	long long fractional_add;
	float result;
	//printf("sign: %d, %d\n", float2Sign(a), float2Sign(b));

	// TODO: һ��һ����ӣ����ս����μ��㣿
	if (float2Sign(a) != float2Sign(b))
	{
		// ���aΪ����bΪ������������˳�򼴿�
		if (float2Sign(b) == 0)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		deltaE = float2Exp(a) - float2Exp(b);  // ָ��֮��

		if (float2Sign(a) == 0)   // aΪ������bΪ����
		{
			//printf("deltaE: %d\n", deltaE);
			if (deltaE > 0)   // aָ����b��
			{
				sign_bit_value = 0;  //��Ϊ����				
				// TODO: ������֮���Сʱ�����ս��ָ��ֵ���ȡ��
				exponent_add = float2Exp(a);
				if ((float2Frac(a) - (float2Frac(b) >> deltaE)) > 0)  // aС�����ִ���bС������,���ս��Ӧ��Ϊ��ֵ
				{
					//printf("case1: һ��һ����a��ָ����b����aС�����ִ���bС�����֣��������䣬���Ϊ��ֵ\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					exponent_add = float2Exp(a);
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = float2Frac(a) - (b_frac >> deltaE);   // С�����ȶԽ�, �������õ�С�����ֵĺ�
					//printf("fractional_add: %llx\n", fractional_add);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				// TODO: δ��ɣ���Ҫ��λ������
				else if ((float2Frac(a) - (float2Frac(b) >> deltaE)) < 0)
				{
					//printf("case2: һ��һ����a��ָ����b�󣬵�aС������С��bС�����֣���Ҫ��λ������������䣬���Ϊ��ֵ\n");
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = (b_frac >> deltaE) - float2Frac(a);   // С�����ȶԽ�, �������õ�С�����ֵĺ�
					exponent_add = exponent_add - 1;
					//printf("fractional_add: %llx\n", fractional_add);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				else
				{
					//printf("case3: һ��һ����a��ָ����b����aС�����ֵ���bС�����֣���Ҫ��λ������������䣬���Ϊ��ֵ\n");
					sign_bit_value = 0;
					exponent_add = float2Exp(b);   // �������a���ñ�b��һ��2^exp(b)
					fractional_add = 0;
				}
			}
			// TODO: ָ����ͬ������֮���С�����ս����ָ��ֵ���ȡ��
			else if (deltaE == 0)  // a,bָ����ͬ
			{
				if ((float2Frac(a) - float2Frac(b)) > 0)  // aС�����ִ���bС������,���ս��Ӧ��Ϊ��ֵ
				{
					//printf("case4: һ��һ����a��bָ����ͬ����aС�����ִ���bС�����֣��������ܻ�䣬���Ϊ��ֵ\n");
					sign_bit_value = 0;
					exponent_add = float2Exp(a);
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = float2Frac(a) - b_frac;
					//printf("fractional_add: %llx\n", fractional_add);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				// TODO: δ��ɣ���Ҫ��λ������
				else if ((float2Frac(a) - float2Frac(b)) < 0) // aС������С��bС������,���ս��Ӧ��Ϊ��ֵ
				{
					//printf("case5: һ��һ����a��bָ����ͬ����aС������С��bС�����֣�������䣬���Ϊ��ֵ\n");
					sign_bit_value = 1;
					exponent_add = float2Exp(a);
					//fractional_add = float2Frac(b) - float2Frac(a);  // �͵�С������ӦΪb-a
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = b_frac - float2Frac(a);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				else
				{
					//printf("case6: һ��һ����a��bָ����ͬ����a��bС��������ͬ�����Ϊ0\n");
					sign_bit_value = 0;
					exponent_add = 0;
					fractional_add = 0;
				}
			}
			else   // aָ����bС�����ս��Ϊ����
			{

				sign_bit_value = 1;
				exponent_add = float2Exp(b);
				if (((float2Frac(a) >> (-deltaE)) - float2Frac(b)) > 0)  // a��λ��С�����ִ���bС������
				{
					//printf("case7: һ��һ����aָ����bС����aС�����ִ���bС�����֣��������ܻ�䣬���Ϊ��ֵ\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a) >> (-deltaE));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = (float2Frac(a) >> (-deltaE)) - b_frac;   // С�����ȶԽ�, �������õ�С�����ֵĺ�
					//printf("fractional_add: %llx\n", fractional_add);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				// TODO: δ��ɣ���Ҫ��λ������
				else if (((float2Frac(a) >> (-deltaE)) - float2Frac(b)) < 0)     // a��λ��С������С��bС������
				{
					//printf("case8: һ��һ����aָ����bС����aС������С��bС�����֣��������ܻ�䣬���Ϊ��ֵ\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a) >> (-deltaE));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					unsigned long long b_frac = 0;    // ����һ����ʱֵ����b��С������
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // ��2^24���ȡb�Ĳ���,���������λ��1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = b_frac - (float2Frac(a) >> (-deltaE));   // С�����ȶԽ�, �������õ�С�����ֵĺ�
					//printf("fractional_add: %llx\n", fractional_add);
					while (1)
					{
						if (fractional_add < (pow(2, 23) - 1))
						{
							fractional_add = fractional_add << 1;
							exponent_add = exponent_add - 1;
							//printf("while:\n");
							//printf("fractional_add: %llx\n", fractional_add);
							//printf("exponent_add: %d\n", exponent_add);
						}
						else if (fractional_add > (pow(2, 23) - 1) && fractional_add < pow(2, 24))
						{
							fractional_add = fractional_add | (1 << 23);
							break;
						}
						else if (fractional_add > pow(2, 24)) {
							fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
							exponent_add++;
							break;
						}
						else break;
					}
				}
				else
				{
					//printf("case9: һ��һ����aָ����bС����aС�����ֵ���bС�����֣�������䣬���Ϊ��ֵ\n");
					sign_bit_value = 1;
					exponent_add = float2Exp(a);
					fractional_add = 0;
				}
			}
		}
	}
	// TODO: ����жϣ����
	else if ((float2Sign(a) == 0) && (float2Sign(b) == 0)) // a,b��������
	{
		deltaE = float2Exp(a) - float2Exp(b);  // ָ��֮��
		sign_bit_value = 0;
		//printf("detlaE: %d\n", deltaE);
		if (deltaE > 0)   // aָ����bָ��С
		{
			//printf("case10: ��������aָ����b�󣬽������ܻ�䣬���Ϊ��ֵ\n");
			exponent_add = float2Exp(a);
			fractional_add = float2Frac(a) + (float2Frac(b) >> deltaE);
			//printf("fractional_add: %llx\n", fractional_add);
			if (fractional_add > pow(2, 24)) {
				fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);   // ���ʱС������������һλͬʱ�����λ��0(���)
				exponent_add++;
			}
			if (exponent_add > 126) {
				exponent_add = 127;
				fractional_add = 0.0;
			}
			if (exponent_add < -126) {
				return 0.0;
			}
		}
		else
		{
			//printf("case11,12: ��������aָ����bС����һ�����������ܻ�䣬���Ϊ��ֵ\n");
			exponent_add = float2Exp(b);
			//printf("float2Frac(a): %llx\n", float2Frac(a));
			//printf("float2Frac(b): %llx\n", float2Frac(b));
			fractional_add = float2Frac(b) + (float2Frac(a) >> (-deltaE));   // deltaEΪ����
			//printf("fractional_add: %llx\n", fractional_add);
			if (fractional_add > pow(2, 24)) {
				fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
				exponent_add++;
			}
			if (exponent_add > 126) {
				exponent_add = 127;
				fractional_add = 0.0;
			}
			if (exponent_add < -126) {
				return 0.0;
			}
		}
	}
	else  // a��b���Ǹ���
	{
		deltaE = float2Exp(a) - float2Exp(b);  // ָ��֮��
		sign_bit_value = 1;
		//printf("detlaE: %d\n", deltaE);
		if (deltaE > 0)   // aָ����bָ��С
		{
			//printf("case13: ��������aָ����b�󣬽������ܻ�䣬���Ϊ��ֵ\n");
			exponent_add = float2Exp(a);
			fractional_add = float2Frac(a) + (float2Frac(b) >> deltaE);
			// printf("fractional_add: %llx\n", fractional_add);
			if (fractional_add > pow(2, 24)) {
				fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
				exponent_add++;
			}
			if (exponent_add > 126) {
				exponent_add = 127;
				fractional_add = 0.0;
			}
			if (exponent_add < -126) {
				return 0.0;
			}
		}
		else
		{
			//printf("case14,15: ��������aָ����bС����һ�����������ܻ�䣬���Ϊ��ֵ\n");
			exponent_add = float2Exp(b);
			// printf("float2Frac(a): %llx\n", float2Frac(a));
			// printf("float2Frac(b): %llx\n", float2Frac(b));
			fractional_add = float2Frac(b) + (float2Frac(a) >> (-deltaE));   // deltaEΪ����
			// printf("fractional_add: %llx\n", fractional_add);
			if (fractional_add > pow(2, 24)) {
				fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);
				exponent_add++;
			}
			if (exponent_add > 126) {
				exponent_add = 127;
				fractional_add = 0.0;
			}
			if (exponent_add < -126) {
				return 0.0;
			}
		}
	}

	result = makeFloat(sign_bit_value, exponent_add, fractional_add);

	if (isnan(a) || isnan(b)) {
		return NAN;
	}
	if (isinf(a) || isinf(b)) {
		if (a != 0.0 || b != 0.0) {
			if (float2Sign(a) != float2Sign(b)) {
				return -INFINITY;
			}
			else {
				return INFINITY;
			}
		}
		if (a == 0.0 || b == 0.0) {
			return NAN;
		}
	}
	if (result > 0 && result < 1) {
		while (exponent_add > -126) {
			exponent_add = exponent_add - 1;
			fractional_add = fractional_add << 1;
		}
		while (exponent_add < -126) {
			exponent_add = exponent_add + 1;
			fractional_add = fractional_add >> 1;
		}

	}
	// printf("result sign: %d\n", float2Sign(result));
	return result;
}