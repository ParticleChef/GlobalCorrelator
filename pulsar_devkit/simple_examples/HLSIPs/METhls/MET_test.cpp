#include <cstdio>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

#define TotalN 2
#define NEVENT 2

int main() {

	ap_int<8> allPT[TotalN];
	ap_int<8> missPT;
	ap_int<8> allPhi[TotalN];
	ap_int<8> missPhi;

	for (int test = 0; test < NEVENT; ++test) {

		allPT[0] = 2+test;
		allPT[1] = 4+test;
//		allPT[2] = -8+test;

		allPhi[0] = 0.7+test;
		allPhi[1] = 0.8+test;
//		allPhi[2] = 0.6+test;

		printf("test pt --> %d, %d\n",  int(allPT[0]),  int(allPT[1]) );
		printf("test phi --> %d, %d\n", int(allPhi[0]), int(allPhi[1]) );

		int i;
		int j;

		missPT = 0;

		MET_ref(allPT, missPT, allPhi, missPhi);
		MET_hw( allPT, missPT, allPhi, missPhi);

		printf( "test MET = %d \n", int(missPT) );
		printf( "test Phi = %d \n", int(missPhi) );

	}

	return 0;
}
