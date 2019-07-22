#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

//#define TotalN 2
//#define NEVENT 2

int main() {
	//std::ofstream METout;
	//std::ofstream PHIout;
	//METout.open("./MET_out.txt");
	//PHIout.open("./PHI_out.txt");

	FILE *METout;
	FILE *PHIout;
	FILE *METout_;
	FILE *PHIout_;
	FILE *METrefout;
	FILE *PHIrefout;
	METout=fopen("MET_out.txt","w");
	PHIout=fopen("PHI_out.txt","w");
	METout_=fopen("MET_out_.txt","w");
	PHIout_=fopen("PHI_out_.txt","w");
	METrefout=fopen("METref_out.txt","w");
	PHIrefout=fopen("PHIref_out.txt","w");

	double allPT_ref[TotalN];
	double missPT_ref;
	double allPhi_ref[TotalN];
	double missPhi_ref;

	pt_t allPT_hw[TotalN];
	fixed10_t missPT_hw;
	etaphi_t allPhi_hw[TotalN];
	fixed10_t missPhi_hw;

	int input_pt[TotalN] = {0}; // {10, 12};
	int input_ph[TotalN] = {0}; // {85, -25};

	srand((unsigned int)time(NULL));
	for (int test = 0; test < NEVENT; ++test) {
		std::cout<<"==== NEVENT "<<test+1<<" ===="<<std::endl;

		for(int nn = 0; nn < TotalN; ++nn){
			//srand((unsigned int)time(NULL));
			input_pt[nn] = rand()%291 + 10; //10 ~ 300
			input_ph[nn] = rand()%360 - 180; // -180 ~ 180

			allPT_hw[nn] = input_pt[nn];
			allPhi_hw[nn] = input_ph[nn];

			allPT_ref[nn] = input_pt[nn];
			allPhi_ref[nn] = input_ph[nn];

		}

//		printf("test pt --> %d, %d\n",  allPT_ref[0],  allPT_ref[1] );// int(allPT_ref[0]),  int(allPT_ref[1]) );
//		printf("test phi --> %d, %d\n", allPhi_ref[0], allPhi_ref[1] );//int(allPhi_ref[0]), int(allPhi_ref[1]) );

		std::cout<<"test: pT  --> "; for(int i = 0; i <TotalN-1; i++) std::cout<<allPT_ref[i]<<", "; std::cout<<allPT_ref[TotalN-1]<<std::endl;
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

		//std::cout<<"MET_ref = "<<missPT_ref<<", MET_hw = "<<(double)missPT_hw<<std::endl;

		//if((missPhi_hw < 0 || missPhi_hw > 90)||(missPhi_ref < 0|| missPhi_hw > 90)) continue;

		double wrt_pT = missPT_ref - (double)missPT_hw;
		double wrt_ph = missPhi_ref - (double)missPhi_hw;
		fprintf(METout, "%f\n", wrt_pT);
		fprintf(PHIout, "%f\n", wrt_ph);
		fprintf(METout_, "%f\n", wrt_pT/missPT_ref);
		fprintf(PHIout_, "%f\n", wrt_ph/missPT_ref);
		fprintf(METrefout, "%f\n", missPT_ref);
		fprintf(PHIrefout, "%f\n", missPhi_ref);
//		printf( "test MET_ref = %f \n", missPT_ref );
//		printf( "test Phi_ref = %f \n", missPhi_ref );

//		printf( "test MET_hw = %f \n", missPT_hw );
//		printf( "test Phi_hw = %f \n", missPhi_hw );

	}

	fclose(METout);
	fclose(PHIout);
	fclose(METout_);
	fclose(PHIout_);
	fclose(METrefout);
	fclose(PHIrefout);

	return 0;
}
