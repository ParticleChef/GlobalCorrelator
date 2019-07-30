#include "MET.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

//#define TotalN 3
//#define NEVENT 1

void MET_hw(  pt_t allPT_hw[TotalN], fixed10_t &missPT_hw,  etaphi_t allPhi_hw[TotalN],  fixed10_t &missPhi_hw ){
//void MET_hw( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<10> allPhi[TotalN], ap_int<10> &missPhi ) {
	#pragma HLS pipeline II=2
	#pragma HLS array_partition variable=allPT_hw complete // block factor=2
	#pragma HLS array_partition variable=allPhi_hw complete // block factor=2

	int i;
	int j;

	fixedXY_t totalX = 0;
	fixedXY_t totalY = 0;
	fixed5_t co = 0;
	fixed5_t si = 0;
	fixed10_t pt = 0;

//	std::cout<<"hw  : beform loop phi: "<<allPhi_hw [0]<<", "<<allPhi_hw [1]<<std::endl;
//	std::cout<<"hw  : beform loop pt: "<<allPT_hw [0]<<", "<<allPT_hw [1]<<std::endl;
		//std::cout<<"hw  : pt  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_hw[i]<<", "; std::cout<<allPT_hw[TotalN-1]<<std::endl;
		//std::cout<<"hw  : phi --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPhi_hw[i]<<", "; std::cout<<allPhi_hw[TotalN-1]<<std::endl;

	for( i = 0; i < TotalN; i++){
		cos<etaphi_t,fixed5_t>(allPhi_hw[i],co);
		sin<etaphi_t,fixed5_t>(allPhi_hw[i],si);
		std::cout<<"hw  : sin("<<allPhi_hw[i]<<") = "<<si<<", cos("<<allPhi_hw[i]<<") = "<<co<<std::endl;
		totalX = totalX -allPT_hw[i]*co;//s(allPhi_hw[i]);
		totalY = totalY -allPT_hw[i]*si;//n(allPhi_hw[i]);
		std::cout<<"hw  : "<<"("<<i<<") "<<"totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<fixedXY_t, fixed10_t>(totalX, totalY, pt);
	//std::cout<<"hw  : calculated MET: "<<pt<<std::endl;

	missPT_hw = pt; // sqrt( totalX*totalX + totalY*totalY );
	//std::cout<<"hw  : missPT_hw  = pt; "<<missPT_hw <<std::endl;

	//double Res = 0;
	//double res_phi = 0;
	float divi = 0;
	float res_phi = 0;
	float degr = 57.32;
	if( missPT_hw == 0 ){ 
		divi = 1E+10; 
		res_phi = 0; 
	}
	else{
		divi = totalX/missPT_hw;// float( totalX/missPT_hw );
	std::cout<<"hw  : totalX/missPT = "<<divi<<std::endl;
		if(totalY < 0){acos<float, float>(divi, res_phi); res_phi = -1*res_phi;}
		if(totalY >= 0) acos<float, float>(divi, res_phi);
		std::cout<<"hw  : phi = "<<res_phi<<std::endl;
	}
//	else{ 
//		divi = float( totalX/missPT_hw );
//		acos<float, float/*, 1024*/>(divi, res_phi );
//		if(divi == 0 ){
//			if(totalY == 0) res_phi = 0;
//			if(totalY < 0) res_phi = -1*res_phi;
//		}
//		if(divi < 0) res_phi = -1*res_phi;;
//		//std::cout<<"Res = "<<Res<<", result_phi = "<<res_phi<<std::endl;
//	}

	std::cout<<"hw  : totalX/missPT = "<<divi<<std::endl;
//	division<etaphi_t, etaphi_t,N_TABLE_SIZE_NUM,N_TABLE_SIZE_DEN>(totalX, missPT_hw, Res);

	//acos<double, double/*, 1024*/>(Res, res_phi );
	missPhi_hw = res_phi*degr;//(180/3.14));
	std::cout<<"hw  : acos(x/PT) = "<<missPhi_hw <<std::endl;
//	missPhi_hw = acos( totalX / missPT_hw );

}
