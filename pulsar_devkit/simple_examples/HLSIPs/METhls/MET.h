#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

//#include <vector>
#include "ap_int.h"

#define TotalPt 3
#define NEVENT 1

//void MET_ref( ap_int<32> inA, ap_int<32> &outA );
//void MET_hw( ap_int<32> inA, ap_int<32> &outA );
void MET_ref( ap_int<8> allPT[TotalPt], ap_int<8> missPT[NEVENT] );
void MET_hw( ap_int<8> allPT[TotalPt], ap_int<8> missPT[NEVENT] );

#endif
