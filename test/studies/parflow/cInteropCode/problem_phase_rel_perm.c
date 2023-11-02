/*BHEADER*********************************************************************
 *
 *  Copyright (c) 1995-2009, Lawrence Livermore National Security,
 *  LLC. Produced at the Lawrence Livermore National Laboratory. Written
 *  by the Parflow Team (see the CONTRIBUTORS file)
 *  <parflow@lists.llnl.gov> CODE-OCEC-08-103. All rights reserved.
 *
 *  This file is part of Parflow. For details, see
 *  http://www.llnl.gov/casc/parflow
 *
 *  Please read the COPYRIGHT file or Our Notice and the LICENSE file
 *  for the GNU Lesser General Public License.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License (as published
 *  by the Free Software Foundation) version 2.1 dated February 1999.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms
 *  and conditions of the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 *  USA
 **********************************************************************EHEADER*/

#include "parflow.h"

#include <assert.h>

#ifdef PARFLOW_HAVE_CHAPEL
#include "ChapelImpl.h"
#endif
/*--------------------------------------------------------------------------
 * Structures
 *--------------------------------------------------------------------------*/

typedef struct {
  NameArray regions;

  int type;     /* input type */
  void  *data;  /* pointer to Type structure */

  int time_index;
} PublicXtra;

typedef struct {
  Grid   *grid;
  double *temp_data;
} InstanceXtra;

typedef struct {
  int num_regions;
  int    *region_indices;
  double *values;
} Type0;


typedef struct {
  double min_pressure_head;
  int num_sample_points;

  double *x;
  double *a;
  double *d;
  double *a_der;
  double *d_der;

  /* used by linear interpolation method */
  double *slope;
  double *slope_der;

  int interpolation_method;

  double interval;
} VanGTable;

typedef struct {
  int num_regions;
  int    *region_indices;
  int data_from_file;
  double *alphas;
  double *ns;
  char   *alpha_file;
  char   *n_file;
  Vector *alpha_values;
  Vector *n_values;

  VanGTable **lookup_tables;

#ifdef PF_PRINT_VG_TABLE
  int     *print_table;
#endif
} Type1;                      /* Van Genuchten Rel. Perm. */

typedef struct {
  int num_regions;
  int    *region_indices;
  double *As;
  double *gammas;
} Type2;                      /* Haverkamp, et.al. Rel. Perm. */

typedef struct {
  int num_regions;
  int    *region_indices;
} Type3;                      /* Data points for Rel. Perm. */

typedef struct {
  int num_regions;
  int    *region_indices;
  int    *degrees;
  double **coefficients;
} Type4;                      /* Polynomial Function for Rel. Perm. */


