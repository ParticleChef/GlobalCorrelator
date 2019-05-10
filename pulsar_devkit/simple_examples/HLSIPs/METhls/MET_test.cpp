#include <cstdio>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

#define TotalPt 3
#define NEVENT 1

int main() {

	ap_int<8> allPT[TotalPt];
	ap_int<8> missPT[NEVENT];

	for (int test = 0; test < NEVENT; ++test) {

		//inA = 1;
		//inB = 2;
		//outA = 0;

		allPT[0] = 2;
		allPT[1] = 4;
		allPT[2] = -8;

		printf("test pt --> %i, %i, %i\n", int(allPT[0]), int(allPT[1]), int(allPT[2]) );

		int i;
		int j;

		missPT[test] = 0;

//		for( i = 0; i < NEVENT; j++){
//			uint10_t pt = allPT[i];
//			missPT[test] -= pt;
//		}
		MET_ref(allPT, missPT);
		MET_hw(allPT, missPT);

		printf( "test MET = %i \n", int(missPT[NEVENT]) );

	}

	return 0;
}
