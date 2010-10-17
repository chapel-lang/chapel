
use Level_def;
use CFUtilities;



//|\""""""""""""""""""""""""""""|\
//| >    CoarseOverlap class    | >
//|/____________________________|/
//------------------------------------------------------------------
// Describes the overlap of a Grid with a coarser level.  This
// is the intersection of the Grid's ghost cells with the interior
// of each coarse grid.  As such, each overlap is typically a union
// of rectangles, and must be described by a MultiDomain.
//------------------------------------------------------------------
class CoarseOverlap {  

  const full_multidomain: MultiDomain(dimension,stridable=true);

  const neighbors:    domain(Grid);
  const multidomains: [neighbors] MultiDomain(dimension,stridable=true);
  const subranges:    [neighbors] range;
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def CoarseOverlap(
    coarse_level: Level,
    fine_level:   Level,
    grid:         Grid)
  {
    //==== Calculate refinement ratio ====
    const ref_ratio = refinementRatio(coarse_level, fine_level);
    
    //==== Check each grid on coarse_level for overlap ====
    for coarse_grid in coarse_level.grids {
      
      //==== Determine overlap ====
      var overlap_multidomain = new MultiDomain(dimension,stridable=true);
      overlap_multidomain.add(fine_level.boundary(grid));
      overlap_multidomain.intersect( refine(coarse_grid.cells,ref_ratio) );

      //==== Update data structures ====
      if overlap_multidomain.domains.numElements > 0 {
        neighbors.add(coarse_grid);
        multidomains(coarse_grid) = overlap_multidomain;
      }
      else
        delete overlap_multidomain;
    }
    
    //==== Build full MultiDomain of overlaps ====
    full_multidomain = new MultiDomain(dimension,stridable=true);
    for coarse_nbr in neighbors {
      var subrange_low = full_multidomain.indices.high+1;
      full_multidomain.add(multidomains(coarse_nbr));
      subranges(coarse_nbr) = subrange_low .. full_multidomain.indices.high;
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
      yield (nbr, multidomains(nbr), subranges(nbr));
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
  var domains:   [neighbors] domain(dimension,stridable=true);
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def FineOverlap(
    coarse_level: Level,
    fine_level:   Level,
    coarse_grid:  Grid)
  {
    //==== Calculate refinement ratio ====
    const ref_ratio = refinementRatio(coarse_level, fine_level);
    
    //==== Check each grid on fine_level for overlap ====
    for fine_grid in fine_level.grids {
      var overlap = coarse_grid.cells( coarsen(fine_grid.cells, ref_ratio) );
      
      if overlap.numIndices>0 {
        neighbors.add(fine_grid);
        domains(fine_grid) = overlap;
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
      yield (nbr, domains(nbr));
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
      coarse_overlaps(fine_grid) = new CoarseOverlap(coarse_level, fine_level, fine_grid);
      
    for coarse_grid in coarse_level.grids do
      fine_overlaps(coarse_grid) = new FineOverlap(coarse_level, fine_level, coarse_grid);
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    initialize() method    < |
  // \|............................\|
  
}
// /|"""""""""""""""""""""""""/|
//< |    CFBoundary class    < |
// \|_________________________\|