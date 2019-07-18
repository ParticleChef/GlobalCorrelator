#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "ap_fixed.h"
#include "MET.h"

//#define TotalN 3
//#define NEVENT 1

void MET_ref( double allPT_ref[TotalN], double &missPT_ref, double allPhi_ref[TotalN], double &missPhi_ref ){
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
	double pt = 0;

//	std::cout<<"ref : before loop phi: "<<allPhi_ref[0]<<", "<<allPhi_ref[1]<<std::endl;
//	std::cout<<"ref : before loop pt : "<<allPT_ref[0]<<", "<<allPT_ref[1]<<std::endl;
		std::cout<<"ref: pt  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_ref[i]<<", "; std::cout<<allPT_ref[TotalN-1]<<std::endl;
		std::cout<<"ref: phi --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPhi_ref[i]<<", "; std::cout<<allPhi_ref[TotalN-1]<<std::endl;

	for( i = 0; i < TotalN; i++){
		Cos<double,double>(allPhi_ref[i],co);
		Sin<double,double>(allPhi_ref[i],si);
		std::cout<<"ref : sin,cos: "<<co<<", "<<si<<std::endl;
		totalX = totalX - allPT_ref[i]*co;//s(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*si;//n(allPhi_ref[i]);
		std::cout<<"ref : totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<double,double>(totalX, totalY, pt);
	std::cout<<"ref : calculated pt: "<<pt<<std::endl;
	missPT_ref = pt; // sqrt( totalX*totalX + totalY*totalY );
	std::cout<<"ref : missPT_ref = pt; "<<missPT_ref<<std::endl;
	

	double Res = 0;
	double res_phi = 0;
	if(missPT_ref == 0){ 
		Res = 1E+10;
		res_phi = 0.; }
	else{ 
		Res = totalX/pt; //missPT_ref;
		if(Res == 0 ){
			if(totalY == 0) res_phi = 0;
			else if(totalY < 0) res_phi = -acos(Res);
			else res_phi = acos(Res);
		}
		else if(Res < 0) res_phi = -acos(-Res);
		else res_phi = acos(Res);
		//acos<double, double/*, 1024*/>(Res, res_phi);
		//res_phi = acos(Res);
	}
//
//	division<double, double>(totalX, missPT_ref, Res);
	std::cout<<"ref : Res = totalX/missPT: "<<Res<<std::endl;

	//acos<double, double/*, 1024*/>(Res, res_phi);
	missPhi_ref =  res_phi*(180/3.14) ;
	std::cout<<"ref : acos(x/PT) missPhi = "<<missPhi_ref<<std::endl;
//	missPhi_ref = acos( totalX / missPT_ref );

}

