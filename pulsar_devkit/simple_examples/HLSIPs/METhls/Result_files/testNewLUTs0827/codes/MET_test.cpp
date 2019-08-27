#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MET.h"
#include "ap_int.h"

#define NTEST 1

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

	FILE *METout;
	FILE *PHIout;
	FILE *METout_;
	FILE *PHIout_;
	FILE *METrefout;
	FILE *PHIrefout;
	//FILE *RANDdist;
	FILE *PTin;
	FILE *PHIin;
	METout=fopen("MET_out.txt","w");
	PHIout=fopen("PHI_out.txt","w");
	METout_=fopen("MET_out_.txt","w");
	PHIout_=fopen("PHI_out_.txt","w");
	METrefout=fopen("METref_out.txt","w");
	PHIrefout=fopen("PHIref_out.txt","w");
	//RANDdist=fopen("RAND_dist.txt","w");
	PTin=fopen("PT_in.txt","w");
	PHIin=fopen("PHI_in.txt","w");

	double allPT_ref[TotalN];
	double missPT_ref;
	double allPhi_ref[TotalN];
	double missPhi_ref;

	pt_t allPT_hw[TotalN];
	pt_t missPT_hw;
	etaphi_t allPhi_hw[TotalN];
	etaphi_t missPhi_hw;

	double input_pt[TotalN] = {0}; // {10, 12};
	double input_ph[TotalN] = {0}; // {85, -25};

	long n = 1;
	srand((unsigned int)time(NULL));
	for (int test = 0; test < NEVENT; ++test) {
		std::cout<<"==== NEVENT "<<test+1<<" ===="<<std::endl;

		for(int nn = 0; nn < TotalN; ++nn){
			//fprintf(RANDdist, "%f\n", rand_input(&n));
			//input_pt[nn] = rand()%291 + 10; //10 ~ 300
			//input_ph[nn] = rand()%360 - 180; // -180 ~ 180
			input_pt[nn] = rand_input(&n)*290.+10.;//%291 + 10; //10 ~ 300
			input_ph[nn] = rand_input(&n)*360.-180.;//ran0(n);//%360 - 180; // -180 ~ 180

			fprintf(PTin, "%f\n", input_pt[nn]);
			fprintf(PHIin, "%f\n", input_ph[nn]);

			allPT_hw[nn] =  input_pt[nn];
			allPhi_hw[nn] = input_ph[nn];

			allPT_ref[nn] =  input_pt[nn];
			allPhi_ref[nn] = input_ph[nn];
		}
//		printf("test pt --> %d, %d\n",  allPT_ref[0],  allPT_ref[1] );// int(allPT_ref[0]),  int(allPT_ref[1]) );
//		printf("test phi --> %d, %d\n", allPhi_ref[0], allPhi_ref[1] );//int(allPhi_ref[0]), int(allPhi_ref[1]) );

		std::cout<<"test: pT  --> "; for(int i = 0; i <TotalN; i++) std::cout<<allPT_ref[i]<<", "; std::cout<<allPT_ref[TotalN]<<std::endl;
		std::cout<<"test: phi --> "; for(int i = 0; i <TotalN; i++) std::cout<<allPhi_ref[i]<<", "; std::cout<<allPhi_ref[TotalN]<<std::endl;

		int i;
		int j;

		MET_ref(allPT_ref, missPT_ref, allPhi_ref, missPhi_ref);

		std::cout<<"test: ref process end"<<std::endl;

		MET_hw( allPT_hw, missPT_hw, allPhi_hw, missPhi_hw);

		std::cout<<"test: hw process end"<<std::endl;

		std::cout<<"test: MET_ref = "<<missPT_ref<<std::endl;
		std::cout<<"test: Phi_ref = "<<missPhi_ref<<std::endl;
		std::cout<<"test: MET_hw = "<<missPT_hw<<std::endl;
		std::cout<<"test: Phi_hw = "<<missPhi_hw<<std::endl;

		//std::cout<<"MET_ref = "<<missPT_ref<<", MET_hw = "<<(double)missPT_hw<<std::endl;

		double wrt_pT = missPT_ref - (double)missPT_hw;
		double wrt_ph = missPhi_ref - (double)missPhi_hw;
		if( wrt_ph > 180 ) wrt_ph = wrt_ph - 2*180;
		if( wrt_ph <= -180)wrt_ph = wrt_ph + 2*180;
		fprintf(METout, "%f\n", wrt_pT);
		fprintf(PHIout, "%f\n", wrt_ph);
		fprintf(METout_, "%f\n", wrt_pT/missPT_ref);
		fprintf(PHIout_, "%f\n", wrt_ph/missPT_ref);
		fprintf(METrefout, "%f\n", missPT_ref);
		fprintf(PHIrefout, "%f\n", missPhi_ref);

	}

	fclose(METout);
	fclose(PHIout);
	fclose(METout_);
	fclose(PHIout_);
	fclose(METrefout);
	fclose(PHIrefout);
	//fclose(RANDdist);
	fclose(PTin);
	fclose(PHIin);

	return 0;
}
