#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"
#include "ap_fixed.h"

// size of the LUT
#define N_TABLE_SIZE 1024
#define SINE_RANGE 2

//typedef ap_int<16> pt_t;
//typedef ap_int<10> etaphi_t;
//typedef ap_fixed<16,10> pt_t;
//typedef ap_fixed<16,10> etaphi_t;

// Type define (ap_fixed<X,Y>)
typedef ap_int<16> pt_t;
typedef ap_fixed<18,9> etaphi_t;
typedef ap_fixed<10,3> sincos_t;
typedef ap_fixed<12,4> rad_t;

typedef ap_fixed<16,12> fixed10_t;
typedef ap_fixed<16,5> fixed5_t;
typedef ap_fixed<24,13> fixedXY_t;

typedef ap_fixed<18,5> temp_t;


#define PI_VAL temp_t(3.1415)

#define TotalN 15
#define NEVENT 10000

struct activ_config{
	static const unsigned n_in = 10;
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
//	for (int ii = 0; ii < N_TABLE; ii++) {
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
	//index = (1-data/SINE_RANGE)*TABLE_SIZE; // original
	index = ((data+1)/SINE_RANGE)*TABLE_SIZE;
	std::cout<<"index = : "<<index<<std::endl;

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
		//float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/N_TABLE) - PI_VAL; 
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE; 

