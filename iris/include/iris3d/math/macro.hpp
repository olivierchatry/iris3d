/*********************************************************
**  File name : Iris.hpp
**	Iris Engine V0.8 "tada"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
**	some of the code is taken from 
**				heinrich tillack <tillack@web.de>
*********************************************************/
#ifndef __MATH_MACRO__
#define __MATH_MACRO__

#define mat_trans_single_iris(x, y, z) { \
	register float __x __asm__("fr0") = (x); \
	register float __y __asm__("fr1") = (y); \
	register float __z __asm__("fr2") = (z); \
	__asm__ __volatile__( \
		"fldi1	fr3\n" \
		"ftrv	xmtrx,fv0\n" \
		: "=f" (__x), "=f" (__y), "=f" (__z) \
		: "0" (__x), "1" (__y), "2" (__z) \
		: "fr3" ); \
	x = __x; y = __y; z = __z; \
}



//XXX: new optimzed version from kgl-x gldraw.c
//TODO : are we really need to homogenize coordinate ?

#define mat_trans_single_my(x, y, z) { \
	register float __x __asm__("fr0") = (x); \
	register float __y __asm__("fr1") = (y); \
	register float __z __asm__("fr2") = (z); \
	__asm__ __volatile__( \
		"fldi1	fr3\n" \
		"ftrv	xmtrx,fv0\n" \
		"fmov fr3,fr2\n"\
		: "=f" (__x), "=f" (__y), "=f" (__z) \
		: "0" (__x), "1" (__y), "2" (__z) \
		: "fr3" ); \
	x = __x; y = __y; z = __z; \
}


/*
 	use this like

 	float result = mat_sqrtipr(x,y,z)

	for

	result= SQRT(x*x+y*y+z*z)
*/

#define mat_sqrtipr(x, y, z) ({ \
	register float __x __asm__("fr0") = (x); \
	register float __y __asm__("fr1") = (y); \
	register float __z __asm__("fr2") = (z); \
	register float __result __asm__("fr3"); \
	__asm__ __volatile__( \
		"fldi0	fr3\n" \
		"fipr	fv0,fv0\n" \
		"fsqrt	fr3\n" \
		: "=f" (__result) \
		: "f" (__x), "f" (__y), "f" (__z) \
		); \
	__result; \
})


/*
 	use this like

 	float result = mat_ipr(x,y,z)

	for

	result= (x*x+y*y+z*z)
*/
#define mat_ipr(x, y, z) ({ \
	register float __x __asm__("fr0") = (x); \
	register float __y __asm__("fr1") = (y); \
	register float __z __asm__("fr2") = (z); \
	register float __result __asm__("fr3"); \
	__asm__ __volatile__( \
		"fldi0	fr3\n" \
		"fipr	fv0,fv0\n" \
		: "=f" (__result) \
		: "f" (__x), "f" (__y), "f" (__z) \
		); \
	__result; \
})
	
#endif
