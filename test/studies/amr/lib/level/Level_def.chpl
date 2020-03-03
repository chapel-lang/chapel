
public use Grid_def;
public use MultiDomain_def;

private use IO;

//|\""""""""""""""""""""|\
//| >    Level class    | >
//|/____________________|/

//-------------------------------------------------------------------
// A Level describes a collection of Grids, each of whichdiscretizes
// a subset of a single, larger rectangle.  These Grids share a 
// global index space, in which the lowest face of the rectangle in 
// each dimension occupies index 0.
//
// The fields x_low, x_high, n_cells, and dx all refer to the global
// discretization of the rectangle, and each child grid will have
// cells of the same size dx.
//
// The field n_ghost_cells states the width of the layer of ghost 
// cells that will be used by each Grid.
//-------------------------------------------------------------------

class Level {

  var is_complete: bool = false;
  
  const x_low, x_high: dimension*real;
  const n_cells:       dimension*int;
  const n_ghost_cells: dimension*int;
  const dx:            dimension*real;


  //==== Cell domains ====
  //--------------------------------------------------------------
  // These domains index the possible cells occupied by a grid on 
  // the level.
  //--------------------------------------------------------------
  
  const possible_cells:       domain(dimension, stridable=true);
  const possible_ghost_cells: domain(dimension, stridable=true);


  //==== Child grid info ====
  //---------------------------------------------------------------------
  // 'grids' is an associative domain, which stores a list of indices of
  // type Grid.  It initializes to empty, and new indices are added via
  // the 'add' method.
  //
  // The fields 'sibling_ghost_regions' and 'boundary' are arrays declared
  // on 'grids'; as seen here, the syntax is identical to that for
  // arithmetic domains.  Iteration syntax is the same as well.
  //---------------------------------------------------------------------

  var grids:                 domain(unmanaged Grid);
  var sibling_ghost_regions: [grids] unmanaged SiblingGhostRegion?;
  var boundary:              [grids] unmanaged MultiDomain(dimension,stridable=true)?;



  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/