VanGTable *VanGComputeTable(
                            int    interpolation_method,
                            int    num_sample_points,
                            double min_pressure_head,
                            double alpha,
                            double n)
{
  double *x, *a, *d, *a_der, *d_der;

  VanGTable *new_table = ctalloc(VanGTable, 1);

  new_table->interpolation_method = interpolation_method;

  new_table->num_sample_points = num_sample_points;
  new_table->min_pressure_head = min_pressure_head;

  new_table->x = ctalloc(double, num_sample_points + 1); // interpolation points
  new_table->a = ctalloc(double, num_sample_points + 1); // function value at interpolation point
  new_table->d = ctalloc(double, num_sample_points + 1); // derivative used in monotonic spline
  new_table->a_der = ctalloc(double, num_sample_points + 1); // function derivative value
  new_table->d_der = ctalloc(double, num_sample_points + 1); // derivative of function derivative

  /* Fill in slope for linear interpolation */
  if (interpolation_method == 1)
  {
    new_table->slope = ctalloc(double, num_sample_points + 1);      // slope for linear interpolation
    new_table->slope_der = ctalloc(double, num_sample_points + 1);  // slope for linear interpolation
  }

  x = new_table->x;
  a = new_table->a;
  d = new_table->d;
  a_der = new_table->a_der;
  d_der = new_table->d_der;

  double h[num_sample_points + 1];
  double f[num_sample_points + 1], del[num_sample_points + 1], f_der[num_sample_points + 1];
  double del_der[num_sample_points + 1];
  double alph, beta, magn;
  int index;
  double interval, m;

  // Loop over sample min_pressure_head to 0.0, min_pressure_head/num_sample_points step
  interval = min_pressure_head / (double)(num_sample_points - 1);
  new_table->interval = fabs(interval);
  m = 1.0e0 - (1.0e0 / n);

  // evenly spaced interpolation points (future: variably spaced points)
  for (index = 0; index <= num_sample_points; index++)
  {
    x[index] = fabs(index * interval);
    double opahn = 1.0 + pow(alpha * x[index], n);
    double ahnm1 = pow(alpha * x[index], n - 1);
    // calculating function at interpolation points
    a[index] = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
               / pow(opahn, (m / 2));

    double coeff = 1.0 - ahnm1 * pow(opahn, -m);
    // calculating derivative at interpolation points
    a_der[index] = 2.0 * (coeff / (pow(opahn, (m / 2))))
                   * ((n - 1) * pow(alpha * x[index], n - 2) * alpha
                      * pow(opahn, -m)
                      - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                   + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                   * n * alpha * ahnm1;
    //CPS fix of 1<n<2, K is infinite at pressure head = 0
    if ((n < 2) && (index == 0))
    {
      a_der[index] = 0;
    }
  }

  /* Fill in slope for linear interpolation */
  if (interpolation_method == 1)
  {
    for (index = 0; index < num_sample_points; index++)
    {
      new_table->slope[index] = (a[index + 1] - a[index]) /
                                new_table->interval;
      new_table->slope_der[index] = (a_der[index + 1] - a_der[index]) /
                                    new_table->interval;
    }
  }

  // begin monotonic spline (see Fritsch and Carlson, SIAM J. Num. Anal., 17 (2), 1980)
  for (index = 0; index < num_sample_points; index++)
  {
    h[index] = x[index + 1] - x[index];
    f[index] = a[index + 1] - a[index];
    del[index] = f[index] / h[index];
    f_der[index] = a_der[index + 1] - a_der[index];
    del_der[index] = f_der[index] / h[index];
  }
  d[0] = del[0];
  d[num_sample_points] = del[num_sample_points - 1];
  d_der[0] = del_der[0];
  d_der[num_sample_points] = del_der[num_sample_points - 1];

  for (index = 1; index < num_sample_points; index++)
  {
    d[index] = (del[index - 1] + del[index]) / 2;
    d_der[index] = (del_der[index - 1] + del_der[index]) / 2;
  }


  for (index = 0; index < num_sample_points; index++)
  {
    if (del[index] == 0.0)
    {
      d[index] = 0;
      d[index + 1] = 0;
    }
    else
    {
      alph = d[index] / del[index];
      beta = d[index + 1] / del[index];
      magn = pow(alph, 2) + pow(beta, 2);
      if (magn > 9.0)
      {
        d[index] = 3 * alph * del[index] / magn;
        d[index + 1] = 3 * beta * del[index] / magn;
      }
    }

    if (del_der[index] == 0.0)
    {
      d_der[index] = 0;
      d_der[index + 1] = 0;
    }
    else
    {
      // to ensure monotonicity
      alph = d_der[index] / del_der[index];
      beta = d_der[index + 1] / del_der[index];
      magn = pow(alph, 2) + pow(beta, 2);
      if (magn > 9.0)
      {
        d_der[index] = 3 * alph * del_der[index] / magn;
        d_der[index + 1] = 3 * beta * del_der[index] / magn;
      }
    }
  }
  return new_table;
}

__host__ __device__
static inline double VanGLookupSpline(
                                      double     pressure_head,
                                      VanGTable *lookup_table,
                                      int        fcn)
{
  double rel_perm, t;
  int pt = 0;
  int num_sample_points = lookup_table->num_sample_points;
  double min_pressure_head = lookup_table->min_pressure_head;
  int max = num_sample_points + 1;

  // This table goes from 0 to fabs(min_pressure_head)
  assert(pressure_head >= 0);

  // SGS TODO add warning in output?
  // Make sure values are in the table range, if lower then set to the 0.0 which is limit of VG curve
  if (pressure_head >= fabs(min_pressure_head))
  {
    return 0.0;
  }
  else
  {
    // Use direct table lookup to avoid using this binary search since
    // we have uniformly spaced points.
    double interval = lookup_table->interval;
    pt = (int)floor(pressure_head / interval);
    if (pt > max)
    {
      pt = max - 1;
    }

#if 0
    // When using variably spaced interpolation points, use binary
    // search to find the interval
    {
      int min = 0;
      int mid;

      while (max != min + 1)
      {
        mid = min + floor((max - min) / 2);
        if (pressure_head == lookup_table->x[mid])
        {
          min = mid;
          max = min + 1;
        }
        if (pressure_head < lookup_table->x[mid])
        {
          max = mid;
        }
        else
        {
          min = mid;
        }
      }
      pt = min;
    }
#endif
  }

  double x = lookup_table->x[pt];
  double a = lookup_table->a[pt];
  double d = lookup_table->d[pt];
  double a_der = lookup_table->a_der[pt];
  double d_der = lookup_table->d_der[pt];

  // using cubic Hermite interpolation
  if (fcn == CALCFCN)
  {
    t = (pressure_head - x) / (lookup_table->x[pt + 1] - x);
    rel_perm = (2.0 * pow(t, 3) - 3.0 * pow(t, 2) + 1.0) * a
               + (pow(t, 3) - 2.0 * pow(t, 2) + t)
               * (lookup_table->x[pt + 1] - x) * d + (-2.0 * pow(t, 3)
                                                      + 3.0 * pow(t, 2)) * (lookup_table->a[pt + 1])
               + (pow(t, 3) - pow(t, 2)) * (lookup_table->x[pt + 1] - x)
               * (lookup_table->d[pt + 1]);
  }
  else
  {
    t = (pressure_head - x) / (lookup_table->x[pt + 1] - x);
    rel_perm = (2.0 * pow(t, 3) - 3.0 * pow(t, 2) + 1.0) * a_der
               + (pow(t, 3) - 2.0 * pow(t, 2) + t)
               * (lookup_table->x[pt + 1] - x) * d_der + (-2.0 * pow(t, 3)
                                                          + 3.0 * pow(t, 2)) * (lookup_table->a_der[pt + 1])
               + (pow(t, 3) - pow(t, 2)) * (lookup_table->x[pt + 1] - x)
               * (lookup_table->d_der[pt + 1]);
  }

  return rel_perm;
}

static inline double VanGLookupLinear(
                                      double     pressure_head,
                                      VanGTable *lookup_table,
                                      int        fcn)
{
  double rel_perm = 0.0;
  int pt = 0;
  int num_sample_points = lookup_table->num_sample_points;
  double min_pressure_head = lookup_table->min_pressure_head;
  int max = num_sample_points + 1;
  PF_UNUSED(max);

  // This table goes from 0 to fabs(min_pressure_head)
  assert(pressure_head >= 0);

  // SGS TODO add warning in output?
  // Make sure values are in the table range, if lower then set to the 0.0 which is limit of VG curve
  if (pressure_head < fabs(min_pressure_head))
  {
    double interval = lookup_table->interval;

    // Use direct table lookup to avoid using this binary search since
    // we have uniformly spaced points.

    pt = (int)floor(pressure_head / interval);
    assert(pt < max);

    // using cubic Hermite interpolation


    if (fcn == CALCFCN)
    {
      rel_perm = lookup_table->a[pt] + lookup_table->slope[pt] * (pressure_head - lookup_table->x[pt]);
    }
    else
    {
      rel_perm = lookup_table->a_der[pt] + lookup_table->slope_der[pt] * (pressure_head - lookup_table->x[pt]);
    }
  }

  return rel_perm;
}


/*--------------------------------------------------------------------------
 * PhaseRelPerm:
 *    This routine calculates relative permeabilities given a set of
 *    pressures.
 *--------------------------------------------------------------------------*/

void         PhaseRelPerm(

                          Vector *     phase_rel_perm, /* Vector of return rel. perms. at each block */
                          Vector *     phase_pressure, /* Vector of pressures at each block */
                          Vector *     phase_density, /* Vector of densities at each block */
                          double       gravity, /* Magnitude of gravity in neg. z direction */
                          ProblemData *problem_data, /* Contains geometry info for the problem */
                          int          fcn) /* Flag determining what to calculate
                                             * fcn = CALCFCN => calculate the function value
                                             * fcn = CALCDER => calculate the function
                                             *                  derivative */

{
  fprintf(stdout, "PhaseRelPerm\n");
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Grid          *grid = VectorGrid(phase_rel_perm);

  GrGeomSolid   *gr_solid;

  Type0         *dummy0;
  Type1         *dummy1;
  Type2         *dummy2;
  Type4         *dummy4;

  Subvector     *pr_sub;
  Subvector     *pp_sub;
  Subvector     *pd_sub;
  Subvector     *n_values_sub;
  Subvector     *alpha_values_sub;

  double        *prdat, *ppdat, *pddat;
  double        *n_values_dat, *alpha_values_dat;

  SubgridArray  *subgrids = GridSubgrids(grid);

  Subgrid       *subgrid;

  int sg;

  int ix, iy, iz;
  int nx, ny, nz;

  int i, j, k, r;

  int num_regions, *region_indices;
  int ir, *fdir;

  /*-----------------------------------------------------------------------
   * Begin timing
   *-----------------------------------------------------------------------*/

  BeginTiming(public_xtra->time_index);


  /* Initialize relative permeabilities to 0.0 */
  InitVectorAll(phase_rel_perm, 0.0);

  switch ((public_xtra->type))
  {
    case 0:   /* Constant relative permeability within regions */
    {
      double  *values;
      int ir;

      dummy0 = (Type0*)(public_xtra->data);

      num_regions = (dummy0->num_regions);
      region_indices = (dummy0->region_indices);
      values = (dummy0->values);

      /* Compute rel perms for Dirichlet boundary conditions */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);
          pr_sub = VectorSubvector(phase_rel_perm, sg);

          ix = SubgridIX(subgrid);
          iy = SubgridIY(subgrid);
          iz = SubgridIZ(subgrid);

          nx = SubgridNX(subgrid);
          ny = SubgridNY(subgrid);
          nz = SubgridNZ(subgrid);

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);

          if (fcn == CALCFCN)
          {
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              prdat[ipr] = values[ir];
            });
          }
          else    /* fcn = CALCDER */
          {
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              prdat[ipr] = 0.0;
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End loop over regions */

      /* Compute rel perms inside regions */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);
          pr_sub = VectorSubvector(phase_rel_perm, sg);

          ix = SubgridIX(subgrid) - 1;
          iy = SubgridIY(subgrid) - 1;
          iz = SubgridIZ(subgrid) - 1;

          nx = SubgridNX(subgrid) + 2;
          ny = SubgridNY(subgrid) + 2;
          nz = SubgridNZ(subgrid) + 2;

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          if (fcn == CALCFCN)
          {
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              prdat[ipr] = values[ir];
            });
          }
          else    /* fcn = CALCDER */
          {
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              prdat[ipr] = 0.0;
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End loop over regions */
      break;
    }        /* End case 0 */

    case 1: /* Van Genuchten relative permeability */
    {
      int data_from_file;
      double  *alphas, *ns;

      Vector  *n_values, *alpha_values;

      dummy1 = (Type1*)(public_xtra->data);

      num_regions = (dummy1->num_regions);
      region_indices = (dummy1->region_indices);
      alphas = (dummy1->alphas);
      ns = (dummy1->ns);
      data_from_file = (dummy1->data_from_file);

      /* Compute rel perms for Dirichlet boundary conditions */
      if (data_from_file == 0)  /* alphas and ns given by region */
      {
        for (ir = 0; ir < num_regions; ir++)
        {
          gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

          ForSubgridI(sg, subgrids)
          {
            subgrid = SubgridArraySubgrid(subgrids, sg);

            pr_sub = VectorSubvector(phase_rel_perm, sg);
            pp_sub = VectorSubvector(phase_pressure, sg);
            pd_sub = VectorSubvector(phase_density, sg);

            ix = SubgridIX(subgrid);
            iy = SubgridIY(subgrid);
            iz = SubgridIZ(subgrid);

            nx = SubgridNX(subgrid);
            ny = SubgridNY(subgrid);
            nz = SubgridNZ(subgrid);

            r = SubgridRX(subgrid);

            prdat = SubvectorData(pr_sub);
            ppdat = SubvectorData(pp_sub);
            pddat = SubvectorData(pd_sub);

#ifdef PF_PRINT_VG_TABLE
            /*
             * This is a debugging tool that prints out function eval
             * and table values */
            if (dummy1->print_table[ir] && (dummy1->lookup_tables[ir]))
            {
              dummy1->print_table[ir] = 0;

              for (double head = 0.0; head < fabs((dummy1->lookup_tables[ir])->min_pressure_head); head += 0.001)
              {
                double alpha = alphas[ir];
                double n = ns[ir];
                double m = 1.0e0 - (1.0e0 / n);

                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                double f_val = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
                               / pow(opahn, (m / 2));

                printf("DebugTableFn, %d, %e, %e, %e, %e, %e\n",
                       ir,
                       head,
                       f_val,
                       VanGLookupLinear(head,
                                        dummy1->lookup_tables[ir],
                                        CALCFCN),
                       fabs(VanGLookupLinear(head,
                                             dummy1->lookup_tables[ir],
                                             CALCFCN) - f_val),
                       fabs((VanGLookupLinear(head,
                                              dummy1->lookup_tables[ir],
                                              CALCFCN) - f_val) / f_val)

                       );
              }

              for (double head = 0.0; head < fabs((dummy1->lookup_tables[ir])->min_pressure_head); head += 0.001)
              {
                double alpha = alphas[ir];
                double n = ns[ir];
                double m = 1.0e0 - (1.0e0 / n);

                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                double coeff = 1.0 - ahnm1 * pow(opahn, -m);

                double f_val = 2.0 * (coeff / (pow(opahn, (m / 2))))
                               * ((n - 1) * pow(alpha * head, n - 2) * alpha
                                  * pow(opahn, -m)
                                  - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                               + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                               * n * alpha * ahnm1;

                printf("DebugTableDer, %d, %e, %e, %e, %e, %e\n",
                       ir,
                       head,
                       f_val,
                       VanGLookupLinear(head,
                                        dummy1->lookup_tables[ir],
                                        CALCDER),
                       fabs(VanGLookupLinear(head,
                                             dummy1->lookup_tables[ir],
                                             CALCDER) - f_val),
                       fabs((VanGLookupLinear(head,
                                              dummy1->lookup_tables[ir],
                                              CALCDER) - f_val) / f_val)
                       );
              }
            }
#endif

            if (fcn == CALCFCN)
            {
              if (dummy1->lookup_tables[ir])
              {
                switch (dummy1->lookup_tables[ir]->interpolation_method)
                {
                  // Spline
                  case 0:
                  {
                    fprintf(stderr, "Van Genuchten, CALCFCN, spline\n");
                    fflush(stderr);
                    GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                                   nx, ny, nz,
                    {
                      /* Table Lookup */

                      int ipr = SubvectorEltIndex(pr_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipp = SubvectorEltIndex(pp_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipd = SubvectorEltIndex(pd_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 1.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                        prdat[ipr] = VanGLookupSpline(head,
                                                      dummy1->lookup_tables[ir],
                                                      CALCFCN);
                      }
                    });
                  }
                  break;

                  // Linear
                  case 1:
                  {
                    VanGTable *lookup_table = dummy1->lookup_tables[ir];
                    double interval = lookup_table->interval;
                    double min_pressure_head = lookup_table->min_pressure_head;
                    int num_sample_points = lookup_table->num_sample_points;
                    int max = num_sample_points + 1;
		    PF_UNUSED(max);
                    fprintf(stderr, "Van Genuchten, CALCFCN, linear\n");
                    fflush(stderr);
                    
                    GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                                   nx, ny, nz,
                    {
                      /* Table Lookup */

                      int ipr = SubvectorEltIndex(pr_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipp = SubvectorEltIndex(pp_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipd = SubvectorEltIndex(pd_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 1.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                        if (head < fabs(min_pressure_head))
                        {
                          int pt = (int)floor(head / interval);
                          assert(pt < max);

                          prdat[ipr] = lookup_table->a[pt] + lookup_table->slope[pt] *
                                       (head - lookup_table->x[pt]);
                        }
                        else
                        {
                          prdat[ipr] = 0.0;
                        }
                      }
                    });
                  }
                  break;
                }
              }
              else
              {
                /* Compute VanG curve */

                
                #if defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                double * pr_sub_chapel = pr_sub->data;
                double * pp_sub_chapel = pp_sub->data;
                double * pd_sub_chapel = pd_sub->data;
                double * alphas_chapel = alphas;
                double * ns_chapel = ns;
                calcfcn_compute_vang_curve_surface(gr_solid, r, ix, iy, iz, nx, ny, nz, pr_sub_chapel, pp_sub_chapel, pd_sub_chapel, alphas_chapel, ns_chapel, gravity, ir, SubvectorIX(pr_sub), SubvectorIY(pr_sub), SubvectorIZ(pr_sub), SubvectorNX(pr_sub), SubvectorNY(pr_sub));

                #endif
                #if !defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                
                GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                               nx, ny, nz,
                {

                  int ipr = SubvectorEltIndex(pr_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);
                  int ipp = SubvectorEltIndex(pp_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);
                  int ipd = SubvectorEltIndex(pd_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);
                  if (ipr != ipp || ipr != ipd) {
                    printf("ipr,ipp,ipd: %d %d %d\n", ipr, ipp, ipd);
                  }
                  
                  if (ppdat[ipp] >= 0.0)
                    prdat[ipr] = 1.0;
                  else
                  {
                    double alpha = alphas[ir];
                    double n = ns[ir];
                    double m = 1.0e0 - (1.0e0 / n);

                    double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                    double opahn = 1.0 + pow(alpha * head, n);
                    double ahnm1 = pow(alpha * head, n - 1);
                    prdat[ipr] = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
                                 / pow(opahn, (m / 2));

                  }
                });
                #endif
               
              }
            }
            else  /* fcn = CALCDER */
            {
              if (dummy1->lookup_tables[ir])
              {
                switch (dummy1->lookup_tables[ir]->interpolation_method)
                {
                  // Spline
                  case 0:
                  {
                    fprintf(stderr, "Van Genuchten, CALCDER, spline\n");
                    fflush(stderr);
                    GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                                   nx, ny, nz,
                    {
                      /* Table Lookup */

                      int ipr = SubvectorEltIndex(pr_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipp = SubvectorEltIndex(pp_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipd = SubvectorEltIndex(pd_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 0.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                        prdat[ipr] = VanGLookupSpline(head,
                                                      dummy1->lookup_tables[ir],
                                                      CALCDER);
                      }
                    });
                  }
                  break;

                  // Linear
                  case 1:
                  {
                    VanGTable *lookup_table = dummy1->lookup_tables[ir];
                    double interval = lookup_table->interval;
                    double min_pressure_head = lookup_table->min_pressure_head;
                    int num_sample_points = lookup_table->num_sample_points;
                    int max = num_sample_points + 1;
		    PF_UNUSED(max);
                    fprintf(stderr, "Van Genuchten, CALCDER, linear\n");
                    fflush(stderr);
                    GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                                   nx, ny, nz,
                    {
                      /* Table Lookup */

                      int ipr = SubvectorEltIndex(pr_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipp = SubvectorEltIndex(pp_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);
                      int ipd = SubvectorEltIndex(pd_sub,
                                              i + fdir[0], j + fdir[1], k + fdir[2]);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 0.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                        if (ppdat[ipp] >= 0.0)
                          prdat[ipr] = 1.0;
                        else
                        {
                          head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                          if (head < fabs(min_pressure_head))
                          {
                            int pt = (int)floor(head / interval);
                            assert(pt < max);

                            prdat[ipr] = lookup_table->a_der[pt] + lookup_table->slope_der[pt] *
                                         (head - lookup_table->x[pt]);
                          }
                          else
                          {
                            prdat[ipr] = 0.0;
                          }
                        }
                      }
                    });
                  }
                  break;
                }
              }
              else
              {
                /* Compute VanG curve */
                //fprintf(stderr, "Van Genuchten, CALCDER, calc curve\n");
                //fflush(stderr);
                #if defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                double * pr_sub_chapel = pr_sub->data;
                double * pp_sub_chapel = pp_sub->data;
                double * pd_sub_chapel = pd_sub->data;
                double * alphas_chapel = alphas;
                double * ns_chapel = ns;
                
                calcder_compute_vang_curve_surface(gr_solid, r, ix, iy, iz, nx, ny, nz, pr_sub_chapel, pp_sub_chapel, pd_sub_chapel, alphas_chapel, ns_chapel, gravity, ir, SubvectorIX(pr_sub), SubvectorIY(pr_sub), SubvectorIZ(pr_sub), SubvectorNX(pr_sub), SubvectorNY(pr_sub));
                #endif
                #if !defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                               nx, ny, nz,
                {
                  int ipr = SubvectorEltIndex(pr_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);
                  int ipp = SubvectorEltIndex(pp_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);
                  int ipd = SubvectorEltIndex(pd_sub,
                                          i + fdir[0], j + fdir[1], k + fdir[2]);

                  if (ppdat[ipp] >= 0.0)
                    prdat[ipr] = 0.0;
                  else
                  {
                    double alpha = alphas[ir];
                    double n = ns[ir];
                    double m = 1.0e0 - (1.0e0 / n);

                    double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                    double opahn = 1.0 + pow(alpha * head, n);
                    double ahnm1 = pow(alpha * head, n - 1);
                    double coeff = 1.0 - ahnm1 * pow(opahn, -m);

                    prdat[ipr] = 2.0 * (coeff / (pow(opahn, (m / 2))))
                                 * ((n - 1) * pow(alpha * head, n - 2) * alpha
                                    * pow(opahn, -m)
                                    - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                                 + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                                 * n * alpha * ahnm1;
                  }
                });
                #endif
                
              }
            }   /* End else clause */
          }     /* End subgrid loop */
        }       /* End loop over regions */
      }         /* End if data not from file */

      else if (data_from_file == 1)  /* ns and alphas from pfb file */
      {
        gr_solid = ProblemDataGrDomain(problem_data);
        n_values = dummy1->n_values;
        alpha_values = dummy1->alpha_values;

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);
          pd_sub = VectorSubvector(phase_density, sg);

          n_values_sub = VectorSubvector(n_values, sg);
          alpha_values_sub = VectorSubvector(alpha_values, sg);

          ix = SubgridIX(subgrid);
          iy = SubgridIY(subgrid);
          iz = SubgridIZ(subgrid);

          nx = SubgridNX(subgrid);
          ny = SubgridNY(subgrid);
          nz = SubgridNZ(subgrid);

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);
          pddat = SubvectorData(pd_sub);

          n_values_dat = SubvectorData(n_values_sub);
          alpha_values_dat = SubvectorData(alpha_values_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "Other1, CALCFCN\n");
            fflush(stderr);
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipd = SubvectorEltIndex(pd_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);

              int n_index = SubvectorEltIndex(n_values_sub, i, j, k);
              int alpha_index = SubvectorEltIndex(alpha_values_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 1.0;
              else
              {
                double alpha = alpha_values_dat[alpha_index];
                double n = n_values_dat[n_index];
                double m = 1.0e0 - (1.0e0 / n);

                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                prdat[ipr] = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
                             / pow(opahn, (m / 2));
              }
            });
          }
          else    /* fcn = CALCDER */
          {
            fprintf(stderr, "Other1, CALCDER\n");
            fflush(stderr);
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipd = SubvectorEltIndex(pd_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);

              int n_index = SubvectorEltIndex(n_values_sub, i, j, k);
              int alpha_index = SubvectorEltIndex(alpha_values_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 0.0;
              else
              {
                double alpha = alpha_values_dat[alpha_index];
                double n = n_values_dat[n_index];
                double m = 1.0e0 - (1.0e0 / n);

                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                double coeff = 1.0 - ahnm1 * pow(opahn, -m);

                prdat[ipr] = 2.0 * (coeff / (pow(opahn, (m / 2))))
                             * ((n - 1) * pow(alpha * head, n - 2) * alpha
                                * pow(opahn, -m)
                                - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                             + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                             * n * alpha * ahnm1;
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End if data_from_file */

      /* Compute rel. perms. on interior */
      if (data_from_file == 0)  /* alphas and ns given by region */
      {
        for (ir = 0; ir < num_regions; ir++)
        {
          gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

          ForSubgridI(sg, subgrids)
          {
            subgrid = SubgridArraySubgrid(subgrids, sg);

            pr_sub = VectorSubvector(phase_rel_perm, sg);
            pp_sub = VectorSubvector(phase_pressure, sg);
            pd_sub = VectorSubvector(phase_density, sg);

            ix = SubgridIX(subgrid) - 1;
            iy = SubgridIY(subgrid) - 1;
            iz = SubgridIZ(subgrid) - 1;

            nx = SubgridNX(subgrid) + 2;
            ny = SubgridNY(subgrid) + 2;
            nz = SubgridNZ(subgrid) + 2;

            r = SubgridRX(subgrid);

            prdat = SubvectorData(pr_sub);
            ppdat = SubvectorData(pp_sub);
            pddat = SubvectorData(pd_sub);

            if (fcn == CALCFCN)
            {
              if (dummy1->lookup_tables[ir])
              {
                switch (dummy1->lookup_tables[ir]->interpolation_method)
                {
                  // Spline
                  case 0:
                  {
                    fprintf(stderr, "GrGeomInLoop, CALCFCN Spline\n");
                    GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                    {
                      /* Table Lookup */
                      int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                      int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                      int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 1.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                        prdat[ipr] = VanGLookupSpline(head,
                                                      dummy1->lookup_tables[ir],
                                                      CALCFCN);
                      }
                    });
                  }
                  break;

                  // Linear
                  case 1:
                  {
                    VanGTable *lookup_table = dummy1->lookup_tables[ir];
                    double interval = lookup_table->interval;
                    double min_pressure_head = lookup_table->min_pressure_head;
                    int num_sample_points = lookup_table->num_sample_points;
                    int max = num_sample_points + 1;
		    PF_UNUSED(max);
                    fprintf(stderr, "GrGeomInLoop, CALCFCN Linear\n");
                    GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                    {
                      /* Table Lookup */
                      int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                      int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                      int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 1.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                        if (ppdat[ipp] >= 0.0)
                          prdat[ipr] = 1.0;
                        else
                        {
                          head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                          if (head < fabs(min_pressure_head))
                          {
                            int pt = (int)floor(head / interval);
                            assert(pt < max);

                            prdat[ipr] = lookup_table->a[pt] + lookup_table->slope[pt] *
                                         (head - lookup_table->x[pt]);
                          }
                          else
                          {
                            prdat[ipr] = 0.0;
                          }
                        }
                      }
                    });
                  }
                  break;
                }
              }
              else
              {
                /* Compute VanG curve */
                #if defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)

                double * pr_sub_chapel = pr_sub->data;
                double * pp_sub_chapel = pp_sub->data;
                double * pd_sub_chapel = pd_sub->data;
                double * alphas_chapel = alphas;
                double * ns_chapel = ns;
                calcfcn_compute_vang_curve_interior(gr_solid, r, ix, iy, iz, nx, ny, nz, pr_sub_chapel, pp_sub_chapel, pd_sub_chapel, alphas_chapel, ns_chapel, gravity, ir, SubvectorIX(pr_sub), SubvectorIY(pr_sub), SubvectorIZ(pr_sub), SubvectorNX(pr_sub), SubvectorNY(pr_sub));

                #endif
                #if !defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                {
                  int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                  int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                  int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                  if (ppdat[ipp] >= 0.0)
                    prdat[ipr] = 1.0;
                  else
                  {
                    double alpha = alphas[ir];
                    double n = ns[ir];
                    double m = 1.0e0 - (1.0e0 / n);

                    double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                    double opahn = 1.0 + pow(alpha * head, n);
                    double ahnm1 = pow(alpha * head, n - 1);
                    prdat[ipr] = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
                                 / pow(opahn, (m / 2));
                  }
                });
                #endif

              }
            }    /* End if clause */
            else /* fcn = CALCDER */
            {
              if (dummy1->lookup_tables[ir])
              {
                switch (dummy1->lookup_tables[ir]->interpolation_method)
                {
                  // Spline
                  case 0:
                  {
                    fprintf(stderr, "GrGeomInLoop, CALCDER spline\n");
                    GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                    {
                      /* Table Lookup */
                      int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                      int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                      int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 0.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                        prdat[ipr] = VanGLookupSpline(head,
                                                      dummy1->lookup_tables[ir],
                                                      CALCDER);
                      }
                    });
                  }
                  break;

                  case 1:
                  {
                    VanGTable *lookup_table = dummy1->lookup_tables[ir];
                    double interval = lookup_table->interval;
                    double min_pressure_head = lookup_table->min_pressure_head;
                    int num_sample_points = lookup_table->num_sample_points;
                    int max = num_sample_points + 1;
		    PF_UNUSED(max);
                    fprintf(stderr, "GrGeomInLoop, CALCDER case 1\n");
                    GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                    {
                      /* Table Lookup */
                      int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                      int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                      int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                      if (ppdat[ipp] >= 0.0)
                        prdat[ipr] = 0.0;
                      else
                      {
                        double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);

                        if (head < fabs(min_pressure_head))
                        {
                          int pt = (int)floor(head / interval);
                          assert(pt < max);

                          prdat[ipr] = lookup_table->a_der[pt] + lookup_table->slope_der[pt] *
                                       (head - lookup_table->x[pt]);
                        }
                        else
                        {
                          prdat[ipr] = 0.0;
                        }
                      }
                    });
                  }
                  break;
                }
              }
              else
              {
                /* Compute VanG curve */
                #if defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)

                double * pr_sub_chapel = pr_sub->data;
                double * pp_sub_chapel = pp_sub->data;
                double * pd_sub_chapel = pd_sub->data;
                double * alphas_chapel = alphas;
                double * ns_chapel = ns;
                calcder_compute_vang_curve_interior(gr_solid, r, ix, iy, iz, nx, ny, nz, pr_sub_chapel, pp_sub_chapel, pd_sub_chapel, alphas_chapel, ns_chapel, gravity, ir, SubvectorIX(pr_sub), SubvectorIY(pr_sub), SubvectorIZ(pr_sub), SubvectorNX(pr_sub), SubvectorNY(pr_sub));

                #endif
                #if !defined(PARFLOW_HAVE_CHAPEL) || defined(PARFLOW_CALL_ONLY)
                GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
                {
                  int ipr = SubvectorEltIndex(pr_sub, i, j, k);
                  int ipp = SubvectorEltIndex(pp_sub, i, j, k);
                  int ipd = SubvectorEltIndex(pd_sub, i, j, k);

                  if (ppdat[ipp] >= 0.0)
                    prdat[ipr] = 0.0;
                  else
                  {
                    double alpha = alphas[ir];
                    double n = ns[ir];
                    double m = 1.0e0 - (1.0e0 / n);

                    double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                    double opahn = 1.0 + pow(alpha * head, n);
                    double ahnm1 = pow(alpha * head, n - 1);
                    double coeff = 1.0 - ahnm1 * pow(opahn, -m);

                    prdat[ipr] = 2.0 * (coeff / (pow(opahn, (m / 2))))
                                 * ((n - 1) * pow(alpha * head, n - 2) * alpha
                                    * pow(opahn, -m)
                                    - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                                 + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                                 * n * alpha * ahnm1;
                  }
                });
                #endif
               
              }
            }   /* End else clause */
          }     /* End subgrid loop */
        }       /* End subregion loop */
      }         /* End if data not given by file */
      else if (data_from_file == 1) /* alphas and ns given in pfb files */
      {
        gr_solid = ProblemDataGrDomain(problem_data);
        n_values = dummy1->n_values;
        alpha_values = dummy1->alpha_values;

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);
          pd_sub = VectorSubvector(phase_density, sg);

          n_values_sub = VectorSubvector(n_values, sg);
          alpha_values_sub = VectorSubvector(alpha_values, sg);

          ix = SubgridIX(subgrid) - 1;
          iy = SubgridIY(subgrid) - 1;
          iz = SubgridIZ(subgrid) - 1;

          nx = SubgridNX(subgrid) + 2;
          ny = SubgridNY(subgrid) + 2;
          nz = SubgridNZ(subgrid) + 2;

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);
          pddat = SubvectorData(pd_sub);

          n_values_dat = SubvectorData(n_values_sub);
          alpha_values_dat = SubvectorData(alpha_values_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "GrGeomInLoop, CALCFCN 1336\n");
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);
              int ipd = SubvectorEltIndex(pd_sub, i, j, k);

              int n_index = SubvectorEltIndex(n_values_sub, i, j, k);
              int alpha_index = SubvectorEltIndex(alpha_values_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 1.0;
              else
              {
                double alpha = alpha_values_dat[alpha_index];
                double n = n_values_dat[n_index];
                double m = 1.0e0 - (1.0e0 / n);

                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                prdat[ipr] = pow(1.0 - ahnm1 / (pow(opahn, m)), 2)
                             / pow(opahn, (m / 2));
              }
            });
          }      /* End if clause */
          else   /* fcn = CALCDER */
          {
            fprintf(stderr, "GrGeomInLoop, CALCDER 1364\n");
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);
              int ipd = SubvectorEltIndex(pd_sub, i, j, k);

              int n_index = SubvectorEltIndex(n_values_sub, i, j, k);
              int alpha_index = SubvectorEltIndex(alpha_values_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 0.0;
              else
              {
                double alpha = alpha_values_dat[alpha_index];
                double n = n_values_dat[n_index];
                double m = 1.0e0 - (1.0e0 / n);

                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double opahn = 1.0 + pow(alpha * head, n);
                double ahnm1 = pow(alpha * head, n - 1);
                double coeff = 1.0 - ahnm1 * pow(opahn, -m);

                prdat[ipr] = 2.0 * (coeff / (pow(opahn, (m / 2))))
                             * ((n - 1) * pow(alpha * head, n - 2) * alpha
                                * pow(opahn, -m)
                                - ahnm1 * m * pow(opahn, -(m + 1)) * n * alpha * ahnm1)
                             + pow(coeff, 2) * (m / 2) * pow(opahn, (-(m + 2) / 2))
                             * n * alpha * ahnm1;
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End if data given by file */
      break;
    }        /* End case 1 */

    case 2:  /* Haverkamp et.al. relative permeability */
    {
      double  *As, *gammas;

      dummy2 = (Type2*)(public_xtra->data);

      num_regions = (dummy2->num_regions);
      region_indices = (dummy2->region_indices);
      As = (dummy2->As);
      gammas = (dummy2->gammas);

      /* Compute rel. perms. for Dirichlet BC's */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);
          pd_sub = VectorSubvector(phase_density, sg);

          ix = SubgridIX(subgrid);
          iy = SubgridIY(subgrid);
          iz = SubgridIZ(subgrid);

          nx = SubgridNX(subgrid);
          ny = SubgridNY(subgrid);
          nz = SubgridNZ(subgrid);

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);
          pddat = SubvectorData(pd_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "GrGeomSurfLoop, CALCFCN 1441\n");
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipd = SubvectorEltIndex(pd_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 1.0;
              else
              {
                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double tmp = As[ir] + pow(head, gammas[ir]);
                prdat[ipr] = As[ir] / tmp;
              }
            });
          }      /* End if clause */
          else   /* fcn = CALCDER */
          {
            fprintf(stderr, "GrGeomSurfLoop, CALCDER 1464\n");
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipd = SubvectorEltIndex(pd_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 0.0;
              else
              {
                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double tmp = pow(head, gammas[ir]);
                prdat[ipr] = As[ir] * gammas[ir]
                             * pow(head, gammas[ir] - 1) / pow(tmp, 2);
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End subregion loop */

      /* Compute rel. perms. on interior */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);
          pd_sub = VectorSubvector(phase_density, sg);

          ix = SubgridIX(subgrid) - 1;
          iy = SubgridIY(subgrid) - 1;
          iz = SubgridIZ(subgrid) - 1;

          nx = SubgridNX(subgrid) + 2;
          ny = SubgridNY(subgrid) + 2;
          nz = SubgridNZ(subgrid) + 2;

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);
          pddat = SubvectorData(pd_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "GrGeomInLoop, CALCFCN 1518\n");
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);
              int ipd = SubvectorEltIndex(pd_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 1.0;
              else
              {
                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double tmp = As[ir] + pow(head, gammas[ir]);
                prdat[ipr] = As[ir] / tmp;
              }
            });
          }      /* End if clause */
          else   /* fcn = CALCDER */
          {
            fprintf(stderr, "GrGeomInLoop, CALCDER 1537\n");
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);
              int ipd = SubvectorEltIndex(pd_sub, i, j, k);

              if (ppdat[ipp] >= 0.0)
                prdat[ipr] = 0.0;
              else
              {
                double head = fabs(ppdat[ipp]) / (pddat[ipd] * gravity);
                double tmp = pow(head, gammas[ir]);
                prdat[ipr] = As[ir] * gammas[ir]
                             * pow(head, gammas[ir] - 1) / pow(tmp, 2);
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End subregion loop */

      break;
    }        /* End case 2 */

    case 3:  /* Data relative permeability */
    {
      // dummy3 = (Type3*)(public_xtra->data);

      if (!amps_Rank(amps_CommWorld))
        printf("Data curves for rel perms not supported currently.\n");
      break;
    }        /* End case 3 */

    case 4:  /* Polynomial function of pressure relative permeability */
    {
      int     *degrees;
      double **coefficients, *region_coeffs;

      dummy4 = (Type4*)(public_xtra->data);

      num_regions = (dummy4->num_regions);
      region_indices = (dummy4->region_indices);
      degrees = (dummy4->degrees);
      coefficients = (dummy4->coefficients);

      /* Compute rel. perms. for Dirichlet BC's */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);
        region_coeffs = coefficients[ir];

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);

          ix = SubgridIX(subgrid);
          iy = SubgridIY(subgrid);
          iz = SubgridIZ(subgrid);

          nx = SubgridNX(subgrid);
          ny = SubgridNY(subgrid);
          nz = SubgridNZ(subgrid);

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "GrGeomSurfLoop, CALCFCN 1610\n");
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              if (ppdat[ipp] == 0.0)
                prdat[ipr] = region_coeffs[0];
              else
              {
                prdat[ipr] = 0.0;
                for (int dg = 0; dg < degrees[ir] + 1; dg++)
                {
                  prdat[ipr] += region_coeffs[dg] * pow(ppdat[ipp], dg);
                }
              }
            });
          }      /* End if clause */
          else   /* fcn = CALCDER */
          {
            fprintf(stderr, "GrGeomSurfLoop, CALCDER 1632\n");
            GrGeomSurfLoop(i, j, k, fdir, gr_solid, r, ix, iy, iz,
                           nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              int ipp = SubvectorEltIndex(pp_sub,
                                      i + fdir[0], j + fdir[1], k + fdir[2]);
              if (ppdat[ipp] == 0.0)
                prdat[ipr] = 0.0;
              else
              {
                prdat[ipr] = 0.0;
                for (int dg = 1; dg < degrees[ir] + 1; dg++)
                {
                  prdat[ipr] += region_coeffs[dg] * dg
                                * pow(ppdat[ipp], (dg - 1));
                }
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End subregion loop */

      /* Compute rel. perms. in interior */
      for (ir = 0; ir < num_regions; ir++)
      {
        gr_solid = ProblemDataGrSolid(problem_data, region_indices[ir]);
        region_coeffs = coefficients[ir];

        ForSubgridI(sg, subgrids)
        {
          subgrid = SubgridArraySubgrid(subgrids, sg);

          pr_sub = VectorSubvector(phase_rel_perm, sg);
          pp_sub = VectorSubvector(phase_pressure, sg);

          ix = SubgridIX(subgrid) - 1;
          iy = SubgridIY(subgrid) - 1;
          iz = SubgridIZ(subgrid) - 1;

          nx = SubgridNX(subgrid) + 2;
          ny = SubgridNY(subgrid) + 2;
          nz = SubgridNZ(subgrid) + 2;

          r = SubgridRX(subgrid);

          prdat = SubvectorData(pr_sub);
          ppdat = SubvectorData(pp_sub);

          if (fcn == CALCFCN)
          {
            fprintf(stderr, "GrGeomInLoop, CALCFCN 1684\n");

            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);

              if (ppdat[ipp] == 0.0)
                prdat[ipr] = region_coeffs[0];
              else
              {
                prdat[ipr] = 0.0;
                for (int dg = 0; dg < degrees[ir] + 1; dg++)
                {
                  prdat[ipr] += region_coeffs[dg] * pow(ppdat[ipp], dg);
                }
              }
            });
          }      /* End if clause */
          else   /* fcn = CALCDER */
          {
            fprintf(stderr, "GrGeomInLoop, CALCDER 1705\n");
            GrGeomInLoop(i, j, k, gr_solid, r, ix, iy, iz, nx, ny, nz,
            {
              int ipr = SubvectorEltIndex(pr_sub, i, j, k);
              int ipp = SubvectorEltIndex(pp_sub, i, j, k);

              if (ppdat[ipp] == 0.0)
                prdat[ipr] = 0.0;
              else
              {
                prdat[ipr] = 0.0;
                for (int dg = 1; dg < degrees[ir] + 1; dg++)
                {
                  prdat[ipr] += region_coeffs[dg] * dg
                                * pow(ppdat[ipp], (dg - 1));
                }
              }
            });
          }     /* End else clause */
        }       /* End subgrid loop */
      }         /* End subregion loop */

      break;
    }        /* End case 4 */
  }          /* End switch */

  /*-----------------------------------------------------------------------
   * End timing
   *-----------------------------------------------------------------------*/
  fprintf(stdout, "end PhaseRelPerm\n");
  IncFLOPCount(1);
  EndTiming(public_xtra->time_index);
}



