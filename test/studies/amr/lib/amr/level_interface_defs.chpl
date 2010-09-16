use level_base_defs;
use level_array_defs;


//|""""""""""""""""""""""""""""\
//|===> LevelInterface class ===>
//|____________________________/
class LevelInterface {

  const coarse_level: Level;
  const fine_level:   Level;
  const ref_ratio:    dimension*int;

  var coarse_neighbors: [fine_level.grids] domain(Grid);
  var fine_neighbors:   [coarse_level.grids] domain(Grid);

}
// /""""""""""""""""""""""""""""|
//<=== LevelInterface class <===|
// \____________________________|






//|""""""""""""""""""""""""""""""""""""""""""\
//|===> LevelInterface.setNeighbors method ===>
//|__________________________________________/
def LevelInterface.setNeighbors() {

  for coarse_grid in coarse_level.grids {
    for fine_grid in fine_level.grids {

      var intersection: domain(dimension, stridabe=true);

      intersection = coarse_grid.cells( coarsen(fine_grid.cells) );
      if intersection.numIndices > 0 then
	fine_neighbors(coarse_grid).add(fine_grid);

      intersection = fine_grid.ext_cells( refine(coarse_grid.cells) );
      if intersection.numIndices > 0 && intersection != fine_grid.cells then
	coarse_neighbors(fine_grid).add(coarse_grid);

    }
  }

}
// /""""""""""""""""""""""""""""""""""""""""""|
//<=== LevelInterface.setNeighbors method <===|
// \__________________________________________|






//|""""""""""""""""""""""""""""""""""""""\
//|===> LevelInterface.complete method ===>
//|______________________________________/
//---------------------------------------------------------------
// This looks like a pointless method as is, but I want to leave
// the door open to do other operations here.  If not, I want to
// keep the name "complete", and I'll just move the setNeighbors
// code to this method.
//---------------------------------------------------------------
def LevelInterface.complete() {

  setNeighbors();

}
// /""""""""""""""""""""""""""""""""""""""|
//<=== LevelInterface.complete method <===|
// \______________________________________|







//""""""""""""""""""""""""""""""""""""\
//===> LevelInterface.refine method ===>
//____________________________________/
//-----------------------------------------------------------------
// Refines a domain of cells using the LevelInterface's ref_ratio.
//-----------------------------------------------------------------
def LevelInterface.refine(coarse_cells: subdomain(coarse_level.cells)) {
 
  //=== Index bounds for refined domain ====
  var fine_cells_low  = coarse_cells.low;
  var fine_cells_high = coarse_cells.high;

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

//---------------------------------------------------------------------
// Refine a single coarse cell rather than a domain.  No safety check.
//---------------------------------------------------------------------
def LevelInterface.refine(coarse_cell: dimension*int) {

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
// /"""""""""""""""""""""""""""""""""""|
//<    LevelInterface.refine method    |
// \___________________________________|






//|""""""""""""""""""""""""""""""""""""\
//|    LevelInterface.coarsen method    >
//|____________________________________/
//------------------------------------------------------------------
// Coarsens a domain of cells using the LevelInterface's ref_ratio.
//------------------------------------------------------------------
def LevelInterface.coarsen(fine_cells: subdomain(fine_level.cells)) {
 
  writeln("### Coarsening domain ", fine_cells);

  //=== Index bounds for coarsened domain ====
  var low_coarse  = fine_cells.low;
  var high_coarse = fine_cells.high;

  for d in dimensions do {
    //==== Move cell centers to vertices ====
    low_coarse(d)  -= 1;
    high_coarse(d) += 1;

    //==== Make sure coarsening is valid ====
    assert(low_coarse(d)  % ref_ratio(d) == 0);
    assert(high_coarse(d) % ref_ratio(d) == 0);

    //==== Coarsen ====
    low_coarse(d)  /= ref_ratio(d);
    high_coarse(d) /= ref_ratio(d);

    //==== Move vertices back to cell centers ====
    low_coarse(d)  += 1;
    high_coarse(d) -= 1;
  }


  //==== Set and return new domain ====
  var ranges: dimension*range(stridable=true);

  for d in dimensions do
    ranges(d) = low_coarse(d) .. high_coarse(d) by 2;

  var coarse_cells: subdomain(coarse_level.cells) = ranges;

  writeln("### Result is ", coarse_cells);
  return coarse_cells;
  
}


//--------------------------------------------------
// Coarsen a single fine cell rather than a domain.
//--------------------------------------------------
def LevelInterface.coarsen(fine_cell: dimension*int) {

  var coarse_cell: dimension*int;

  //-------------------------------------------------------------
  // Dividing by ref_ratio (and throwing away remainder) maps to
  // lower vertex of the coarse cell.  Adding 1 shifts index to
  // the cell center.
  //-------------------------------------------------------------
  for d in dimensions {
    coarse_cell(d) = fine_cell(d) / ref_ratio(d);
    if coarse_cell(d) % 2 == 0 then
      coarse_cell(d) += 1;
  }

  return coarse_cell;

}
// /"""""""""""""""""""""""""""""""""""""|
//<=== LevelInterface.coarsen method <===|
// \_____________________________________|