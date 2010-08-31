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

  var c2f_clusters: [coarse_grids] [fine_grids] CFClusters;

  class CFClusters {
    var coarse_cells: sparse subdomain(coarse_level.cells);
    var fine_cells: [coarse_cells] sparse subdomain(fine_level.cells);
  }


}
//<=== LevelInterface class <===
//<=============================




//===> LevelInterface.refine method ===>
//=====================================>
//-----------------------------------------------------------------
// Refines a domain of cells using the LevelInterface's ref_ratio.
//-----------------------------------------------------------------
def LevelInterface.refine(coarse_cells: subdomain(coarse_level.cells)) {
 
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

//---------------------------------------------------------------------
// Refine a single coarse cell rather than a domain.  No safety check.
//---------------------------------------------------------------------
def LevelInterface.refine(coarse_cell: dimension*int) {

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
//<=== LevelInterface.refine method <===
//<=====================================





//===> LevelInterface.coarsen method ===>
//======================================>
//------------------------------------------------------------------
// Coarsens a domain of cells using the LevelInterface's ref_ratio.
//------------------------------------------------------------------
def LevelInterface.coarsen(fine_cells: subdomain(fine_level.cells)) {
 
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


//--------------------------------------------------
// Coarsen a single fine cell rather than a domain.
//--------------------------------------------------
def LevelInterface.coarsen(fine_cell: dimension*int) {

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
//<=== LevelInterface.coarsen method <===
//<======================================






//===> LevelInterface.createC2FClusters ===>
//=========================================>
//-----------------------------------------------------------------
// Identifies cells (as associative domains) that will be involved
// in coarse-to-fine interpolation to fill fine ghost cells on the
// way down an AMR cycle.
//-----------------------------------------------------------------
def createC2FClusters() {
    
  for coarse_grid in coarse_grids {

    for fine_grid in fine_grids {


      //==== Calculate coarse-fine intersection ====
      var unclustered_fine_cells: sparse subdomain(fine_grid.ext_cells);


      //===> Populate fine cell list ===>
      var fine_intersection = intersectDomains(fine_grid.ext_cells,
		                               refineCells(coarse_grid.cells));
      if fine_intersection.numIndices > 0 {

	//==== Add fine ghost cells in the intersection ====
	for cell in fine_grid.ghost_cells {
	  if fine_intersection.member(cell) then
	    unclustered_fine_cells.add(cell);
	}

	//==== Remove any fine_level.shared_ghosts ====
	for sib in fine_grids {
	  for cell in fine_level.shared_ghosts(fine_grid)(sib) {
	    if unclustered_fine_cells.member(cell) then
	      unclustered_fine_cells.remove(cell);
	  }
	}

      }
      //<=== Populate fine cell list <===


      //===> Create coarse-fine clusters ===>
      var clusters = new CFClusters();

      for fine_cell in unclustered_fine_cells {
	var coarse_cell = coarsen(fine_cell);

	//==== Chapel should filter out duplicates... ====	
	clusters.coarse_cells.add(coarse_cell);

	clusters.fine_cells(coarse_cell).add(fine_cell);
      }

      //==== Place reference in c2f clusters array ====
      c2f_clusters(coarse_grid)(fine_grid) = clusters;
      //<=== Create coarse-fine clusters <===


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




//===> LevelInterface.LinearBoundaryInterpolator method ===>
//=========================================================>
def LevelInterface.LinearBoundaryInterpolator(
  q_coarse: LevelArray,
  q_fine:   LevelArray) 
{

  //===> Fill coarse ghost cells for interpolation ===>
  //-----------------------------------------------------------------------
  // Shared ghost cells are brought up to date at this point, and boundary
  // ghosts are filled by linear extrapolation from the interior.  This
  // avoids reasoning about where one-sided differences are needed when
  // computing the coarse differentials later on.
  //-----------------------------------------------------------------------
  coarse_level.fillSharedGhosts(q_coarse);
  coforall grid in coarse_level.grids {

    forall cell in level.boundary_ghosts(grid) {
      var inward_shift: dimension*int;

      //==== Compute the inward shift direction ====
      for d in dimensions {
	if cell(d) < grid.i_low(d) then
	  inward_shift(d) = 2;
	else if cell(d) > grid.i_high(d) then
	  inward_shift(d) = -2;
      }

      //==== Extrapolate the boundary cell value ====
      q_coarse(grid).value(cell) = 2.0*q_coarse(grid).value(cell + inward_shift)
                                     - q_coarse(grid).value(cell + 2*inward_shift);

    }

  }
  //<=== Fill coarse ghost cells for interpolation <===


  for coarse_grid in coarse_grids {
    for fine_grid in fine_grids {

      var clusters = c2f_clusters(coarse_grid)(fine_grid);
      var q_coarse_grid = q_coarse(coarse_grid);

      //===> Fill in coarse data for interpolants ===>
      // LOCALE: COARSE GRID
      //------------------------------------------------------------------
      // The coarse differential in each dimension is the minimum of each
      // one-sided difference and the centered difference.  If the signs
      // of the one-sided differences disagree, it's set to zero.
      //------------------------------------------------------------------
      var coarse_values = q_coarse_grid(clusters.coarse_cells);
      var coarse_diffs: [clusters.coarse_cells] [dimensions] real;

      for cell in clusters.coarse_cells {
	var diff_mag, diff_sign, diff_low, diff_high, diff_cen: real;

	for d in dimensions {
	  var shift: dimension*int;
	  shift(d) = 2;
	  
	  var diff_low  = q_coarse_grid.value(cell) - q_coarse_grid.value(cell-shift);
	  var diff_high = q_coarse_grid.value(cell+shift) - q_coarse_grid.value(cell);
	  var diff_cen  = (diff_high - diff_low) / 2.0;

	  if diff_low*diff_high > 0 {
	    diff_sign = diff_low / abs(diff_low);
	    diff_mag = min( abs(diff_low), abs(diff_high), abs(diff_cen) );
	    coarse_diffs(cell)(d) = diff_sign * diff_mag;
	  }
	  else
	    coarse_diffs(cell)(d) = 0.0;
	}
      }
      //<=== Fill in coarse data for interpolants <===


      //===> Interpolate on each fine cell ===>
      // LOCALE: FINE GRID
      //-----------------------------------------------------------------------
      // Compute in batches, one for each coarse-fine cluster.
      // On each cell, the fine value is initialized to the coarse value.
      // Then, for each dimension, and update is applied based on displacement
      // from the coarse cell-center.
      //-----------------------------------------------------------------------
      for coarse_cell in clusters.coarse_cells {
	
	var fine_cells = clusters.fine_cells(coarse_cell);
	var fine_values: [fine_cells] real;

	for fine_cell in fine_cells {
	  
	  var fine_displacement: real;
	  var fine_values(fine_cell) = coarse_values(coarse_cell);

	  for d in dimensions {
	    //==== Move to coarse indices ====
	    fine_displacement = fine_cell(d):real / ref_ratio(d):real;

	    //==== Compute displacement ====
	    fine_displacement -= coarse_cell(d):real;

	    //==== Rescale: One cell occupies 2 indices ====
	    fine_displacement /= 2.0;

	    //==== Modify fine_value ====
	    fine_values(fine_cell) += fine_displacement * coarse_diffs(coarse_cell)(d);
	  }
	}

	q_fine(fine_grid).value(fine_cells) = fine_values;

      }
      //<=== Interpolate fine values <===

    } // end for coarse_grid in coarse_grids
  } // end for fine_grid in fine_grids

}
//<=== LevelInterface.LinearBoundaryInterpolator method <===
//<=========================================================







/* //===> LevelInterface.C2FInteriorInterpolator method ===> */
/* //======================================================> */
/* //--------------------------------------------------------------------- */
/* //  */
/* //  */
/* //  */
/* //--------------------------------------------------------------------- */
/* def LevelInterface.C2FInteriorInterpolator( */
/*   q_coarse: LevelArray, */
/*   q_fine:   LevelArray,) */
/* { */

/*   for coarse_grid in coarse_grids { */
/*     for fine_grid in fine_grids { */

/*       var cluster = c2f_clusters(coarse_grid)(fine_grid); */
/*       var q_coarse_grid = q_coarse(coarse_grid); */


/*       //==== Compute differentials on the coarse grid's locale ==== */
/*       var coarse_values = q_coarse_grid(cluster.coarse_cells); */
/*       var coarse_diffs  = minmodDiffs(q_coarse_grid, cluster.coarse_cells); */

/*       // Send coarse_values and coarse_diffs to the fine grid's locale */


/*       //==== Evaluate the interpolant on refined image of each coarse cell ==== */
/*       forall coarse_cell in cluster.coarse_cells { */
/* 	for fine_cell in cluster.fine_cells(coarse_cell) { */
	  
/* 	  var fine_displacement: real; */
/* 	  var fine_value = coarse_values(coarse_cell); */

/* 	  for d in dimensions { */
/* 	    //==== Move to coarse indices ==== */
/* 	    fine_displacement = fine_cell(d):real / ref_ratio(d):real; */

/* 	    //==== Compute displacement ==== */
/* 	    fine_displacement -= coarse_cell(d):real; */

/* 	    //==== Rescale: One cell occupies 2 indices ==== */
/* 	    fine_displacement /= 2.0; */

/* 	    //==== Modify fine_value ==== */
/* 	    fine_value += fine_displacement * coarse_diffs(coarse_cell)(d); */
/* 	  } */

/* 	  //==== Store fine_value ==== */
/* 	  q_fine(fine_grid)(fine_cell) = fine_value; */

/* 	} */
/*       } */


/*     } // end for fine_grid in fine_grids */
/*   } // end for coarse_grid in coarse_grids */

/* } */
/* //<=== LevelInterface.C2FInteriorInterpolator method <=== */
/* //<============================================== */



/* // Next up: create minmodDiffs method */



/* //===> LevelInterface.minmodDiffs ===> */
/* //===================================> */
/* //---------------------------------------------------- */
/* // */
/* // */
/* def LevelInterface.minmodDiffs( */
/*   q:     GridArray,  */
/*   cells: subdomain(coarse_level.cells)) */
/* { */

/*   var differentials: [cells] [dimensions] real; */

/*   forall cell in cells { */

/*     //===> Maximum allowed differential ===> */
/*     var max_rise = 0.0, */
/*       max_drop = 0.0; */

/*     //==== First compute the max and min values in the neighborhood ==== */
/*     for nbr in neighborhood(cell,1) { */
/*       max_rise = max( max_rise, sol.value(nbr) ); */
/*       max_drop = min( max_drop, sol.value(nbr) ); */
/*     } */

/*     //==== Now adjust accordingly to get the diffs ==== */
/*     max_rise = max_rise - sol.value(cell); */
/*     max_drop = sol.value(cell) - max_drop; */
/*     var max_allowed_diff = min(max_rise, max_drop); */
/*     //<=== Maximum allowed differential <=== */


/*     //==== Estimate differentials with centered differences ==== */
/*     for d in dimensions { */
/*       var shift: dimension*int; */
/*       shift(d) = 2; */
/*       differentials(cell)(d) = (  q.value(cell + shift)  */
/* 		                - q.value(cell - shift) */
/* 		               ) / 2.0; */
/*     } */

/*     //==== Maximum of centered differentials ==== */
/*     var max_centered_diff = 0.0; */
/*     for d in dimensions do  */
/*       max_centered_diff = max(max_centered_diff,  */
/*                               abs(differentials(cell)(d))); */


/*     //==== Compute limiter ==== */
/*     var limiter =  */
/*       if max_centered_diff > max_allowed_diff then  */
/*         max_allowed_diff / max_centered_diff */
/*       else  */
/* 	1; */


/*     //==== Limit the estimated differentials ==== */
/*     differentials(cell) *= limiter; */

/*   } */


/* } */
/* //<=== LevelInterface.minmodDiffs <=== */
/* //<=================================== */