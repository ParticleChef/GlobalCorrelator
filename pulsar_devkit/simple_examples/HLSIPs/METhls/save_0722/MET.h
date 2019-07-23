#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"
#include "ap_fixed.h"

// size of the LUT
#define N_TABLE_SIZE_NUM 1533 //Maximum number is 2045 for some reason (SIGSEGV otherwise)
#define N_TABLE_SIZE_DEN 1533 //Maximum number is 2045 for some reason (SIGSEGV otherwise)
#define N_TABLE_SIZE 1024
#define SINE_RANGE 2

typedef ap_int<16> pt_t;
typedef ap_int<10> etaphi_t;
//typedef ap_uint<16> val_t;

// Type used for LUT (ap_fixed<X,Y>)
#define AP_FIXED_SIZE 16
#define AP_FIXED_DEC 12
typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> fixed10_t;
typedef ap_fixed<16,5> fixed5_t;
typedef ap_fixed<24,13> fixedXY_t;

//typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> pt_t;
//typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> etaphi_t;

#define TotalN 10
#define NEVENT 10000

//fixed10_t t_PI = 57.32; // 180/3.14
//void MET_ref( ap_int<16> allPT_ref[TotalN], ap_int<16> &missPT_ref, ap_int<10> allPhi_ref[TotalN], ap_int<10> &missPhi_ref );
//void MET_hw(  ap_int<16> allPT_hw[TotalN], ap_int<16> &missPT_hw, ap_int<10> allPhi_hw[TotalN], ap_int<10> &missPhi_hw );

//void MET_ref( pt_t allPT_ref[TotalN], fixed10_t &missPT_ref, etaphi_t allPhi_ref[TotalN], fixed10_t &missPhi_ref );
void MET_ref( double allPT_ref[TotalN], double &missPT_ref, double allPhi_ref[TotalN], double &missPhi_ref );
void MET_hw(  pt_t allPT_hw[TotalN],  fixed10_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  fixed10_t &missPhi_hw );

/**/
// *************************************************
//       Acosine
// *************************************************
template<class data_T, int N_TABLE>
void init_acos_table(data_T table_out[N_TABLE]) {
	int INDEX = 0;
	//	Implement tanh lookup
//	for (int ii = 0; ii < N_TABLE; ii++) {
	for (int ii = N_TABLE-1; ii > -1; ii--) {
		// Original: 
		// First, convert from table index to X-value (signed 8-bit, range -4 to +4)
		//float in_val = 2*4.0*(ii-float(N_TABLE)/2.0)/float(N_TABLE);

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
	data_T datareg;
	int index;

#pragma HLS PIPELINE

	//index = (1-data/SINE_RANGE)*TABLE_SIZE; // original
	index = ((data+1)/SINE_RANGE)*TABLE_SIZE;
	std::cout<<"index = : "<<index<<std::endl;

	if (index < 0) index = 0;
	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
	res = acos_table[index];
	std::cout<<"acos_table[index] = "<<res<<std::endl;

	return;
}

// Default table size provided here:
template<class data_T, class res_T>
void acos(data_T &data, res_T &res) { 
	// Get the value from the LUT 
		acos<data_T, res_T, N_TABLE_SIZE>(data, res); 
	return;
}

// *************************************************
//       Math tables
// *************************************************
//template<class data_T, int N_TABLE>
//void init_cos_table(data_T table_out[N_TABLE]) {
//	//	Implement cos lookup
//	for (int ii = 0; ii < N_TABLE; ii++) {
//		// Convert from table index to X-value (unsigned 4-bit, range 0 to +4)
//		float in_val = (SINE_RANGE)*((N_TABLE-1)-ii)/float(N_TABLE);
//
//		// Next, compute lookup table function
//		data_T real_val = cos(in_val);
//		table_out[ii] = real_val;
//	}
//	return;
//}
//template<class data_T, class res_T, int TABLE_SIZE>//=1024>
//void Cos(data_T &data, res_T &res) {
//	// Initialize the lookup table
//	res_T cos_table[TABLE_SIZE];
//	init_cos_table<res_T, TABLE_SIZE>(cos_table);
//
//	// Index into the lookup table based on data
//	data_T datareg;
//	int index;
//
//#pragma HLS PIPELINE
//
//	index = (1-data/SINE_RANGE)*TABLE_SIZE;
//	std::cout<<"index = "<<index<<std::endl;
//
//	if (index < 0) index = 0;
//	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
//	res = cos_table[index];
//	std::cout<<"cos_table[index] = "<<res<<std::endl;
//
//	return;
//}

template<typename T_co, typename rT_co>
void Cos(T_co &Inp, rT_co &Out){
	double Input_co = 0.;
	Input_co = Inp * (M_PI / 180);
	//std::cout<<"Input_cos: "<<Input_co<<std::endl;
	Out = cos(Input_co);
}

////// Sin table /////
//template<class data_T, int N_TABLE>
//void init_sin_table(data_T table_out[N_TABLE]) {
//	//	Implement cos lookup
//	for (int ii = 0; ii < N_TABLE; ii++) {
//		// Convert from table index to X-value (unsigned 4-bit, range 0 to +4)
//		float in_val = (SINE_RANGE)*((N_TABLE-1)-ii)/float(N_TABLE);
//
//		// Next, compute lookup table function
//		data_T real_val = sin(in_val);
//		table_out[ii] = real_val;
//	}
//	return;
//}
//template<class data_T, class res_T, int TABLE_SIZE>//=1024>
//void Sin(data_T &data, res_T &res) {
//	// Initialize the lookup table
//	res_T sin_table[TABLE_SIZE];
//	init_sin_table<res_T, TABLE_SIZE>(sin_table);
//
//	// Index into the lookup table based on data
//	data_T datareg;
//	int index;
//
//#pragma HLS PIPELINE
//
//	index = (1-data/SINE_RANGE)*TABLE_SIZE;
//
//	if (index < 0) index = 0;
//	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
//	res = sin_table[index];
//
//	return;
//}
template<typename T_si, typename rT_si>
void Sin(T_si &Inp, rT_si &Out){
	double Input_si = 0.;
	Input_si = Inp * (M_PI / 180);
	//std::cout<<"Input_sin: "<<Input_si<<std::endl;
	Out = sin(Input_si);
}

template<typename in_T, typename ou_T>
void Sqsqrt(in_T &Inp1, in_T &Inp2, ou_T &Out){
	double Out_res = 0.;
	Out_res = Inp1*Inp1 + Inp2*Inp2;
	Out =  sqrt(Out_res);
}

/**/

#endif
