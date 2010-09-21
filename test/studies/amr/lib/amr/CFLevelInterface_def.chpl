use Level_def;
use LevelArray_def;


//|""""""""""""""""""""""""""""""\
//|===> CFLevelInterface class ===>
//|______________________________/
class CFLevelInterface {

  const coarse_level: Level;
  const fine_level:   Level;
  const ref_ratio:    dimension*int;

  var coarse_neighbors: [fine_level.grids] domain(Grid);
  var fine_neighbors:   [coarse_level.grids] domain(Grid);

  def complete() {
    setNeighbors();
  }

}
// /""""""""""""""""""""""""""""""/
//<=== CFLevelInterface class <==<
// \______________________________\






//|""""""""""""""""""""""""""""""""""""""""""""\
//|===> CFLevelInterface.setNeighbors method ===>
//|____________________________________________/
def CFLevelInterface.setNeighbors() {

  for coarse_grid in coarse_level.grids {
    for fine_grid in fine_level.grids {

      var intersection: domain(dimension, stridable=true);

      intersection = coarse_grid.cells( coarsen(fine_grid.cells) );
      if intersection.numIndices > 0 then
        fine_neighbors(coarse_grid).add(fine_grid);

      // Technically, this is wrong if the coarse grid covers the
      // exact same region as the fine grid...but that's pathological,
      // and should just cause some empty iterations later on.
      intersection = fine_grid.ext_cells( refine(coarse_grid.cells) );
      if intersection.numIndices > 0 then
        coarse_neighbors(fine_grid).add(coarse_grid);

    }
  }

}
// /""""""""""""""""""""""""""""""""""""""""""""/
//<=== CFLevelInterface.setNeighbors method <==<
// \____________________________________________\







//""""""""""""""""""""""""""""""""""""""\
//===> CFLevelInterface.refine method ===>
//______________________________________/
//-----------------------------------------------------------------
// Refines a domain of cells using the CFLevelInterface's ref_ratio.
//-----------------------------------------------------------------
def CFLevelInterface.refine(coarse_cells: subdomain(coarse_level.cells)) {
 
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
def CFLevelInterface.refine(coarse_cell: dimension*int) {

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
// /""""""""""""""""""""""""""""""""""""""/
//<=== CFLevelInterface.refine method <==<
// \______________________________________\






//|"""""""""""""""""""""""""""""""""""""""\
//|===> CFLevelInterface.coarsen method ===>
//|_______________________________________/
//------------------------------------------------------------------
// Coarsens a domain of cells using the CFLevelInterface's ref_ratio.
//------------------------------------------------------------------
def CFLevelInterface.coarsen(fine_cells: subdomain(fine_level.cells)) {
 
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


//--------------------------------------------------
// Coarsen a single fine cell rather than a domain.
//--------------------------------------------------
def CFLevelInterface.coarsen(fine_cell: dimension*int) {

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
// /"""""""""""""""""""""""""""""""""""""""/
//<=== CFLevelInterface.coarsen method <==<
// \_______________________________________\