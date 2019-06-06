#include <cstdio>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

#define TotalN 2
#define NEVENT 2

int main() {

	ap_int<16> allPT_ref[TotalN];
	ap_int<16> missPT_ref;
	ap_int<16> allPhi_ref[TotalN];
	ap_int<16> missPhi_ref;

	ap_int<16> allPT_hw[TotalN];
	ap_int<16> missPT_hw;
	ap_int<16> allPhi_hw[TotalN];
	ap_int<16> missPhi_hw;

	for (int test = 0; test < NEVENT; ++test) {

		allPT_hw[0] = 2+test;
		allPT_hw[1] = 4+test;
//		allPT_hw[2] = -8+test;

		allPhi_hw[0] = 0.7+test;
		allPhi_hw[1] = 0.8+test;
//		allPhi_hw[2] = 0.6+test;

		allPT_ref[0] = 2+test;
		allPT_ref[1] = 4+test;
//		allPT_ref[2] = -8+test;

		allPhi_ref[0] = 0.7+test;
		allPhi_ref[1] = 0.8+test;
//		allPhi_ref[2] = 0.6+test;

		printf("test pt --> %d, %d\n",  int(allPT_ref[0]),  int(allPT_ref[1]) );
		printf("test phi --> %d, %d\n", int(allPhi_ref[0]), int(allPhi_ref[1]) );

		int i;
		int j;

		missPT_ref = 0;
		missPT_hw = 0;

		MET_ref(allPT_ref, missPT_ref, allPhi_ref, missPhi_ref);
		MET_hw( allPT_hw, missPT_hw, allPhi_hw, missPhi_hw);

		printf( "test MET = %d \n", int(missPT_ref) );
		printf( "test Phi = %d \n", int(missPhi_ref) );

	}

	return 0;
}
