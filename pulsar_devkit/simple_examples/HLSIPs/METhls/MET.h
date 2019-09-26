#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_math.h"

// size of the LUT
#define N_TABLE_SIZE 1024
#define SINE_RANGE 2

#define N_NUMERATOR 1500
#define N_DENOMINATOR 1500
#define DIVI_TABLE_SIZE(x,y) int(x*((2*x)-(y))*0.5)

// Type define (ap_fixed<X,Y>)
//typedef ap_int<16> pt_t;
typedef ap_fixed<16,14> pt_t;
typedef ap_fixed<16,9> etaphi_t;
typedef ap_fixed<10,3> sincos_t;
typedef ap_fixed<12,4> rad_t;

typedef ap_fixed<16,12> fixed10_t;
typedef ap_fixed<16,5> fixed5_t;
typedef ap_fixed<24,13> fixedXY_t;

typedef ap_fixed<18,5> temp_t;


#define PI_VAL temp_t(3.1415)

#define TotalN 10
#define NEVENT 3

struct activ_config{
	static const unsigned n_in = TotalN;
	static const unsigned table_size = 1024;
	//static const unsigned NUME_size = N_NUMERATOR;
	//static const unsigned DENO_size = N_DENOMINATOR;
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
		//float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/N_TABLE) - PI_VAL; 
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
	//rT_co cos_table[CONFIG_T::table_size];

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	rad_t data_rad = data * temp_t(0.01745); // (PI_VAL/180);
	//std::cout<<"data = "<<data_rad<<std::endl;
	index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;
	//std::cout<<"cos index = "<<index<<std::endl;

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
	//for (int ii = N_TABLE-1; ii > -1; ii--) {
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Convert from table index to X-value
		//float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/N_TABLE) - PI_VAL; 
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE;

		// Next, compute lookup table function
		sincos_t real_val = sin(in_val);
		table_out[ii] =  real_val;
		//std::cout<<"("<<ii<<") cos("<<in_val<<") = "<<real_val<<std::endl;
		INDEX++;
	}   
}
template<class T_co, class rT_co, typename CONFIG_T>
void Sin(T_co &data, rT_co &res) {
	// Initialize the lookup table
	typename CONFIG_T::table_t sin_table[CONFIG_T::table_size];
	init_sin_table<CONFIG_T, CONFIG_T::table_size>(sin_table);
	//rT_co cos_table[CONFIG_T::table_size];
	//init_cos_table<rT_co, CONFIG_T::table_size>(cos_table);

	// Index into the lookup table based on data
	int index;
#pragma HLS PIPELINE

	rad_t data_rad = data * temp_t(0.01745); // (PI_VAL/180);
	//std::cout<<"data = "<<data_rad<<std::endl;
	index = ((data_rad+PI_VAL)/(2*PI_VAL))*CONFIG_T::table_size;
	//std::cout<<"cos index = "<<index<<std::endl;

	if (index < 0) index = 0;
	if (index > CONFIG_T::table_size-1) index = CONFIG_T::table_size-1;
	res = (rT_co) sin_table[index];
	//std::cout<<"sin_table["<<index<<"] = sin("<<data_rad<<") ="<<res<<","<<sin_table[index]<<std::endl;

}
// *************************************************
//       Division
// *************************************************
void divi_init(temp_t init_table[N_NUMERATOR*N_DENOMINATOR]){
	temp_t res = 0.; 
	int index = 0;
	for(float nu = 0; nu < N_NUMERATOR; nu++){
		for(float de = nu; de < N_DENOMINATOR; de++){
			if (de == 0) res = 0;
			else
				res =temp_t( nu/de );
			index = nu*N_NUMERATOR + de; 
			init_table[index] = res;

			//printf("Table[%d] = %f / %f =  %f\n", index,nu, de, res);

		}   
	}   
}
temp_t division(fixedXY_t numerator, pt_t denominator){
	temp_t output[N_NUMERATOR*N_DENOMINATOR];
	divi_init(output);
	temp_t result;
	int INDEX = numerator*N_NUMERATOR + denominator;
	
	result = output[INDEX];
	std::cout<<"division result: "<<result<<std::endl;
	//printf("division result: %f \n", result);
	return result;
}



template<typename divi_T >
void divi_init(divi_T init_table[N_NUMERATOR*N_DENOMINATOR]){
	divi_T res = 0;
	int index = 0;
	std::cout<<"done?"<<std::endl;
	for(float nu = 0; nu < N_NUMERATOR; nu++){
		for(float de = nu; de < N_DENOMINATOR; de++){
			if(de == 0) res = 0;
			else res = nu/de;
			index = nu*N_NUMERATOR + de;
			init_table[index] = res;
		}
	}
}
template<typename Nu_T, typename De_T, typename res_T>
res_T division(Nu_T numerator, De_T denominator){
	res_T output[N_NUMERATOR*N_DENOMINATOR];
	divi_init<res_T>(output);
	res_T result;

	if(numerator < 0) numerator = -1*numerator;
	if(denominator < 0) denominator = -1*denominator;

	int INDEX = numerator*N_NUMERATOR + denominator;
	result = output[INDEX];
	std::cout<<"division result: "<<result<<std::endl;

	return result;
}



//template<class data_T, int NUMERATOR_SIZE, int DENOMINATOR_SIZE>
////void init_division_table(data_T table_out[DIVI_TABLE_SIZE(DENOMINATOR_SIZE,1)]){
//void init_division_table(data_T table_out[2250001]){
//	for(int nu = 0; nu < NUMERATOR_SIZE; nu++){
//		for(int de = 0; de < DENOMINATOR_SIZE; de++){
//			int INDEX = (nu*NUMERATOR_SIZE)+de;
//			float real_val = (de==0) ? 0: (float) nu / de;
//			if( nu == 1 ){ std::cout<<"start init division"<<std::endl; }
//
//			table_out[INDEX] = real_val;
//
//		}
//		if( nu == 1 ) std::cout<<"end first loop"<<std::endl;
//	}
//	std::cout<<"finish init lut"<<std::endl;
//}
//template<class T_nu, class T_de, class T_res, int NUME_SIZE, int DENO_SIZE>
//void Division(T_nu &nume, T_de &deno, T_res &res){
//	T_res division_table[2250001];
//	init_division_table<T_res, NUME_SIZE, DENO_SIZE>(division_table);
//
//	int index;
//
//	bool is_negative = false;
//	if(nume*deno <0) is_negative = true;
//
//	if(nume < 0) nume = -nume;
//	if(deno < 0) deno = -deno;
//	if(nume > NUME_SIZE-1) nume = NUME_SIZE-1;
//	if(deno > DENO_SIZE-1) deno = DENO_SIZE-1;
//
//	index = (nume*N_NUMERATOR) + deno;
//	if(is_negative) res = -division_table[index];
//	if(!is_negative) res = division_table[index];
//}
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