		// Next, compute lookup table function
		//typename CONFIG_T::table_t real_val = cos(in_val);
		sincos_t real_val = cos(in_val);
		//table_out[INDEX] =  real_val;
		table_out[ii] =  real_val;
		//std::cout<<"("<<ii<<") cos("<<in_val<<") = "<<real_val<<std::endl;
		INDEX++;
	}   
}
template<class T_co, class rT_co, typename CONFIG_T>
void Cos(T_co data[CONFIG_T::n_in], rT_co res[CONFIG_T::n_in]) {
	// Initialize the lookup table
	typename CONFIG_T::table_t cos_table[CONFIG_T::table_size];
	init_cos_table<CONFIG_T, CONFIG_T::table_size>(cos_table);
	//rT_co cos_table[CONFIG_T::table_size];
	//init_cos_table<rT_co, CONFIG_T::table_size>(cos_table);

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	for(int ii=0; ii < CONFIG_T::n_in; ii++){
		rad_t data_rad = data[ii] * temp_t(0.01745); // (PI_VAL/180);
		std::cout<<"data = "<<data_rad<<std::endl;
		index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;
		//std::cout<<"cos index = "<<index<<std::endl;

		if (index < 0) index = 0;
		if (index > CONFIG_T::table_size-1) index = CONFIG_T::table_size-1;
		res[ii] = (rT_co) cos_table[index];
		std::cout<<"cos_table["<<index<<"] = cos("<<data_rad<<") ="<<res[ii]<<","<<cos_table[index]<<std::endl;
	}
}
//// *************************************************
////       Sine tables
//// *************************************************
template<typename CONFIG_T, int N_TABLE>
void init_sin_table(typename CONFIG_T::table_t table_out[N_TABLE]) {
	int INDEX = 0;
	//  Implement cos lookup
	//for (int ii = N_TABLE-1; ii > -1; ii--) {
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Convert from table index to X-value
		//float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/N_TABLE) - PI_VAL; 
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE;

		// Next, compute lookup table function
		//typename CONFIG_T::table_t real_val = cos(in_val);
		sincos_t real_val = sin(in_val);
		//table_out[INDEX] =  real_val;
		table_out[ii] =  real_val;
		//std::cout<<"("<<ii<<") cos("<<in_val<<") = "<<real_val<<std::endl;
		INDEX++;
	}   
}
template<class T_co, class rT_co, typename CONFIG_T>
void Sin(T_co data[CONFIG_T::n_in], rT_co res[CONFIG_T::n_in]) {
	// Initialize the lookup table
	typename CONFIG_T::table_t sin_table[CONFIG_T::table_size];
	init_sin_table<CONFIG_T, CONFIG_T::table_size>(sin_table);
	//rT_co cos_table[CONFIG_T::table_size];
	//init_cos_table<rT_co, CONFIG_T::table_size>(cos_table);

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	for(int ii=0; ii < CONFIG_T::n_in; ii++){
		rad_t data_rad = data[ii] * temp_t(0.01745); // (PI_VAL/180);
		std::cout<<"data = "<<data_rad<<std::endl;
		index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;
		//std::cout<<"cos index = "<<index<<std::endl;

		if (index < 0) index = 0;
		if (index > CONFIG_T::table_size-1) index = CONFIG_T::table_size-1;
		res[ii] = (rT_co) sin_table[index];
		std::cout<<"sin_table["<<index<<"] = sin("<<data_rad<<") ="<<res[ii]<<","<<sin_table[index]<<std::endl;
	}
}
// *************************************************
//       Cosine tables
// *************************************************
//template<class rT_co, int N_TABLE>
//void init_cos_table(rT_co table_out[N_TABLE]) {
//	int INDEX = 0;
//	//	Implement cos lookup
//	for (int ii = N_TABLE-1; ii > -1; ii--) {
//		// Convert from table index to X-value
//		float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/float(N_TABLE)) - PI_VAL;
//
//		// Next, compute lookup table function
//		rT_co real_val = cos(in_val);
//		table_out[INDEX] =  real_val;
//		//std::cout<<"("<<INDEX<<") cos("<<in_val<<") = "<<real_val<<std::endl;
//		INDEX++;
//	}
//	return;
//}
//template<class T_co, class rT_co, int TABLE_SIZE>
//void Cos(T_co &data, rT_co &res) {
//	// Initialize the lookup table
//	rT_co cos_table[TABLE_SIZE];
//	init_cos_table<rT_co, TABLE_SIZE>(cos_table);
//
//	// Index into the lookup table based on data
//	int index;
//
//#pragma HLS PIPELINE
//
//	double data_rad = data * 0.0174; // (PI_VAL/180);
//	//std::cout<<"data = "<<data_rad<<std::endl;
//	index = ((data_rad+PI_VAL)/(2*PI_VAL))*TABLE_SIZE;
//	//std::cout<<"cos index = "<<index<<std::endl;
//
//	if (index < 0) index = 0;
//	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
//	res = cos_table[index];
//	//std::cout<<"cos_table["<<index<<"] = "<<res<<std::endl;
//
//	return;
//}
//
//template<class T_co, class rT_co>
//void Cos(T_co &Inp, rT_co &Out){
//	//double Input_co = 0.;
//	//Input_co = Inp * (M_PI / 180);
//	Cos<T_co, rT_co, N_TABLE_SIZE>(Inp, Out);
//	//std::cout<<"Input_cos: "<<Input_co<<std::endl;
//	//Out = cos(Input_co);
//}
//
//// *************************************************
////       Sine tables
//// *************************************************
//template<class rT_co, int N_TABLE>
//void init_sin_table(rT_co table_out[N_TABLE]) {
//	int INDEX = 0;
//	//	Implement cos lookup
//	for (int ii = N_TABLE-1; ii > -1; ii--) {
//		// Convert from table index to X-value
//		float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/float(N_TABLE)) - PI_VAL;
//
//		// Next, compute lookup table function
//		rT_co real_val = sin(in_val);
//		table_out[INDEX] = real_val;
//		//std::cout<<"("<<INDEX<<") sin("<<in_val<<") = "<<real_val<<std::endl;
//		INDEX++;
//	}
//	return;
//}
//template<class T_co, class rT_co, int TABLE_SIZE>
//void Sin(T_co &data, rT_co &res) {
//	// Initialize the lookup table
//	rT_co sin_table[TABLE_SIZE];
//	init_sin_table<rT_co, TABLE_SIZE>(sin_table);
//
//	// Index into the lookup table based on data
//	int index;
//
//#pragma HLS PIPELINE
//
//	double data_rad = data * 0.0174; // (PI_VAL/180);
//	//std::cout<<"data = "<<data_rad<<std::endl;
//	index = ((data_rad+PI_VAL)/(2*PI_VAL))*TABLE_SIZE;
//	//std::cout<<"sin index = "<<index<<std::endl;
//
//	if (index < 0) index = 0;
//	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
//	res = sin_table[index];
//	//std::cout<<"sin_table["<<index<<"] = "<<res<<std::endl;
//
//	return;
//}
//
//template<class T_co, class rT_co>
//void Sin(T_co &Inp, rT_co &Out){
//	//double Input_co = 0.;
//	//Input_co = Inp * (M_PI / 180);
//	Sin<T_co, rT_co, N_TABLE_SIZE>(Inp, Out);
//	//std::cout<<"Input_cos: "<<Input_co<<std::endl;
//	//Out = cos(Input_co);
//}
//

//template<typename T_si, typename rT_si>
//void Sin(T_si &Inp, rT_si &Out){
//	double Input_si = 0.;
//	Input_si = Inp * (M_PI / 180);
//	//std::cout<<"Input_sin: "<<Input_si<<std::endl;
//	Out = sin(Input_si);
//}

template<typename in_T, typename ou_T>
void Sqsqrt(in_T &Inp1, in_T &Inp2, ou_T &Out){
	double Out_res = 0.;
	Out_res = Inp1*Inp1 + Inp2*Inp2;
	Out =  sqrt(Out_res);
}

/**/

#endif
