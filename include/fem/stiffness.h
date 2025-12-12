#pragma once

#include "vec3.h"
#include "sys_utils.h"

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
    const Vec3d n   = cross(AB, AC);
    const double nn = dot(n, n);
    ASSERT_ALWAYS(nn > 0.0);

    const double area = 0.5 * std::sqrt(nn);  // |K| = 1/2 ||AB x AC||
    const double inv_nn = 1.0 / nn;

    // gA = grad(phi_A), gB = grad(phi_B), gC = grad(phi_C)
    const Vec3d gA = cross(n, (AB - AC)) * inv_nn; // n x (B-C)
    const Vec3d gB = cross(n, AC) * inv_nn;        // n x (C-A)
    const Vec3d gC = cross(n, (-AB)) * inv_nn;     // n x (A-B)

    S[0] = area * dot(gA, gA); // 00
    S[1] = area * dot(gB, gB); // 11
    S[2] = area * dot(gC, gC); // 22
    S[3] = area * dot(gA, gB); // 01
    S[4] = area * dot(gB, gC); // 12
    S[5] = area * dot(gC, gA); // 20 (=02)
}
