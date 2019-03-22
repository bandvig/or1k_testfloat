
#include "systfloat.h"

DELETE ME

//-----------------------------------------------------
float64 syst_int64_to_float64( int64 a )
{
  double  dZ;
  float64  z;
  dZ = (double)a;
  z = *((float64*)&dZ);
  return z;
}

//-----------------------------------------------------
int64 syst_float64_to_int64( float64 a )
{
  int64 z;
  double dA;
  dA = *((double*)&a);
  z = (int64)dA;
  return z;
}

//-----------------------------------------------------
int64 syst_float64_to_int64_round_to_zero( float64 a )
{
  int64 z;
  double dA;
  dA = *((double*)&a);
  z = (int64)dA;
  return z;
}

//-----------------------------------------------------
float64 syst_float64_add( float64 a, float64 b )
{
  //double dZ, dA, dB;
  float64 z;
  /*dA = *((double*)&a);
  dB = *((double*)&b);
  dZ = dA + dB;
  z  = *((float64*)&dZ);*/
  z = a + b;
  return (z);
}

//-----------------------------------------------------
float64 syst_float64_sub( float64 a, float64 b )
{
  double dZ, dA, dB;
  float64 z;
  dA = *((double*)&a);
  dB = *((double*)&b);
  dZ = dA - dB;
  z  = *((float64*)&dZ);
  return (z);
}

//-----------------------------------------------------
float64 syst_float64_mul( float64 a, float64 b )
{
  double dZ, dA, dB;
  float64 z;
  dA = *((double*)&a);
  dB = *((double*)&b);
  dZ = dA * dB;
  z  = *((float64*)&dZ);
  return (z);
}

//-----------------------------------------------------
float64 syst_float64_div( float64 a, float64 b )
{
  double dZ, dA, dB;
  float64 z;
  dA = *((double*)&a);
  dB = *((double*)&b);
  dZ = dA / dB;
  z  = *((float64*)&dZ);
  return (z);
}

//-----------------------------------------------------
flag syst_float64_eq( float64 a, float64 b )
{
  flag z;
  double dA, dB;
  dA = *((double*)&a);
  dB = *((double*)&b);
  z = (dA == dB);
  return z;
}

//-----------------------------------------------------
flag syst_float64_le( float64 a, float64 b )
{
  flag z;
  double dA, dB;
  dA = *((double*)&a);
  dB = *((double*)&b);
  z = (dA <= dB);
  return z;
}

//-----------------------------------------------------
flag syst_float64_lt( float64 a, float64 b )
{
  flag z;
  double dA, dB;
  dA = *((double*)&a);
  dB = *((double*)&b);
  z = (dA < dB);
  return z;
}
