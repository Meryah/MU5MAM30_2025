#pragma once

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
	/*On calcule le déterminant de la Jacobienne*/
	/*Même si cross est le pdt vectoriel en 3D on peut écrire (x,y,0) et comme on prend la norme on a exactement ce qu'on veut*/
    
	double detJacobienne = norm(cross(AB,AC));
	

	if (detJacobienne == 0.0)
	{
		 std::cout << "Les points sont alignés : on n'a pas un triangle !" << std::endl;
	}

	else {
		M[0] = detJacobienne/12.0; /*Coefficients diagonaux*/
		M[1] = detJacobienne/24.0; /*Autres coefficients*/
	}
}
