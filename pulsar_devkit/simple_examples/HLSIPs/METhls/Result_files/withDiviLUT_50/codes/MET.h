#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_math.h"

// size of the LUT
#define N_TABLE_SIZE 1024
#define SINE_RANGE 2

#define N_NUMERATOR 1000
#define N_DENOMINATOR 1000
//#define DIVI_TABLE_SIZE(x,y) int(x*((2*x)-(y))*0.5)

//unsigned int N_TOR = N_NUMERATOR*N_DENOMINATOR;
//double DIVI_TABLE_SIZE_ = N_TOR*(1+N_TOR)*0.5;
//unsigned int DIVI_TABLE_SIZE = (unsigned int)DIVI_TABLE_SIZE_;
#define DIVI_TABLE_SIZE 500500

// Type define (ap_fixed<X,Y>)
typedef ap_fixed<16,14> pt_t;
typedef ap_fixed<16,9> etaphi_t;
typedef ap_fixed<10,3> sincos_t;
typedef ap_fixed<12,4> rad_t;

typedef ap_fixed<24,13> fixedXY_t;
#define DIVI_SIZE 18
typedef ap_fixed<DIVI_SIZE,5> temp_t;


#define PI_VAL temp_t(3.1415)

#define TotalN 50
#define NEVENT 10000

struct activ_config{
	static const unsigned n_in = TotalN;
	static const unsigned table_size = 1024;
	typedef ap_fixed<18,8> table_t;
};

void MET_ref( double allPT_ref[TotalN], double &missPT_ref, double allPhi_ref[TotalN], double &missPhi_ref );
void MET_hw(  pt_t allPT_hw[TotalN],  pt_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  etaphi_t &missPhi_hw );

// *************************************************
//       Accosine
// *************************************************
template<class data_T, int N_TABLE>
void init_acos_table(data_T table_out[N_TABLE]) {
	int INDEX = 0;
	//	Implement tanh lookup
	for (int ii = N_TABLE-1; ii > -1; ii--) {
		// Convert from table index to X-value (unsigned 4-bit, range 0 to +4)
		float in_val = (SINE_RANGE)*((N_TABLE-1)-ii)/float(N_TABLE) - 1;
		// Next, compute lookup table function
		data_T real_val = acos(in_val);
		//std::cout<<"("<<INDEX<<")in_val = "<<in_val<<", real_val = "<<real_val<<std::endl;
		table_out[INDEX] = real_val;
		INDEX++;
	}
	return;
}
template<class data_T, class res_T, int TABLE_SIZE>//=1024>
void acos(data_T &data, res_T &res) {
	// Initialize the lookup table
	res_T acos_table[TABLE_SIZE];
	init_acos_table<res_T, TABLE_SIZE>(acos_table);

	// Index into the lookup table based on data
	int index;

#pragma HLS PIPELINE
	index = ((data+1)/SINE_RANGE)*TABLE_SIZE;
	//std::cout<<"index = : "<<index<<std::endl;
	if (index < 0) index = 0;
	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
	res = acos_table[index];
	//std::cout<<"acos_table[index] = "<<res<<std::endl;

	return;
}

// Default table size provided here:
template<class data_T, class res_T>
void acos(data_T &data, res_T &res) { 
	// Get the acos value from the LUT 
		acos<data_T, res_T, N_TABLE_SIZE>(data, res); 
	return;
}

// *************************************************
//       Cosine tables
// *************************************************
template<typename CONFIG_T, int N_TABLE>
void init_cos_table(typename CONFIG_T::table_t table_out[N_TABLE]) {
	int INDEX = 0;
	//  Implement cos lookup
	//for (int ii = N_TABLE-1; ii > -1; ii--) {
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Convert from table index to X-value
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE; 

		// Next, compute lookup table function
		sincos_t real_val = cos(in_val);
		table_out[ii] =  real_val;
		//std::cout<<"("<<ii<<") cos("<<in_val<<") = "<<real_val<<std::endl;
		INDEX++;
	}   
}
template<class T_co, class rT_co, typename CONFIG_T>
void Cos(T_co &data, rT_co &res) {
	// Initialize the lookup table
	typename CONFIG_T::table_t cos_table[CONFIG_T::table_size];
	init_cos_table<CONFIG_T, CONFIG_T::table_size>(cos_table);

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	rad_t data_rad = data * temp_t(0.01745); // (PI_VAL/180);
	index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;

	if (index < 0) index = 0;
	if (index > CONFIG_T::table_size-1) index = CONFIG_T::table_size-1;
	res = (rT_co) cos_table[index];
	//std::cout<<"cos_table["<<index<<"] = cos("<<data_rad<<") ="<<res<<","<<cos_table[index]<<std::endl;
}
//// *************************************************
////       Sine tables
//// *************************************************
template<typename CONFIG_T, int N_TABLE>
void init_sin_table(typename CONFIG_T::table_t table_out[N_TABLE]) {
	int INDEX = 0;
	//  Implement cos lookup
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Convert from table index to X-value
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE;

		// Next, compute lookup table function
		sincos_t real_val = sin(in_val);
		table_out[ii] =  real_val;
		//std::cout<<"("<<ii<<") cos("<<in_val<<") = "<<real_val<<std::endl;
		INDEX++;
	}   
	return;
}
template<class T_co, class rT_co, typename CONFIG_T>
void Sin(T_co &data, rT_co &res) {
	// Initialize the lookup table
	typename CONFIG_T::table_t sin_table[CONFIG_T::table_size];
	init_sin_table<CONFIG_T, CONFIG_T::table_size>(sin_table);

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	rad_t data_rad = data * temp_t(0.01745); // (PI_VAL/180);
	index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;

	if (index < 0) index = 0;
	if (index > CONFIG_T::table_size-1) index = CONFIG_T::table_size-1;
	res = (rT_co) sin_table[index];
	//std::cout<<"sin_table["<<index<<"] = sin("<<data_rad<<") ="<<res<<","<<sin_table[index]<<std::endl;

	return;
}
// *************************************************
//       Division
// *************************************************

//void divi_init(temp_t init_table[DIVI_TABLE_SIZE]){
//	temp_t res;
//	unsigned int index;
//	init_table[0] = 0;
//	for(int de = 1; de < N_DENOMINATOR+1; de++){
//		for(int nu = 0; nu < de; nu++){
//			res = (float)nu/(float)de;
//			index = de*(1+de)*0.5-nu;
//			init_table[index] = res;
//			std::cout<<"init_table["<<index<<"] = "<<res<<std::endl;
//		}   
//	}   
//}
//
//temp_t divide_result(fixedXY_t numerator, pt_t denominator){
//	temp_t output[N_NUMERATOR*N_DENOMINATOR];
//	divi_init(output);
//	temp_t result;
//	temp_t half_ = 0.5;
//	unsigned int INDEX = denominator*(1+denominator)*half_-numerator;
//	result = output[INDEX];
//	printf("division result: %f \n", result);
//	return result;
//}

template<typename in_T, typename ou_T>
void Sqsqrt(in_T &Inp1, in_T &Inp2, ou_T &Out){
	double Out_res = 0.;
	Out_res = Inp1*Inp1 + Inp2*Inp2;
	Out =  sqrt(Out_res);
}

#endif
