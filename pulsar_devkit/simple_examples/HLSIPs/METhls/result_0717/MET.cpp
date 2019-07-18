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

	fixed10_t totalX = 0;
	fixed10_t totalY = 0;
	fixed5_t co = 0;
	fixed5_t si = 0;
	fixed10_t pt = 0;

//	std::cout<<"hw  : beform loop phi: "<<allPhi_hw [0]<<", "<<allPhi_hw [1]<<std::endl;
//	std::cout<<"hw  : beform loop pt: "<<allPT_hw [0]<<", "<<allPT_hw [1]<<std::endl;
		std::cout<<"hw  : pt  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_hw[i]<<", "; std::cout<<allPT_hw[TotalN-1]<<std::endl;
		std::cout<<"hw  : phi --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPhi_hw[i]<<", "; std::cout<<allPhi_hw[TotalN-1]<<std::endl;

	for( i = 0; i < TotalN; i++){
		//Cos<etaphi_t,double>(allPhi_hw[i],co);
		//Sin<etaphi_t,double>(allPhi_hw[i],si);
		Cos<etaphi_t,fixed5_t>(allPhi_hw[i],co);
		Sin<etaphi_t,fixed5_t>(allPhi_hw[i],si);
		std::cout<<"hw  : sin,cos: "<<co<<", "<<si<<std::endl;
		totalX = totalX -allPT_hw[i]*co;//s(allPhi_hw[i]);
		totalY = totalY -allPT_hw[i]*si;//n(allPhi_hw[i]);
		std::cout<<"hw  : totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<fixed10_t, fixed10_t>(totalX, totalY, pt);
	std::cout<<"hw  : calculated pt: "<<pt<<std::endl;
	missPT_hw = pt; // sqrt( totalX*totalX + totalY*totalY );
	std::cout<<"hw  : missPT_hw  = pt; "<<missPT_hw <<std::endl;

	//double Res = 0;
	//double res_phi = 0;
	float Res = 0;
	float res_phi = 0;
	float degr = 57.32;
	if( missPT_hw == 0 ){ 
		Res = 1E+10; 
		res_phi = 0; }
	else{ 
		Res = float( totalX/missPT_hw );
		std::cout<<"Res = totalX/missPT_hw : "<<Res<<" = "<<totalX<<" / "<<missPT_hw<<std::endl;
		acos<float, float/*, 1024*/>(Res, res_phi );
		if(Res == 0){
			if(totalY == 0) res_phi = 0;
			if(totalY < 0) res_phi = -res_phi;
		}
		std::cout<<"Res = "<<Res<<", result_phi = "<<res_phi<<std::endl;
	}

	std::cout<<"hw  : Res = totalX/missPT: "<<Res<<std::endl;
//	division<etaphi_t, etaphi_t,N_TABLE_SIZE_NUM,N_TABLE_SIZE_DEN>(totalX, missPT_hw, Res);

	//acos<double, double/*, 1024*/>(Res, res_phi );
	missPhi_hw = res_phi*degr;//(180/3.14));
	std::cout<<"hw  : acos(x/PT) Res = "<<missPhi_hw <<std::endl;
//	missPhi_hw = acos( totalX / missPT_hw );

}
