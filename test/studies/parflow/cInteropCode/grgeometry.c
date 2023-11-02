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
*
* Member functions for the GrGeometry (Grid dependent Geometry) class.
*
*****************************************************************************/

#include "parflow.h"
#include "grgeometry.h"
#include "clustering.h"

/*--------------------------------------------------------------------------
 * GrGeomGetOctreeInfo:
 *   This routine returns the spatial information needed to relate
 *   an octree with the background.
 *--------------------------------------------------------------------------*/

int      GrGeomGetOctreeInfo(
                             double *xlp,
                             double *ylp,
                             double *zlp,
                             double *xup,
                             double *yup,
                             double *zup,
                             int *   ixp,
                             int *   iyp,
                             int *   izp)
{
  Background  *bg = GlobalsBackground;
  double dtmp;
  int background_level, n;


  *xlp = BackgroundXLower(bg);
  *ylp = BackgroundYLower(bg);
  *zlp = BackgroundZLower(bg);

  dtmp = ceil(log(BackgroundNX(bg)) / log(2));
  background_level = (int)dtmp;
  dtmp = ceil(log(BackgroundNY(bg)) / log(2));
  background_level = pfmax(background_level, (int)dtmp);
  dtmp = ceil(log(BackgroundNZ(bg)) / log(2));
  background_level = pfmax(background_level, (int)dtmp);

  n = (int)pow(2.0, background_level);

  *xup = BackgroundXLower(bg) + n * BackgroundDX(bg);
  *yup = BackgroundYLower(bg) + n * BackgroundDY(bg);
  *zup = BackgroundZLower(bg) + n * BackgroundDZ(bg);

  *ixp = BackgroundIX(bg);
  *iyp = BackgroundIY(bg);
  *izp = BackgroundIZ(bg);

  return background_level;
}


/*--------------------------------------------------------------------------
 * GrGeomNewExtentArray
 *--------------------------------------------------------------------------*/

GrGeomExtentArray  *GrGeomNewExtentArray(
                                         GrGeomExtents *extents,
                                         int            size)
{
  GrGeomExtentArray   *new_grgeom_extent_array;


  new_grgeom_extent_array = talloc(GrGeomExtentArray, 1);

  (new_grgeom_extent_array->extents) = extents;
  (new_grgeom_extent_array->size) = size;

  return new_grgeom_extent_array;
}


/*--------------------------------------------------------------------------
 * GrGeomFreeExtentArray
 *--------------------------------------------------------------------------*/

void                GrGeomFreeExtentArray(
                                          GrGeomExtentArray *extent_array)
{
  tfree(GrGeomExtentArrayExtents(extent_array));

  tfree(extent_array);
}


/*--------------------------------------------------------------------------
 * GrGeomCreateExtentArray:
 *   The arguments [xyz][lu]_ghost indicate the number of ghost layers
 *   in each of the directions.  A negative value is an indication to
 *   extend the layers out to the edge of the grid background.
 *
 *   Important note: the indices returned in the extent_array are in
 *   *octree* coordinates.  The lower corner background index is index
 *   (0, 0, 0) in the octree.
 *
 *   Another important note: the routine GrGeomOctreeFromTIN requires
 *   at least one ghost layer of geometry info in order to construct
 *   the octree correctly.  This routine insures this.
 *--------------------------------------------------------------------------*/

