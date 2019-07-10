#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "MET.h"

//#define TotalN 3
//#define NEVENT 1

void MET_ref( pt_t allPT_ref[TotalN], pt_t &missPT_ref, etaphi_t allPhi_ref[TotalN], etaphi_t &missPhi_ref ){
//void MET_ref( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<10> allPhi[TotalN], ap_int<10> &missPhi ) {
	#pragma HLS pipeline II=2
	#pragma HLS array_partition variable=allPT_ref complete // block factor=2
	#pragma HLS array_partition variable=allPhi_ref complete // block factor=2

	int i;
	int j;

	double totalX = 0;
	double totalY = 0;
	double co = 0;
	double si = 0;
	pt_t pt = 0;

	std::cout<<"ref : before loop phi: "<<allPhi_ref[0]<<", "<<allPhi_ref[1]<<std::endl;
	std::cout<<"ref : before loop pt : "<<allPT_ref[0]<<", "<<allPT_ref[1]<<std::endl;

	for( i = 0; i < TotalN; i++){
		Cos<etaphi_t,double>(allPhi_ref[i],co);
		Sin<etaphi_t,double>(allPhi_ref[i],si);
		std::cout<<"ref : sin,cos: "<<co<<", "<<si<<std::endl;
		totalX = totalX - allPT_ref[i]*co;//s(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*si;//n(allPhi_ref[i]);
		std::cout<<"ref : totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<double,pt_t>(totalX, totalY, pt);
	std::cout<<"ref : calculated pt: "<<pt<<std::endl;
	missPT_ref = pt; // sqrt( totalX*totalX + totalY*totalY );
	std::cout<<"ref : missPT_ref = pt; "<<missPT_ref<<std::endl;
	

	double Res = 0;
	if(missPT_ref == 0) Res = 1E+10;
	else Res = totalX /missPT_ref;
//
//	division<etaphi_t, double>(totalX, missPT_ref, Res);
	std::cout<<"ref : Res = totalX/missPT: "<<Res<<std::endl;

	double res_phi = 0;
	acos<double, double, 1024>(Res, res_phi);
	missPhi_ref = res_phi*(180/3.14);
	std::cout<<"ref : acos(x/PT) missPhi = "<<missPhi_ref<<std::endl;
//	missPhi_ref = acos( totalX / missPT_ref );


}

