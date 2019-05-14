#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "MET.h"

#define TotalN 3
#define NEVENT 1

void MET_ref( ap_int<8> allPT[TotalN], ap_int<8> &missPT, ap_int<8> allPhi[TotalN], ap_int<8> &missPhi ) {

	int i;
	int j;

	double totalX = 0;
	double totalY = 0;

	for( i = 0; i < TotalN; i++){
		totalX -= allPT[i]*cos(allPhi[i]);
		totalY -= allPT[i]*sin(allPhi[i]);
	}

	missPT = sqrt(pow(totalX,2)+pow(totalY,2));
	missPhi = acos(double( totalX / missPT ) );


}

