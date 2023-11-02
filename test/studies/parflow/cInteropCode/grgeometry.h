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
/*****************************************************************************
* Geometry class structures and accessors
*
*****************************************************************************/

#ifndef _GR_GEOMETRY_HEADER
#define _GR_GEOMETRY_HEADER

typedef struct GrGeomSolid_ GrGeomSolid;
/*--------------------------------------------------------------------------
 * Miscellaneous structures:
 *--------------------------------------------------------------------------*/

typedef int GrGeomExtents[6];

typedef struct {
  GrGeomExtents  *extents;
  int size;
} GrGeomExtentArray;
//#include "geometry.h"
#include "grgeom_octree.h"

#include "index_space.h"

/*--------------------------------------------------------------------------
 * Solid structures:
 *--------------------------------------------------------------------------*/

struct GrGeomSolid_ {
  GrGeomOctree  *data;

  GrGeomOctree **patches;
  int num_patches;

#if defined(PARFLOW_HAVE_CUDA) || defined(PARFLOW_HAVE_KOKKOS)
  /* Cell flags for 3 GrGeomLoops (0: do not evaluate cell, 1 = evaluate cell)
   *  Bit 0: GrGeomInLoop
   *  Bit 1: GrGeomOutLoop
   *  Bits 2-7: GrGeomSurfLoop (1 bit for each face)
   */

  /* A pointer to an array for each relevant cell. 
   * Bits 0-7 of each element determine if the cell is evaluated by the respective loop. 
   */ 
  char *cell_mask;

  /* The size of cell_mask array in bytes. */
  int mask_size;

  /* Bits 0-7 indicate if cell_mask has been filled (ie. initialized) for the respective loop */
  char loops_masked;

  /* ival indices for parallel exec of patchloops */ 
  int ***ival;
#endif

  /* these fields are used to relate the background with the octree */
  int octree_bg_level;
  int octree_ix, octree_iy, octree_iz;

  /* Boxes for iteration */

  BoxArray* interior_boxes;
  BoxArray* surface_boxes[GrGeomOctreeNumFaces];
  BoxArray** patch_boxes[GrGeomOctreeNumFaces];
};

#include "grgeom_list.h"
/*--------------------------------------------------------------------------
 * Accessor macros:
 *--------------------------------------------------------------------------*/

#define GrGeomExtentsIXLower(extents)  ((extents)[0])
#define GrGeomExtentsIXUpper(extents)  ((extents)[1])
#define GrGeomExtentsIYLower(extents)  ((extents)[2])
#define GrGeomExtentsIYUpper(extents)  ((extents)[3])
#define GrGeomExtentsIZLower(extents)  ((extents)[4])
#define GrGeomExtentsIZUpper(extents)  ((extents)[5])

#define GrGeomExtentArrayExtents(ext_array)  ((ext_array)->extents)
#define GrGeomExtentArraySize(ext_array)     ((ext_array)->size)

#define GrGeomSolidCellFlagData(solid)        ((solid)->cell_mask)
#define GrGeomSolidCellFlagDataSize(solid)    ((solid)->mask_size)
#define GrGeomSolidCellFlagInitialized(solid) ((solid)->loops_masked)
#define GrGeomSolidCellIval(solid, patch, i)  ((solid)->ival[i][patch])

#define GrGeomSolidData(solid)          ((solid)->data)
#define GrGeomSolidPatches(solid)       ((solid)->patches)
#define GrGeomSolidNumPatches(solid)    ((solid)->num_patches)
#define GrGeomSolidOctreeBGLevel(solid) ((solid)->octree_bg_level)
#define GrGeomSolidOctreeIX(solid)      ((solid)->octree_ix)
#define GrGeomSolidOctreeIY(solid)      ((solid)->octree_iy)
#define GrGeomSolidOctreeIZ(solid)      ((solid)->octree_iz)
#define GrGeomSolidPatch(solid, i)      ((solid)->patches[(i)])
#define GrGeomSolidInteriorBoxes(solid) ((solid)->interior_boxes)
#define GrGeomSolidSurfaceBoxes(solid, i)  ((solid)->surface_boxes[(i)])
#define GrGeomSolidPatchBoxes(solid, patch, i)  ((solid)->patch_boxes[(i)][(patch)])

/*==========================================================================
 *==========================================================================*/

/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the inside of a solid.
 *--------------------------------------------------------------------------*/

