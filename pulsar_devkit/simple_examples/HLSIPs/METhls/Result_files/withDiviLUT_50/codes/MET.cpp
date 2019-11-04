#include "MET.h"
#include "divide_result.h"
#include <cmath>
#include "hls_math.h"
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

//#define TotalN 10

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
	//	std::cout<<"("<<i<<") totalX,Y = "<<totalX<<", "<<totalY<<std::endl;
	}

	//uint32_t total = totalX*totalX + totalY*totalY;
	//pt = hls::sqrt(total);
	pt = hls::sqrt(uint32_t(totalX*totalX)+uint32_t(totalY*totalY));

	missPT_hw = pt; 
	std::cout<<"hw  : missPT_hw  = "<<missPT_hw <<std::endl;

	temp_t divi = 0;
	etaphi_t res_phi = 0;
	ap_fixed<14,8> degr = 57.32;
	if( missPT_hw == 0 ){ 
		//divi = 1E+10; 
		res_phi = 0; 
	}
	else{
		//divi = hls::divide(float(totalX),float(missPT_hw));    
		if(missPT_hw > (1<<13)) missPT_hw = (1<<13)-1;
		if(totalX > (1<<13)) totalX = (1<<13)-1;
		std::cout<<"what is wrong :before divi "<<std::endl;
		divi = divide_result(totalX, missPT_hw);
		std::cout<<"what is wrong : "<<divi<<std::endl;
		//division<fixedXY_t, pt_t, temp_t>(totalX, missPT_hw, divi);
		if(totalY < 0){acos<temp_t, etaphi_t>(divi, res_phi); res_phi = -1*res_phi;}
		if(totalY >= 0) acos<temp_t, etaphi_t>(divi, res_phi);
	}

	std::cout<<"hw  : totalX/missPT = "<<divi<<std::endl;
	std::cout<<"hw  : acos(x/PT) = "<<res_phi<<std::endl;

	missPhi_hw = res_phi*degr;   //(180/3.14));
	std::cout<<"hw  : acos(x/PT)*180/3.14 = "<<missPhi_hw <<std::endl;

}
