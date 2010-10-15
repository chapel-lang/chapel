
use Level_def;
use CFUtilities;



//|\""""""""""""""""""""""""""""|\
//| >    CoarseOverlap class    | >
//|/____________________________|/
//------------------------------------------------------------------
// Describes the overlap of a Grid with a coarser level.  This
// is the intersection of the Grid's ghost cells with the interior
// of each coarse grid.  As such, each overlap is typically a union
// of rectangles, and must be described by a DomainSet.
//------------------------------------------------------------------
class CoarseOverlap {  
  var neighbors:           domain(Grid);
  var overlap_domain_sets: [neighbors] DomainSet(dimension,stridable=true);
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def CoarseOverlap(
    grid:         Grid,
    coarse_level: Level)
  {
    //==== Calculate refinement ratio ====
    var ref_ratio: dimension*int;
    for d in dimensions do
      ref_ratio(d) = round(coarse_level.dx(d) / grid.dx(d)): int;
    
    //==== Check each grid on coarse_level for overlap ====
    for coarse_grid in coarse_level.grids {
      var overlap = grid.ext_cells( refine(coarse_grid.cells) ) - grid.cells;
      if overlap.indices.numIndices>0 {
        neighbors.add(coarse_grid);
        overlap_domain_sets(coarse_grid) = overlap;
      }
      else
        delete overlap;
    }
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|
  
  
  //|\'''''''''''''''''''''''''|\
  //| >    these() iterator    | >
  //|/.........................|/
  def these() {
    for nbr in neighbors do
      yield (nbr, overlap_domain_sets(nbr));
  }
  // /|'''''''''''''''''''''''''/|
  //< |    these() iterator    < |
  // \|.........................\|
  

}
// /|""""""""""""""""""""""""""""/|
//< |    CoarseOverlap class    < |
// \|____________________________\|



//|\""""""""""""""""""""""""""|\
//| >    FineOverlap class    | >
//|/__________________________|/
//---------------------------------------------------------------------
// Stores the overlap of a Grid with a finer level, which in this case
// is the intersection of Grid.cells with each fine_grid.cells.  Each
// intersection is a rectangle, and thus each overlap may be stored as
// a domain.
//---------------------------------------------------------------------
class FineOverlap {
  var neighbors: domain(Grid);
  var overlap_domains: [neighbors] domain(dimension,stridable=true);
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def FineOverlap(
    grid:       Grid,
    fine_level: Level)
  {
    //==== Calculate refinement ratio ====
    var ref_ratio: dimension*int;
    for d in dimensions do
      ref_ratio(d) = round(grid.dx(d) / fine_level.dx(d)): int;
    
    //==== Check each grid on fine_level for overlap ====
    for fine_grid in fine_level.grids {
      var overlap = grid.cells( coarsen(fine_grid.cells, ref_ratio) );
      if overlap.numIndices>0 {
        neighbors.add(fine_grid);
        overlap_domains(fine_grid) = overlap;
      }
    }

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|
  
  
  //|\'''''''''''''''''''''''''|\
  //| >    these() iterator    | >
  //|/.........................|/
  def these() {
    for nbr in neighbors do
      yield (nbr, overlap_domains(nbr));
  }
  // /|'''''''''''''''''''''''''/|
  //< |    these() iterator    < |
  // \|.........................\|
  
}
// /|""""""""""""""""""""""""""/|
//< |    FineOverlap class    < |
// \|__________________________\|




//|\"""""""""""""""""""""""""|\
//| >    CFBoundary class    | >
//|/_________________________|/
class CFBoundary {
  
  const coarse_level: Level;
  const fine_level:   Level;
  const ref_ratio:    dimension*int;

  var coarse_overlaps: [fine_level.grids] CoarseOverlap;
  var fine_overlaps:   [coarse_level.grids] FineOverlap;

  //|\''''''''''''''''''''''''''''|\
  //| >    initialize() method    | >
  //|/............................|/
  //----------------------------------------------------------
  // Intended signature is 
  //     CFBoundary(coarse_level: Level, fine_level: Level)
  //----------------------------------------------------------
  def initialize() {
    ref_ratio = refinementRatio(coarse_level, fine_level);
    
    for fine_grid in fine_level.grids do
      coarse_overlaps(fine_grid) = new CoarseOverlap(fine_grid, coarse_level);
      
    for coarse_grid in coarse_level.grids do
      fine_overlaps(coarse_grid) = new FineOverlap(coarse_grid, fine_level)
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    initialize() method    < |
  // \|............................\|
  
}
// /|"""""""""""""""""""""""""/|
//< |    CFBoundary class    < |
// \|_________________________\|