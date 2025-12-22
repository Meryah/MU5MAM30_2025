#pragma once

#include "vec3.h"

/* Given a triangle ABC, computes the (symmetric) 3x3 stiffness matrix S s.t.
 *
 *   S_{ij} := \int_{ABC} \nabla \phi_i \cdot \nabla \phi_j
 *
 * where \phi_0 := \phi_A, \phi_1 := \phi_B, \phi_2 := \phi_C
 * are the shape functions of the P1 Lagrange element associated
 * to ABC.
 *
 * Input : the vectors AB and AC.
 * Output: the six coefficients S_{00} S_{11} S_{22} S_{01} S_{12} S_{20},
 *         corresponding to the interactions A<->A, B<->B, C<->C, A<->B, B<->C,
 *         C<->A
 *
 * Idea behind computation :
 * -------------------------
 *
 * Hiden for now.
 *
 */
void inline stiffness(const Vec3d &AB, const Vec3d &AC, double *__restrict S)
{
	/* Your implementation goes here */

	double norme_AC = norm2(AC);
	double norme_AB = norm2(AB);
	double pdt_scalaire = dot(AC,AB);
	double norme_BC = norm2_AC - 2 * pdt_scalaire + norm2_AB; /* On écrit ||B-C||^2 = ||(C-A)-(B-A)||^2 et on developpe */

	double Aire = (0.5)*(norm(cross(AB,AC)));

	if (Aire == 0.0)
	{
		 std::cout << "Les points sont alignés : on n'a pas un triangle !" << std::endl;
	}

	else{
	/* La matrice est symétrique donc on calcule que 6 coefficients*/

		S[0] = (norme_AC + norme_AB - norme_BC)/(4*Aire);  /* int (gradient phiA * gradient phiA) */
		S[1] = (norme_BC - norme_AB)/(4*Aire);             /* int (gradient phiA * gradient phiB) */
		S[2] = (norme_BC - norme_AC)/(4*Aire);             /* int (gradient phiA * gradient phiC) */
		S[3] = (norme_BC + norme_AB - norme_AC)/(4*Aire) ; /* int (gradient phiB * gradient phiB) */
		S[4] =	(norme_AC - norme_BC)/(4*Aire);            /* int (gradient phiB * gradient phiC) */
		S[5] = (norme_BC + norme_AC - norme_AB)/(4*Aire);  /* int (gradient phiC* gradient phiC) */

	}
}