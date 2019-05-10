#include "MET.h"
#include <cmath>
#include <cassert>
#include <vector>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

#define TotalPt 3
#define NEVENT 1

void MET_hw( ap_int<8> allPT[TotalPt], ap_int<8> missPT[TotalPt] ) {

	int i;
	int j;

	for( j = 0; j < NEVENT; j++){
		for( i = 0; i < TotalPt; i++){
		//	uint10_t pt = allPT[i];
			ap_int<8> pt = allPT[i];
			missPT[j] -= pt;
		}
	}

}
