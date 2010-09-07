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

  const coarse_grids = coarse_level.grids;
  const fine_grids   = fine_level.grids;

  const ref_ratio: dimension*int;

  var f2c_coarse_cells: [coarse_grids] [fine_grids]
                        subdomain(coarse_level.cells);
  var f2c_fine_cells:   [coarse_grids] [fine_grids]
                        subdomain(fine_level.cells);

/*   var c2f_coarse_cells: [coarse_grids] [fine_grids] */
/*                         sparse subdomain(coarse_level.cells); */
/*   var c2f_fine_cells:   [coarse_grids] [fine_grids] */
/*                         sparse subdomain(fine_level.cells); */

  var c2f_clusters: [coarse_grids] [fine_grids] ClusteredCFCells;



  class CFCluster {
    var coarse_cells: sparse subdomain(coarse_level.cells);
    var fine_cells: [coarse_cells] sparse subdomain(fine_level.cells);
  }


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
  var fine_cells_low  = coarse_cells.low;
  var fine_cells_high = coarse_cells.high;

  for d in dimensions {
    //==== Move cell centers to vertices ====
    fine_cells_low(d)  -= 1;
    fine_cells_high(d) += 1;

    //==== Refine ====
    fine_cells_low(d)  *= ref_ratio;
    fine_cells_high(d) *= ref_ratio;

    //==== Move vertices back to cell centers ====
    fine_cells_low(d)  += 1;
    fine_cells_high(d) -= 1;
  }


  //==== Set and return new domain ====
  var range_tuple: dimension*range(stridable=true);

  for d in dimensions do
    range_tuple(d) = fine_cells_low(d) .. fine_cells_high(d) by 2;

  var fine_cells: subdomain(fine_level.cells) = range_tuple;
  return fine_cells;
  
}
//<=== LevelInterface.refineCells method <===
//<==========================================



//===> LevelInterface.refineCell method ===>
//=========================================>
//---------------------------------------------------------------------
// Refine a single coarse cell rather than a domain.  No safety check.
//---------------------------------------------------------------------
def LevelInterface.refineCell(coarse_cell: dimension*int) {

  var fine_cells_low = coarse_cell;
  var fine_cells_high = coarse_cell;

  for d in dimensions {
    //==== Move cell centers to vertices ====
    fine_cells_low(d)  -= 1;
    fine_cells_high(d) += 1;

    //==== Refine ====
    fine_cells_low(d)  *= ref_ratio(d);
    fine_cells_high(d) *= ref_ratio(d);

    //==== Move vertices back to cell centers ====
    fine_cells_low(d)  += 1;
    fine_cells_high(d) -= 1;
  }


  //==== Set and return new domain ====
  var range_tuple: dimension*range(stridable=true);

  for d in dimensions do
    range_tuple(d) = fine_cells_low(d) .. fine_cells_high(d) by 2;

  var fine_cells: subdomain(fine_level.cells) = range_tuple;
  return fine_cells;

}
//<=== LevelInterface.refineCell method <===
//<=========================================





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


//===> LevelInterface.coarsenCell method ===>
//==========================================>
//--------------------------------------------------
// Coarsen a single fine cell rather than a domain.
//--------------------------------------------------
def LevelInterface.coarsenCell(fine_cell: dimension*int) {

  var coarse_cell: dimension*int;

  //-------------------------------------------------------------
  // Dividing by ref_ratio (and throwing away remainder) maps to
  // lower vertex of the coarse cell.  Adding 1 shifts index to
  // the cell center.
  //-------------------------------------------------------------
  for d in dimensions do
    coarse_cell(d) = fine_cell(d) / ref_ratio(d) + 1;

  return coarse_cell;

}
//<=== LevelInterface.coarsenCell method <===
//<==========================================






//===> LevelInterface.createC2FClusters ===>
//=========================================>
//-----------------------------------------------------------------
// Identifies cells (as associative domains) that will be involved
// in coarse-to-fine interpolation to fill fine ghost cells on the
// way down an AMR cycle.
//-----------------------------------------------------------------
def partitionC2FCells() {
    
  for coarse_grid in coarse_grids {

    for fine_grid in fine_grids {


      //==== Calculate coarse-fine intersection ====
      var unclustered_fine_cells: sparse subdomain(fine_grid.ext_cells);


      //===> Populate fine_cell_list ===>
      var fine_intersection = intersectDomains(fine_grid.ext_cells,
		                               refineCells(coarse_grid.cells));
      if fine_intersection.numIndices > 0 {

	//==== Add fine ghost cells in the intersection ====
	for cell in fine_grid.ghost_cells do
	  if fine_intersection.member(cell) then 
	    unclustered_fine_cells.add(cell);

	//==== Remove any fine_level.shared_ghosts ====
	for sib in fine_grids {
	  for cell in fine_level.shared_ghosts(fine_grid)(sib) do
	    if unclustered_fine_cells.member(cell) then 
	      unclustered_fine_cells.remove(cell);
	}

      }
      //<=== Populate fine_cell_list <===


      //==== Create coarse-fine cluster ===
      var cluster = new CFCluster();
      for fine_cell in unclustered_fine_cells {
	var coarse_cell = coarsen(fine_cell);

	//==== Chapel should filter out duplicates... ====	
	cluster.coarse_cells.add(coarse_cell);

	cluster.fine_cells(coarse_cell).add(fine_cell);
      }

      //==== Place reference in c2f cluster array ====
      c2f_clusters(coarse_grid)(fine_grid) = cluster;


    } // end for fine_grid in fine_grids

  } // end for coarse_grid in coarse_grids

}
//<=== LevelInterface.createC2FClusters <===
//<=========================================




