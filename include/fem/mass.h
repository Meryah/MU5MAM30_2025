#pragma once
#include <math.h>
#include "vec3.h"

/* Given a triangle ABC, computes the (symmetric) 3x3 mass M s.t.
 *
 *   M_{ij} := \int_{ABC} \phi_i \phi_j
 *
 * where \phi_0 := \phi_A, \phi_1 := \phi_B, \phi_2 := \phi_C
 * are the shape functions of the P1 Lagrange element associated
 * to ABC.
 *
 * Idea behind computation :
 * -------------------------
 *
 * Hiden for now.
 */
void inline mass(const Vec3d &AB, const Vec3d &AC, double *__restrict M)
{
	Vec3d ABC;
	ABC.x = AB.y*AC.z-AB.z*AC.y;
	ABC.y = AB.z*AC.x-AB.x*AC.z;
	ABC.z = AB.x*AC.y-AB.y*AC.x;
	int 2aire = sqrt(ABC.x**2+ABC.y**2+ABC.z**2)	
	for( int i = 0;i<3;i++){
	for(int j =0;i<3;i++){
		M[3*i+j]=
}
