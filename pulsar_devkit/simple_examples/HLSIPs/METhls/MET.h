#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"

#define TotalN 3
#define NEVENT 1

void MET_ref( ap_int<8> allPT[TotalN], ap_int<8> &missPT, ap_int<8> allPhi[TotalN], ap_int<8> &missPhi );
void MET_hw(  ap_int<8> allPT[TotalN], ap_int<8> &missPT, ap_int<8> allPhi[TotalN], ap_int<8> &missPhi );

#endif
