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
	etaphi_t co = 0;
	etaphi_t si = 0;
	pt_t pt = 0;

	for( i = 0; i < TotalN; i++){
		Cos<etaphi_t>(allPhi_hw[i],co);
		Sin<etaphi_t>(allPhi_hw[i],si);
		totalX = totalX -allPT_hw[i]*co;//s(allPhi_hw[i]);
		totalY = totalY -allPT_hw[i]*si;//n(allPhi_hw[i]);
	}

	Sqsqrt<etaphi_t, pt_t>(totalX, totalY, pt);
	missPT_hw = pt; // sqrt( totalX*totalX + totalY*totalY );

//	etaphi_t Res = 0;
	etaphi_t Res = totalX /missPT_hw;;
//	division<etaphi_t, etaphi_t,N_TABLE_SIZE_NUM,N_TABLE_SIZE_DEN>(totalX, missPT_hw, Res);

	acos<etaphi_t, etaphi_t, 1024>(Res, missPhi_hw );
//	missPhi_hw = acos( totalX / missPT_hw );

}
