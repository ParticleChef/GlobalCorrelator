#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "MET.h"

#define TotalPt 3
#define NEVENT 1

void MET_ref( ap_int<8> allPT[TotalPt], ap_int<8> missPT[NEVENT] ) {

	int i;
	int j;

	for( j = 0; j < NEVENT; j++){
		for( i = 0; i < TotalPt; i++){
			ap_int<8> pt = allPT[i];
			missPT[j] -= pt;
		}
	}

}

