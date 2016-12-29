// haarNoise.cpp: define el punto de entrada de la aplicación de consola.
//

#include  "Noise/Haar.hpp"
#include <cmath>
#include <iostream>
using namespace std;


int  haarNoise1d_2( int aValue )
{
  int   numBits = 31;
  std::vector<float> signal (numBits + 1);    
  std::vector<float> signalReconstructed(numBits + 1);  

  signal[31] = 0;
  
  int value = aValue;
  //16
  signal[0] = value & 31;
  value >>= 5;

  //8
  signal[1] = value & 15;
  signal[2] = value & 15;
  value >>= 4;

  //4
  signal[3] = value & 7;
  signal[4] = value & 7;
  signal[5] = value & 7;
  signal[6] = value & 7;
  value >>= 3;

  //2
  signal[7] = value & 3;
  signal[8] = value & 3;
  signal[9] = value & 3;
  signal[10] = value & 3;
  signal[11] = value & 3;
  signal[12] = value & 3;
  signal[13] = value & 3;
  signal[14] = value & 3;
  value >>= 2;

  //1
  signal[15] = value & 1;
  signal[16] = value & 1;
  signal[17] = value & 1;
  signal[18] = value & 1;
  signal[19] = value & 1;
  signal[20] = value & 1;
  signal[21] = value & 1;
  signal[22] = value & 1;
  signal[23] = value & 1;
  signal[24] = value & 1;
  signal[25] = value & 1;
  signal[26] = value & 1;
  signal[27] = value & 1;
  signal[28] = value & 1;
  signal[29] = value & 1;
  signal[30] = value & 1;


  Haar::haar1Du<float> ( signal, signalReconstructed );

  float retValue = 0;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    retValue += fabs(signalReconstructed[numBits - i]);
  }

  return retValue;
}

int  haarNoise1d( int aValue )
{
  int   numBits = 31;
  std::vector<float> signal (numBits + 1);    
  std::vector<float> signalReconstructed(numBits + 1);  

  signal[31] = 0;
  
  int value = aValue;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    signal[i] = value & 1;
    value = value >> 1;
  }

  Haar::haar1Du<float> ( signal, signalReconstructed );

  float retValue = 0;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    retValue += fabs(signalReconstructed[numBits - i]);
  }

  return retValue;
}


int  daubechiesNoise1d( int aValue )
{
  int   numBits = 31;
  std::vector<float> signal (numBits + 1);    
  std::vector<float> signalReconstructed(numBits + 1);  

  signal[31] = 0;
  
  int value = aValue;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    signal[i] = value & 1;
    value = value >> 1;
  }

  Daubechies daub;
	daub.invDaubTrans( &signal[0], numBits + 1);
	for ( int i = 0; i < numBits; ++i )
		signalReconstructed[i] = signal[i];

  float retValue = 0;
//  for ( int i = 0; i < numBits; ++i )
for ( int i = numBits - 1; i >= 0; --i )
  {
    retValue += fabs(signalReconstructed[i])*pow(2,double(i));
  }

  return retValue;
}

int  haarNoise2d( int aValue1,int aValue2)
{
  int   numBits = 31;
  std::vector<float> signal (numBits*2 + 2);    
  std::vector<float> signalReconstructed(numBits*2 + 2);  

  signal[62] = 0;
  signal[63] = 0;
  
  int value1 = aValue1;
  int value2 = aValue2;
  for ( int i = 0; i < numBits; ++i )
  {
    
    signal[i*2] = (value1 & 3) + ( value2 & 3);
    value1 = value1 >> 2;
    
    value2 = value2 >> 2;
  }

  Haar::haar1Du<float> ( signal, signalReconstructed );

  float retValue = 0;
  for ( int i = 0; i < 64; ++i )
  {
    retValue += fabs(signalReconstructed[63-i]);
    //retValue += (signalReconstructed[i]*0.5 + 0.5) ;
  }

  return retValue;
}

