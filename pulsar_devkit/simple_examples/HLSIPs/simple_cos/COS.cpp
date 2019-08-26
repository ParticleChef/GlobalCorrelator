#include "COS.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

void COS_hw(etaphi_t in_phi[TotalN], sincos_t out_phi[TotalN]){
#pragma HLS PIPELINE

	Cos<etaphi_t, sincos_t, activ_config>(in_phi, out_phi);
}
