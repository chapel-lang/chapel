use level_base_defs;


//===> AMRHierarchy class ===>
//===========================>
class AMRHierarchy {

  const max_level_number: int;
  const level_numbers = [1..max_level_number];

  var level_list: [level_numbers] BaseLevel;
  var levels: domain(BaseLevel);

  var coarse_interfaces: [levels] LevelInterface;
  var fine_interfaces:   [levels] LevelInterface;

  var boundary_ghosts: [levels] LevelBoundaryDomain;

}
//<=== AMRHierarchy class <===
//<===========================




//===> AMRHierarchy.partitionBoundaryGhosts method ===>
//====================================================>
//------------------------------------------------------------------
// Sets up boundary_ghosts for all grids in the hierarchy.
//------------------------------------------------------------------
def partitionBoundaryGhosts() {

  var boundary_domain: domain(dimension);
  var range_tuple:     dimension*range(stridable=true);


  for level in levels {

    boundary_ghosts(level) = new LevelBoundaryDomain(level = level);

    coforall grid in level.grids {

      //===> Set subdomains by orientation ===>
      for orientation in dimensions {

	//==== Set up off-dimensions ====
	for d in [1..orientation-1] do
	  range_tuple(d) = level.cells.dim(d);

	for d in [orientation+1 .. dimension] do
	  range_tuple(d) = level.ext_cells.dim(d);


	//==== Low end ====
	range_tuple(orientation) = level.ext_cells.low(orientation)
                                   .. level.cells.low(orientation) - 2
                                   by 2;

	boundary_domain = range_tuple;

	boundary_ghosts(level)(grid).low(d) = intersectDomains(low_ghost_cells(d), boundary_domain);


	//==== High end ====
	range_tuple(orientation) = level.cells.high(orientation) + 2
                                   .. level.ext_cells.high(orientation)
                                   by 2;

	boundary_domain = range_tuple;

	boundary_ghosts(level)(grid).high(d) = intersectDomains(high_ghost_cells(d), boundary_domain);
      }
      //<=== Set subdomains by orientation <===


    } // end coforall grid in level.grids

  } // end for level in levels

}
//<=== AMRHierarchy.partitionBoundaryGhosts method <===
//<====================================================






//===> LevelInterface class ===>
//=============================>
class LevelInterface {

  const coarse_level: BaseLevel;
  const fine_level:   BaseLevel;

  const ref_ratio: dimension*int;

  var f2c_interp_cells: [coarse_level.grids]
                        [fine_level.grids]
                        subdomain(coarse_level.cells);

  var c2f_interp_cells: [fine_level.grids]
                        [coarse_level.grids]
                        sparse subdomain(fine_level.cells);

}
//<=== LevelInterface class <===
//<=============================




//===> LevelInterface.refineCells method ===>
//==========================================>
//-----------------------------------------------------------------
// Refines a domain of cells using the LevelInterface's ref_ratio.
//-----------------------------------------------------------------
def LevelInterface.refineCells(coarse_cells: subdomain(coarse_level.cells)) {
 
  //=== Index bounds for refined domain ====
  var low_fine  = coarse_cells.low;
  var high_fine = coarsecells.high;

  for d in dimensions {
    //==== Move cell centers to vertices ====
    low_fine(d)  -= 1;
    high_fine(d) += 1;

    //==== Refine ====
    low_fine(d)  *= ref_ratio;
    high_fine(d) *= ref_ratio;

    //==== Move vertices back to cell centers ====
    low_fine(d)  += 1;
    high_fine(d) -= 1;
  }


  //==== Set and return new domain ====
  var range_tuple: dimension*range(stridable=true);

  for d in dimensions do
    range_tuple(d) = low_fine(d) .. high_fine(d) by 2;

  var fine_cells: subdomain(fine_level.cells) = range_tuple;
  return fine_cells;
  
}
//<=== LevelInterface.refineCells method <===
//<==========================================




//===> LevelInterface.coarsenCells method ===>
//===========================================>
//------------------------------------------------------------------
// Coarsens a domain of cells using the LevelInterface's ref_ratio.
//------------------------------------------------------------------
def LevelInterface.coarsenCells(fine_cells: subdomain(fine_level.cells)) {
 
  //=== Index bounds for coarsened domain ====
  var low_coarse  = fine_cells.low;
  var high_coarse = fine_cells.high;

  for d in dimensions do {
    //==== Move cell centers to vertices ====
    low_coarse(d)  -= 1;
    high_coarse(d) += 1;

    //==== Make sure coarsening is valid ====
    assert(low_coarse(d)  % ref_ratio(d) == 0);
    assert(high_coarse(d) % ref_ratio(d) == 0);

    //==== Coarsen ====
    low_coarse(d)  /= ref_ratio(d);
    high_coarse(d) /= ref_ratio(d);

    //==== Move vertices back to cell centers ====
    low_coarse(d)  -= 1;
    high_coarse(d) += 1;
  }


  //==== Set and return new domain ====
  var range_tuple: dimension*range(stridable=true);

  for d in dimensions do
    range_tuple(d) = low_coarse(d) .. coarse(d) by 2;

  var coarse_cells: subdomain(coarse_level.cells) = range_tuple;
  return coarse_cells;
  
}
//<=== LevelInterface.coarsen method <===
//<======================================





