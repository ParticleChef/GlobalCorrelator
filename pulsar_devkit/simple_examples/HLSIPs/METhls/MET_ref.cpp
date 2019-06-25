#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "MET.h"

#define TotalN 3
#define NEVENT 1

void MET_ref( pt_t allPT_ref[TotalN], pt_t &missPT_ref, etaphi_t allPhi_ref[TotalN], etaphi_t &missPhi_ref ){
//void MET_ref( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<10> allPhi[TotalN], ap_int<10> &missPhi ) {
	#pragma HLS pipeline II=2
	#pragma HLS array_partition variable=allPT_ref complete // block factor=2
	#pragma HLS array_partition variable=allPhi_ref complete // block factor=2

	int i;
	int j;

	etaphi_t totalX = 0;
	etaphi_t totalY = 0;

	for( i = 0; i < TotalN; i++){
		totalX = totalX - allPT_ref[i]*cosf(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*sinf(allPhi_ref[i]);
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

	missPT_ref = sqrtf( totalX*totalX + totalY*totalY );
	missPhi_ref = acosf( totalX / missPT_ref );
//	missPhi = acosf( res );


}

