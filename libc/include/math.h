/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * from: @(#)fdlibm.h 5.1 93/09/24
 * $FreeBSD$
 */

#ifndef _MATH_H_
#define _MATH_H_

#include <sys/cdefs.h>
#include <limits.h>

#define __BIONIC_MATH_INLINE extern __inline__ __always_inline __attribute__((gnu_inline)) __attribute__((__artificial__))

__BEGIN_DECLS
#pragma GCC visibility push(default)

#define HUGE_VAL	__builtin_huge_val()

#if __ISO_C_VISIBLE >= 1999
#define FP_ILOGB0	(-INT_MAX)
#define FP_ILOGBNAN	INT_MAX

#define HUGE_VALF	__builtin_huge_valf()
#define HUGE_VALL	__builtin_huge_vall()
#define INFINITY	__builtin_inff()
#define NAN		__builtin_nanf("")

#define MATH_ERRNO	1
#define MATH_ERREXCEPT	2
#define math_errhandling	MATH_ERREXCEPT

#if defined(__FP_FAST_FMA)
#define FP_FAST_FMA 1
#endif
#if defined(__FP_FAST_FMAF)
#define FP_FAST_FMAF 1
#endif
#if defined(__FP_FAST_FMAL)
#define FP_FAST_FMAL 1
#endif

/* Symbolic constants to classify floating point numbers. */
#define FP_INFINITE	0x01
#define FP_NAN		0x02
#define FP_NORMAL	0x04
#define FP_SUBNORMAL	0x08
#define FP_ZERO		0x10
#define fpclassify(x) \
    __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)

#define isfinite(x) __builtin_isfinite(x)
#define isinf(x) __builtin_isinf(x)
#define isnan(x) __builtin_isnan(x)
#define isnormal(x) __builtin_isnormal(x)

#define isgreater(x, y) __builtin_isgreater((x), (y))
#define isgreaterequal(x, y) __builtin_isgreaterequal((x), (y))
#define isless(x, y) __builtin_isless((x), (y))
#define islessequal(x, y) __builtin_islessequal((x), (y))
#define islessgreater(x, y) __builtin_islessgreater((x), (y))
#define isunordered(x, y) __builtin_isunordered((x), (y))

#define signbit(x) \
    ((sizeof(x) == sizeof(float)) ? __builtin_signbitf(x) \
    : (sizeof(x) == sizeof(double)) ? __builtin_signbit(x) \
    : __builtin_signbitl(x))

typedef double __double_t;
typedef __double_t double_t;
typedef float __float_t;
typedef __float_t float_t;
#endif /* __ISO_C_VISIBLE >= 1999 */

/*
 * XOPEN/SVID
 */
#if __BSD_VISIBLE || __XSI_VISIBLE
#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#define	MAXFLOAT	((float)3.40282346638528860e+38)
extern int signgam;
#endif /* __BSD_VISIBLE || __XSI_VISIBLE */

#if __USE_GNU
#define M_El            2.718281828459045235360287471352662498L /* e */
#define M_LOG2El        1.442695040888963407359924681001892137L /* log 2e */
#define M_LOG10El       0.434294481903251827651128918916605082L /* log 10e */
#define M_LN2l          0.693147180559945309417232121458176568L /* log e2 */
#define M_LN10l         2.302585092994045684017991454684364208L /* log e10 */
#define M_PIl           3.141592653589793238462643383279502884L /* pi */
#define M_PI_2l         1.570796326794896619231321691639751442L /* pi/2 */
#define M_PI_4l         0.785398163397448309615660845819875721L /* pi/4 */
#define M_1_PIl         0.318309886183790671537767526745028724L /* 1/pi */
#define M_2_PIl         0.636619772367581343075535053490057448L /* 2/pi */
#define M_2_SQRTPIl     1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
#define M_SQRT2l        1.414213562373095048801688724209698079L /* sqrt(2) */
#define M_SQRT1_2l      0.707106781186547524400844362104849039L /* 1/sqrt(2) */
#endif

#if __BSD_VISIBLE
#if 0
/* Old value from 4.4BSD-Lite math.h; this is probably better. */
#define	HUGE		HUGE_VAL
#else
#define	HUGE		MAXFLOAT
#endif
#endif /* __BSD_VISIBLE */

/*
 * Most of these functions depend on the rounding mode and have the side
 * effect of raising floating-point exceptions, so they are not declared
 * as __pure2.  In C99, FENV_ACCESS affects the purity of these functions.
 */

