
use CFBoundary_def;


//|\""""""""""""""""""""""""""""""""|\
//| >    FineBoundaryArray class    | >
//|/________________________________|/
class FineBoundaryArray {
  
  const cf_boundary: CFBoundary;
  
  var array_sets: [cf_boundary.fine_level.grids] ArraySet(dimension,true,real);
  
  
  //|\''''''''''''''''''''''''''''|\
  //| >    initialize() method    | >
  //|/............................|/
  //-------------------------------------------------
  // Intended constructor signature:
  //     FineBoundaryArray(cf_boundary: CFBoundary)
  //-------------------------------------------------
  def initialize() {
    for grid in cf_boundary.fine_level.grids {
      var overlap_domains = new DomainSet(dimension,stridable=true);
      
      for (coarse_nbr, overlap_set) in CFBoundary.fine_overlap_data(fine_grid) {
        overlap_domains.add(overlap_set);
      }

      array_sets(grid).allocate( overlap_domains );
      
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
    return array_sets(grid);
  }
  // /|''''''''''''''''''''/|
  //< |    this method    < |
  // \|....................\|
  
}
// /|""""""""""""""""""""""""""""""""/|
//< |    FineBoundaryArray class    < |
// \|________________________________\|




