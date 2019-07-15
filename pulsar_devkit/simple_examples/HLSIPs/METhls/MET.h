#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"
#include "ap_fixed.h"

// size of the LUT
#define N_TABLE_SIZE_NUM 1533 //Maximum number is 2045 for some reason (SIGSEGV otherwise)
#define N_TABLE_SIZE_DEN 1533 //Maximum number is 2045 for some reason (SIGSEGV otherwise)
#define N_TABLE_SIZE 1024
#define SINE_RANGE 4

typedef ap_int<16> pt_t;
typedef ap_int<10> etaphi_t;

// Type used for LUT (ap_fixed<X,Y>)
#define AP_FIXED_SIZE 16
#define AP_FIXED_DEC 10
//typedef ap_uint<16> val_t;
typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> fixed10_t;
typedef ap_fixed<16,5> fixed5_t;

//typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> pt_t;
//typedef ap_fixed<AP_FIXED_SIZE,AP_FIXED_DEC> etaphi_t;

#define TotalN 2
#define NEVENT 1

//void MET_ref( ap_int<16> allPT_ref[TotalN], ap_int<16> &missPT_ref, ap_int<10> allPhi_ref[TotalN], ap_int<10> &missPhi_ref );
//void MET_hw(  ap_int<16> allPT_hw[TotalN], ap_int<16> &missPT_hw, ap_int<10> allPhi_hw[TotalN], ap_int<10> &missPhi_hw );

void MET_ref( pt_t allPT_ref[TotalN], pt_t &missPT_ref, etaphi_t allPhi_ref[TotalN], fixed10_t &missPhi_ref );
void MET_hw(  pt_t allPT_hw[TotalN],  pt_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  fixed10_t &missPhi_hw );

// *************************************************
//       Division
// *************************************************
/**/ 
template<class data_T, int N_TABLE_NUM, int N_TABLE_DEN>
void init_division_table(data_T table_out[N_TABLE_NUM*N_TABLE_DEN]) {
    // Implement division lookup
    for (int inum = 0; inum < N_TABLE_NUM; inum++) {
        for (int iden = 0; iden < N_TABLE_DEN; iden++) {
            int index = (inum*N_TABLE_NUM)+iden;
            // Compute lookup table function
            data_T real_val = (iden>0) ? float(inum)/iden : 0;
            table_out[index] = real_val;
        }
    }
    return;
}

template<class data_T, class res_T, int TABLE_SIZE_NUM, int TABLE_SIZE_DEN>
void division(data_T &data_num, data_T &data_den, res_T &res) {
    // Initialize the lookup table
    res_T division_table[TABLE_SIZE_NUM*TABLE_SIZE_DEN];
    init_division_table<res_T, TABLE_SIZE_NUM, TABLE_SIZE_DEN>(division_table);

    // Index into the lookup table based on data
    int index_num, index_den, index;

    //#pragma HLS PIPELINE

    if (data_num < 0) data_num = 0;
    if (data_den < 0) data_den = 0;
    if (data_num > TABLE_SIZE_NUM-1) data_num = TABLE_SIZE_NUM-1;
    if (data_den > TABLE_SIZE_DEN-1) data_den = TABLE_SIZE_DEN-1;
    index = (data_num*TABLE_SIZE_NUM) + data_den;
    res = division_table[index];

    return;
}

// Default table size provided here:
template<class data_T, class res_T>
void division(data_T &data_num, data_T &data_den, res_T &res) { 
    // Get the division value from the LUT 
    if(data_den==0) {
        std::cout << "WARNING::division::data_den==0" << std::endl;
        return;
    }
    division<data_T, res_T, N_TABLE_SIZE_NUM, N_TABLE_SIZE_DEN>(data_num, data_den, res); 
    return;
}
/**/
/**/
// *************************************************
//       Acosine
// *************************************************
template<class data_T, int N_TABLE>
void init_acos_table(data_T table_out[N_TABLE]) {
	//	Implement tanh lookup
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Original: 
		// First, convert from table index to X-value (signed 8-bit, range -4 to +4)
		//float in_val = 2*4.0*(ii-float(N_TABLE)/2.0)/float(N_TABLE);

		// Convert from table index to X-value (unsigned 4-bit, range 0 to +4)
		float in_val = (SINE_RANGE)*((N_TABLE-1)-ii)/float(N_TABLE);

		// Next, compute lookup table function
		data_T real_val = acos(in_val);
		//std::cout << "Tanh:  Lookup table Index: " <<  ii<< " In Value: " << in_val << " Result: " << real_val << std::endl;
		table_out[ii] = real_val;
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
	// Original:
	//data_round = data.read()*TABLE_SIZE/8; // original 8-bit
	//index = data_round + 4*TABLE_SIZE/8;   // original 8-bit (makes value positive)

	index = (1-data/SINE_RANGE)*TABLE_SIZE;

	if (index < 0) index = 0;
	if (index > TABLE_SIZE-1) index = TABLE_SIZE-1;
	res = acos_table[index];

	return;
}

// Default table size provided here:
template<class data_T, class res_T>
void acos(data_T &data, res_T &res) { 
	// Get the tanh value from the LUT 
	if (data < 0) {
		data = -1*data;
		acos<data_T, res_T, N_TABLE_SIZE>(data, res); 
		res  = -1*res;
	}
	else{
		acos<data_T, res_T, N_TABLE_SIZE>(data, res); 
	}
	return;
}

template<typename T_co, typename rT_co>
void Cos(T_co &Inp, rT_co &Out){
	double Input_co = 0.;
	Input_co = Inp * (M_PI / 180);
	std::cout<<"Input_cos: "<<Input_co<<std::endl;
	Out = cos(Input_co);
}

template<typename T_si, typename rT_si>
void Sin(T_si &Inp, rT_si &Out){
	double Input_si = 0.;
	Input_si = Inp * (M_PI / 180);
	std::cout<<"Input_sin: "<<Input_si<<std::endl;
	Out = sin(Input_si);
}

template<typename in_T, typename ou_T>
void Sqsqrt(in_T &Inp1, in_T &Inp2, ou_T &Out){
	double Out_res = 0.;
	Out_res = Inp1*Inp1 + Inp2*Inp2;
	Out = round( sqrt(Out_res));
}

/**/

#endif