//===> LevelInterface.partitionF2CCells method ===>
//================================================>
//---------------------------------------------------------------------
// Identifies cells (as arithmetic domains) that will be involved in
// fine-to-coarse averaging on the way up an AMR cycle.
//---------------------------------------------------------------------
def LevelInterface.partitionF2CCells() {

  for coarse_grid in coarse_grids {
    for fine_grid in fine_grids {

      f2c_coarse_cells(coarse_grid)(fine_grid)
          = intersectDomains(coarse_grid.cells, 
                             coarsenCells(fine_grid.cells));

      f2c_fine_cells(coarse_grid)(fine_grid)
          = refineCells( f2c_coarse_cells(coarse_grid)(fine_grid) );

    }
  }

}
//<=== LevelInterface.partitionF2CCells method <===
//<================================================




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

  for coarse_grid in coarse_grids {
    for fine_grid in fine_grids {

      var cluster = c2f_clusters(coarse_grid)(fine_grid);
      var q_coarse_grid = q_coarse(coarse_grid);


      //==== Compute differentials on the coarse grid's locale ====
      var coarse_values = q_coarse_grid(cluster.coarse_cells);
      var coarse_diffs  = minmodDiffs(q_coarse_grid, cluster.coarse_cells);

      // Send coarse_values and coarse_diffs to the fine grid's locale


      //==== Evaluate the interpolant on refined image of each coarse cell ====
      forall coarse_cell in cluster.coarse_cells {
	for fine_cell in cluster.fine_cells(coarse_cell) {
	  
	  var fine_displacement: real;
	  var fine_value = coarse_values(coarse_cell);

	  for d in dimensions {
	    //==== Move to coarse indices ====
	    fine_displacement = fine_cell(d):real / ref_ratio(d):real;

	    //==== Compute displacement ====
	    fine_displacement -= coarse_cell(d):real;

	    //==== Rescale: One cell occupies 2 indices ====
	    fine_displacement /= 2.0;

	    //==== Modify fine_value ====
	    fine_value += fine_displacement * coarse_diffs(coarse_cell)(d);
	  }

	  //==== Store fine_value ====
	  q_fine(fine_grid)(fine_cell) = fine_value;

	}
      }


    } // end for fine_grid in fine_grids
  } // end for coarse_grid in coarse_grids

}
//<=== LevelInterface.C2FInterpolator method <===
//<==============================================



// Next up: create minmodDiffs method



//===> LevelInterface.minmodDiffs ===>
//===================================>
//----------------------------------------------------
//
//
def LevelInterface.minmodDiffs(
  q:     GridArray, 
  cells: subdomain(coarse_level.cells))
{

  var differentials: [cells] [dimensions] real;

  forall cell in cells {

    //===> Maximum allowed differential ===>
    var max_rise = 0.0,
      max_drop = 0.0;

    //==== First compute the max and min values in the neighborhood ====
    for nbr in neighborhood(cell,1) {
      max_rise = max( max_rise, sol.value(nbr) );
      max_drop = min( max_drop, sol.value(nbr) );
    }

    //==== Now adjust accordingly to get the diffs ====
    max_rise = max_rise - sol.value(cell);
    max_drop = sol.value(cell) - max_drop;
    var max_allowed_diff = min(max_rise, max_drop);
    //<=== Maximum allowed differential <===


    //==== Estimate differentials with centered differences ====
    for d in dimensions {
      var shift: dimension*int;
      shift(d) = 2;
      differentials(cell)(d) = (  q.value(cell + shift) 
		                - q.value(cell - shift)
		               ) / 2.0;
    }

    //==== Maximum of centered differentials ====
    var max_centered_diff = 0.0;
    for d in dimensions do 
      max_centered_diff = max(max_centered_diff, 
                              abs(differentials(cell)(d)));


    //==== Compute limiter ====
    var limiter = 
      if max_centered_diff > max_allowed_diff then 
        max_allowed_diff / max_centered_diff
      else 
	1;


    //==== Limit the estimated differentials ====
    differentials(cell) *= limiter;

  }


}
//<=== LevelInterface.minmodDiffs <===
//<===================================