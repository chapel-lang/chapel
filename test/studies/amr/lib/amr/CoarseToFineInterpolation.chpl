use CFLevelInterface_def;
use LevelSolution_def;
use LevelGhostArraySet_def;





//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    Coarse-to-fine spatial interpolation    | >
//|/_____________________________________________|/

//|\''''''''''''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    LevelGhostArraySet.interpolateFromCoarse_Linear    | >
//|/........................................................|/
def LevelGhostArraySet.interpolateFromCoarse_Linear(
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{

  //==== Prepare ghost data of q_coarse ====
  q_coarse.extrapolateGhostData();
  q_coarse.fillSharedGhosts();

  //==== Interpolate on each fine grid ====
  for grid in level.grids do
    ghost_array_sets(grid).interpolateFromCoarse_Linear(q_coarse, interface);

}
// /|''''''''''''''''''''''''''''''''''''''''''''''''''''''''/|
//< |    LevelGhostArraySet.interpolateFromCoarse_Linear    < |
// \|........................................................\|



//|\''''''''''''''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    GhostArraySet.interpolateFromCoarse_Linear method    | >
//|/..........................................................|/
def GhostArraySet.interpolateFromCoarse_Linear(
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{
  var overlap: domain(dimension, stridable=true);

  for coarse_grid in interface.coarse_neighbors(this.grid) {

    for ghost_array in array_set {
      var ghost_domain = ghost_array.dom;
      overlap = ghost_domain( interface.refine(coarse_grid.cells) );
      
      if overlap.numIndices > 0 then
	ghost_array.value(overlap) =
	  q_coarse(coarse_grid).interpolateToFine_Linear(overlap, interface);
    } // end for ghost_array in array_set 

  } // end for coarse_grid in interface.coarse_neighbors(this.grid)

}
// /|''''''''''''''''''''''''''''''''''''''''''''''''''''''''''/|
//< |    GhostArraySet.interpolateFromCoarse_Linear method    < |
// \|..........................................................\|



//|\''''''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    GridArray.interpolateToFine_Linear method    | >
//|/..................................................|/
def GridArray.interpolateToFine_Linear(
  fine_cells: domain(dimension, stridable=true),
  interface:  CFLevelInterface)
{
  
  var coarse_cells  = grid.cells( interface.coarsen(fine_cells) );
  
  var coarse_values = value(coarse_cells);
  var coarse_diffs: [coarse_cells] [dimensions] real;


  //===> Form interpolant data (values and differentials ===>
  forall cell in coarse_cells {
    var diff_mag, diff_sign, diff_low, diff_high, diff_cen: real;
    var shift: dimension*int;

    for d in dimensions {
      shift *= 0;
      shift(d) = 2;

      diff_low  = value(cell) - value(cell-shift);
      diff_high = value(cell+shift) - value(cell);
      diff_cen  = (diff_high - diff_low) / 2.0;

      if diff_low*diff_high > 0 {
        diff_sign = diff_low / abs(diff_low);
        diff_mag = min( abs(diff_low), abs(diff_high), abs(diff_cen) );
        coarse_diffs(cell)(d) = diff_sign * diff_mag;
      }
      else
        coarse_diffs(cell)(d) = 0.0;
    }
  }
  //<=== Form interpolant data (values and differentials) <===


  //===> Evaluate interpolant on fine cells ===>
  var fine_values: [fine_cells] real;

  forall fine_cell in fine_cells {
    var coarse_cell = interface.coarsen(fine_cell);
    fine_values(fine_cell) = coarse_values(coarse_cell);

    var fine_displacement: real = 0.0;

    for d in dimensions {
      //==== Move to coarse indices ====
      fine_displacement = fine_cell(d):real / interface.ref_ratio(d):real;

      //==== Compute displacement ====
      fine_displacement -= coarse_cell(d):real;

      //==== Rescale: One cell occupies 2 indices ====
      fine_displacement /= 2.0;

      //==== Modify fine_value ====
      fine_values(fine_cell) += fine_displacement * coarse_diffs(coarse_cell)(d);
    }      
    
  }
  //<=== Evaluate interpolant on fine cells <===


  return fine_values;

}
// /|''''''''''''''''''''''''''''''''''''''''''''''''''/|
//< |    GridArray.interpolateToFine_Linear method    < |
// \|..................................................\|



//|\'''''''''''''''''''''''''''''''''''''|\
//| >    extrapolateGhostData methods    | >
//|/.....................................|/
//-----------------------------------------------------------------
// Fills the first layer of ghost cells with linearly extrapolated
// data from the interior.
//-----------------------------------------------------------------
def LevelArray.extrapolateGhostData() {
  
  for grid_array in grid_arrays do
    grid_array.extrapolateGhostData();

}


def GridArray.extrapolateGhostData() {

  for ghost_domain in grid.ghost_domain_set {
    var loc = grid.relativeLocation(ghost_domain);
    var shift = -1*loc;

    forall cell in ghost_domain do
      value(cell) = 2.0*value(cell+shift) - value(cell+2*shift);
  }

}
// /|'''''''''''''''''''''''''''''''''''''/|
//< |    extrapolateGhostData methods    < |
// \|.....................................\|

// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    Coarse-to-fine spatial interpolation    < |
// \|_____________________________________________\|