//===> LevelInterface.partitionC2FInterpCells ===>
//===============================================>
//----------------------------------------------------------------
// Identifies fine ghost cells on which function values should be
// interpolated from the coarse grid.
//----------------------------------------------------------------
def partitionC2FInterpCells() {
    
  for fine_grid in fine_level.grids {

    //==== Nicknamed pointer, for convenience ====
    var interp_cells => c2f_interp_cells(fine_grid);


    for coarse_grid in coarse_level.grids {

      //==== Calculate coarse-fine intersection ====
      var cf_intersection = intersectDomains(fine_grid.ext_cells, 
		                             refineCells(coarse_grid.cells));

      //===> Populate interp_cells(coarse_grid) ===>
      if cf_intersection.numIndices > 0 {

	//==== Add all ghost cells in coarse-fine intersection to the temp list ====
	for cell in fine_grid.ghost_cells do
	  if cf_intersection.member(cell) then 
	    interp_cells(coarse_grid).add(cell);

      
	//==== Remove any fine_level.shared_ghosts from the temp list ====
	for sib in fine_level.grids {
	  for cell in fine_level.shared_ghosts(fine_grid)(sib) do
	    if interp_cells(coarse_grid).member(cell) then 
	      interp_cells(coarse_grid).remove(cell);
	}

      }
      //<=== Populate interp_cells(coarse_grid) <===


    } // end for coarse_grid in coarse_level.grids

  } // end for fine_grid in fine_level.grids

}
//<=== LevelInterface.partitionC2FInterpCells method <===
//<======================================================




//===> LevelInterface.partitionF2CInterpCells method ===>
//======================================================>
//---------------------------------------------------------------------
// Identifies coarse cells which are shadowed by fine cells, and thus
// should have their values overwritten by fine grid values on the way
// back up an AMR cycle.
//---------------------------------------------------------------------
def LevelInterface.partitionF2CInterpCells() {

  for coarse_grid in coarse_level.grids {
    for fine_grid in fine_level.grids {

      f2c_interp_cells(coarse_grid)(fine_grid)
          = intersectDomains(coarse_grid.cells, 
                             coarsenCells(fine_grid.cells));

    }
  }

}
//<=== LevelInterface.partitionF2CInterpCells method <===
//<======================================================




//===> LevelInterface.C2FInterpolator method ===>
//==============================================>
//---------------------------------------------------------------------
// Interpolates data from q_coarse into the appropriate ghost cells of
// q_fine, using a minmod-type method for limiting.  (Same method as
// in the Chombo spec, but a bit more descriptive.)
//---------------------------------------------------------------------
def LevelInterface.C2FInterpolator(
  q_coarse: LevelArray,
  q_fine:   LevelArray,)
{

  coforall fine_grid in fine_level.grids {
    for coarse_grid in coarse_level.grids {

      var c2f_preimage = c2f_coarse_cells(fine_grid)(coarse_grid);

      // Compute differentials on the coarse grid's locale
      var coarse_values = q_coarse(coarse_grid).value(c2f_preimage);
      var coarse_diffs  = minmodDiffs(q_coarse, c2f_preimage);

      // Send to the fine grid's locale

      // Evaluate the interpolant on refined image of each coarse cell
      forall coarse_cell in c2f_preimage {
	for fine_cell in refineCell(coarse_cell) {
	  var fine_disp: real;
	  var fine_value = coarse_values(coarse_cell);

	  for d in dimensions {
	    fine_disp = fine_cell(d) / ref_ratio(d);
	    fine_disp -= coarse_cell(d);
	    fine_disp *= 0.5;

	    fine_value += fine_disp * coarse_diffs(coarse_cell)(d);
	  }

	  q_fine(fine_grid)(fine_cell) = fine_value;
	}
      }

    }
  }

}
//<=== LevelInterface.C2FInterpolator method <===
//<==============================================



// Next up: change c2f storage; create minmodDiffs method; refineCell method