/*--------------------------------------------------------------------------
 * PhaseRelPermInitInstanceXtra
 *--------------------------------------------------------------------------*/

PFModule  *PhaseRelPermInitInstanceXtra(
                                        Grid *  grid,
                                        double *temp_data)
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);
  InstanceXtra  *instance_xtra;

  Type1         *dummy1;
  VectorUpdateCommHandle   *handle;

  if (PFModuleInstanceXtra(this_module) == NULL)
    instance_xtra = ctalloc(InstanceXtra, 1);
  else
    instance_xtra = (InstanceXtra*)PFModuleInstanceXtra(this_module);

  if (grid != NULL)
  {
    /* set new data */
    (instance_xtra->grid) = grid;

    /* Use a spatially varying field */
    if (public_xtra->type == 1)
    {
      dummy1 = (Type1*)(public_xtra->data);
      if ((dummy1->data_from_file) == 1)
      {
        (dummy1->n_values) = NewVectorType(grid, 1, 1, vector_cell_centered);
        (dummy1->alpha_values) = NewVectorType(grid, 1, 1, vector_cell_centered);

        ReadPFBinary((dummy1->alpha_file),
                     (dummy1->alpha_values));
        handle = InitVectorUpdate(dummy1->alpha_values, VectorUpdateAll);
        FinalizeVectorUpdate(handle);     // This is needed to initalize ghost cells after reading the pfb

        ReadPFBinary((dummy1->n_file),
                     (dummy1->n_values));
        handle = InitVectorUpdate(dummy1->n_values, VectorUpdateAll);
        FinalizeVectorUpdate(handle);
      }
    }
  }

  if (temp_data != NULL)
  {
    (instance_xtra->temp_data) = temp_data;
  }

  PFModuleInstanceXtra(this_module) = instance_xtra;
  return this_module;
}


