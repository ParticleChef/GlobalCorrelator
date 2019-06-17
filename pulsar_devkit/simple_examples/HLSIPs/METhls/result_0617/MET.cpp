#include "MET.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

#define TotalN 3
#define NEVENT 1

void MET_hw( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<16> allPhi[TotalN], ap_int<16> &missPhi ) {
	#pragma HLS pipeline II=2
	#pragma HLS array_partition variable=allPT complete // block factor=2
	#pragma HLS array_partition variable=allPhi complete // block factor=2

	int i;
	int j;

	ap_int<16> totalX = 0;
	ap_int<16> totalY = 0;

	for( i = 0; i < TotalN; i++){
		totalX = totalX -allPT[i]*cosf(allPhi[i]);
		totalY = totalY -allPT[i]*sinf(allPhi[i]);
	}

	val_t num = totalX;
	val_t den = missPT;
	result_t res;
//	res = 0;

//	division<ap_int<16>, result_t>(totalX, missPT, divi);
//	division(num, den, divi);
//	division<val_t, result_t>(num, den, res);

//	missPT = sqrt( totalX*totalX + totalY*totalY );
//	missPhi = acos( totalX / missPT );
//	missPhi = acos( res );

	missPT = sqrtf( totalX*totalX + totalY*totalY );
	missPhi = acosf( res );

}
