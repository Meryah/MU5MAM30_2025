#include <algorithm>
#include <vector>
#include <stdint.h>

#include "fem/P1.h"
#include "fem/stiffness.h"
#include "hash-table.h"
#include "mesh/mesh.h"
#include "sys_utils.h"

// Pack (i,j) with i>=j into a single 64-bit key
static inline uint64_t pack_ij(uint32_t i, uint32_t j)
{
	// assume we store only lower triangle (i >= j)
	return (static_cast<uint64_t>(i) << 32) | static_cast<uint64_t>(j);
}

void build_P1_CSRPattern(const Mesh &m, CSRPattern &P)
{
	const uint32_t n = static_cast<uint32_t>(m.vertex_count());
	P.symmetric = true;
	P.rows = n;
	P.cols = n;

	// Gather adjacency per row (lower triangle only)
	std::vector<std::vector<uint32_t>> rows(n);

	const uint32_t *idx = m.indices.data;
	const size_t nt = m.triangle_count();

	for (size_t t = 0; t < nt; ++t) {
		const uint32_t a = idx[3 * t + 0];
		const uint32_t b = idx[3 * t + 1];
		const uint32_t c = idx[3 * t + 2];
		const uint32_t v[3] = {a, b, c};

		// Add all (i,j) couples from the element connectivity
		// Store only i>=j to keep a symmetric lower-triangular pattern
		for (int ii = 0; ii < 3; ++ii) {
			for (int jj = 0; jj < 3; ++jj) {
				uint32_t r = v[ii];
				uint32_t col = v[jj];
				if (r < col)
					std::swap(r, col);
				rows[r].push_back(col);
			}
		}
	}

	// Build CSR row_start and col
	P.row_start = TArray<uint32_t>(n + 1);
	P.row_start[0] = 0;

	size_t nnz = 0;
	for (uint32_t i = 0; i < n; ++i) {
		auto &row = rows[i];
		std::sort(row.begin(), row.end());
		row.erase(std::unique(row.begin(), row.end()), row.end());

		nnz += row.size();
		P.row_start[i + 1] = static_cast<uint32_t>(nnz);
	}

	P.nnz = nnz;
	P.col = TArray<uint32_t>(nnz);

	size_t k = 0;
	for (uint32_t i = 0; i < n; ++i) {
		for (uint32_t j : rows[i]) {
			P.col[k++] = j;
		}
	}
}

void build_P1_stiffness_matrix(const Mesh &m, const CSRPattern &P, CSRMatrix &K)
{
	ASSERT_ALWAYS(P.rows == m.vertex_count());
	ASSERT_ALWAYS(P.cols == m.vertex_count());
	ASSERT_ALWAYS(P.symmetric == true);

	// Bind K to the pattern
	K.symmetric = true;
	K.rows = P.rows;
	K.cols = P.cols;
	K.nnz = P.nnz;
	K.row_start = (uint32_t *)P.row_start.data; // pattern must outlive K
	K.col = (uint32_t *)P.col.data;
	K.data = TArray<double>(P.nnz, 0.0);

	// Build (i,j)->k lookup table to avoid scanning CSR rows during assembly
	// Expected keys: nnz
	HashTable<uint64_t, uint32_t> lut(P.nnz);

	for (uint32_t i = 0; i < static_cast<uint32_t>(P.rows); ++i) {
		for (uint32_t kk = P.row_start[i]; kk < P.row_start[i + 1]; ++kk) {
			const uint32_t j = P.col[kk];
			// By construction we store i>=j
			const uint64_t key = pack_ij(i, j);
			lut.set_at(key, kk);
		}
	}

	const Vec3 *pos = m.positions.data;
	const uint32_t *idx = m.indices.data;
	const size_t nt = m.triangle_count();

	auto add_sym = [&](uint32_t i, uint32_t j, double val) {
		if (i < j)
			std::swap(i, j); // enforce lower triangle storage
		const uint64_t key = pack_ij(i, j);
		uint32_t *pkk = lut.get(key);
		ASSERT_ALWAYS(pkk != nullptr && "Missing CSR entry in LUT/pattern");
		K.data[*pkk] += val;
	};

	for (size_t t = 0; t < nt; ++t) {
		const uint32_t ia = idx[3 * t + 0];
		const uint32_t ib = idx[3 * t + 1];
		const uint32_t ic = idx[3 * t + 2];

		// read 3D vertices
		const Vec3d A(pos[ia].x, pos[ia].y, pos[ia].z);
		const Vec3d B(pos[ib].x, pos[ib].y, pos[ib].z);
		const Vec3d C(pos[ic].x, pos[ic].y, pos[ic].z);

		// build edge vectors
		const Vec3d AB = B - A;
		const Vec3d AC = C - A;

		// local stiffness on triangle ABC (P1), packed as:
		// [S00, S11, S22, S01, S12, S20]
		double Ke[6];
		stiffness(AB, AC, Ke);

		// assemble into global CSR (lower triangle)
		add_sym(ia, ia, Ke[0]);
		add_sym(ib, ib, Ke[1]);
		add_sym(ic, ic, Ke[2]);

		add_sym(ia, ib, Ke[3]); // A-B
		add_sym(ib, ic, Ke[4]); // B-C
		add_sym(ic, ia, Ke[5]); // C-A
	}
}