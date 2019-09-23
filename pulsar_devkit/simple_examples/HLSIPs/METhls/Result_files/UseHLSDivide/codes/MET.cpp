#include "MET.h"
#include <cmath>
#include "hls_math.h"
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
	sincos_t co = 0;
	sincos_t si = 0;
	pt_t pt = 0;

	for( i = 0; i < TotalN; i++){
		Cos<etaphi_t,sincos_t,activ_config>(allPhi_hw[i],co);
		Sin<etaphi_t,sincos_t,activ_config>(allPhi_hw[i],si);
		totalX = totalX -allPT_hw[i]*co;
		totalY = totalY -allPT_hw[i]*si;//n(allPhi_hw[i]);
		std::cout<<"("<<i<<") totalX,Y = totalX,Y - "<<allPT_hw[i]<<" * "<<co<<", "<<si<<" = "<<totalX<<", "<<totalY<<std::endl;
	}

	std::cout<<"hw  : Before sqsqrt X = "<<totalX<<", Y = "<<totalY<<std::endl;
	//Sqsqrt<fixedXY_t, pt_t>(totalX, totalY, pt);
	int32_t total = totalX*totalX + totalY*totalY;
	pt = hls::sqrt(total);
	//std::cout<<"hw  : calculated MET: "<<pt<<std::endl;

	missPT_hw = pt; 
	std::cout<<"hw  : missPT_hw  = pt; "<<missPT_hw <<std::endl;

	temp_t divi = 0;
	etaphi_t res_phi = 0;
	ap_fixed<14,8> degr = 57.32;
	if( missPT_hw == 0 ){ 
		divi = 1E+10; 
		res_phi = 0; 
	}
	else{
		//divi = totalX/missPT_hw;    
		//Division<fixedXY_t,pt_t,temp_t,N_NUMERATOR,N_DENOMINATOR>(totalX,missPT_hw,divi);
		divi = hls::divide(float(totalX),float(missPT_hw));    
		if(totalY < 0){acos<temp_t, etaphi_t>(divi, res_phi); res_phi = -1*res_phi;}
		if(totalY >= 0) acos<temp_t, etaphi_t>(divi, res_phi);
	}

	std::cout<<"hw  : totalX/missPT = "<<divi<<std::endl;

	missPhi_hw = res_phi*degr;   //(180/3.14));
	std::cout<<"hw  : acos(x/PT) = "<<missPhi_hw <<std::endl;

}
