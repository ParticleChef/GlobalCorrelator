#include "MET.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

#define TotalN 3
#define NEVENT 1

void MET_hw(  pt_t allPT_hw[TotalN],  pt_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  etaphi_t &missPhi_hw ){
//void MET_hw( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<10> allPhi[TotalN], ap_int<10> &missPhi ) {
	#pragma HLS pipeline II=2
	#pragma HLS array_partition variable=allPT_hw complete // block factor=2
	#pragma HLS array_partition variable=allPhi_hw complete // block factor=2

	int i;
	int j;

	etaphi_t totalX = 0;
	etaphi_t totalY = 0;

	for( i = 0; i < TotalN; i++){
		totalX = totalX -allPT_hw[i]*cosf(allPhi_hw[i]);
		totalY = totalY -allPT_hw[i]*sinf(allPhi_hw[i]);
	}

//	val_t num = totalX;
//	val_t den = missPT;
//	result_t res;
//	res = 0;

//	division<ap_int<16>, result_t>(totalX, missPT, divi);
//	division(num, den, divi);
//	division<val_t, result_t>(num, den, res);

//	missPT = sqrt( totalX*totalX + totalY*totalY );
//	missPhi = acos( totalX / missPT );
//	missPhi = acos( res );

	missPT_hw = sqrtf( totalX*totalX + totalY*totalY );
	missPhi_hw = acosf( totalX / missPT_hw );

}