#define GrGeomInLoopBoxes_default(i, j, k, grgeom, ix, iy, iz, nx, ny, nz, body) \
  {                                                                      \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                  \
    int *PV_visiting = NULL;                                             \
    PF_UNUSED(PV_visiting);                                              \
    BoxArray* boxes = GrGeomSolidInteriorBoxes(grgeom);                  \
    for (int PV_box = 0; PV_box < BoxArraySize(boxes); PV_box++)         \
    {                                                                    \
      Box box = BoxArrayGetBox(boxes, PV_box);                           \
      /* find octree and region intersection */                          \
      PV_ixl = pfmax(ix, box.lo[0]);                                     \
      PV_iyl = pfmax(iy, box.lo[1]);                                     \
      PV_izl = pfmax(iz, box.lo[2]);                                     \
      PV_ixu = pfmin((ix + nx - 1), box.up[0]);                          \
      PV_iyu = pfmin((iy + ny - 1), box.up[1]);                          \
      PV_izu = pfmin((iz + nz - 1), box.up[2]);                          \
                                                                         \
      for (k = PV_izl; k <= PV_izu; k++)                                 \
        for (j = PV_iyl; j <= PV_iyu; j++)                               \
          for (i = PV_ixl; i <= PV_ixu; i++)                             \
          {                                                              \
            body;                                                        \
          }                                                              \
    }                                                                    \
  }

#define GrGeomInLoop(i, j, k, grgeom,                                    \
                     r, ix, iy, iz, nx, ny, nz, body)                    \
  {                                                                      \
    if (r == 0 && GrGeomSolidInteriorBoxes(grgeom))                      \
    {                                                                    \
      GrGeomInLoopBoxes(i, j, k, grgeom,                                 \
                        ix, iy, iz, nx, ny, nz, body);                   \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      GrGeomOctree  *PV_node;                                            \
      double PV_ref = pow(2.0, r);                                       \
                                                                         \
      i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                     \
      j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                     \
      k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                     \
      GrGeomOctreeInteriorNodeLoop(i, j, k, PV_node,                     \
                                   GrGeomSolidData(grgeom),              \
                                   GrGeomSolidOctreeBGLevel(grgeom) + r, \
                                   ix, iy, iz, nx, ny, nz,               \
                                   TRUE,                                 \
                                   body);                                \
    }                                                                    \
  }

/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the inside of a solid with non-unitary strides.
 *--------------------------------------------------------------------------*/

/**
 *  Interior version of this would improve speed; but this loop is not
 * currently used.
 */
#define GrGeomInLoop2(i, j, k, grgeom,                             \
                      r, ix, iy, iz, nx, ny, nz, sx, sy, sz, body) \
  {                                                                \
    GrGeomOctree  *PV_node;                                        \
    double PV_ref = pow(2.0, r);                                   \
                                                                   \
                                                                   \
    i = GrGeomSolidOctreeIX(grgeom) * PV_ref;                      \
    j = GrGeomSolidOctreeIY(grgeom) * PV_ref;                      \
    k = GrGeomSolidOctreeIZ(grgeom) * PV_ref;                      \
    GrGeomOctreeNodeLoop2(i, j, k, PV_node,                        \
                          GrGeomSolidData(grgeom),                 \
                          GrGeomSolidOctreeBGLevel(grgeom) + r,    \
                          ix, iy, iz, nx, ny, nz, sx, sy, sz,      \
                          (GrGeomOctreeNodeIsInside(PV_node) ||    \
                           GrGeomOctreeNodeIsFull(PV_node)),       \
                          body);                                   \
  }

/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the outside of a solid.
 *--------------------------------------------------------------------------*/

#define GrGeomOutLoop_default(i, j, k, grgeom,                                 \
                      r, ix, iy, iz, nx, ny, nz, body)                 \
  {                                                                    \
    GrGeomOctree  *PV_node;                                            \
    double PV_ref = pow(2.0, r);                                       \
                                                                       \
                                                                       \
    i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                     \
    j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                     \
    k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                     \
    GrGeomOctreeExteriorNodeLoop(i, j, k, PV_node,                     \
                                 GrGeomSolidData(grgeom),              \
                                 GrGeomSolidOctreeBGLevel(grgeom) + r, \
                                 ix, iy, iz, nx, ny, nz,               \
                                 TRUE,                                 \
                                 body);                                \
  }

/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the outside of a solid with non-unitary strides.
 *--------------------------------------------------------------------------*/

