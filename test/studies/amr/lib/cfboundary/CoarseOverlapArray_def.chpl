
use CFBoundary_def;


//|\""""""""""""""""""""""""""""""""|\
//| >    CoarseOverlapArray class    | >
//|/________________________________|/
class CoarseOverlapArray {
  
  const cf_boundary: CFBoundary;
  
  var multiarrays: [cf_boundary.fine_level.grids] MultiArray(dimension,true,real);
  
  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    for multiarray in multiarrays do delete multiarray;
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|
  
  
  //|\''''''''''''''''''''''''''''|\
  //| >    initialize() method    | >
  //|/............................|/
  //-------------------------------------------------
  // Intended constructor signature:
  //     CoarseOverlapArray(cf_boundary: CFBoundary)
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
  //-------------------------------------------------------
  // Return the MultiArray corresponding to a single Grid.
  //-------------------------------------------------------
  def this(grid: Grid) {
    return multiarrays(grid);
  }
  // /|''''''''''''''''''''/|
  //< |    this method    < |
  // \|....................\|
  
}
// /|""""""""""""""""""""""""""""""""/|
//< |    CoarseOverlapArray class    < |
// \|________________________________\|




