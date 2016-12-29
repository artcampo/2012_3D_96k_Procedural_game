#ifndef _HAAR_HPP_
#define _HAAR_HPP_

#include <vector>
#include <cmath>

class Daubechies {
   private:
   /** forward transform scaling coefficients */
   float h0, h1, h2, h3;
   /** forward transform wave coefficients */
   float g0, g1, g2, g3;

   float Ih0, Ih1, Ih2, Ih3;
   float Ig0, Ig1, Ig2, Ig3;

   /**
     Forward Daubechies D4 transform
    */
   void transform( float* a, const int n )
   {
      if (n >= 4) {
         int i, j;
         const int half = n >> 1;

         float* tmp = new float[n];

         for (i = 0, j = 0; j < n-3; j += 2, i++) {
            tmp[i]      = a[j]*h0 + a[j+1]*h1 + a[j+2]*h2 + a[j+3]*h3;
            tmp[i+half] = a[j]*g0 + a[j+1]*g1 + a[j+2]*g2 + a[j+3]*g3;
         }

         tmp[i]      = a[n-2]*h0 + a[n-1]*h1 + a[0]*h2 + a[1]*h3;
         tmp[i+half] = a[n-2]*g0 + a[n-1]*g1 + a[0]*g2 + a[1]*g3;

         for (i = 0; i < n; i++) {
            a[i] = tmp[i];
         }
         delete [] tmp;
      }
   }

   /**
     Inverse Daubechies D4 transform
    */
   void invTransform( float* a, const int n )
   {
     if (n >= 4) {
       int i, j;
       const int half = n >> 1;
       const int halfPls1 = half + 1;

       float* tmp = new float[n];

       //      last smooth val  last coef.  first smooth  first coef
       tmp[0] = a[half-1]*Ih0 + a[n-1]*Ih1 + a[0]*Ih2 + a[half]*Ih3;
       tmp[1] = a[half-1]*Ig0 + a[n-1]*Ig1 + a[0]*Ig2 + a[half]*Ig3;
       for (i = 0, j = 2; i < half-1; i++) {
         //     smooth val     coef. val       smooth val    coef. val
         tmp[j++] = a[i]*Ih0 + a[i+half]*Ih1 + a[i+1]*Ih2 + a[i+halfPls1]*Ih3;
         tmp[j++] = a[i]*Ig0 + a[i+half]*Ig1 + a[i+1]*Ig2 + a[i+halfPls1]*Ig3;
       }
       for (i = 0; i < n; i++) {
         a[i] = tmp[i];
       }
       delete [] tmp;
     }
   }


   public:

   Daubechies()
   {
      const float sqrt_3 = sqrt( 3.0f );
      const float denom = 4 * sqrt( 2.0f );

      //
      // forward transform scaling (smoothing) coefficients
      //
      h0 = (1 + sqrt_3)/denom;
      h1 = (3 + sqrt_3)/denom;
      h2 = (3 - sqrt_3)/denom;
      h3 = (1 - sqrt_3)/denom;
      //
      // forward transform wavelet coefficients
      //
      g0 =  h3;
      g1 = -h2;
      g2 =  h1;
      g3 = -h0;

      Ih0 = h2;
      Ih1 = g2;  // h1
      Ih2 = h0;
      Ih3 = g0;  // h3

      Ig0 = h3;
      Ig1 = g3;  // -h0
      Ig2 = h1;
      Ig3 = g1;  // -h2
   }

   void daubTrans( float* ts, int N )
   {
      int n;
      for (n = N; n >= 4; n >>= 1) {
         transform( ts, n );
      }
   }


   void invDaubTrans( float* coef, int N )
   {
      int n;
      for (n = 4; n <= N; n <<= 1) {
         invTransform( coef, n );
      }
   }

}; // Daubechies


namespace Haar{
  template <class Scalar>
  void  haar1Dt ( std::vector<Scalar>& aSignal, std::vector<Scalar>& aSignalOut )
  {
    double sq2d2 = sqrt(2.0)/2.0;
    int size = Signals::extendLengthPower2<Scalar> ( aSignal );
    aSignalOut.resize( size );

    for ( int i = 0 ; i < size ; ++i )
      aSignalOut[i] = aSignal[i];

    for ( int level = Signals::log2integer( size ); level > 0 ; --level )
    {
      int sizeTemp = Signals::exp2integer(level);
      std::vector<Scalar> signalTemp;
      signalTemp.resize( sizeTemp );

      int j = 0;
      for ( int i = 0 ; i < sizeTemp ; i=(i + 2), ++j )
      {
        Scalar s = Scalar ( (aSignalOut[i] + aSignalOut[i + 1])*sq2d2 );
        Scalar d = Scalar ( (aSignalOut[i] - aSignalOut[i + 1])*sq2d2 );
        signalTemp[j] = s;
        signalTemp[j + (sizeTemp/2)] = d;
      }

      for ( int i = 0 ; i < sizeTemp ; ++i )
        aSignalOut[i] = signalTemp[i];
    }
  }
  
 template <class Scalar>
  void  haar1Du ( std::vector<Scalar>& aSignal, std::vector<Scalar>& aSignalOut )
  {
    double sq2d2 = sqrt(2.0)/2.0;
    double sq2 = sqrt(2.0);
    int size = Signals::extendLengthPower2<Scalar> ( aSignal );
    aSignalOut.resize( size );

    for ( int i = 0 ; i < size ; ++i )
      aSignalOut[i] = aSignal[i];

    int maxLevel = Signals::log2integer( size );
    for ( int level = 1; level <= maxLevel ; ++level )
    {
      int sizeTemp = Signals::exp2integer(level);
      std::vector<Scalar> signalTemp;
      signalTemp.resize( sizeTemp );

      int j = 0;
      for ( int i = 0 ; i < sizeTemp ; i=(i + 2), ++j )
      {
        Scalar s = aSignalOut[j];
        Scalar d = aSignalOut[j + (sizeTemp/2)];
        Scalar l = Scalar((s + d) / sq2);
        Scalar r = Scalar((s - d) / sq2);
        signalTemp[i] = l;
        signalTemp[i + 1] = r;
      }

      for ( int i = 0 ; i < sizeTemp ; ++i )
        aSignalOut[i] = signalTemp[i];
    }
  }
};

namespace Signals{
  template <class Scalar>
  int   extendLengthPower2 ( std::vector<Scalar>& aSignal )
  {
    int size = aSignal.size();
    return size;
  }

  int   log2integer ( int aNumber );
  int   exp2integer ( int aExp );

};


#endif