#define GrGeomOutLoop2(i, j, k, grgeom,                             \
                       r, ix, iy, iz, nx, ny, nz, sx, sy, sz, body) \
  {                                                                 \
    GrGeomOctree  *PV_node;                                         \
    double PV_ref = pow(2.0, r);                                    \
                                                                    \
                                                                    \
    i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                  \
    j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                  \
    k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                  \
    GrGeomOctreeNodeLoop2(i, j, k, PV_node,                         \
                          GrGeomSolidData(grgeom),                  \
                          GrGeomSolidOctreeBGLevel(grgeom) + r,     \
                          ix, iy, iz, nx, ny, nz, sx, sy, sz,       \
                          (GrGeomOctreeNodeIsOutside(PV_node) ||    \
                           GrGeomOctreeNodeIsEmpty(PV_node)),       \
                          body);                                    \
  }

/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the faces of a solid surface.
 *--------------------------------------------------------------------------*/
#if 0
#define GrGeomSurfLoop(i, j, k, fdir, grgeom,                  \
                       r, ix, iy, iz, nx, ny, nz, body)        \
  {                                                            \
    GrGeomOctree  *PV_node;                                    \
    double PV_ref = pow(2.0, r);                               \
                                                               \
    i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;             \
    j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;             \
    k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;             \
    GrGeomOctreeFaceLoop(i, j, k, fdir, PV_node,               \
                         GrGeomSolidData(grgeom),              \
                         GrGeomSolidOctreeBGLevel(grgeom) + r, \
                         ix, iy, iz, nx, ny, nz, body);        \
  }

#else

//  \todo SGS 12/3/2008 can optimize fdir by using 1 assignment to static.  Should
// elimiate 2 assignment statements and switch and replace with table:
// fdir = FDIR[PV_f] type of thing.
//

#define GrGeomSurfLoopBoxes_default(i, j, k, fdir, grgeom, ix, iy, iz, nx, ny, nz, body) \
  {                                                                              \
    int PV_fdir[3];                                                              \
                                                                                 \
    fdir = PV_fdir;                                                              \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                          \
    int *PV_visiting = NULL;                                                     \
    PF_UNUSED(PV_visiting);                                                      \
    for (int PV_f = 0; PV_f < GrGeomOctreeNumFaces; PV_f++)                      \
    {                                                                            \
      switch (PV_f)                                                              \
      {                                                                          \
        case GrGeomOctreeFaceL:                                                  \
          fdir[0] = -1; fdir[1] = 0; fdir[2] = 0;                                \
          break;                                                                 \
        case GrGeomOctreeFaceR:                                                  \
          fdir[0] = 1; fdir[1] = 0; fdir[2] = 0;                                 \
          break;                                                                 \
        case GrGeomOctreeFaceD:                                                  \
          fdir[0] = 0; fdir[1] = -1; fdir[2] = 0;                                \
          break;                                                                 \
        case GrGeomOctreeFaceU:                                                  \
          fdir[0] = 0; fdir[1] = 1; fdir[2] = 0;                                 \
          break;                                                                 \
        case GrGeomOctreeFaceB:                                                  \
          fdir[0] = 0; fdir[1] = 0; fdir[2] = -1;                                \
          break;                                                                 \
        case GrGeomOctreeFaceF:                                                  \
          fdir[0] = 0; fdir[1] = 0; fdir[2] = 1;                                 \
          break;                                                                 \
        default:                                                                 \
          fdir[0] = -9999; fdir[1] = -9999; fdir[2] = -99999;                    \
          break;                                                                 \
      }                                                                          \
                                                                                 \
      BoxArray* boxes = GrGeomSolidSurfaceBoxes(grgeom, PV_f);                   \
      for (int PV_box = 0; PV_box < BoxArraySize(boxes); PV_box++)               \
      {                                                                          \
        Box box = BoxArrayGetBox(boxes, PV_box);                                 \
        /*printf("box: (%d, %d, %d) (%d, %d, %d)\n", box.lo[0], box.lo[1], box.lo[2], box.up[0], box.up[1], box.up[2]); */ \
        /* find octree and region intersection */                                \
        PV_ixl = pfmax(ix, box.lo[0]);                                           \
        PV_iyl = pfmax(iy, box.lo[1]);                                           \
        PV_izl = pfmax(iz, box.lo[2]);                                           \
        PV_ixu = pfmin((ix + nx - 1), box.up[0]);                                \
        PV_iyu = pfmin((iy + ny - 1), box.up[1]);                                \
        PV_izu = pfmin((iz + nz - 1), box.up[2]);                                \
                for (k = PV_izl; k <= PV_izu; k++)                                       \
          for (j = PV_iyl; j <= PV_iyu; j++)                                     \
            for (i = PV_ixl; i <= PV_ixu; i++)                                   \
            {                                                                    \
              body;                                                              \
            }                                                                    \
      }                                                                          \
    }                                                                            \
  }


