#include "MET.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif


void MET_hw(  pt_t allPT_hw[TotalN], pt_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  etaphi_t &missPhi_hw ){
	#pragma HLS ARRAY_PARTITION variable=allPT_hw complete // block factor=2
	#pragma HLS ARRAY_PARTITION variable=allPhi_hw complete // block factor=2
	#pragma HLS PIPELINE 

	int i;
	int j;

	fixedXY_t totalX = 0;
	fixedXY_t totalY = 0;
	sincos_t co[TotalN] = {0};
	sincos_t si[TotalN] = {0};
	pt_t pt = 0;

#pragma HLS ARRAY_PARTITION variable=co complete
#pragma HLS ARRAY_PARTITION variable=si complete

	Cos<etaphi_t,sincos_t,activ_config>(allPhi_hw,co);
	Sin<etaphi_t,sincos_t,activ_config>(allPhi_hw,si);
	for( i = 0; i < TotalN; i++){
		totalX = totalX -allPT_hw[i]*co[i];
		totalY = totalY -allPT_hw[i]*si[i];//n(allPhi_hw[i]);
	}

	Sqsqrt<fixedXY_t, pt_t>(totalX, totalY, pt);
	std::cout<<"hw  : calculated MET: "<<pt<<std::endl;

	missPT_hw = pt; 
	//std::cout<<"hw  : missPT_hw  = pt; "<<missPT_hw <<std::endl;

	temp_t divi = 0;
	etaphi_t res_phi = 0;
	ap_fixed<14,8> degr = 57.32;
	if( missPT_hw == 0 ){ 
		divi = 1E+10; 
		res_phi = 0; 
	}
	else{
		divi = totalX/missPT_hw;    
		if(totalY < 0){acos<temp_t, etaphi_t>(divi, res_phi); res_phi = -1*res_phi;}
		if(totalY >= 0) acos<temp_t, etaphi_t>(divi, res_phi);
	}

	std::cout<<"hw  : totalX/missPT = "<<divi<<std::endl;

	missPhi_hw = res_phi*degr;   //(180/3.14));
	std::cout<<"hw  : acos(x/PT) = "<<missPhi_hw <<std::endl;

}
