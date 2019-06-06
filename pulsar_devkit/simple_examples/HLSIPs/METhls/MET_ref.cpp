#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "MET.h"

#define TotalN 3
#define NEVENT 1

void MET_ref( ap_int<16> allPT[TotalN], ap_int<16> &missPT, ap_int<16> allPhi[TotalN], ap_int<16> &missPhi ) {

	int i;
	int j;

	ap_int<16> totalX = 0;
	ap_int<16> totalY = 0;

	for( i = 0; i < TotalN; i++){
		totalX = totalX - allPT[i]*cosf(allPhi[i]);
		totalY = totalX - allPT[i]*sinf(allPhi[i]);
	}

	missPT = sqrtf( totalX*totalX + totalY*totalY );
	missPhi = acosf( totalX / missPT );


}