#define GrGeomSurfLoop(i, j, k, fdir, grgeom,                                \
                       r, ix, iy, iz, nx, ny, nz, body)                      \
  {                                                                          \
    if (r == 0 && GrGeomSolidSurfaceBoxes(grgeom, GrGeomOctreeNumFaces - 1)) \
    {                                                                        \
      GrGeomSurfLoopBoxes(i, j, k, fdir, grgeom,                             \
                          ix, iy, iz, nx, ny, nz, body);                     \
    }                                                                        \
    else                                                                     \
    {                                                                        \
      GrGeomOctree  *PV_node;                                                \
      double PV_ref = pow(2.0, r);                                           \
      i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                         \
      j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                         \
      k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                         \
      GrGeomOctreeFaceLoop(i, j, k, fdir, PV_node,                           \
                           GrGeomSolidData(grgeom),                          \
                           GrGeomSolidOctreeBGLevel(grgeom) + r,             \
                           ix, iy, iz, nx, ny, nz, body);                    \
    }                                                                        \
  }

#endif



/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 *   Macro for looping over the faces of a solid patch.
 *--------------------------------------------------------------------------*/

#if 1

#define GrGeomPatchLoopBoxes_default(i, j, k, fdir, grgeom, patch_num, ix, iy, iz, nx, ny, nz, body) \
  {                                                                                          \
    int PV_fdir[3];                                                                          \
                                                                                             \
    fdir = PV_fdir;                                                                          \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                                      \
    int *PV_visiting = NULL;                                                                 \
    PF_UNUSED(PV_visiting);						                     \
    for (int PV_f = 0; PV_f < GrGeomOctreeNumFaces; PV_f++)                                  \
    {                                                                                        \
      switch (PV_f)                                                                          \
      {                                                                                      \
        case GrGeomOctreeFaceL:                                                              \
          fdir[0] = -1; fdir[1] = 0; fdir[2] = 0;                                            \
          break;                                                                             \
        case GrGeomOctreeFaceR:                                                              \
          fdir[0] = 1; fdir[1] = 0; fdir[2] = 0;                                             \
          break;                                                                             \
        case GrGeomOctreeFaceD:                                                              \
          fdir[0] = 0; fdir[1] = -1; fdir[2] = 0;                                            \
          break;                                                                             \
        case GrGeomOctreeFaceU:                                                              \
          fdir[0] = 0; fdir[1] = 1; fdir[2] = 0;                                             \
          break;                                                                             \
        case GrGeomOctreeFaceB:                                                              \
          fdir[0] = 0; fdir[1] = 0; fdir[2] = -1;                                            \
          break;                                                                             \
        case GrGeomOctreeFaceF:                                                              \
          fdir[0] = 0; fdir[1] = 0; fdir[2] = 1;                                             \
          break;                                                                             \
        default:                                                                             \
          fdir[0] = -9999; fdir[1] = -9999; fdir[2] = -99999;                                \
          break;                                                                             \
      }                                                                                      \
                                                                                             \
      BoxArray* boxes = GrGeomSolidPatchBoxes(grgeom, patch_num, PV_f);                      \
      for (int PV_box = 0; PV_box < BoxArraySize(boxes); PV_box++)                           \
      {                                                                                      \
        Box box = BoxArrayGetBox(boxes, PV_box);                                             \
        /* find octree and region intersection */                                            \
        PV_ixl = pfmax(ix, box.lo[0]);                                                       \
        PV_iyl = pfmax(iy, box.lo[1]);                                                       \
        PV_izl = pfmax(iz, box.lo[2]);                                                       \
        PV_ixu = pfmin((ix + nx - 1), box.up[0]);                                            \
        PV_iyu = pfmin((iy + ny - 1), box.up[1]);                                            \
        PV_izu = pfmin((iz + nz - 1), box.up[2]);                                            \
                                                                                             \
        for (k = PV_izl; k <= PV_izu; k++)                                                   \
          for (j = PV_iyl; j <= PV_iyu; j++)                                                 \
            for (i = PV_ixl; i <= PV_ixu; i++)                                               \
            {                                                                                \
              body;                                                                          \
            }                                                                                \
      }                                                                                      \
    }                                                                                        \
  }

