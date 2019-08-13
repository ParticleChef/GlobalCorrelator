#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "ap_fixed.h"
#include "MET.h"


void MET_ref( double allPT_ref[TotalN], double &missPT_ref, double allPhi_ref[TotalN], double &missPhi_ref ){

	int i;
	int j;

	double totalX = 0;
	double totalY = 0;
	double co = 0;
	double si = 0;
	double pt = 0;

		//std::cout<<"ref: pt  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_ref[i]<<", "; std::cout<<allPT_ref[TotalN-1]<<std::endl;
		//std::cout<<"ref: phi --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPhi_ref[i]<<", "; std::cout<<allPhi_ref[TotalN-1]<<std::endl;

	for( i = 0; i < TotalN; i++){
		co = cos(allPhi_ref[i]*(PI_VAL/180));
		si = sin(allPhi_ref[i]*(PI_VAL/180));
		//std::cout<<"ref : sin,cos: "<<co<<", "<<si<<std::endl;
		totalX = totalX - allPT_ref[i]*co;//s(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*si;//n(allPhi_ref[i]);
		//std::cout<<"ref : ("<<i<<") "<<"totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<double,double>(totalX, totalY, pt);
	std::cout<<"ref : calculated MET: "<<pt<<std::endl;
	missPT_ref = pt; 
	//std::cout<<"ref : missPT_ref = pt; "<<missPT_ref<<std::endl;
	

	double divi = 0;
	double res_phi = 0;
	if(missPT_ref == 0){ 
		divi = 1E+10;
		res_phi = 0.; 
	}
	else{
		divi = totalX/pt;
		if(totalY < 0) res_phi = -acos(divi);
		if(totalY >= 0) res_phi = acos(divi);

	}
//	}
	std::cout<<"acos(x/pT) = "<<res_phi<<std::endl;
//
	std::cout<<"ref : totalX/missPT = "<<divi<<std::endl;

	missPhi_ref =  res_phi*(180/3.14) ;
	std::cout<<"ref : acos(x/PT) missPhi = "<<missPhi_ref<<std::endl;

}

