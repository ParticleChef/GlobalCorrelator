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
	etaphi_t co = 0;
	etaphi_t si = 0;
	pt_t pt = 0;

	for( i = 0; i < TotalN; i++){
		Cos<etaphi_t>(allPhi_ref[i],co);
		Sin<etaphi_t>(allPhi_ref[i],si);
		totalX = totalX - allPT_ref[i]*co;//s(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*si;//n(allPhi_ref[i]);
	}

	Sqsqrt<etaphi_t, pt_t>(totalX, totalY, pt);
	missPT_ref = pt; // sqrt( totalX*totalX + totalY*totalY );
	


//	etaphi_t Res = 0;
	etaphi_t Res = totalX /missPT_ref;;
//	division<etaphi_t, etaphi_t,N_TABLE_SIZE_NUM,N_TABLE_SIZE_DEN>(totalX, missPT_ref, Res);

	acos<etaphi_t, etaphi_t, 1024>(Res, missPhi_ref );
//	missPhi_ref = acos( totalX / missPT_ref );


}