#define GrGeomPatchLoopBoxesNoFdir_default(i, j, k, grgeom, patch_num, ovrlnd,				\
                                   ix, iy, iz, nx, ny, nz,              \
                                   locals, setup,                       \
                                   f_left, f_right,                     \
                                   f_down, f_up,                        \
                                   f_back, f_front,                     \
                                   finalize)                            \
  {                                                                     \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                 \
    int *PV_visiting = NULL;                                            \
    PF_UNUSED(PV_visiting);                                             \
    UNPACK(locals);                                                     \
                                                                        \
    for (int PV_f = 0; PV_f < GrGeomOctreeNumFaces; PV_f++)             \
    {                                                                   \
      BoxArray* boxes = GrGeomSolidPatchBoxes(grgeom, patch_num, PV_f); \
      for (int PV_box = 0; PV_box < BoxArraySize(boxes); PV_box++)      \
      {                                                                 \
        Box box = BoxArrayGetBox(boxes, PV_box);                        \
        /* find octree and region intersection */                       \
        PV_ixl = pfmax(ix, box.lo[0]);                                  \
        PV_iyl = pfmax(iy, box.lo[1]);                                  \
        PV_izl = pfmax(iz, box.lo[2]);                                  \
        PV_ixu = pfmin((ix + nx - 1), box.up[0]);                       \
        PV_iyu = pfmin((iy + ny - 1), box.up[1]);                       \
        PV_izu = pfmin((iz + nz - 1), box.up[2]);                       \
                                                                        \
        for (k = PV_izl; k <= PV_izu; k++)                              \
          for (j = PV_iyl; j <= PV_iyu; j++)                            \
            for (i = PV_ixl; i <= PV_ixu; i++)                          \
            {                                                           \
              setup;                                                    \
              switch(PV_f)                                              \
              {                                                         \
                f_left;                                                 \
                f_right;                                                \
                f_down;                                                 \
                f_up;                                                   \
                f_back;                                                 \
                f_front;                                                \
              }                                                         \
              finalize;                                                 \
            }                                                           \
      }                                                                 \
    }                                                                   \
  }

#define GrGeomPatchLoop(i, j, k, fdir, grgeom, patch_num,                             \
                        r, ix, iy, iz, nx, ny, nz, body)                              \
  {                                                                                   \
    if (r == 0 && GrGeomSolidPatchBoxes(grgeom, patch_num, GrGeomOctreeNumFaces - 1)) \
    {                                                                                 \
      GrGeomPatchLoopBoxes(i, j, k, fdir, grgeom, patch_num,                          \
                           ix, iy, iz, nx, ny, nz, body);                             \
    }                                                                                 \
    else                                                                              \
    {                                                                                 \
      GrGeomOctree  *PV_node;                                                         \
      double PV_ref = pow(2.0, r);                                                    \
                                                                                      \
                                                                                      \
      i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                                  \
      j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                                  \
      k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                                  \
      GrGeomOctreeFaceLoop(i, j, k, fdir, PV_node,                                    \
                           GrGeomSolidPatch(grgeom, patch_num),                       \
                           GrGeomSolidOctreeBGLevel(grgeom) + r,                      \
                           ix, iy, iz, nx, ny, nz, body);                             \
    }                                                                                 \
  }

#define GrGeomPatchLoopNoFdir(i, j, k, grgeom, patch_num, ovrlnd,       \
                              r, ix, iy, iz, nx, ny, nz,                \
                              locals, setup,                            \
                              f_left, f_right,                          \
                              f_down, f_up,                             \
                              f_back, f_front,                          \
                              finalize)                                 \
  {                                                                     \
    if (r == 0 && GrGeomSolidPatchBoxes(grgeom, patch_num, GrGeomOctreeNumFaces - 1)) \
    {                                                                   \
      GrGeomPatchLoopBoxesNoFdir(i, j, k, grgeom, patch_num, ovrlnd,    \
                                 ix, iy, iz, nx, ny, nz,                \
                                 locals, setup,                         \
                                 f_left, f_right,                       \
                                 f_down, f_up,                          \
                                 f_back, f_front,                       \
                                 finalize)                              \
    }                                                                   \
    else                                                                \
    {                                                                   \
      GrGeomOctree  *PV_node;                                           \
      double PV_ref = pow(2.0, r);                                      \
                                                                        \
                                                                        \
      i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;                    \
      j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;                    \
      k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;                    \
      GrGeomOctreeFaceLoopNoFdir(i, j, k, PV_node,                      \
                                 GrGeomSolidPatch(grgeom, patch_num),   \
                                 GrGeomSolidOctreeBGLevel(grgeom) + r,  \
                                 ix, iy, iz, nx, ny, nz,                \
                                 locals, setup,                         \
                                 f_left, f_right,                       \
                                 f_down, f_up,                          \
                                 f_back, f_front,                       \
                                 finalize)                              \
    }                                                                   \
  }

