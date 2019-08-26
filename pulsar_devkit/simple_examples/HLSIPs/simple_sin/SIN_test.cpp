#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "SIN.h"
#include "ap_int.h"

#define NTEST 1

//#define TotalN 2
//#define NEVENT 2

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define MASK 123459876

float rand_input(long *idum)
{
	long k;
	float ans;

	*idum ^= MASK;
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM; 
	ans=AM*(*idum);
	*idum ^= MASK;
	return ans;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef MASK

int main() {

	//double allPT_ref[TotalN];
	//double missPT_ref;
	//double allPhi_ref[TotalN];
	//double missPhi_ref;

	double in_phi_ref[TotalN] = {0};
	double out_phi_ref[TotalN] = {0};

	etaphi_t in_phi[TotalN] = {0};
	sincos_t out_phi[TotalN] = {0};

	//double input_pt[TotalN] =  {10, 12};
	double input_ph[TotalN] = {85, -25, 150, 60, -45, 33, 100, -178, 3, 61};

	long n = 2;
	//srand((unsigned int)time(NULL));
	for (int test = 0; test < NEVENT; ++test) {
		std::cout<<"==== NEVENT "<<test+1<<" ===="<<std::endl;

		for(int nn = 0; nn < TotalN; nn++){
			//input_ph[nn] = rand_input(&n)*360.-180.;//ran0(n);//%360 - 180; // -180 ~ 180

			in_phi_ref[nn] = input_ph[nn];
			in_phi[nn] = input_ph[nn];
		}
		std::cout<<"test: phi --> "; for(int i = 0; i <TotalN; i++) std::cout<<in_phi[i]<<", "; std::cout<<std::endl;
		std::cout<<"test: phir--> "; for(int i = 0; i <TotalN; i++) std::cout<<in_phi[i]*temp_t(0.01745)<<", "; std::cout<<std::endl;

		int i;
		int j;

		SIN_hw( in_phi, out_phi);

		for(int nn = 0; nn < TotalN; nn++){
			out_phi_ref[nn] = sin(in_phi_ref[nn]*0.01745);
		}
		std::cout<<"test: hw process end"<<std::endl;

		std::cout<<"hw : out_phi = "; for(int i = 0; i < TotalN; i++) std::cout<<out_phi[i]<<", "; std::cout<<std::endl;
		std::cout<<"ref: out_phi = "; for(int i = 0; i < TotalN; i++) std::cout<<out_phi_ref[i]<<", "; std::cout<<std::endl;


	}


	return 0;
}
