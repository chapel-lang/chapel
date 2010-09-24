use CFLevelInterface_def;
use LevelArray_def;



//|\""""""""""""""""""""""""""""""""""|\
//| >    Fine-to-coarse correction    | >
//|/__________________________________|/

//|\''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    LevelArray.interpolateFromFine_Linear    | >
//|/..............................................|/
def LevelArray.interpolateFromFine_Linear(
  q_fine:    LevelArray,
  interface: CFLevelInterface)
{

  for grid_array in grid_arrays do
    grid_array.interpolateFromFine_Linear(q_fine, interface);

}
// /|''''''''''''''''''''''''''''''''''''''''''''''/|
//< |    LevelArray.interpolateFromFine_Linear    < |
// \|..............................................\|



//|\'''''''''''''''''''''''''''''''''''''''''''''|\
//| >    GridArray.interpolateFromFine_Linear    | >
//|/.............................................|/
def GridArray.interpolateFromFine_Linear(
  q:         LevelArray,
  interface: CFLevelInterface)
{

  var overlap: domain(dimension, stridable=true);

  for fine_grid in interface.fine_neighbors(this.grid) {
    overlap = this.grid.cells( interface.coarsen(fine_grid.cells) );
    // Transfer data between processors here
    value(overlap) = q(fine_grid).interpolateToCoarse_Linear(overlap, interface);
  }

}
// /|'''''''''''''''''''''''''''''''''''''''''''''/|
//< |    GridArray.interpolateFromFine_Linear    < |
// \|.............................................\|



//|\'''''''''''''''''''''''''''''''''''''''''''''|\
//| >    GridArray.interpolateToCoarse_Linear    | >
//|/.............................................|/
def GridArray.interpolateToCoarse_Linear(
  coarse_cells: domain(dimension, stridable=true),
  interface:    CFLevelInterface)
{

  //==== Volume fraction is 1/product(ref_ratio) ====
  // Could store this in the interface...
  var volume_fraction: real = 1.0;
  for d in dimensions do
    volume_fraction /= interface.ref_ratio(d):real;

  
  //==== Compute coarse averages ====
  var coarse_values: [coarse_cells] real;

  forall coarse_cell in coarse_cells {
    var fine_cells = interface.refine(coarse_cell);
    for fine_cell in fine_cells do
      coarse_values(coarse_cell) += value(fine_cell);
    coarse_values(coarse_cell) *= volume_fraction;
  }


  return coarse_values;

}
// /|'''''''''''''''''''''''''''''''''''''''''''''/|
//< |    GridArray.interpolateToCoarse_Linear    < |
// \|.............................................\|

// /|"""""""""""""""""""""""""""""""""/|
//< |    Fine-to-coarse correction   < |
// \|_________________________________\|