use level_base_defs;
use level_array_defs;


//~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> AMRHierarchy class ===>
//__________________________/
class AMRHierarchy {

  const x_low, x_high:                  dimension*real;
  const n_coarsest_cells, n_ghost_cells: dimension*int;

  var levels:           domain(BaseLevel);
  var level_numbers:    [levels] int;
  var top_level:        BaseLevel;
  var bottom_level:     BaseLevel;
  
  var coarse_interfaces: [levels] LevelInterface;
  var fine_interfaces:   [levels] LevelInterface;

  var boundary_ghosts: [levels] LevelBoundaryDomain;

  def bottom_level_number {
    return level_numbers(bottom_level);
  }

  def ordered_levels {
    for level_number in [1..bottom_level_number] do
      yield levelFromNumber(level_number);
  }


  //===> levelFromNumber method ===>
  //===============================>
  //---------------------------------------------
  // Retrieve a level based on its level number.
  //---------------------------------------------
  def levelFromNumber(level_number: int) {

    for level in levels {
      if level_numbers(level) == level_number then
        return level;
    }

    halt("levelByNumber: Requested invalid level number.");

  }
  //<=== levelFromNumber method <===
  //<===============================


  def initialize() {

    //==== Create the top level ====
    top_level = new BaseLevel(x_low = x_low,  x_high = x_high,
                              n_cells       = n_coarsest_cells,
                              n_ghost_cells = n_ghost_cells);
    levels.add(top_level);
    level_numbers(top_level) = 1;
    bottom_level = top_level;

  }

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== AMRHierarchy class <===
// \_________________________|






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> AMRHierarchy.addRefinedLevel method ===>
//___________________________________________/
//---------------------------------------------------------------------
// Provided a refinement ratio, this creates a new level which refines
// the current bottom level.
//---------------------------------------------------------------------
def AMRHierarchy.addRefinedLevel(ref_ratio: dimension*int) {
  
  //==== Build refined level (new_bottom_level) ====
  //----------------------------------------------------------------------
  // The number of cells for new_bottom_level is found by applying the 
  // refiement ratio to old_bottom_level's n_cells.
  //----------------------------------------------------------------------
  var old_bottom_level = bottom_level;
  var n_refined_cells  = old_bottom_level.n_cells * ref_ratio;

  var new_bottom_level = new BaseLevel(x_low = x_low, x_high = x_high,
                                       n_cells       = n_refined_cells,
                                       n_ghost_cells = n_ghost_cells);

  levels.add(new_bottom_level);
  level_numbers(new_bottom_level) = level_numbers(old_bottom_level) + 1;
  bottom_level = new_bottom_level;


  //==== Build new LevelInterface ====
  //----------------------------------------------------------------------
  // Note that the LevelInterface has some information on grid structure,
  // which will need to be filled in later.
  //----------------------------------------------------------------------
  var interface = new LevelInterface(coarse_level = old_bottom_level,
                                     fine_level   = new_bottom_level,
                                     ref_ratio    = ref_ratio);

  fine_interfaces(old_bottom_level)    = interface;
  coarse_interfaces(new_bottom_level)  = interface;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== AMRHierarchy.addRefinedLevel method <===
// \__________________________________________|






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> AMRHierarchy.addGridToLevel method ===>
//__________________________________________/
//-----------------------------------------------------------------------
// Adds a grid with corners at x_low and x_high to the input level,
// snapped to the nearest vertices of the coarser level, if applicable.
// This ensures proper blocking of the grid.  In a full AMR system, that
// should be handled by the regridding algorithm, but this simplifies
// the manual development of some test hierarchies.
//-----------------------------------------------------------------------
def AMRHierarchy.addGridToLevel(
  level:  BaseLevel,
  x_low:  dimension*real,
  x_high: dimension*real)
{

  //==== Find index bounds for new grid ====
  var i_low, i_high: dimension*int;

  if level == top_level {
    i_low  = level.snapToVertex(x_low);
    i_high = level.snapToVertex(x_high);
  }
  else {
    var coarser_level = coarse_interfaces(level).coarse_level;
    var ref_ratio     = coarse_interfaces(level).ref_ratio;
    i_low  = coarser_level.snapToVertex(x_low) * ref_ratio;
    i_high = coarser_level.snapToVertex(x_high) * ref_ratio;
  }


  //==== Create new grid ====
  level.addGrid(i_low, i_high);

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== AMRHierarchy.addGridToLevel method <===
// \_________________________________________|





//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> AMRHierarchy.completeLevel method ===>
//|_________________________________________/
//---------------------------------------------------------------------
// After all grids have been added to a level, it should be completed. 
// At this point, it is safe to compute its boundary ghosts, and to
// complete its coarse interface.
//---------------------------------------------------------------------
def AMRHierarchy.completeLevel(level: BaseLevel) {
  writeln("Completing level ", level_numbers(level));
  level.complete();
  partitionBoundaryGhosts(level);

  if level != top_level then
    coarse_interfaces(level).complete();
}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== AMRHierarchy.completeLevel method <===|
// \_________________________________________|




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> AMRHierarchy.partitionBoundaryGhosts method ===>
//___________________________________________________/
//-------------------------------------------------
// Sets up boundary_ghosts for a particular level.
//-------------------------------------------------
def AMRHierarchy.partitionBoundaryGhosts(level: BaseLevel) {

  var boundary_domain: domain(dimension, stridable=true);
  var ranges:          dimension*range(stridable=true);

  boundary_ghosts(level) = new LevelBoundaryDomain(level = level);

  coforall grid in level.grids {

    //===> Set subdomains by orientation ===>
    for d in dimensions {

      //==== Set up off-dimensions ====
      for d_low in [1..d-1] do 
	ranges(d_low) = level.ext_cells.dim(d_low);
      for d_high in [d+1..dimension] do 
 	ranges(d_high) = level.ext_cells.dim(d_high);
    
    
      //==== Low end ====
      ranges(d) = level.ext_cells.low(d) .. level.cells.low(d)-2 by 2;
      boundary_domain = ranges;
      boundary_ghosts(level)(grid).low(d) 
	        = intersectDomains(grid.ghost_cells.low(d), boundary_domain);


      //==== High end ====
      ranges(d) = level.cells.high(d)+2 .. level.ext_cells.high(d) by 2;
      boundary_domain = ranges;
      boundary_ghosts(level)(grid).high(d)
                = intersectDomains(grid.ghost_cells.high(d), boundary_domain);
    }
    //<=== Set subdomains by orientation <===


  } // end coforall grid in level.grids

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== AMRHierarchy.partitionBoundaryGhosts method <===
// \__________________________________________________|












//~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> LevelInterface class ===>
//____________________________/
class LevelInterface {

  const coarse_level: BaseLevel;
  const fine_level:   BaseLevel;
  const ref_ratio:    dimension*int;

  var coarse_grids: domain(BaseGrid);
  var fine_grids:   domain(BaseGrid);

  var f2c_coarse_cells: [coarse_grids] [fine_grids]
                        subdomain(coarse_level.cells);

  var c2f_clusters: [coarse_grids] [fine_grids] CFClusters;


  //===> CFClusters subclass ===>
  //============================>
  //-----------------------------------------------------------------------
  // Stores a list of implicit "clusters", each of which is a coarse cell,
  // along with a list of fine cells in its refined image.  This allows
  // the coarse-to-fine transfer operations to be grouped into batches
  // appropriately.
  //-----------------------------------------------------------------------
  class CFClusters {
    var coarse_cells: sparse subdomain(coarse_level.cells);
    var fine_cells: [coarse_cells] sparse subdomain(fine_level.cells);
  }
  //<=== CFClusters subclass <===
  //<============================


}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface class <===
// \___________________________|







//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> LevelInterface.complete method ===>
//|______________________________________/
def LevelInterface.complete() {

  coarse_grids = coarse_level.grids;
  fine_grids   = fine_level.grids;

  buildC2FClusters();
  partitionF2CCells();

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.complete method <===|
// \______________________________________|




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> LevelInterface.refine method ===>
//____________________________________/
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
    fine_cells_low(d)  *= ref_ratio(d);
    fine_cells_high(d) *= ref_ratio(d);

    //==== Move vertices back to cell centers ====
    fine_cells_low(d)  += 1;
    fine_cells_high(d) -= 1;
  }


  //==== Set and return new domain ====
  var ranges: dimension*range(stridable=true);

  for d in dimensions do
    ranges(d) = fine_cells_low(d) .. fine_cells_high(d) by 2;

  var fine_cells: subdomain(fine_level.cells) = ranges;
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
  var ranges: dimension*range(stridable=true);

  for d in dimensions do
    ranges(d) = fine_cells_low(d) .. fine_cells_high(d) by 2;

  var fine_cells: subdomain(fine_level.cells) = ranges;
  return fine_cells;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<    LevelInterface.refine method    |
// \___________________________________|






//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|    LevelInterface.coarsen method    >
//|____________________________________/
//------------------------------------------------------------------
// Coarsens a domain of cells using the LevelInterface's ref_ratio.
//------------------------------------------------------------------
def LevelInterface.coarsen(fine_cells: subdomain(fine_level.cells)) {
 
  writeln("### Coarsening domain ", fine_cells);

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
    low_coarse(d)  += 1;
    high_coarse(d) -= 1;
  }


  //==== Set and return new domain ====
  var ranges: dimension*range(stridable=true);

  for d in dimensions do
    ranges(d) = low_coarse(d) .. high_coarse(d) by 2;

  var coarse_cells: subdomain(coarse_level.cells) = ranges;

  writeln("### Result is ", coarse_cells);
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
  for d in dimensions {
    coarse_cell(d) = fine_cell(d) / ref_ratio(d);
    if coarse_cell(d) % 2 == 0 then
      coarse_cell(d) += 1;
  }

  return coarse_cell;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.coarsen method <===
// \____________________________________|






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> LevelInterface.buildC2FClusters ===>
//_______________________________________/
//-----------------------------------------------------------------
// Identifies cells (as associative domains) that will be involved
// in coarse-to-fine interpolation to fill fine ghost cells on the
// way down an AMR cycle.
//-----------------------------------------------------------------
def LevelInterface.buildC2FClusters() {
    
  for coarse_grid in coarse_grids {

    for fine_grid in fine_grids {


      //==== Calculate coarse-fine intersection ====
      var unclustered_fine_cells: sparse subdomain(fine_grid.ext_cells);


      //===> Populate fine cell list ===>
      var fine_intersection = intersectDomains(fine_grid.ext_cells,
                                               refine(coarse_grid.cells));
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
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.buildC2FClusters <===
// \______________________________________|





//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> LevelInterface.partitionF2CCells method ===>
//|_______________________________________________/
//---------------------------------------------------------------------
// Identifies cells (as arithmetic domains) that will be involved in
// fine-to-coarse averaging on the way up an AMR cycle.
//---------------------------------------------------------------------
def LevelInterface.partitionF2CCells() {

  for coarse_grid in coarse_grids {
    for fine_grid in fine_grids {

      f2c_coarse_cells(coarse_grid)(fine_grid)
         = intersectDomains(coarse_grid.cells, coarsen(fine_grid.cells));
    }
  }

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.partitionF2CCells method <===|
// \_______________________________________________|






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//===> LevelInterface.LinearBoundaryInterpolator method ===>
//________________________________________________________/
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

    forall cell in coarse_level.boundary_ghosts(grid) {
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
       fine_values(fine_cell) = coarse_values(coarse_cell);

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
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.LinearBoundaryInterpolator method <===
// \_______________________________________________________|











//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> LevelInterface.f2cAverage method ===>
//|________________________________________/
//-------------------------------------------------------------------------
// Conservatively averages data from the fine level onto the coarse level.
//-------------------------------------------------------------------------
def LevelInterface.f2cAverage(
  q_coarse: LevelArray,
  q_fine:   LevelArray
) {

  //==== Volume fraction is 1/product(ref_ratio) ====
  var volume_fraction: real = 1.0;
  for d in dimensions do
    volume_fraction /= ref_ratio(d):real;


  for coarse_grid in coarse_grids {
    for fine_grid in fine_grids {

      //==== Compute coarse averages ====
      // LOCALE: FINE GRID
      var coarse_cells = f2c_coarse_cells(coarse_grid)(fine_grid);
      var coarse_values: [coarse_cells] real; // init to 0

      forall coarse_cell in coarse_cells {
        var fine_cells = refine(coarse_cell);
        for fine_cell in fine_cells do
          coarse_values(coarse_cell) += q_fine(fine_grid).value(fine_cell);
        coarse_values(coarse_cell) *= volume_fraction;
      }

      // Transfer coarse_values to coarse grid locale

      //==== Update coarse grid values ====
      // LOCALE: COARSE GRID
      q_coarse(coarse_grid).value(coarse_cells) = coarse_values;

    } // end for fine_grid in fine_grids
  } // end for coarse_grid in coarse_grids

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== LevelInterface.f2cAverage method <===|
// \________________________________________|








//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> hierarchyFromInputFile routine ===>
//|______________________________________/
def hierarchyFromInputFile(file_name: string){
 
  //==== Open input file ====
  var input_file = new file(file_name, FileAccessMode.read);
  input_file.open();


  //==== Safety check the dimension ====
  var dim_input: int;
  input_file.readln(dim_input);
  assert(dim_input == dimension,
         "error: dimension of input file inconsistent with compiled dimension.");
  input_file.readln(); // skip line


  //==== Problem domain / Top level ====
  var x_low, x_high: dimension*real;
  var n_cells, n_ghost_cells: dimension*int;
  input_file.readln( (...x_low) );
  input_file.readln( (...x_high) );
  input_file.readln( (...n_cells) );
  input_file.readln( (...n_ghost_cells) );
  input_file.readln(); // skip line

  var hierarchy = new AMRHierarchy(x_low = x_low, x_high = x_high,
                                   n_coarsest_cells = n_cells,
                                   n_ghost_cells = n_ghost_cells);

  hierarchy.addGridToLevel(hierarchy.top_level, x_low, x_high);
  hierarchy.completeLevel(hierarchy.top_level);
  

  //===> Refined levels ===>
  var n_levels: int;
  input_file.readln(n_levels);
  input_file.readln(); // skip line

  var ref_ratio: dimension*int;
  var n_grids:   int;

  for i_level in [2..n_levels] {

    input_file.readln( (...ref_ratio) );
    input_file.readln( n_grids );
    input_file.readln(); // skip line

    hierarchy.addRefinedLevel(ref_ratio);

    for i_grid in [1..n_grids] {
      input_file.readln( (...x_low) );
      input_file.readln( (...x_high) );
      input_file.readln(); // skip line

      hierarchy.addGridToLevel(hierarchy.bottom_level, x_low, x_high);
    }

    hierarchy.completeLevel(hierarchy.bottom_level);

  }
  //<=== Refined levels <===


  //==== Close input file and return ====
  input_file.close();
  return hierarchy;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== hierarchyFromInputFile routine <===|
// \______________________________________|








/* def main { */

/*   var hierarchy = hierarchyFromInputFile("space.txt"); */
/*   var level: BaseLevel; */

/*   for i_level in [1..hierarchy.level_numbers(hierarchy.bottom_level)] { */
    
/*     level = hierarchy.levelFromNumber(i_level); */
/*     writeln("Level number ", hierarchy.level_numbers(level), ":"); */
/*     writeln(""); */

/*     for grid in level.grids { */
/*       writeln("   x_low:   ", grid.x_low); */
/*       writeln("   x_high:  ", grid.x_high); */
/*       writeln("   i_low:   ", grid.i_low); */
/*       writeln("   i_high:  ", grid.i_high); */
/*       writeln("   n_cells: ", grid.n_cells); */
/*       writeln(""); */
/*     } */

/*     writeln(""); */
/*   } */

/* } */









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
/*      for fine_cell in cluster.fine_cells(coarse_cell) { */
          
/*        var fine_displacement: real; */
/*        var fine_value = coarse_values(coarse_cell); */

/*        for d in dimensions { */
/*          //==== Move to coarse indices ==== */
/*          fine_displacement = fine_cell(d):real / ref_ratio(d):real; */

/*          //==== Compute displacement ==== */
/*          fine_displacement -= coarse_cell(d):real; */

/*          //==== Rescale: One cell occupies 2 indices ==== */
/*          fine_displacement /= 2.0; */

/*          //==== Modify fine_value ==== */
/*          fine_value += fine_displacement * coarse_diffs(coarse_cell)(d); */
/*        } */

/*        //==== Store fine_value ==== */
/*        q_fine(fine_grid)(fine_cell) = fine_value; */

/*      } */
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
/*                              - q.value(cell - shift) */
/*                             ) / 2.0; */
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
/*      1; */


/*     //==== Limit the estimated differentials ==== */
/*     differentials(cell) *= limiter; */

/*   } */


/* } */
/* //<=== LevelInterface.minmodDiffs <=== */
/* //<=================================== */



/* def neighborhood(cell: dimension*int, k: int) { */
/*   var ranges: dimension*range(stridable=true); */
/*   for d in dimensions do ranges(d) = -2*k..2*k by 2; */
/*   var shifts = [(...ranges)]; */

/*   for shift in shifts do */
/*     yield cell + shift; */

/* } */


/* def intVec( x ...? n ) { */
/*   var int_vec: dimension*int; */
/*   for param i in 1..n do */
/*     int_vec(x(i)(2)) = x(i)(1); */
/*   return int_vec; */
/* } */