/*-------------------------------------------------------------------------
 * PhaseRelPermFreeInstanceXtra
 *-------------------------------------------------------------------------*/

void  PhaseRelPermFreeInstanceXtra()
{
  PFModule *this_module = ThisPFModule;
  PublicXtra *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);
  InstanceXtra *instance_xtra = (InstanceXtra*)PFModuleInstanceXtra(this_module);

  if (instance_xtra)
  {
    switch ((public_xtra->type))
    {
      case 1:
      {
        Type1            *dummy1;

        dummy1 = (Type1*)(public_xtra->data);

        if (dummy1->data_from_file == 1)
        {
          if (dummy1->alpha_values)
          {
            FreeVector(dummy1->alpha_values);
            dummy1->alpha_values = NULL;
            FreeVector(dummy1->n_values);
            dummy1->n_values = NULL;
          }
        }
      }
    }

    tfree(instance_xtra);
    PFModuleInstanceXtra(this_module) = NULL;
  }
}

/*--------------------------------------------------------------------------
 * PhaseRelPermNewPublicXtra
 *--------------------------------------------------------------------------*/

PFModule   *PhaseRelPermNewPublicXtra()
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra;

  Type0            *dummy0;
  Type1            *dummy1;
  Type2            *dummy2;
  Type3            *dummy3;
  Type4            *dummy4;

  int num_regions, ir, ic;

  char *switch_name;
  char *region;

  char key[IDB_MAX_KEY_LEN];

  NameArray type_na;

  /*----------------------------------------------------------
   * The name array to map names to switch values
   *----------------------------------------------------------*/
  type_na = NA_NewNameArray("Constant VanGenuchten Haverkamp Data Polynomial");

  public_xtra = ctalloc(PublicXtra, 1);

  switch_name = GetString("Phase.RelPerm.Type");
  public_xtra->type = NA_NameToIndex(type_na, switch_name);

  NA_FreeNameArray(type_na);

  switch_name = GetString("Phase.RelPerm.GeomNames");
  public_xtra->regions = NA_NewNameArray(switch_name);

  num_regions = NA_Sizeof(public_xtra->regions);

  switch ((public_xtra->type))
  {
    case 0:
    {
      dummy0 = ctalloc(Type0, 1);

      dummy0->num_regions = num_regions;

      (dummy0->region_indices) = ctalloc(int, num_regions);
      (dummy0->values) = ctalloc(double, num_regions);

      for (ir = 0; ir < num_regions; ir++)
      {
        region = NA_IndexToName(public_xtra->regions, ir);

        dummy0->region_indices[ir] =
          NA_NameToIndex(GlobalsGeomNames, region);

        sprintf(key, "Geom.%s.RelPerm.Value", region);
        dummy0->values[ir] = GetDouble(key);
      }

      (public_xtra->data) = (void*)dummy0;

      break;
    }

    case 1:
    {
      dummy1 = ctalloc(Type1, 1);

      sprintf(key, "Phase.RelPerm.VanGenuchten.File");
      dummy1->data_from_file = GetIntDefault(key, 0);

      if ((dummy1->data_from_file) == 0)
      {
        dummy1->num_regions = num_regions;

        (dummy1->region_indices) = ctalloc(int, num_regions);
        (dummy1->alphas) = ctalloc(double, num_regions);
        (dummy1->ns) = ctalloc(double, num_regions);
#if PF_PRINT_VG_TABLE
        (dummy1->print_table) = ctalloc(int, num_regions);
#endif

        (dummy1->lookup_tables) = ctalloc(VanGTable*, num_regions);

        for (ir = 0; ir < num_regions; ir++)
        {
#if PF_PRINT_VG_TABLE
          dummy1->print_table[ir] = 1;
#endif

          region = NA_IndexToName(public_xtra->regions, ir);

          dummy1->region_indices[ir] =
            NA_NameToIndex(GlobalsGeomNames, region);

          sprintf(key, "Geom.%s.RelPerm.Alpha", region);
          dummy1->alphas[ir] = GetDouble(key);

          sprintf(key, "Geom.%s.RelPerm.N", region);
          dummy1->ns[ir] = GetDouble(key);

          sprintf(key, "Geom.%s.RelPerm.NumSamplePoints", region);

          int num_sample_points = GetIntDefault(key, 0);

          if (num_sample_points)
          {
            sprintf(key, "Geom.%s.RelPerm.MinPressureHead", region);
            double min_pressure_head = GetDouble(key);

            type_na = NA_NewNameArray("Spline Linear");

            sprintf(key, "Geom.%s.RelPerm.InterpolationMethod", region);
            switch_name = GetStringDefault(key, "Spline");
            int interpolation_method = NA_NameToIndex(type_na, switch_name);

            if (interpolation_method < 0)
            {
              InputError("Error: invalid type <%s> for key <%s>\n",
                         switch_name, key);
            }

            NA_FreeNameArray(type_na);

            dummy1->lookup_tables[ir] = VanGComputeTable(
                                                         interpolation_method,
                                                         num_sample_points,
                                                         min_pressure_head,
                                                         dummy1->alphas[ir],
                                                         dummy1->ns[ir]);
          }
          else
          {
            dummy1->lookup_tables[ir] = NULL;
          }
        }

        dummy1->alpha_file = NULL;
        dummy1->n_file = NULL;
        dummy1->alpha_values = NULL;
        dummy1->n_values = NULL;
      }
      else
      {
        sprintf(key, "Geom.%s.RelPerm.Alpha.Filename", "domain");
        dummy1->alpha_file = GetString(key);
        sprintf(key, "Geom.%s.RelPerm.N.Filename", "domain");
        dummy1->n_file = GetString(key);

        dummy1->num_regions = 0;
        dummy1->region_indices = NULL;
        dummy1->alphas = NULL;
        dummy1->ns = NULL;
      }

      (public_xtra->data) = (void*)dummy1;

      break;
    }

    case 2:
    {
      dummy2 = ctalloc(Type2, 1);

      dummy2->num_regions = num_regions;

      (dummy2->region_indices) = ctalloc(int, num_regions);
      (dummy2->As) = ctalloc(double, num_regions);
      (dummy2->gammas) = ctalloc(double, num_regions);

      for (ir = 0; ir < num_regions; ir++)
      {
        region = NA_IndexToName(public_xtra->regions, ir);

        dummy2->region_indices[ir] =
          NA_NameToIndex(GlobalsGeomNames, region);

        sprintf(key, "Geom.%s.RelPerm.A", region);
        dummy2->As[ir] = GetDouble(key);

        sprintf(key, "Geom.%s.RelPerm.gamma", region);
        dummy2->gammas[ir] = GetDouble(key);
      }

      (public_xtra->data) = (void*)dummy2;

      break;
    }

    case 3:
    {
      dummy3 = ctalloc(Type3, 1);

      dummy3->num_regions = num_regions;

      (dummy3->region_indices) = ctalloc(int, num_regions);

      for (ir = 0; ir < num_regions; ir++)
      {
        region = NA_IndexToName(public_xtra->regions, ir);

        dummy3->region_indices[ir] =
          NA_NameToIndex(GlobalsGeomNames, region);
      }

      (public_xtra->data) = (void*)dummy3;

      break;
    }

    case 4:
    {
      int degree;

      dummy4 = ctalloc(Type4, 1);

      dummy4->num_regions = num_regions;

      (dummy4->region_indices) = ctalloc(int, num_regions);
      (dummy4->degrees) = ctalloc(int, num_regions);
      (dummy4->coefficients) = ctalloc(double*, num_regions);

      for (ir = 0; ir < num_regions; ir++)
      {
        region = NA_IndexToName(public_xtra->regions, ir);

        dummy4->region_indices[ir] =
          NA_NameToIndex(GlobalsGeomNames, region);

        sprintf(key, "Geom.%s.RelPerm.Degree", region);
        dummy4->degrees[ir] = GetInt(key);

        degree = (dummy4->degrees[ir]);
        dummy4->coefficients[ir] = ctalloc(double, degree + 1);

        for (ic = 0; ic < degree + 1; ic++)
        {
          sprintf(key, "Geom.%s.RelPerm.Coeff.%d", region, ic);
          dummy4->coefficients[ir][ic] = GetDouble(key);
        }
      }

      (public_xtra->data) = (void*)dummy4;

      break;
    }

    default:
    {
      InputError("Error: invalid type <%s> for key <%s>\n",
                 switch_name, key);
    }
  }      /* End switch */

  

  (public_xtra->time_index) = RegisterTiming("PhaseRelPerm");

  PFModulePublicXtra(this_module) = public_xtra;
  return this_module;
}

