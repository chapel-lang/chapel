
use CFBoundary_def;


//|\""""""""""""""""""""""""""""""""|\
//| >    FineBoundaryArray class    | >
//|/________________________________|/
class FineBoundaryArray {
  
  const cf_boundary: CFBoundary;
  
  var multiarrays: [cf_boundary.fine_level.grids] MultiArray(dimension,true,real);
  
  
  //|\''''''''''''''''''''''''''''|\
  //| >    initialize() method    | >
  //|/............................|/
  //-------------------------------------------------
  // Intended constructor signature:
  //     FineBoundaryArray(cf_boundary: CFBoundary)
  //-------------------------------------------------
  def initialize() {
    for grid in cf_boundary.fine_level.grids {
      multiarrays(grid) = new MultiArray(dimension,true,real);
      multiarrays(grid).allocate( cf_boundary.coarse_overlaps(grid).full_multidomain );
    }
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    initialize() method    < |
  // \|............................\|
  
  
  //|\''''''''''''''''''''|\
  //| >    this method    | >
  //|/....................|/
  //-----------------------------------------------------
  // Return the ArraySet corresponding to a single Grid.
  //-----------------------------------------------------
  def this(grid: Grid) {
    return multiarrays(grid);
  }
  // /|''''''''''''''''''''/|
  //< |    this method    < |
  // \|....................\|
  
}
// /|""""""""""""""""""""""""""""""""/|
//< |    FineBoundaryArray class    < |
// \|________________________________\|




