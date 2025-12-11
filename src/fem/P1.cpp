#pragma once

// #include "fem_matrix.h"
// #include "sparse_matrix.h"

// File: src/fem/P1.cpp

// 1. Include own definitions (to check function prototypes)
#include "fem/P1.h"

// 2. Include dependencies for implementation:
#include "common/vec3.h"           // For Vec3, Vec3d, etc.
#include "fem/mass.h"              // For the local mass function mass()
#include "fem/stiffness.h"         // For the local stiffness function stiffness()
#include "matrix/fem_matrix.h"     // For the FEMatrix structure
#include "matrix/sparse_matrix.h"  // For CSRPattern, CSRMatrix, etc.
#include "mesh/mesh.h"             // For the Mesh structure

void build_P1_mass_matrix(const Mesh& m, FEMatrix& M);
void build_P1_stiffness_matrix(const Mesh& m, FEMatrix& S);

void build_P1_CSRPattern(const Mesh& m, CSRPattern& P);
void build_P1_mass_matrix(const Mesh& m, const CSRPattern& P, CSRMatrix& M);
void build_P1_stiffness_matrix(const Mesh& m, const CSRPattern& P, CSRMatrix& S);

void build_P1_SKLPattern(const Mesh& m, SKLPattern& P);
void build_P1_mass_matrix(const Mesh& m, const SKLPattern& P, SKLMatrix& M);
void build_P1_stiffness_matrix(const Mesh& m, const SKLPattern& P, SKLMatrix& S);

/*
 * Builds the global P1 Mass Matrix (M) for the given mesh.
 *  The global matrix is assembled by summing the contributions of the local
 * mass matrix (M_e) for every element (triangle) in the mesh.
 */
void build_P1_mass_matrix(const Mesh& m, FEMatrix& M) {}