/*--------------------------------------------------------------------------
 * PhaseRelPermFreePublicXtra
 *--------------------------------------------------------------------------*/

void  PhaseRelPermFreePublicXtra()
{
  PFModule    *this_module = ThisPFModule;
  PublicXtra  *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Type0       *dummy0;
  Type1       *dummy1;
  Type2       *dummy2;
  Type3       *dummy3;
  Type4       *dummy4;

  int num_regions, ir;

  if (public_xtra)
  {
    NA_FreeNameArray(public_xtra->regions);

    switch ((public_xtra->type))
    {
      case 0:
      {
        dummy0 = (Type0*)(public_xtra->data);

        tfree(dummy0->region_indices);
        tfree(dummy0->values);
        tfree(dummy0);

        break;
      }

      case 1:
      {
        dummy1 = (Type1*)(public_xtra->data);

        tfree(dummy1->region_indices);
        tfree(dummy1->alphas);
        tfree(dummy1->ns);

        num_regions = (dummy1->num_regions);
        for (ir = 0; ir < num_regions; ir++)
        {
          if (dummy1->lookup_tables[ir])
          {
            tfree(dummy1->lookup_tables[ir]->x);
            tfree(dummy1->lookup_tables[ir]->a);
            tfree(dummy1->lookup_tables[ir]->d);
            tfree(dummy1->lookup_tables[ir]->a_der);
            tfree(dummy1->lookup_tables[ir]->d_der);

            tfree(dummy1->lookup_tables[ir]->slope);
            tfree(dummy1->lookup_tables[ir]->slope_der);

            tfree(dummy1->lookup_tables[ir]);
          }
        }

        tfree(dummy1->lookup_tables);

        tfree(dummy1);

        break;
      }

      case 2:
      {
        dummy2 = (Type2*)(public_xtra->data);

        tfree(dummy2->region_indices);
        tfree(dummy2->As);
        tfree(dummy2->gammas);
        tfree(dummy2);

        break;
      }

      case 3:
      {
        dummy3 = (Type3*)(public_xtra->data);

        tfree(dummy3->region_indices);
        tfree(dummy3);

        break;
      }

      case 4:
      {
        dummy4 = (Type4*)(public_xtra->data);

        num_regions = (dummy4->num_regions);
        for (ir = 0; ir < num_regions; ir++)
        {
          tfree(dummy4->coefficients[ir]);
        }

        tfree(dummy4->region_indices);
        tfree(dummy4->degrees);
        tfree(dummy4->coefficients);
        tfree(dummy4);

        break;
      }
    }
    tfree(public_xtra);
  }
}

/*--------------------------------------------------------------------------
 * PhaseRelPermSizeOfTempData
 *--------------------------------------------------------------------------*/

int  PhaseRelPermSizeOfTempData()
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Type1         *dummy1;

  int sz = 0;

  if (public_xtra->type == 1)
  {
    dummy1 = (Type1*)(public_xtra->data);
    if ((dummy1->data_from_file) == 1)
    {
      /* add local TempData size to `sz' */
      sz += SizeOfVector(dummy1->n_values);
      sz += SizeOfVector(dummy1->alpha_values);
    }
  }

  return sz;
}