#else

#define GrGeomPatchLoop(i, j, k, fdir, grgeom, patch_num,      \
                        r, ix, iy, iz, nx, ny, nz, body)       \
  {                                                            \
    GrGeomOctree  *PV_node;                                    \
    double PV_ref = pow(2.0, r);                               \
                                                               \
                                                               \
    i = GrGeomSolidOctreeIX(grgeom) * (int)PV_ref;             \
    j = GrGeomSolidOctreeIY(grgeom) * (int)PV_ref;             \
    k = GrGeomSolidOctreeIZ(grgeom) * (int)PV_ref;             \
    GrGeomOctreeFaceLoop(i, j, k, fdir, PV_node,               \
                         GrGeomSolidPatch(grgeom, patch_num),  \
                         GrGeomSolidOctreeBGLevel(grgeom) + r, \
                         ix, iy, iz, nx, ny, nz, body);        \
  }

#endif


/*--------------------------------------------------------------------------
 * GrGeomSolid looping macro:
 * Macro for looping over the inside of a solid as a set of boxes.
 * This will pick both active and inactive cells along the boundary but
 * will avoid regions that are totally inactive.
 *
 * This may be used in place of the GrGeomInLoop macro if computing in
 * the inactive region is OK but not desired.  The advantage over the
 * GrGeomInLoop is this may be more computationally efficient as it is
 * looping over box (patch/block) of data rather than potentially
 * looping over individual elements.
 *
 * int i,j,k                     the starting index values for each box.
 * int num_i, num_j, num_k       the number of points in each box.
 * grgeom                        GrGeomSolid to loop over.
 * box_size_power                Smallest size of box to loop over as a power of 2.
 *                               The boxes will be 2^box_size_power cubed.
 *                               Boxes may be larger than this for interior regions.
 *                               Boxes may be smaller than this near boundaries.
 *                               Power of 2 restriction is imposed by the octree
 *                               representation.
 *--------------------------------------------------------------------------*/

// Note that IsInside is here to make sure everything is
// included.   If IsInside is actually true then that means single
// cells are being looped over which would be really bad for
// performance reasons.

#define GrGeomInBoxLoop(                                          \
                        i, j, k,                                  \
                        num_i, num_j, num_k,                      \
                        grgeom, box_size_power,                   \
                        ix, iy, iz, nx, ny, nz,                   \
                        body)                                     \
  {                                                               \
    GrGeomOctree  *PV_node;                                       \
    int PV_level_of_interest;                                     \
    PV_level_of_interest = GrGeomSolidOctreeBGLevel(grgeom) -     \
                           box_size_power - 1;                    \
    PV_level_of_interest = pfmax(0, PV_level_of_interest);        \
                                                                  \
    i = GrGeomSolidOctreeIX(grgeom);                              \
    j = GrGeomSolidOctreeIY(grgeom);                              \
    k = GrGeomSolidOctreeIZ(grgeom);                              \
                                                                  \
    GrGeomOctreeNodeBoxLoop(i, j, k,                              \
                            num_i, num_j, num_k,                  \
                            PV_node,                              \
                            GrGeomSolidData(grgeom),              \
                            GrGeomSolidOctreeBGLevel(grgeom),     \
                            PV_level_of_interest,                 \
                            ix, iy, iz, nx, ny, nz,               \
                            (GrGeomOctreeHasChildren(PV_node) ||  \
                             GrGeomOctreeNodeIsInside(PV_node) || \
                             GrGeomOctreeNodeIsFull(PV_node)),    \
    {                                                             \
      body;                                                       \
    });                                                           \
  }



#endif
