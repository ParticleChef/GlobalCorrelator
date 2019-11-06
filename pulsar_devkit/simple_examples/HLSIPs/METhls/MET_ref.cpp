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

	for( i = 0; i < TotalN; i++){
		co = cos(allPhi_ref[i]*0.01740); //(pi_value_ref/180));
		si = sin(allPhi_ref[i]*0.01740); //(pi_value_ref/180));
		totalX = totalX - allPT_ref[i]*co;//s(allPhi_ref[i]);
		totalY = totalY - allPT_ref[i]*si;//n(allPhi_ref[i]);
//		std::cout<<"ref : ("<<i<<") "<<"totalX,Y: "<<totalX<<", "<<totalY<<std::endl;
	}

	Sqsqrt<double,double>(totalX, totalY, pt);
	std::cout<<"ref : MET= "<<pt<<std::endl;
	missPT_ref = pt; // sqrt( totalX*totalX + totalY*totalY );
	
	double divi = 0;
	double res_phi = 0;
	if(missPT_ref == 0){ 
		divi = 1E+10;
		res_phi = 0.; 
	}
	else{
		divi = fabs(totalX)/pt;
		if(totalY < 0) res_phi = -acos(divi);
		if(totalY >= 0) res_phi = acos(divi);

	}
	std::cout<<"ref : totalX/missPT = "<<divi<<std::endl;
	std::cout<<"ref : acos(x/pT) = "<<res_phi<<std::endl;
//

	missPhi_ref =  res_phi*(180/3.14) ;
	std::cout<<"ref : acos(x/pT)[deg] = "<<missPhi_ref<<std::endl;

}

