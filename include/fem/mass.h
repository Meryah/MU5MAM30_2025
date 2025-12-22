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

	/* Your implementation goes here ! */

void inline mass(const Vec3d &AB, const Vec3d &AC, double *__restrict M)
{
<<<<<<< HEAD
	/*On calcule le déterminant de la Jacobienne*/
	/*Même si cross est le pdt vectoriel en 3D on peut écrire (x,y,0) et comme on prend la norme on a exactement ce qu'on veut*/
=======
	/*On calcule le determinant de la Jacobienne*/
	/*Meme si cross est le pdt vectoriel en 3D on peut ecrire (x,y,0) et comme on prend la norme on a exactement ce qu'on veut*/
>>>>>>> eb4e3af (MAJ mass.h)
    
	double detJacobienne = norm(cross(AB,AC));
	

	if (detJacobienne == 0.0)
	{
<<<<<<< HEAD
		 std::cout << "Les points sont alignés : on n'a pas un triangle !" << std::endl;
=======
		 std::cout << "Les points sont alignes : on n'a pas un triangle !" << std::endl;
>>>>>>> eb4e3af (MAJ mass.h)
	}

	else {
		M[0] = detJacobienne/12.0; /*Coefficients diagonaux*/
		M[1] = detJacobienne/24.0; /*Autres coefficients*/
	}
}