GrGeomExtentArray  *GrGeomCreateExtentArray(
                                            SubgridArray *subgrids,
                                            int           xl_ghost,
                                            int           xu_ghost,
                                            int           yl_ghost,
                                            int           yu_ghost,
                                            int           zl_ghost,
                                            int           zu_ghost)
{
  Background         *bg = GlobalsBackground;

  GrGeomExtentArray  *extent_array;
  GrGeomExtents      *extents;
  int size;

  Subgrid            *subgrid;

  int ref;
  int bg_ix, bg_iy, bg_iz;
  int bg_nx, bg_ny, bg_nz;
  int is;


  size = SubgridArraySize(subgrids);
  extents = ctalloc(GrGeomExtents, size);

  ForSubgridI(is, subgrids)
  {
    subgrid = SubgridArraySubgrid(subgrids, is);

    /* compute background grid extents on MaxRefLevel index space */
    ref = (int)pow(2.0, GlobalsMaxRefLevel);
    bg_ix = BackgroundIX(bg) * ref;
    bg_iy = BackgroundIY(bg) * ref;
    bg_iz = BackgroundIZ(bg) * ref;
    bg_nx = BackgroundNX(bg) * ref;
    bg_ny = BackgroundNY(bg) * ref;
    bg_nz = BackgroundNZ(bg) * ref;

    ref = (int)Pow2(GlobalsMaxRefLevel);

    /*------------------------------------------
     * set the lower extent values
     *------------------------------------------*/

    if (xl_ghost > -1)
    {
      xl_ghost = pfmax(xl_ghost, 1);
      GrGeomExtentsIXLower(extents[is]) =
        (SubgridIX(subgrid) - xl_ghost) * ref;
    }
    else
    {
      GrGeomExtentsIXLower(extents[is]) = bg_ix;
    }

    if (yl_ghost > -1)
    {
      yl_ghost = pfmax(yl_ghost, 1);
      GrGeomExtentsIYLower(extents[is]) =
        (SubgridIY(subgrid) - yl_ghost) * ref;
    }
    else
    {
      GrGeomExtentsIYLower(extents[is]) = bg_iy;
    }

    if (zl_ghost > -1)
    {
      zl_ghost = pfmax(zl_ghost, 1);
      GrGeomExtentsIZLower(extents[is]) =
        (SubgridIZ(subgrid) - zl_ghost) * ref;
    }
    else
    {
      GrGeomExtentsIZLower(extents[is]) = bg_iz;
    }

    /*------------------------------------------
     * set the upper extent values
     *------------------------------------------*/

    if (xu_ghost > -1)
    {
      xu_ghost = pfmax(xu_ghost, 1);
      GrGeomExtentsIXUpper(extents[is]) =
        (SubgridIX(subgrid) + SubgridNX(subgrid) + xu_ghost) * ref - 1;
    }
    else
    {
      GrGeomExtentsIXUpper(extents[is]) = bg_ix + bg_nx - 1;
    }

    if (yu_ghost > -1)
    {
      yu_ghost = pfmax(yu_ghost, 1);
      GrGeomExtentsIYUpper(extents[is]) =
        (SubgridIY(subgrid) + SubgridNY(subgrid) + yu_ghost) * ref - 1;
    }
    else
    {
      GrGeomExtentsIYUpper(extents[is]) = bg_iy + bg_ny - 1;
    }

    if (zu_ghost > -1)
    {
      zu_ghost = pfmax(zu_ghost, 1);
      GrGeomExtentsIZUpper(extents[is]) =
        (SubgridIZ(subgrid) + SubgridNZ(subgrid) + zu_ghost) * ref - 1;
    }
    else
    {
      GrGeomExtentsIZUpper(extents[is]) = bg_iz + bg_nz - 1;
    }

    /*------------------------------------------
     * convert to "octree coordinates"
     *------------------------------------------*/

    /* Moved into the loop by SGS 7/8/98, was lying outside the is
     * loop which was an error (accessing invalid array elements)
     */

    GrGeomExtentsIXLower(extents[is]) -= bg_ix;
    GrGeomExtentsIYLower(extents[is]) -= bg_iy;
    GrGeomExtentsIZLower(extents[is]) -= bg_iz;
    GrGeomExtentsIXUpper(extents[is]) -= bg_ix;
    GrGeomExtentsIYUpper(extents[is]) -= bg_iy;
    GrGeomExtentsIZUpper(extents[is]) -= bg_iz;
  }


  extent_array = GrGeomNewExtentArray(extents, size);

  return extent_array;
}


/*--------------------------------------------------------------------------
 * GrGeomNewSolid
 *--------------------------------------------------------------------------*/

GrGeomSolid   *GrGeomNewSolid(
                              GrGeomOctree * data,
                              GrGeomOctree **patches,
                              int            num_patches,
                              int            octree_bg_level,
                              int            octree_ix,
                              int            octree_iy,
                              int            octree_iz)
{
  GrGeomSolid   *new_grgeomsolid;

  new_grgeomsolid = talloc(GrGeomSolid, 1);

  new_grgeomsolid->data = data;
  new_grgeomsolid->patches = patches;
  new_grgeomsolid->num_patches = num_patches;
  new_grgeomsolid->octree_bg_level = octree_bg_level;
  new_grgeomsolid->octree_ix = octree_ix;
  new_grgeomsolid->octree_iy = octree_iy;
  new_grgeomsolid->octree_iz = octree_iz;

  new_grgeomsolid->interior_boxes = NULL;

#if defined(PARFLOW_HAVE_CUDA) || defined(PARFLOW_HAVE_KOKKOS)
  GrGeomSolidCellFlagData(new_grgeomsolid) = NULL; 
  GrGeomSolidCellFlagDataSize(new_grgeomsolid) = 0;
  GrGeomSolidCellFlagInitialized(new_grgeomsolid) = 0;

  (new_grgeomsolid->ival) = talloc(int**, GrGeomOctreeNumFaces);
  for (int f = 0; f < GrGeomOctreeNumFaces; f++)
  { 
    (new_grgeomsolid->ival[f]) = talloc(int*, 2 * num_patches);    
    for (int ipatch = 0; ipatch < 2 * num_patches; ipatch++)
      (new_grgeomsolid->ival[f][ipatch]) = NULL;
  }
#endif

  for (int f = 0; f < GrGeomOctreeNumFaces; f++)
  {
    GrGeomSolidSurfaceBoxes(new_grgeomsolid, f) = NULL;

    new_grgeomsolid->patch_boxes[f] = talloc(BoxArray *, num_patches);

    for (int patch = 0; patch < num_patches; patch++)
    {
      GrGeomSolidPatchBoxes(new_grgeomsolid, patch, f) = NULL;
    }
  }

  if (GlobalsUseClustering)
  {
    ComputeBoxes(new_grgeomsolid);
  }
  return new_grgeomsolid;
}


