#include <cstdio>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

//#define TotalN 2
//#define NEVENT 2

int main() {

	double allPT_ref[TotalN];
	double missPT_ref;
	double allPhi_ref[TotalN];
	double missPhi_ref;

	pt_t allPT_hw[TotalN];
	fixed10_t missPT_hw;
	etaphi_t allPhi_hw[TotalN];
	fixed10_t missPhi_hw;

	int input_pt[TotalN] = {100, 200, 120};
	int input_ph[TotalN] = {0, 90, -135};

	for (int test = 0; test < NEVENT; ++test) {

		for(int nn = 0; nn < TotalN; ++nn){
			allPT_hw[nn] = input_pt[nn];
			allPhi_hw[nn] = input_ph[nn];

			allPT_ref[nn] = input_pt[nn];
			allPhi_ref[nn] = input_ph[nn];

		}

//		printf("test pt --> %d, %d\n",  allPT_ref[0],  allPT_ref[1] );// int(allPT_ref[0]),  int(allPT_ref[1]) );
//		printf("test phi --> %d, %d\n", allPhi_ref[0], allPhi_ref[1] );//int(allPhi_ref[0]), int(allPhi_ref[1]) );

		std::cout<<"test: pt  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_ref[i]<<", "; std::cout<<allPT_ref[TotalN-1]<<std::endl;
		std::cout<<"test: phi --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPhi_ref[i]<<", "; std::cout<<allPhi_ref[TotalN-1]<<std::endl;

		int i;
		int j;

//		missPT_ref = 0;
//		missPT_hw = 0;

		MET_ref(allPT_ref, missPT_ref, allPhi_ref, missPhi_ref);

		std::cout<<"test: ref process end"<<std::endl;

		MET_hw( allPT_hw, missPT_hw, allPhi_hw, missPhi_hw);

		std::cout<<"test: hw process end"<<std::endl;

//		printf( "test MET_ref = %d \n", missPT_ref ); // int(missPT_ref) );
//		printf( "test Phi_ref = %d \n", missPhi_ref );//int(missPhi_ref) );
//
//		printf( "test MET_hw = %d \n", missPT_hw );// int(missPT_hw) );
//		printf( "test Phi_hw = %d \n", missPhi_hw );//int(missPhi_hw) );

		std::cout<<"test: MET_ref = "<<missPT_ref<<std::endl;
		std::cout<<"test: Phi_ref = "<<missPhi_ref<<std::endl;
		std::cout<<"test: MET_hw = "<<missPT_hw<<std::endl;
		std::cout<<"test: Phi_hw = "<<missPhi_hw<<std::endl;

//		printf( "test MET_ref = %f \n", missPT_ref );
//		printf( "test Phi_ref = %f \n", missPhi_ref );

//		printf( "test MET_hw = %f \n", missPT_hw );
//		printf( "test Phi_hw = %f \n", missPhi_hw );

	}

	return 0;
}
