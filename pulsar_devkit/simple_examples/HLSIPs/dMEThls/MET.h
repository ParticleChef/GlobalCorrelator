#ifndef SIMPLE_ALGO_H
#define SIMPLE_ALGO_H

#include "ap_int.h"

#define TotalN 3
#define NEVENT 1

void MET_ref( double allPT[TotalN], double &missPT, double allPhi[TotalN], double &missPhi );
void MET_hw(  double allPT[TotalN], double &missPT, double allPhi[TotalN], double &missPhi );

#endif