/*--------------------------------------------------------------------------
 * GrGeomFreeSolid
 *--------------------------------------------------------------------------*/

void          GrGeomFreeSolid(
                              GrGeomSolid *solid)
{
  int i;

  if (GrGeomSolidInteriorBoxes(solid))
  {
    FreeBoxArray(GrGeomSolidInteriorBoxes(solid));
  }

  for (int f = 0; f < GrGeomOctreeNumFaces; f++)
  {
    if (GrGeomSolidSurfaceBoxes(solid, f))
    {
      FreeBoxArray(GrGeomSolidSurfaceBoxes(solid, f));
    }

    for (int patch = 0; patch < GrGeomSolidNumPatches(solid); patch++)
    {
      if (GrGeomSolidPatchBoxes(solid, patch, f))
      {
        FreeBoxArray(GrGeomSolidPatchBoxes(solid, patch, f));
      }
    }

    tfree(solid->patch_boxes[f]);
  }

#if defined(PARFLOW_HAVE_CUDA) || defined(PARFLOW_HAVE_KOKKOS)
  // Internal _tfree_device function is used because unified memory is not active in this comp unit
  if(GrGeomSolidCellFlagData(solid)) _tfree_device(GrGeomSolidCellFlagData(solid));

  for (int f = 0; f < GrGeomOctreeNumFaces; f++)
  {
    for (int ipatch = 0; ipatch < 2 * GrGeomSolidNumPatches(solid); ipatch++)
    {
      int *ival = GrGeomSolidCellIval(solid, ipatch, f);
      if(ival)
        _tfree_device(ival);
    }
    tfree(solid->ival[f]);
  }
  tfree(solid->ival);
#endif

  GrGeomFreeOctree(GrGeomSolidData(solid));
  for (i = 0; i < GrGeomSolidNumPatches(solid); i++)
    GrGeomFreeOctree(GrGeomSolidPatch(solid, i));
  tfree(GrGeomSolidPatches(solid));

  tfree(solid);
}


/*--------------------------------------------------------------------------
 * GrGeomSolidFromInd
 *--------------------------------------------------------------------------*/

void             GrGeomSolidFromInd(
                                    GrGeomSolid **solid_ptr,
                                    Vector *      indicator_field,
                                    int           indicator)
{
  GrGeomOctree *solid_octree;

  int octree_bg_level;
  int ix, iy, iz;
  double xl, yl, zl, xu, yu, zu;

  /*------------------------------------------------------
   * Create the GrGeom solids, converting only the first
   * `nsolids' indicator solids
   *------------------------------------------------------*/

  octree_bg_level = GrGeomGetOctreeInfo(&xl, &yl, &zl, &xu, &yu, &zu,
                                        &ix, &iy, &iz);

  GrGeomOctreeFromInd(&solid_octree, indicator_field, indicator,
                      xl, yl, zl, xu, yu, zu,
                      octree_bg_level, ix, iy, iz);

  *solid_ptr = GrGeomNewSolid(solid_octree, NULL, 0, octree_bg_level, ix, iy, iz);
}


/*--------------------------------------------------------------------------
 * GrGeomSolidFromGeom
 *--------------------------------------------------------------------------*/

void                GrGeomSolidFromGeom(
                                        GrGeomSolid **     solid_ptr,
                                        GeomSolid *        geom_solid,
                                        GrGeomExtentArray *extent_array)
{
  GrGeomSolid    *solid;

  GrGeomOctree   *solid_octree;
  GrGeomOctree  **patch_octrees;
  int num_patches = 0;
  int octree_bg_level = 0, ix, iy, iz;


  /*------------------------------------------------------
   * Convert to GrGeomOctree format
   *------------------------------------------------------*/

  switch (GeomSolidType(geom_solid))
  {
    case GeomTSolidType:
    {
      GeomTSolid  *solid_data = (GeomTSolid*)GeomSolidData(geom_solid);

      GeomTIN     *surface;
      int        **patches;
      int         *num_patch_triangles;

      double xl, yl, zl, xu, yu, zu;


      surface = (solid_data->surface);
      patches = (solid_data->patches);
      num_patches = (solid_data->num_patches);
      num_patch_triangles = (solid_data->num_patch_triangles);

      octree_bg_level = GrGeomGetOctreeInfo(&xl, &yl, &zl, &xu, &yu, &zu,
                                            &ix, &iy, &iz);

      GrGeomOctreeFromTIN(&solid_octree, &patch_octrees,
                          surface, patches, num_patches, num_patch_triangles,
                          extent_array, xl, yl, zl, xu, yu, zu,
                          octree_bg_level,
                          octree_bg_level + GlobalsMaxRefLevel);

      break;
    }
  }

  solid = GrGeomNewSolid(solid_octree, patch_octrees, num_patches,
                         octree_bg_level, ix, iy, iz);

  *solid_ptr = solid;
}


