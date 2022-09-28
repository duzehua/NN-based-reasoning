#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>


unsigned int bitRep(float x) {  // 返回的是无符号的整数，与float2BiasExp配合得到无偏指数
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


// windows下long为32位，long long为64位，函数返回x的小数部分乘以2^23
long long float2Frac(float x) { // returns the fractional part of x times 2^23
	// Special cases
	if (isinf(x)) return 0;
	if (isnan(x)) return 1;
	// return the rightmost 23 bits of the bit representation of x
	unsigned int xi = bitRep(x);
	long long frac = xi & ((1 << 23) - 1); // May be denormal!
	if (isNormal(x) == 0) return frac; // denormal... no 1.
	return frac | (1 << 23); // Or in the 1. in front of the decimal!（规格化）
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
	// 防止溢出，进行规格化
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

	// TODO: 一正一负相加，最终结果如何计算？
	if (float2Sign(a) != float2Sign(b))
	{
		// 如果a为负，b为正，交换两者顺序即可
		if (float2Sign(b) == 0)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		deltaE = float2Exp(a) - float2Exp(b);  // 指数之差

		if (float2Sign(a) == 0)   // a为正数，b为负数
		{
			//printf("deltaE: %d\n", deltaE);
			if (deltaE > 0)   // a指数比b大
			{
				sign_bit_value = 0;  //和为正数				
				// TODO: 当两者之差很小时，最终结果指数值如何取？
				exponent_add = float2Exp(a);
				if ((float2Frac(a) - (float2Frac(b) >> deltaE)) > 0)  // a小数部分大于b小数部分,最终结果应该为正值
				{
					//printf("case1: 一正一负，a的指数比b大，且a小数部分大于b小数部分，阶数不变，结果为正值\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					exponent_add = float2Exp(a);
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = float2Frac(a) - (b_frac >> deltaE);   // 小的数先对阶, 做减法得到小数部分的和
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
				// TODO: 未完成，需要借位做减法
				else if ((float2Frac(a) - (float2Frac(b) >> deltaE)) < 0)
				{
					//printf("case2: 一正一负，a的指数比b大，但a小数部分小于b小数部分，需要借位减法，阶数会变，结果为正值\n");
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = (b_frac >> deltaE) - float2Frac(a);   // 小的数先对阶, 做减法得到小数部分的和
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
					//printf("case3: 一正一负，a的指数比b大，且a小数部分等于b小数部分，需要借位减法，阶数会变，结果为正值\n");
					sign_bit_value = 0;
					exponent_add = float2Exp(b);   // 这种情况a正好比b多一个2^exp(b)
					fractional_add = 0;
				}
			}
			// TODO: 指数相同，两者之差很小，最终结果的指数值如何取？
			else if (deltaE == 0)  // a,b指数相同
			{
				if ((float2Frac(a) - float2Frac(b)) > 0)  // a小数部分大于b小数部分,最终结果应该为正值
				{
					//printf("case4: 一正一负，a、b指数相同，且a小数部分大于b小数部分，阶数可能会变，结果为正值\n");
					sign_bit_value = 0;
					exponent_add = float2Exp(a);
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
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
				// TODO: 未完成，需要借位做减法
				else if ((float2Frac(a) - float2Frac(b)) < 0) // a小数部分小于b小数部分,最终结果应该为负值
				{
					//printf("case5: 一正一负，a、b指数相同，但a小数部分小于b小数部分，阶数会变，结果为负值\n");
					sign_bit_value = 1;
					exponent_add = float2Exp(a);
					//fractional_add = float2Frac(b) - float2Frac(a);  // 和的小数部分应为b-a
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
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
					//printf("case6: 一正一负，a、b指数相同，且a、b小数部分相同，结果为0\n");
					sign_bit_value = 0;
					exponent_add = 0;
					fractional_add = 0;
				}
			}
			else   // a指数比b小，最终结果为负数
			{

				sign_bit_value = 1;
				exponent_add = float2Exp(b);
				if (((float2Frac(a) >> (-deltaE)) - float2Frac(b)) > 0)  // a移位后小数部分大于b小数部分
				{
					//printf("case7: 一正一负，a指数比b小，但a小数部分大于b小数部分，阶数可能会变，结果为负值\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a) >> (-deltaE));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = (float2Frac(a) >> (-deltaE)) - b_frac;   // 小的数先对阶, 做减法得到小数部分的和
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
				// TODO: 未完成，需要借位做减法
				else if (((float2Frac(a) >> (-deltaE)) - float2Frac(b)) < 0)     // a移位后小数部分小于b小数部分
				{
					//printf("case8: 一正一负，a指数比b小，且a小数部分小于b小数部分，阶数可能会变，结果为负值\n");
					//printf("float2Frac(a): %llx\n", float2Frac(a) >> (-deltaE));
					//printf("float2Frac(b): %llx\n", float2Frac(b));
					unsigned long long b_frac = 0;    // 设置一个临时值保存b的小数部分
					b_frac = float2Frac(b) ^ ((long long)1 << 24) & (((long long)1 << 24) - 1);  // 与2^24异或，取b的补数,再消除最高位的1
					//printf("b_frac: %llx\n", b_frac);
					fractional_add = b_frac - (float2Frac(a) >> (-deltaE));   // 小的数先对阶, 做减法得到小数部分的和
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
					//printf("case9: 一正一负，a指数比b小，但a小数部分等于b小数部分，阶数会变，结果为负值\n");
					sign_bit_value = 1;
					exponent_add = float2Exp(a);
					fractional_add = 0;
				}
			}
		}
	}
	// TODO: 溢出判断，规格化
	else if ((float2Sign(a) == 0) && (float2Sign(b) == 0)) // a,b都是正数
	{
		deltaE = float2Exp(a) - float2Exp(b);  // 指数之差
		sign_bit_value = 0;
		//printf("detlaE: %d\n", deltaE);
		if (deltaE > 0)   // a指数比b指数小
		{
			//printf("case10: 两正数，a指数比b大，阶数可能会变，结果为正值\n");
			exponent_add = float2Exp(a);
			fractional_add = float2Frac(a) + (float2Frac(b) >> deltaE);
			//printf("fractional_add: %llx\n", fractional_add);
			if (fractional_add > pow(2, 24)) {
				fractional_add = ((fractional_add >> 1) & ((long long)1 << 24) - 1);   // 溢出时小数部分向右移一位同时将最高位置0(规格化)
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
			//printf("case11,12: 两正数，a指数比b小或者一样，阶数可能会变，结果为正值\n");
			exponent_add = float2Exp(b);
			//printf("float2Frac(a): %llx\n", float2Frac(a));
			//printf("float2Frac(b): %llx\n", float2Frac(b));
			fractional_add = float2Frac(b) + (float2Frac(a) >> (-deltaE));   // deltaE为负数
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
	else  // a，b都是负数
	{
		deltaE = float2Exp(a) - float2Exp(b);  // 指数之差
		sign_bit_value = 1;
		//printf("detlaE: %d\n", deltaE);
		if (deltaE > 0)   // a指数比b指数小
		{
			//printf("case13: 两负数，a指数比b大，阶数可能会变，结果为负值\n");
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
			//printf("case14,15: 两负数，a指数比b小或者一样，阶数可能会变，结果为负值\n");
			exponent_add = float2Exp(b);
			// printf("float2Frac(a): %llx\n", float2Frac(a));
			// printf("float2Frac(b): %llx\n", float2Frac(b));
			fractional_add = float2Frac(b) + (float2Frac(a) >> (-deltaE));   // deltaE为负数
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