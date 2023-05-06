/////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (c) 2005
//	Lukasz Matecki
//	lutecki@op.pl
//All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SHARP_H_
#define _SHARP_H_



#define SCANER


unsigned int sharp_scan(unsigned int adc_val);
volatile unsigned char sharp_scan_tbl[256];
volatile unsigned int global_sharp_dist;

unsigned char sharpGP2D12(unsigned int adc_val);


#endif //_SHARP_H_
