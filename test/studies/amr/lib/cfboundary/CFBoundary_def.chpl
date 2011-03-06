
use Level_def;
use CFUtilities;



//|\"""""""""""""""""""""""""""""""""""""|\
//| >    CoarseningTransferZone class    | >
//|/_____________________________________|/

//---------------------------------------------------------------------
// Stores the region on which a coarse grid will receive data from a
// finer level.  This consists of grid.cells, intersected with the
// coarsening of fine_grid.cells.
//---------------------------------------------------------------------

class CoarseningTransferZone {
  const neighbors: domain(Grid);
  const domains:   [neighbors] domain(dimension,stridable=true);
  
  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    neighbors.clear();
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def CoarseningTransferZone(
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
// /|"""""""""""""""""""""""""""""""""""""/|
//< |    CoarseningTransferZone class    < |
// \|_____________________________________\|






//|\"""""""""""""""""""""""""""""""""""|\
//| >    RefiningTransferZone class    | >
//|/___________________________________|/

//------------------------------------------------------------------
// Describes the overlap of a Grid with a coarser level.  This
// is the intersection of the Grid's ghost cells with the interior
// of each coarse grid.  As such, each overlap is typically a union
// of rectangles, and must be described by a MultiDomain.
//------------------------------------------------------------------

class RefiningTransferZone {  

  const full_multidomain: MultiDomain(dimension,stridable=true);

  const neighbors:    domain(Grid);
  const multidomains: [neighbors] MultiDomain(dimension,stridable=true);
  const subranges:    [neighbors] range;
  
  
  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    delete full_multidomain;
    for neighbor in neighbors do delete multidomains(neighbor);
    neighbors.clear();
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|
  
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def RefiningTransferZone(
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
      var subrange_low = full_multidomain.subindices.high+1;
      full_multidomain.add(multidomains(coarse_nbr));
      subranges(coarse_nbr) = subrange_low .. full_multidomain.subindices.high;
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
//< |    RefiningTransferZone class    < |
// \|____________________________\|










//|\"""""""""""""""""""""""""|\
//| >    CFBoundary class    | >
//|/_________________________|/
class CFBoundary {
  
  const coarse_level: Level;
  const fine_level:   Level;
  const ref_ratio:    dimension*int;

  var coarse_overlaps: [fine_level.grids] RefiningTransferZone;
  var fine_overlaps:   [coarse_level.grids] CoarseningTransferZone;

  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    for grid in fine_level.grids {
      coarse_overlaps(grid).clear();
      delete coarse_overlaps(grid);
    }
    
    for grid in coarse_level.grids {
      fine_overlaps(grid).clear();
      delete fine_overlaps(grid);
    }
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|


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
      coarse_overlaps(fine_grid) = new RefiningTransferZone(coarse_level, fine_level, fine_grid);
      
    for coarse_grid in coarse_level.grids do
      fine_overlaps(coarse_grid) = new CoarseningTransferZone(coarse_level, fine_level, coarse_grid);
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    initialize() method    < |
  // \|............................\|
  
}
// /|"""""""""""""""""""""""""/|
//< |    CFBoundary class    < |
// \|_________________________\|