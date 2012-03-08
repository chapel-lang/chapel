/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include "heat.h"

/* default values */
static unsigned ntheta = 32+2;
static unsigned nthick = 32+2;
static unsigned nblocks = 16;
static unsigned nbigblocks = 8;
static unsigned shape = 0;
static unsigned pinned = 0;
static unsigned check = 0;
static unsigned version = 2;
static unsigned use_cg = 0; /* use a LU decomposition of CG ? */
static unsigned no_prio = 0;

extern void do_conjugate_gradient(float *nzvalA, float *vecb, float *vecx, uint32_t nnz,
              		unsigned nrow, uint32_t *colind, uint32_t *rowptr);


static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-cg") == 0)
		{
			use_cg = 1;
		}

		if (strcmp(argv[i], "-shape") == 0)
		{
		        char *argptr;
			shape = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nthick") == 0)
		{
		        char *argptr;
			nthick = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-ntheta") == 0)
		{
		        char *argptr;
			ntheta = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nblocks") == 0)
		{
		        char *argptr;
			nblocks = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nbigblocks") == 0)
		{
		        char *argptr;
			nbigblocks = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-v1") == 0)
		{
			version = 1;
		}

		if (strcmp(argv[i], "-v2") == 0)
		{
			version = 2;
		}

		if (strcmp(argv[i], "-v3") == 0)
		{
			version = 3;
		}

		if (strcmp(argv[i], "-v4") == 0)
		{
			version = 4;
		}

		if (strcmp(argv[i], "-pin") == 0)
		{
			pinned = 1;
		}

		if (strcmp(argv[i], "-check") == 0)
		{
			check = 1;
		}

		if (strcmp(argv[i], "-no-prio") == 0)
		{
			no_prio = 1;
		}

		if (strcmp(argv[i], "-size") == 0)
		{
			char *argptr;
			unsigned size = strtol(argv[++i], &argptr, 10);
			nthick = 130;
			ntheta = (size/128) + 2;
			STARPU_ASSERT((nthick - 2)*(ntheta - 2) == size);
		}

		if (strcmp(argv[i], "-h") == 0)
		{
			printf("usage : %s [-v1|-v2|-v3] [-pin] [-nthick number] [-ntheta number] [-shape [0|1|2]] [-cg] [-size number] [-no-prio]\n", argv[0]);
		}
	}
}



/*
 * The Finite element method code 
 *
 *   B              C
 *	**********
 *	*  0   * *
 *	*    *   *
 *	*  *   1 *
 *	**********
 *   A             D
 */

static inline float diff_psi(unsigned theta_tr, unsigned thick_tr, unsigned side_tr,
		 unsigned theta_psi, unsigned thick_psi, unsigned xy, point *pmesh)
{
	float xa,ya,xb,yb,xc,yc;
	float tmp;

	assert(theta_tr + 2 <= ntheta);
	assert(thick_tr + 2 <= nthick);

	/* A */
	xa = pmesh[NODE_NUMBER(theta_tr, thick_tr)].x;
	ya = pmesh[NODE_NUMBER(theta_tr, thick_tr)].y;

	/* B */
	if (side_tr)
	{
		/* lower D is actually B here */
		xb = pmesh[NODE_NUMBER(theta_tr+1, thick_tr)].x;
		yb = pmesh[NODE_NUMBER(theta_tr+1, thick_tr)].y;
	}
	else
	{
		/* upper */
		xb = pmesh[NODE_NUMBER(theta_tr, thick_tr+1)].x;
		yb = pmesh[NODE_NUMBER(theta_tr, thick_tr+1)].y;
	}

	xc = pmesh[NODE_NUMBER(theta_tr+1, thick_tr+1)].x;
	yc = pmesh[NODE_NUMBER(theta_tr+1, thick_tr+1)].y;

	/* now look for the actual psi node */
	if (NODE_NUMBER(theta_tr, thick_tr) == NODE_NUMBER(theta_psi, thick_psi))
	{
		/* A nothing to do */
	}
	else if (NODE_NUMBER(theta_tr+1, thick_tr+1) == NODE_NUMBER(theta_psi, thick_psi))
	{
		/* psi matches C */
		/* swap A and C coordinates  */
		tmp = xa; xa = xc; xc = tmp;
		tmp = ya; ya = yc; yc = tmp;
	}
	else if (side_tr && (NODE_NUMBER(theta_tr+1, thick_tr) == NODE_NUMBER(theta_psi, thick_psi)))
	{
		/* psi is D (that was stored in C) XXX */
		tmp = xa; xa = xb; xb = tmp;
		tmp = ya; ya = yb; yb = tmp;
	}
	else if	(!side_tr && (NODE_NUMBER(theta_tr, thick_tr+1) == NODE_NUMBER(theta_psi, thick_psi)))
	{
		/* psi is C */
		tmp = xa; xa = xb; xb = tmp;
		tmp = ya; ya = yb; yb = tmp;
	}
	else
	{
		/* the psi node is not a node of the current triangle */
		return 0.0f;
	}

	/* now the triangle should have A as the psi node */
	float denom;
	float value;

	denom = (xa - xb)*(yc - ya) - (xc - xb)*(ya - yb);

	switch (xy)
	{
		case X:
			value = (yc - yb)/denom;
			break;
		case Y:
			value = -(xc - xb)/denom;
			break;
		default:
			assert(0);
	}

	return value;
}

static inline float diff_y_psi(unsigned theta_tr, unsigned thick_tr, unsigned side_tr,
		 unsigned theta_psi, unsigned thick_psi, point *pmesh)
{
	return diff_psi(theta_tr, thick_tr, side_tr, theta_psi, thick_psi, Y, pmesh);
}

static inline float diff_x_psi(unsigned theta_tr, unsigned thick_tr, unsigned side_tr,
		 unsigned theta_psi, unsigned thick_psi, point *pmesh)
{
	return diff_psi(theta_tr, thick_tr, side_tr, theta_psi, thick_psi, X, pmesh);
}

static inline float surface_triangle(unsigned theta_tr, unsigned thick_tr, unsigned side_tr, point *pmesh)
{
	float surface;
	float tmp;

	float xi, xj, xk, yi, yj, yk;

	STARPU_ASSERT(theta_tr + 2 <= ntheta);
	STARPU_ASSERT(thick_tr + 2 <= nthick);

	xi = pmesh[NODE_NUMBER(theta_tr, thick_tr)].x;
	yi = pmesh[NODE_NUMBER(theta_tr, thick_tr)].y;

	xj = pmesh[NODE_NUMBER(theta_tr+1, thick_tr+1)].x;
	yj = pmesh[NODE_NUMBER(theta_tr+1, thick_tr+1)].y;

	if (side_tr)
	{
		/* lower */
		xk = pmesh[NODE_NUMBER(theta_tr+1, thick_tr)].x;
		yk = pmesh[NODE_NUMBER(theta_tr+1, thick_tr)].y;
	}
	else
	{
		xk = pmesh[NODE_NUMBER(theta_tr, thick_tr+1)].x;
		yk = pmesh[NODE_NUMBER(theta_tr, thick_tr+1)].y;
	}

	tmp = (xi - xj)*(yk -yj) - (xk - xj)*(yi -yj);

	surface = 0.5*fabs(tmp);

	return surface;
}

static inline float integral_triangle(int theta_tr, int thick_tr, unsigned side_tr,
			unsigned theta_i, unsigned thick_i, unsigned theta_j, unsigned thick_j, point *pmesh)
{
	float surface;
	float value;

	float dxi, dxj, dyi, dyj;

	if (theta_tr < 0) return 0.0f;
	if (theta_tr + 2  > (int)ntheta) return 0.0f;

	if (thick_tr < 0) return 0.0f;
	if (thick_tr + 2 > (int)nthick) return 0.0f;

	dxi = diff_x_psi(theta_tr, thick_tr, side_tr, theta_i, thick_i, pmesh);
	dyi = diff_y_psi(theta_tr, thick_tr, side_tr, theta_i, thick_i, pmesh);
	dxj = diff_x_psi(theta_tr, thick_tr, side_tr, theta_j, thick_j, pmesh);
	dyj = diff_y_psi(theta_tr, thick_tr, side_tr, theta_j, thick_j, pmesh);

	surface = surface_triangle(theta_tr, thick_tr, side_tr, pmesh);

	value = (dxi*dxj + dyi*dyj)*surface;

	return value;
}

static inline float integrale_sum(unsigned theta_i, unsigned thick_i, unsigned theta_j, unsigned thick_j, point *pmesh)
{
	float integral = 0.0f;

	integral += integral_triangle(theta_i - 1, thick_i - 1, 1, theta_i, thick_i, theta_j, thick_j, pmesh);
	integral += integral_triangle(theta_i - 1, thick_i - 1, 0, theta_i, thick_i, theta_j, thick_j, pmesh);
	integral += integral_triangle(theta_i - 1, thick_i, 1, theta_i, thick_i, theta_j, thick_j, pmesh);
	integral += integral_triangle(theta_i, thick_i, 0, theta_i, thick_i, theta_j, thick_j, pmesh);
	integral += integral_triangle(theta_i, thick_i, 1, theta_i, thick_i, theta_j, thick_j, pmesh);
	integral += integral_triangle(theta_i, thick_i - 1, 0, theta_i, thick_i, theta_j, thick_j, pmesh);

	return integral;
}


static float compute_A_value(unsigned i, unsigned j, point *pmesh)
{
	float value = 0.0f;

	unsigned thick_i, thick_j;
	unsigned theta_i, theta_j;

	/* add all contributions from all connex triangles  */
	thick_i = NODE_TO_THICK(i);
	thick_j = NODE_TO_THICK(j);

	theta_i = NODE_TO_THETA(i);
	theta_j = NODE_TO_THETA(j);

	/* Compute the Sum of all the integral over all triangles */
	if ( (abs(thick_i - thick_j) <= 1) && (abs(theta_i - theta_j) <= 1) )
	{
		if ( (theta_j == theta_i -1) && (thick_j == thick_i +1))
			goto done;

		if ( (theta_j == theta_i + 1) && (thick_j == thick_i  - 1))
			goto done;

		/* this may not be a null entry */
		value += integrale_sum(theta_i, thick_i, theta_j, thick_j, pmesh);
	}

done:
	return value;
}


#define TRANSLATE(k)	(RefArray[(k)])
#define TRANSLATEBACK(k)	(RefArrayBack[(k)])

static void solve_system(unsigned size, unsigned subsize, float *result, int *RefArray, float *Bformer, float *A, float *B)
{
	unsigned i;

	/* solve the actual problem LU X = B */
        /* solve LX' = Y with X' = UX */
        /* solve UX = X' */
	FPRINTF(stderr, "Solving the problem ...\n");

	float *savedB = NULL;
	float *LUB = NULL;

	if (check)
	{
		savedB = malloc(subsize*sizeof(float));
		memcpy(savedB, B, subsize*sizeof(float));
		LUB = malloc(subsize*sizeof(float));
	}

		/* L */
		STRSV("L", "N", "N", subsize, A, subsize, B, 1);
	
		/* U */
	        STRSV("U", "N", "U", subsize, A, subsize, B, 1);
	
		STARPU_ASSERT(DIM == size);
	
	if (check)
	{
		/* compute the error on (LUB - savedB) which should be 0 */
	
		/* LUB = B */
		memcpy(LUB, B, subsize*sizeof(float));
	
	
		/* LUB = U * LUB */
		STRMV("U", "N", "U", subsize, A, subsize, LUB, 1);
		
		/* LUB = L * LUB */
		STRMV("L", "N", "N", subsize, A, subsize, LUB, 1);
	
		/* LUB -= B */
		SAXPY(subsize, -1.0f, savedB, 1, LUB, 1);
	
		/* check if LUB is close to the 0 vector */
		int maxind = ISAMAX(subsize, LUB, 1);
		FPRINTF(stderr, "max error (LUX - B) = %e\n",LUB[maxind - 1]);

		float sum = SASUM(subsize, LUB, 1);
		FPRINTF(stderr,"avg. error %e\n", sum/subsize);
	
		free(LUB);
		free(savedB);
	}

	/* now display back the ACTUAL result */
	for (i = 0; i < subsize; i++)
	{
		result[TRANSLATE(i)] = B[i];
	}

	for (i = subsize ; i < size; i++)
	{
		result[TRANSLATE(i)] = Bformer[TRANSLATE(i)];
	}

}

unsigned compute_pivot_array(int *RefArray, int *RefArrayBack, unsigned size)
{
	unsigned k;
	unsigned index = 0;
	unsigned theta, thick;
	unsigned newsize;

	for (k = 0; k < size; k++)
	{
		RefArray[k] = k;
		RefArrayBack[k] = k;
	}

	/* first inner nodes */
	for (theta = 1; theta < ntheta - 1 ; theta++)
	{
		for (thick = 1; thick < nthick - 1; thick++) 
		{
			/* inner nodes are unknown */
			RefArrayBack[NODE_NUMBER(theta, thick)] = index;
			RefArray[index] = NODE_NUMBER(theta, thick);

			index++;
		}
	}

	newsize = index;

	for (theta=0; theta < ntheta; theta++)
	{
		/* Lower boundary "South" */
		RefArrayBack[NODE_NUMBER(theta, 0)] = index;
		RefArray[index++] = NODE_NUMBER(theta, 0);
		
		/* Upper boundary "North" */
		RefArrayBack[NODE_NUMBER(theta, nthick-1)] = index;
		RefArray[index++] = NODE_NUMBER(theta, nthick-1);
	}

	for (thick = 1; thick < nthick -1; thick++)
	{
		/* "West "*/
		RefArrayBack[NODE_NUMBER(0, thick)] = index;
		RefArray[index++] = NODE_NUMBER(0, thick);

		/* "East" */
		RefArrayBack[NODE_NUMBER(ntheta-1, thick)] = index;
		RefArray[index++] = NODE_NUMBER(ntheta-1, thick);
	}

	assert(index == size);

	return newsize;
}

void build_mesh(point *mesh)
{
	unsigned theta, thick;

	/* first build the mesh by determining all points positions */
	for (theta = 0; theta < ntheta; theta++)
	{
		float angle;
		angle = (ntheta - 1 - theta) * Pi/(ntheta-1);

		for (thick = 0; thick < nthick; thick++)
		{
			float r;
			r = thick * (RMAX - RMIN)/(nthick - 1) + RMIN;

			switch (shape)
			{
				default:
				case 0:
					mesh[NODE_NUMBER(theta,thick)].x = r*cosf(angle);
					mesh[NODE_NUMBER(theta,thick)].y = r*sinf(angle);
					break;
				case 1:
					mesh[NODE_NUMBER(theta,thick)].x =
							-100 + RMIN+((RMAX-RMIN)*theta)/(ntheta - 1);
					mesh[NODE_NUMBER(theta,thick)].y = 
							RMIN+((RMAX-RMIN)*thick)/(nthick - 1);
					break;
				case 2:
					mesh[NODE_NUMBER(theta,thick)].x = r*(2.0f*theta/(ntheta - 1)- 1.0f);
					mesh[NODE_NUMBER(theta,thick)].y = r*(2.0f*thick/(nthick - 1)- 1.0f);
					break;
			}
		}
	}
}

static unsigned long build_neighbour_vector(unsigned long*neighbours, unsigned node, int *RefArray, int *RefArrayBack)
{
	/* where is that point in the former space ? */
	int former = TRANSLATE(node);
	int former_thick, former_theta;
	former_thick= (int)NODE_TO_THICK(former);
	former_theta = (int)NODE_TO_THETA(former);

	/* do a list of all the possible neighbours */
	unsigned nneighbours = 0;

	int dtheta, dthick;
	for (dthick = -1; dthick <= 1; dthick++)
	{
		if ((former_thick + dthick) >= 0 && (former_thick + dthick) <= (int)nthick )
		{
			for (dtheta = -1; dtheta <= 1; dtheta++)
			{
				if ((former_theta + dtheta) >= 0 && (former_theta + dtheta) <= (int)ntheta )
				{
					/* we got a possible neighbour */
					unsigned pnode = 
						NODE_NUMBER((former_theta + dtheta), (former_thick + dthick));

					neighbours[nneighbours++] = TRANSLATEBACK(pnode);
				}
			}
		}
	}

	unsigned i;
	/* order that list */
	for (i = 0; i < nneighbours; i++)
	{
		/* find the i^th smallest entry for position i */
		unsigned index;
		unsigned min , min_index;

		min = neighbours[i];
		min_index = i;

		for (index = i+1; index < nneighbours; index++)
		{
			STARPU_ASSERT(neighbours[i] != neighbours[index]);

			if (neighbours[index] < min)
			{
				min = neighbours[index];
				min_index = index;
			}
		}

		/* swap values */
		neighbours[min_index] = neighbours[i];
		neighbours[i] = min;
	}

	return nneighbours;
}

static void build_sparse_stiffness_matrix_B(point *pmesh, float *B, float *Bformer, unsigned size, unsigned newsize, int *RefArray, int *RefArrayBack)
{
	unsigned i,j;

	/* first give the value of known nodes (at boundaries) */
	for (i = 0; i < size; i++)
	{
		Bformer[i] = 0.0f;
	}

	for (i = 0; i < nthick; i++)
	{
		Bformer[i] = 200.0f;
		Bformer[size-1-i] = 200.0f;
	}

	for (i = 1; i < ntheta-1; i++)
	{
		Bformer[i*nthick] = 200.0f;
		Bformer[(i+1)*nthick-1] = 100.0f;
	}

	/* now the actual stiffness (reordered) matrix*/
	for (j = 0 ; j < newsize ; j++)
	{

		unsigned long neighbour;
		unsigned long nneighbours;
		unsigned long neighbours[9];

		nneighbours = build_neighbour_vector(&neighbours[0], j, RefArray, RefArrayBack);

		B[j] = Bformer[TRANSLATE(j)];

		for (neighbour = 0; neighbour < nneighbours; neighbour++)
		{
			unsigned n = neighbours[neighbour]; 
			if (n >= newsize)
			{
				B[j] -= compute_A_value(TRANSLATE(n), TRANSLATE(j), pmesh)*Bformer[TRANSLATE(n)];
			}
		}
	}
}

static unsigned build_sparse_stiffness_matrix_A(point *pmesh, float **nzval, uint32_t **colind, 
						uint32_t *rowptr, unsigned newsize, int *RefArray, int *RefArrayBack)
{
	unsigned j;

	unsigned pos = 0;

	*nzval = NULL;
	*colind = NULL;

	/* now the actual stiffness (reordered) matrix*/
	for (j = 0 ; j < newsize ; j++)
	{
		rowptr[j] = pos;

		unsigned long neighbour;
		unsigned long nneighbours;
		unsigned long neighbours[9];

		nneighbours = build_neighbour_vector(&neighbours[0], j, RefArray, RefArrayBack);

		for (neighbour = 0; neighbour < nneighbours; neighbour++)
		{
			float val;
			unsigned nodeneighbour =  neighbours[neighbour];

			if (nodeneighbour < newsize)
			{

				val = compute_A_value(TRANSLATE(j), TRANSLATE(nodeneighbour), pmesh);
	
				if (val != 0.0f)
				{
					*nzval = realloc(*nzval, (pos+1)*sizeof(float));
					*colind = realloc(*colind, (pos+1)*sizeof(uint32_t));
	
					(*nzval)[pos] = val;
					(*colind)[pos] = nodeneighbour;

					pos++;
				}
			}
		}
	}

	rowptr[newsize] = pos;



	return pos;
}

static void build_dense_stiffness_matrix_A(point *pmesh, float *A, unsigned newsize, int *RefArray, int *RefArrayBack)
{
	unsigned long j;

	/* touch all the memory */
	memset(A, 0, newsize*newsize*sizeof(float));

	/* now the actual stiffness (reordered) matrix*/
	for (j = 0 ; j < newsize ; j++)
	{
		unsigned long neighbour;
		unsigned long nneighbours;
		unsigned long neighbours[9];

		nneighbours = build_neighbour_vector(&neighbours[0], j, RefArray, RefArrayBack);

		for (neighbour = 0; neighbour < nneighbours; neighbour++)
		{
			unsigned long nodeneighbour =  neighbours[neighbour];

			if (nodeneighbour < newsize)
			{
				float val;
				val = compute_A_value(TRANSLATE(j), TRANSLATE(nodeneighbour), pmesh);
				A[j+ (unsigned long)newsize*nodeneighbour] = val;
			}
		}
	}
}

int main(int argc, char **argv)
{
	float *A;
	float *B;

	unsigned newsize;
	float *result;
	int *RefArray, *RefArrayBack;
	point *pmesh;
	float *Bformer;

	parse_args(argc, argv);

	pmesh = malloc(DIM*sizeof(point));
	RefArray = malloc(DIM*sizeof(int));
	RefArrayBack = malloc(DIM*sizeof(int));
	Bformer = malloc(DIM*sizeof(float));
	result = malloc(DIM*sizeof(float));

	build_mesh(pmesh);

	/* now simplify that problem given the boundary conditions 
	 * to do so, we remove the already known variables from the system
	 * by pivoting the various know variable, RefArray keep track of that
	 * pivoting */ 
	newsize = compute_pivot_array(RefArray, RefArrayBack, DIM);

	/* we can either use a direct method (LU decomposition here) or an 
	 * iterative method (conjugate gradient here) */
	if (use_cg)
	{
		unsigned nnz;
		float *nzval;
		uint32_t *colind;
		uint32_t *rowptr;

		rowptr = malloc((newsize+1)*sizeof(uint32_t));

		B = malloc(newsize*sizeof(float));

		build_sparse_stiffness_matrix_B(pmesh, B, Bformer, DIM, newsize, RefArray, RefArrayBack);

		nnz = build_sparse_stiffness_matrix_A(pmesh, &nzval, &colind, rowptr, newsize, RefArray, RefArrayBack);

		do_conjugate_gradient(nzval, B, result, nnz, newsize, colind, rowptr);

		/* XXX */
		memcpy(B, result, newsize*sizeof(float));

		/* now display back the ACTUAL result */
		unsigned i;
		for (i = 0; i < newsize; i++)
		{
			result[TRANSLATE(i)] = B[i];
		}
	
		for (i = newsize ; i < DIM; i++)
		{
			result[TRANSLATE(i)] = Bformer[TRANSLATE(i)];
		}
	
	}
	else
	{

		/* unfortunately CUDA does not allow late memory registration, 
		 * we need to do the malloc using CUDA itself ... */
		initialize_system(&A, &B, newsize, pinned);

		/* then build the stiffness matrix A */
		build_sparse_stiffness_matrix_B(pmesh, B, Bformer, DIM, newsize, RefArray, RefArrayBack);

		build_dense_stiffness_matrix_A(pmesh, A, newsize, RefArray, RefArrayBack);

		FPRINTF(stderr, "Problem size : %ux%u (%ux%u) (%lu MB)\n", newsize, newsize, DIM, DIM, ((unsigned long)newsize*newsize*4UL)/(1024*1024));

		STARPU_ASSERT(newsize % nblocks == 0);

		switch (version)
		{
			case 1:
			case 2:
				dw_factoLU(A, newsize, newsize, nblocks, version, no_prio);
				break;
			case 3:
				dw_factoLU_tag(A, newsize, newsize, nblocks, no_prio);
				break;
			case 4:
				dw_factoLU_grain(A, newsize, newsize, nblocks, nbigblocks);
				break;
			default:
				STARPU_ABORT();
		}

		display_stat_heat();

		if (check)
			solve_system(DIM, newsize, result, RefArray, Bformer, A, B);

		starpu_shutdown();
		free_system(A, B, newsize, pinned);
	}

#ifdef STARPU_OPENGL_RENDER
	opengl_render(ntheta, nthick, result, pmesh, argc, argv);
#endif

	free(pmesh);
	free(RefArray);
	free(RefArrayBack);
	free(Bformer);
	free(result);

	return 0;
}
