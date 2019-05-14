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

		printf("test pt --> %lf, %lf\n",  double(allPT[0]),  double(allPT[1]) );
		printf("test phi --> %lf, %lf\n", double(allPhi[0]), double(allPhi[1]) );

		int i;
		int j;

		missPT = 0;

		MET_ref(allPT, missPT, allPhi, missPhi);
		MET_hw( allPT, missPT, allPhi, missPhi);

		printf( "test MET = %lf \n", double(missPT) );
		printf( "test Phi = %lf \n", double(missPhi) );

	}

	return 0;
}
