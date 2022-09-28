#ifndef FLOATFUNCTION_H
#define FLOATFUNCTION_H

unsigned int bitRep(float x);

int float2BiasExp(float x);

long long float2Frac(float x);

int float2Exp(float x);

int float2Sign(float x);

float makeFloat(int sign, int exp, int frac);

void floatPrint(char const* pref, float a);

float floatProd(float a, float b);

float floatAdd(float a, float b);

#endif
