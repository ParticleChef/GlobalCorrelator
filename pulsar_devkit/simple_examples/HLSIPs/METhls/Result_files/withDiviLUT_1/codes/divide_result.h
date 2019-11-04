#ifndef DIVIDE_RESULT
#define DIVIDE_RESULT

#include "ap_fixed.h"

#define N_NUMERATOR 1000
#define N_DENOMINATOR 1000
//#define DIVI_TABLE_SIZE(x,y) int(x*((2*x)-(y))*0.5)

//unsigned int N_TOR = N_NUMERATOR*N_DENOMINATOR;
//double DIVI_TABLE_SIZE_ = N_TOR*(1+N_TOR)*0.5;
//unsigned int DIVI_TABLE_SIZE = (unsigned int)DIVI_TABLE_SIZE_;
#define DIVI_TABLE_SIZE 500500

// Type define (ap_fixed<X,Y>)
typedef ap_fixed<16,14> pt_t;

typedef ap_fixed<24,13> fixedXY_t;

typedef ap_fixed<18,5> temp_t;

// *************************************************
//       Division
// *************************************************

void divi_init(temp_t init_table[DIVI_TABLE_SIZE]){
	temp_t res;
	unsigned int index;
	init_table[0] = 0;
	for(int de = 1; de < N_DENOMINATOR+1; de++){
		for(int nu = 0; nu < de; nu++){
			res = (float)nu/de;
			index = de*(1+de)*0.5-nu;
			init_table[index] = res;
			//std::cout<<"init_table["<<index<<"] = "<<res<<std::endl;
		}   
	}   
}

temp_t divide_result(fixedXY_t numerator, pt_t denominator){
	temp_t output[DIVI_TABLE_SIZE];
	divi_init(output);
	if(numerator < 0) numerator = -numerator;
	temp_t result;
	temp_t half_ = 0.5;
	unsigned int INDEX = denominator*(1+denominator)*half_-numerator;
	if(INDEX > DIVI_TABLE_SIZE) INDEX = DIVI_TABLE_SIZE-1;
	result = output[INDEX];
	std::cout<<"division result: "<<result<<std::endl;
	return result;
}

#endif