  proc init(
    x_low: dimension*real,
    x_high: dimension*real,
    n_cells: dimension*int,
    n_ghost_cells: dimension*int)
  {
    
    this.x_low = x_low;
    this.x_high = x_high;
    this.n_cells = n_cells;
    this.n_ghost_cells = n_ghost_cells;
    
    dx = (x_high - x_low) / n_cells;


    //---- Possible cells ----
    var ranges: dimension*range(stridable = true);
    for d in dimensions do ranges(d) = 1 .. 2*n_cells(d)-1 by 2;
    possible_cells = ranges;


    //---- Possible ghost cells ----
    //---------------------------------------------------------------
    // The 'expand' method of an arithmetic domain extends both its
    // lower and upper bounds by the input.  In this case, the input
    // must be multiplied by 2 because a cell is 2 indices wide.
    //---------------------------------------------------------------

    possible_ghost_cells = possible_cells.expand(2*n_ghost_cells);

  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () 
  {
    for grid in grids
    {
      delete sibling_ghost_regions(grid);          
      delete boundary(grid);    
      delete grid;
    }
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|
  
  

  //|\''''''''''''''''''''''''''''|\
  //| >    snapToVertex method    | >
  //|/............................|/

  //------------------------------------------------------
  // Provided a real coordinate, returns the index of the
  // nearest vertex.  Useful when defining a grid by
  // its real bounds.
  //------------------------------------------------------

  proc snapToVertex (x: dimension*real) {

    var idx: dimension*int;

    for d in dimensions do
      idx(d) = 2 * round((x(d) - x_low(d)) / dx(d)) : int;

    return idx;

  }
  // /|''''''''''''''''''''''''''''/|
  //< |    snapToVertex method    < |
  // \|............................\|



  //|\'''''''''''''''''''''''''|\
  //| >    writeThis method    | >
  //|/.........................|/

  //-----------------------------------------------------------
  // Defines the output of the intrinsic 'write' and 'writeln' 
  // procedures, so that write(Level) will produce something
  // sensible.  Mainly for testing and debugging.
  //-----------------------------------------------------------
  
  proc writeThis(w) {
    writeln("Level bounds: ", x_low, "  ", x_high);
    writeln("Number of cells: ", n_cells);
    writeln("Number of ghost cells: ", n_ghost_cells);
    writeln("========================================");
    var grid_number: int = 0;
    for grid in grids {
      grid_number += 1;
      writeln("Grid ", grid_number);
      writeln(grid);
      writeln("");
    }
    writeln("========================================");
  }
  // /|'''''''''''''''''''''''''/|
  //< |    writeThis method    < |
  // \|.........................\|

}
// /|""""""""""""""""""""/|
//< |    Level class    < |
// \|____________________\|




//|\""""""""""""""""""""""""""""""|\
//| >    Level.addGrid methods    | >
//|/______________________________|/

//--------------------------------------------------------
// This version is based on indices, and probably best to
// use in practice, as integer arithmetic is cleaner than
// real arithmetic.
//--------------------------------------------------------

proc Level.addGrid(
  i_low_grid:  dimension*int,
  i_high_grid: dimension*int)
{

  //==== Safety check ====  
  assert(is_complete == false,
	 "Attempted to add grid to a completed level.");

  //==== Derive grid fields from index bounds and parent (this) info ====
  var x_low_grid, x_high_grid: dimension*real;
  var n_cells_grid:            dimension*int;

  for d in dimensions {
    x_low_grid(d)   = x_low(d) + i_low_grid(d)  * dx(d)/2.0;
    x_high_grid(d)  = x_low(d) + i_high_grid(d) * dx(d)/2.0;
    n_cells_grid(d) = (i_high_grid(d) - i_low_grid(d)) / 2;
  }


  //==== Create and add new grid ====
  var new_grid = new unmanaged Grid(x_low         = x_low_grid,
                          x_high        = x_high_grid,
                          i_low         = i_low_grid,
                          n_cells       = n_cells_grid,
                          n_ghost_cells = n_ghost_cells);

  grids.add(new_grid);
}


//---------------------------------------------------
// This version takes the full domain of grid cells.
//---------------------------------------------------

proc Level.addGrid (grid_cells: domain(dimension,stridable=true))
{
  addGrid(grid_cells.low-1, grid_cells.high+1);
}


//----------------------------------------------------------
// This version takes in real bounds, and snaps them to the
// level's discretization.
//----------------------------------------------------------
proc Level.addGrid(
  x_low_grid:  dimension*real,
  x_high_grid: dimension*real)
{

  var i_low_grid  = snapToVertex(x_low_grid);
  var i_high_grid = snapToVertex(x_high_grid);

  addGrid(i_low_grid, i_high_grid);

}
// /|""""""""""""""""""""""""""""""/|
//< |    Level.addGrid methods    < |
// \|______________________________\|





//|\""""""""""""""""""""""""""""""|\
//| >    Level.complete method    | >
//|/______________________________|/

//----------------------------------------------------------------
// Call after all grids have been added to the level, so now we
// can safely compute how they overlap with one another.
//----------------------------------------------------------------

proc Level.complete ()
{

  //---- Safety check ----
  assert( !is_complete, "Attempted to complete a completed level." );


  //---- Set overlap and boundary data ----

  for grid in grids 
  {
    sibling_ghost_regions(grid) = new unmanaged
      SiblingGhostRegion(_to_unmanaged(this),grid);
    
    boundary(grid) = new unmanaged MultiDomain(dimension,stridable=true);

    for D in grid.ghost_domains do boundary(grid)!.add( D );

    for overlap in sibling_ghost_regions(grid)!.overlaps do
      boundary(grid)!.subtract( overlap );
  }


  //---- Finish ----
  
  is_complete = true;

}
// /|""""""""""""""""""""""""""""""/|
//< |    Level.complete method    < |
// \|______________________________\|




//|\"""""""""""""""""""""""""""""""""|\
//| >    SiblingGhostRegion class    | >
//|/_________________________________|/

//----------------------------------------------------------------------
// Describes the overlap of a Grid with its siblings on a Level.
// Each overlap is the Grid's ghost cells, overlapped with a neighbor's
// interior cells.  However, the intersection will always be a single
// rectangle, and therefore each overlap may be described by a domain.
//
// Overlaps are computed when this object is constructed, so it should
// only be built once all grids have been added to a level.
//----------------------------------------------------------------------

class SiblingGhostRegion {

  const neighbors: domain(unmanaged Grid);
  const overlaps:  [neighbors] domain(dimension,stridable=true);
  
  
  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  
  proc init (
    level: unmanaged Level,
    grid:  unmanaged Grid)
  {
    this.complete();
    for sibling in level.grids 
    {
      if sibling != grid 
      {
        var overlap = grid.extended_cells( sibling.cells );
        if overlap.numIndices>0 
        {
          neighbors.add(sibling);
          overlaps(sibling) = overlap;
        }
      }      
    }
  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () {}

  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|
  
  
 
  //|\'''''''''''''''''''''''''|\
  //| >    these() iterator    | >
  //|/.........................|/
  
  //--------------------------------------------------------------
  // A these() iterator allows iteration over the object:
  //    for (nbr,overlap) in sibling_ghost_regions(grid) ...
  // In this case, it's designed so that iteration will provide
  // both each neighboring grid and the associated domain of
  // overlap, which is what will be needed to access the relevant
  // storage in a LevelVariable.
  //--------------------------------------------------------------
  
  iter these() {
    for nbr in neighbors do
      yield (nbr, overlaps(nbr));
  }
  // /|'''''''''''''''''''''''''/|
  //< |    these() iterator    < |
  // \|.........................\|
  
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    SiblingGhostRegion class    < |
// \|_________________________________\|





//------------------------------------
// Only I/O methods below this line
//------------------------------------
// |   |   |   |   |   |   |   |   |
//\|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/
// V   V   V   V   V   V   V   V   V



//|\"""""""""""""""""""""""""""""""""""""|\
//| >    Level.ordered_grids iterator    | >
//|/_____________________________________|/

//---------------------------------------------------------------
// This yields the grids based on their low index bound, and the
// ordering
//    (i,j) < (k,l) if i<k or (i==k and j<l).
// This is used purely for output, for which it is required to
// make results determinisic.  As such, the calculation of the
// ordering isn't done in a particularly sophisticated fashion.
//---------------------------------------------------------------

iter Level.ordered_grids {    
  var grid_list = grids;
  
  while grid_list.numIndices > 0 {
    var lowest_grid: unmanaged Grid?;
    var i_lowest = possible_ghost_cells.high;

    for grid in grid_list {
      for d in dimensions {
        if grid.i_low(d) > i_lowest(d) then
          break;
        else if grid.i_low(d) < i_lowest(d) {
          i_lowest = grid.i_low;
          lowest_grid = grid;
          break;
        }
        
      }
    }
    
    yield lowest_grid!;
    grid_list.remove(lowest_grid!);
  }
}
// /|"""""""""""""""""""""""""""""""""""""/|
//< |    Level.ordered_grids iterator    < |
// \|_____________________________________\|



//|\"""""""""""""""""""""""""""""""""""|\
//| >    levelFromInputFile routine    | >
//|/___________________________________|/

//------------------------------------------------------------------
// Creates a Level whose parameters are read from an input file,
// the name of which is provided as a string.
//------------------------------------------------------------------

proc readLevel(file_name: string){

  var input_file = open(file_name, iomode.r).reader();

  var dim_in: int;
  input_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of space.txt must equal " + "%i".format(dimension));
  input_file.readln(); // empty line

  var x_low, x_high:    dimension*real;
  var n_cells, n_ghost: dimension*int;

  input_file.readln( (...x_low) );
  input_file.readln( (...x_high) );
  input_file.readln( (...n_cells) );
  input_file.readln( (...n_ghost) );

  var level = new unmanaged Level(x_low         = x_low,
                        x_high        = x_high,
                        n_cells       = n_cells,
                        n_ghost_cells = n_ghost);

  input_file.readln();

  var n_grids: int;
  input_file.readln(n_grids);

  for i_grid in 1..n_grids {
    input_file.readln();
    input_file.readln( (...x_low) );
    input_file.readln( (...x_high) );
    level.addGrid(x_low, x_high);
  }

  level.complete();

  input_file.close();

  return level;

}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    levelFromInputFile routine    < |
// \|___________________________________\|






// proc main {
// 
//   var level = readLevel("input_level.txt");
// 
//   writeln(level);
// 
// 
// }