/*
 * ANSI/POSIX
 */
int	__fpclassifyd(double) __pure2;
int	__fpclassifyf(float) __pure2;
int	__fpclassifyl(long double) __pure2;
int	__isfinitef(float) __pure2;
int	__isfinite(double) __pure2;
int	__isfinitel(long double) __pure2;
int	__isinff(float) __pure2;
int	__isinfl(long double) __pure2;
int	__isnanf(float) __pure2;
int	__isnanl(long double) __pure2;
int	__isnormalf(float) __pure2;
int	__isnormal(double) __pure2;
int	__isnormall(long double) __pure2;
int	__signbit(double) __pure2;
int	__signbitf(float) __pure2;
int	__signbitl(long double) __pure2;

double	acos(double);
double	asin(double);
double	atan(double);
double	atan2(double, double);
double	cos(double);
double	sin(double);
double	tan(double);

double	cosh(double);
double	sinh(double);
double	tanh(double);

double	exp(double);
double	frexp(double, int *);	/* fundamentally !__pure2 */
double	ldexp(double, int);
double	log(double);
double	log10(double);
double	modf(double, double *);	/* fundamentally !__pure2 */

double	pow(double, double);
double	sqrt(double);

double	ceil(double);
double	fabs(double) __pure2;
__BIONIC_MATH_INLINE double fabs(double x) { return __builtin_fabs(x); }
double	floor(double);
double	fmod(double, double);

/*
 * These functions are not in C90.
 */
#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999 || __XSI_VISIBLE
double	acosh(double);
double	asinh(double);
double	atanh(double);
double	cbrt(double);
double	erf(double);
double	erfc(double);
double	exp2(double);
double	expm1(double);
double	fma(double, double, double);
double	hypot(double, double);
int	ilogb(double) __pure2;
int	(isinf)(double) __pure2;
int	(isnan)(double) __pure2;
double	lgamma(double);
long long llrint(double);
long long llround(double);
double	log1p(double);
double	log2(double);
double	logb(double);
long	lrint(double);
long	lround(double);
double	nan(const char *) __pure2;
double	nextafter(double, double);
double	remainder(double, double);
double	remquo(double, double, int *);
double	rint(double);
#endif /* __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999 || __XSI_VISIBLE */

#if __BSD_VISIBLE || __XSI_VISIBLE
double	j0(double);
double	j1(double);
double	jn(int, double);
double	y0(double);
double	y1(double);
double	yn(int, double);

#if __XSI_VISIBLE <= 500 || __BSD_VISIBLE
double	gamma(double);
#endif

#if __XSI_VISIBLE <= 600 || __BSD_VISIBLE
double	scalb(double, double);
#endif
#endif /* __BSD_VISIBLE || __XSI_VISIBLE */

#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999
double	copysign(double, double) __pure2;
double	fdim(double, double);
double	fmax(double, double) __pure2;
double	fmin(double, double) __pure2;
double	nearbyint(double);
double	round(double);
double	scalbln(double, long);
double	scalbn(double, int);
double	tgamma(double);
double	trunc(double);
#endif

/*
 * BSD math library entry points
 */
#if __BSD_VISIBLE
double	drem(double, double);
int	finite(double) __pure2;
int	isnanf(float) __pure2;
long double significandl(long double);

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
double	gamma_r(double, int *);
double	lgamma_r(double, int *);

/*
 * IEEE Test Vector
 */
double	significand(double);
#endif /* __BSD_VISIBLE */

/* float versions of ANSI/POSIX functions */
#if __ISO_C_VISIBLE >= 1999
float	acosf(float);
float	asinf(float);
float	atanf(float);
float	atan2f(float, float);
float	cosf(float);
float	sinf(float);
float	tanf(float);

float	coshf(float);
float	sinhf(float);
float	tanhf(float);

float	exp2f(float);
float	expf(float);
float	expm1f(float);
float	frexpf(float, int *);	/* fundamentally !__pure2 */
int	ilogbf(float) __pure2;
float	ldexpf(float, int);
float	log10f(float);
float	log1pf(float);
float	log2f(float);
float	logf(float);
float	modff(float, float *);	/* fundamentally !__pure2 */

float	powf(float, float);
float	sqrtf(float);

