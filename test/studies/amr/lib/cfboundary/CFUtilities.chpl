
use Level_def;


//|\"""""""""""""""""""""""""""""""""|\
//| >    refinementRatio routines    | >
//|/_________________________________|/
def refinementRatio(coarse_object, fine_object)
  where (coarse_object.type == Grid || coarse_object.type == Level)
     && (fine_object.type == Grid || fine_object.type == Level)
{
  var ref_ratio: dimension*int;
  
  for d in dimensions do
    ref_ratio(d) = round(coarse_object.dx(d) / fine_object.dx(d)): int;
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    refinementRatio routines    < |
// \|_________________________________\|






//|\""""""""""""""""""""""""|\
//| >    refine routines    | >
//|/________________________|/
//----------------------------------------------------------------
// Refine a single coarse cell. Output is a domain of fine cells.
//----------------------------------------------------------------
def refine(
  coarse_cell: dimension*int,
  ref_ratio:   dimension*int)
{

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


//------------------------------------------------------------------
// Refines a domain of coarse cells by refining its lower and upper
// bounds.
//------------------------------------------------------------------
def refine(
  coarse_cells: domain(dimension,stridable=true),
  ref_ratio:    dimension*int)
{
 
  //=== Index bounds for refined domain ====
  //------------------------------------------------------------------
  // Recall that "refine" returns a domain of fine cells, rather than
  // a single cell.
  //------------------------------------------------------------------
  var fine_cells_low  = refine(coarse_cells.low).low;
  var fine_cells_high = refine(coarse_cells.high).high;


  //==== Set and return new domain ====
  var ranges: dimension*range(stridable=true);

  for d in dimensions do
    ranges(d) = fine_cells_low(d) .. fine_cells_high(d) by 2;

  var fine_cells: domain(dimension,stridable=true) = ranges;
  return fine_cells;
  
}
// /|""""""""""""""""""""""""/|
//< |    refine routines    < |
// \|________________________\|




//|\"""""""""""""""""""""""""|\
//| >    coarsen routines    | >
//|/_________________________|/
//-----------------------------
// Coarsen a single fine cell.
//-----------------------------
def coarsen(
  fine_cell: dimension*int,
  ref_ratio: dimension*int)
{

  var coarse_cell: dimension*int;

  for d in dimensions {
    //==== Map to nearest, lower coarse index ====
    coarse_cell(d) = fine_cell(d) / ref_ratio(d);

    //==== Move index to cell center, if needed ====
    if coarse_cell(d) % 2 == 0 then coarse_cell(d) += 1;
  }

  return coarse_cell;

}

//----------------------------------------------------------------------
// Coarsens a domain of cells by coarsening the lower and upper bounds.
//----------------------------------------------------------------------
def coarsen(
  fine_cells: domain(dimension,stridable=true),
  ref_ratio:  dimension*int)
{
 
  //=== Index bounds for coarsened domain ====
  var low_coarse  = coarsen(fine_cells.low);
  var high_coarse = coarsen(fine_cells.high);
  

  //==== Set and return new domain ====
  var ranges: dimension*range(stridable=true);
  for d in dimensions do
    ranges(d) = low_coarse(d) .. high_coarse(d) by 2;


  var coarse_cells: subdomain(coarse_level.cells) = ranges;
  return coarse_cells;
  
}
// /|"""""""""""""""""""""""""/|
//< |    coarsen routines    < |
// \|_________________________\|












//|\"""""""""""""""""""""""""""""""""""""|\
//| >    extrapolateGhostData methods    | >
//|/_____________________________________|/
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
// /|"""""""""""""""""""""""""""""""""""""/|
//< |    extrapolateGhostData methods    < |
// \|_____________________________________\|