#include <cstdio>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

//#define TotalN 2
//#define NEVENT 2

int main() {

	pt_t allPT_ref[TotalN];
	pt_t missPT_ref;
	etaphi_t allPhi_ref[TotalN];
	etaphi_t missPhi_ref;

	pt_t allPT_hw[TotalN];
	pt_t missPT_hw;
	etaphi_t allPhi_hw[TotalN];
	etaphi_t missPhi_hw;

	for (int test = 0; test < NEVENT; ++test) {

		allPT_hw[0] = 2+test;
		allPT_hw[1] = 4+test;
//		allPT_hw[2] = -8+test;

		allPhi_hw[0] = 1+test;
		allPhi_hw[1] = 2+test;
//		allPhi_hw[2] = 0.6+test;

		allPT_ref[0] = 2+test;
		allPT_ref[1] = 4+test;
//		allPT_ref[2] = -8+test;

		allPhi_ref[0] = 1+test;
		allPhi_ref[1] = 2+test;
//		allPhi_ref[2] = 0.6+test;

//		printf("test pt --> %d, %d\n",  allPT_ref[0],  allPT_ref[1] );// int(allPT_ref[0]),  int(allPT_ref[1]) );
//		printf("test phi --> %d, %d\n", allPhi_ref[0], allPhi_ref[1] );//int(allPhi_ref[0]), int(allPhi_ref[1]) );

		std::cout<<"pt  --> "<<allPT_ref[0]<<", "<<allPT_ref[1]<<std::endl;
		std::cout<<"phi --> "<<allPhi_ref[0]<<", "<<allPhi_ref[1]<<std::endl;

//		printf("input pt --> %f, %f\n",  allPT_ref[0],  allPT_ref[1] );
//		printf("input phi --> %f, %f\n", allPhi_ref[0], allPhi_ref[1] );

		int i;
		int j;

//		missPT_ref = 0;
//		missPT_hw = 0;

		MET_ref(allPT_ref, missPT_ref, allPhi_ref, missPhi_ref);
		MET_hw( allPT_hw, missPT_hw, allPhi_hw, missPhi_hw);

//		printf( "test MET_ref = %d \n", missPT_ref ); // int(missPT_ref) );
//		printf( "test Phi_ref = %d \n", missPhi_ref );//int(missPhi_ref) );
//
//		printf( "test MET_hw = %d \n", missPT_hw );// int(missPT_hw) );
//		printf( "test Phi_hw = %d \n", missPhi_hw );//int(missPhi_hw) );

		std::cout<<"MET_ref = "<<missPT_ref<<std::endl;
		std::cout<<"Phi_ref = "<<missPhi_ref<<std::endl;
		std::cout<<"MET_hw = "<<missPT_hw<<std::endl;
		std::cout<<"Phi_hw = "<<missPhi_hw<<std::endl;

//		printf( "test MET_ref = %f \n", missPT_ref );
//		printf( "test Phi_ref = %f \n", missPhi_ref );

//		printf( "test MET_hw = %f \n", missPT_hw );
//		printf( "test Phi_hw = %f \n", missPhi_hw );

	}

	return 0;
}