float	ceilf(float);
float	fabsf(float) __pure2;
__BIONIC_MATH_INLINE float fabsf(float x) { return __builtin_fabsf(x); }
float	floorf(float);
float	fmodf(float, float);
float	roundf(float);

float	erff(float);
float	erfcf(float);
float	hypotf(float, float);
float	lgammaf(float);
float	tgammaf(float);

float	acoshf(float);
float	asinhf(float);
float	atanhf(float);
float	cbrtf(float);
float	logbf(float);
float	copysignf(float, float) __pure2;
long long llrintf(float);
long long llroundf(float);
long	lrintf(float);
long	lroundf(float);
float	nanf(const char *) __pure2;
float	nearbyintf(float);
float	nextafterf(float, float);
float	remainderf(float, float);
float	remquof(float, float, int *);
float	rintf(float);
float	scalblnf(float, long);
float	scalbnf(float, int);
float	truncf(float);

float	fdimf(float, float);
float	fmaf(float, float, float);
float	fmaxf(float, float) __pure2;
float	fminf(float, float) __pure2;
#endif

/*
 * float versions of BSD math library entry points
 */
#if __BSD_VISIBLE
float	dremf(float, float);
int	finitef(float) __pure2;
float	gammaf(float);
float	j0f(float);
float	j1f(float);
float	jnf(int, float);
float	scalbf(float, float);
float	y0f(float);
float	y1f(float);
float	ynf(int, float);

/*
 * Float versions of reentrant version of gamma & lgamma; passes
 * signgam back by reference as the second argument; user must
 * allocate space for signgam.
 */
float	gammaf_r(float, int *);
float	lgammaf_r(float, int *);

/*
 * float version of IEEE Test Vector
 */
float	significandf(float);
#endif	/* __BSD_VISIBLE */

/*
 * long double versions of ISO/POSIX math functions
 */
#if __ISO_C_VISIBLE >= 1999
long double	acoshl(long double);
long double	acosl(long double);
long double	asinhl(long double);
long double	asinl(long double);
long double	atan2l(long double, long double);
long double	atanhl(long double);
long double	atanl(long double);
long double	cbrtl(long double);
long double	ceill(long double);
long double	copysignl(long double, long double) __pure2;
long double	coshl(long double);
long double	cosl(long double);
long double	erfcl(long double);
long double	erfl(long double);
long double	exp2l(long double);
long double	expl(long double);
long double	expm1l(long double);
long double	fabsl(long double) __pure2;
__BIONIC_MATH_INLINE long double fabsl(long double x) { return __builtin_fabsl(x); }
long double	fdiml(long double, long double);
long double	floorl(long double);
long double	fmal(long double, long double, long double);
long double	fmaxl(long double, long double) __pure2;
long double	fminl(long double, long double) __pure2;
long double	fmodl(long double, long double);
long double	frexpl(long double value, int *); /* fundamentally !__pure2 */
long double	hypotl(long double, long double);
int		ilogbl(long double) __pure2;
long double	ldexpl(long double, int);
long double	lgammal(long double);
long long	llrintl(long double);
long long	llroundl(long double);
long double	log10l(long double);
long double	log1pl(long double);
long double	log2l(long double);
long double	logbl(long double);
long double	logl(long double);
long		lrintl(long double);
long		lroundl(long double);
long double	modfl(long double, long double *); /* fundamentally !__pure2 */
long double	nanl(const char *) __pure2;
long double	nearbyintl(long double);
long double	nextafterl(long double, long double);
double		nexttoward(double, long double);
float		nexttowardf(float, long double);
long double	nexttowardl(long double, long double);
long double	powl(long double, long double);
long double	remainderl(long double, long double);
long double	remquol(long double, long double, int *);
long double	rintl(long double);
long double	roundl(long double);
long double	scalblnl(long double, long);
long double	scalbnl(long double, int);
long double	sinhl(long double);
long double	sinl(long double);
long double	sqrtl(long double);
long double	tanhl(long double);
long double	tanl(long double);
long double	tgammal(long double);
long double	truncl(long double);
#endif /* __ISO_C_VISIBLE >= 1999 */

#if __BSD_VISIBLE
long double	lgammal_r(long double, int *);
#endif

#if defined(__USE_GNU)
void sincos(double, double*, double*);
void sincosf(float, float*, float*);
void sincosl(long double, long double*, long double*);
#endif /* __USE_GNU */

#pragma GCC visibility pop
__END_DECLS

#endif /* !_MATH_H_ */
