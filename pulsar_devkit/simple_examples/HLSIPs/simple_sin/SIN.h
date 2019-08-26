#ifndef SIN_H
#define SIN_H

#include "ap_int.h"
#include "ap_fixed.h"

// type define
//typedef ap_int<16> pt_t;
typedef ap_fixed<18,9> etaphi_t;
typedef ap_fixed<10,3> sincos_t;
typedef ap_fixed<12,4> rad_t;

typedef ap_fixed<18,5> temp_t;

#define TotalN 10
#define NEVENT 1

#define PI_VAL temp_t(3.1415)

void SIN_hw(etaphi_t in_phi[TotalN], sincos_t out_phi[TotalN]);

struct activ_config{
	static const unsigned n_in = 10;
	static const unsigned table_size = 1024;
	typedef ap_fixed<18,8> table_t;
};

template<typename CONFIG_T, int N_TABLE>
void init_sin_table(typename CONFIG_T::table_t table_out[N_TABLE]) {
	int INDEX = 0;
	//  Implement cos lookup
	//for (int ii = N_TABLE-1; ii > -1; ii--) {
	for (int ii = 0; ii < N_TABLE; ii++) {
		// Convert from table index to X-value
		//float in_val = ((2*PI_VAL)*((N_TABLE-1)-ii)/N_TABLE) - PI_VAL; /****> better */
		float in_val = 2*3.1415*(ii-N_TABLE/2.0)/N_TABLE; /****> better */

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

#endif
