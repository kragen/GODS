#include "GCore_constants.h"

#ifndef _GMATH_BASE_H
#define _GMATH_BASE_H

namespace god
{
//#define GMATH_PI	3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679 // 8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196
//#define GMATH_2PI	6.2831853071795864769252867665590
//#define GMATH_PI2	1.5707963267948966192313216916398
	static const double GMATH_PI		= 3.14159265358979;
	static const double GMATH_PI2		= 1.57079632679489;
	static const double GMATH_2PI		= 6.28318530717958;
	static const double GMATH_PI_180	= GMATH_PI/180.0;
	static const float	GINFINITY		= 3.402823466e+38F;

	double LinearInterpolate(double a, double b, double x);
	double CosineInterpolate(double a, double b, double x);
	double Noise1D(uint32_t x, uint64_t Seed=15731 );
	double Noise2D(uint32_t x, uint32_t y, uint32_t nWidth, uint64_t Seed=15731 );
	double Noise3D(uint32_t x, uint32_t y, uint32_t z, uint32_t nWidth, uint32_t nHeight, uint64_t Seed=15731 );
	double Noise1D(int64_t x, int64_t Seed=15731 );
	double Noise2D(int64_t x, int64_t y, int64_t nWidth, int64_t Seed=15731 );
	double Noise3D(int64_t x, int64_t y, int64_t z, int64_t nWidth, int64_t nHeight, int64_t Seed=15731 );

	double SmoothNoise2D(int32_t x, int32_t y, uint32_t nWidth);
	double InterpolatedNoise2D(float x, float y, uint32_t nWidth);
	double PerlinNoise2D(int32_t x, int32_t y, float fPersistence, uint32_t nOctaveCount );
	uint32_t weightedChoice(uint32_t weightCount, const uint32_t* weights, uint64_t Seed);

	//typedef class __huge_int_t
	//{
	//public:
	//	int64_t LL;
	//	int64_t LH;
	//	int64_t HL;
	//	int64_t HH;
	//	int64_t flags;	
	//	__huge_int_t( void ){};
	//	__huge_int_t( uint32_t LowLow, uint32_t LowHigh, uint32_t HighLow, uint32_t HighHigh );
	//	__huge_int_t( uint32_t LowLow, uint32_t LowHigh, uint32_t HighLow );
	//	__huge_int_t( uint32_t LowLow, uint32_t LowHigh );
	//	__huge_int_t( uint32_t LowLow );
	//	__huge_int_t( uint16_t LowLow );
	//	__huge_int_t( uint8_t LowLow );
	//} huge_int_t;

#define mix(a, b, factor) ((b)*(factor)+(a)*(1.0f-(factor)))
};

#endif // _GMATH_BASE_H