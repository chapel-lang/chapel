use level_base_defs;


//===> AMRGrid derived class ===>
//==============================>
class AMRGrid: LevelGrid {

  const hierarchy:    AMRHierarchy;
  const parent_level: AMRLevel;

  var low_boundary_ghosts: [dimensions] subdomain(ext_cells),
    high_boundary_ghosts:  [dimensions] subdomain(ext_cells);

  var coarse_neighbors: domain(AMRGrid);
  var coarse_ghosts:    [coarse_neighbors] sparse subdomain(ext_cells);

  var fine_neighbors:  domain(AMRGrid);
  

  //===> partitionBoundaryGhosts method ===>
  //=======================================>
  //------------------------------------------------------------------
  // Sets up boundary ghost domains, for use with the boundary_ghosts
  // iterator.
  //------------------------------------------------------------------
  def partitionBoundaryGhosts() {

    var comparison_domain: domain(dimension);
    var range_tuple: dimension*range(stridable=true);
    
    for orientation in dimensions {

      //===> Set up off-dimensions ===>
      for d in [1..orientation-1] do
        range_tuple(d) = parent_level.i_low(d) + 1
                         .. parent_level.i_high(d) - 1
                         by 2;

      for d in [orientation+1 .. dimension] do
        range_tuple(d) = parent_level.i_low(d) + 1 - 2*n_ghost_cells(d)
                         .. parent_level.i_high(d) - 1 + 2*n_ghost_cells(d)
                         by 2;
      //<=== Set up off-dimensions <===


      //===> Low end ===>
      range_tuple(orientation) = parent_level.i_low(orientation) - 2*n_ghost_cells(d) + 1
                                 .. parent_level.i_low(orientation) - 1
                                 by 2;

      comparison_domain = range_tuple;

      low_boundary_ghosts(d) = intersectDomains(low_ghost_cells(d), comparison_domain);
      //<=== Low end <===


      //===> High end ===>
      range_tuple(orientation) = parent_level.i_high(orientation) + 1
                                 .. parent_level.i_high(orientation) + 2*n_ghost_cells(orientation) - 1
                                 by 2;

      comparison_domain = range_tuple;

      high_boundary_ghosts(d) = intersectDomains(high_ghost_cells(d), comparison_domain);
      //<=== High end <===

    }

  }
  //<=== partitionBoundaryGhosts method <===
  //<=======================================




  //===> partitionCoarseGhosts method ===>
  //=====================================>
  def partitionCoarseGhosts() {
    
    for coarser_grid in parent_level.coarser_level.child_grids {

      var ref_ratio         = hierarchy.ref_ratios(parent_level.ref_index);
      var coarse_projection = refine(coarser_grid.cells, ref_ratio);
      var intersection      = intersectDomains(ext_cells, coarse_projection);

      //===> Check for coarse neighbors ===>
      if intersection.numIndices > 0 {
        var temp_list: sparse subdomain(ext_cells);

        //==== Add all ghost cells in the coarse projection to the temp list ====
        for cell in ghost_cells {
          if intersection.member(cell) then
            temp_list.add(cell);
        }
      
        //==== Remove any level ghosts from the temp list ====
        for nbr in level_neighbors {
          for cell in level_ghosts(nbr) {
            if temp_list.member(cell) then
              temp_list.remove(cell);
          }
        }

        //==== If the temp list remains nonempty, add it to the coarse ghosts ====
        if temp_list.numIndices > 0 {
          coarse_neighbors.add(coarser_grid);
          coarse_ghosts(coarser_grid) = temp_list;
        }

      }
      //<=== Check for coarse neighbors <===

    }

  }
  //<=== partitionCoarseGhosts method <===
  //<=====================================




  //===> boundary_ghosts iterator ===>
  //=================================>
  def boundary_ghosts {
    for d in dimensions {
      for cell in low_boundary_ghosts(d) do
        yield cell;
      for cell in high_boundary_ghosts(d) do
        yield cell;
    }
  }
  //<=== boundary_ghosts iterator <===
  //<=================================


}
//<=== AMRGrid derived class <===
//<==============================







//===> AMRLevel derived class ===>
//===============================>
class AMRLevel: BaseLevel {

  const hierarchy: AMRHierarchy;
  const ref_index: int;

  const coarser_level = hierarchy.child_levels(ref_index);

  var child_grids: domain(AMRGrid);


}
//<=== AMRLevel derived class <===
//<===============================



//===> AMRLevel.addGrid method ===>
//================================>
//---------------------------------------------------------
// These shadow the methods for BaseLevel, modified to add
// AMRGrids rather than LevelGrids.
//---------------------------------------------------------
def AMRLevel.addGrid(
  i_low:  dimension*int,
  i_high: dimension*int)
{
  
  assert(fixed == false);

  var new_grid = new AMRGrid(hierarchy    = hierarchy,
                             parent_level = this,
                             i_low        = i_low,
                             i_high       = i_high);

  child_grids.add(new_grid);
}


def AMRLevel.addGrid(
  x_low_grid:  dimension*real,
  x_high_grid: dimension*real)
{

  //==== Make sure the level isn't fixed ====
  assert(fixed == false);


  //==== Add grid ====
  var new_grid = new AMRGrid(hierarchy    = hierarchy,
                             parent_level = this,
                             i_low        = snapToVertex(x_low_grid),
                             i_high       = snapToVertex(x_high_grid));

  child_grids.add(new_grid);
}
//<=== AMRLevel.addGrid method <===
//<================================


//===> AMRLevel.fix method ===>
//============================>
//----------------------------------------------------------------
// This method is meant to be called after all grids are added to
// the level.  Neighbor data is set on each grid, and other post-
// processing can be added here as needed.
//----------------------------------------------------------------
def BaseLevel.fix() {

  assert(fixed == false);
  fixed = true;

  coforall grid in child_grids {
    grid.partitionLevelGhosts();
    grid.partitionBoundaryGhosts();
    grid.partitionCoarseGhosts();
  }

}
//<=== AMRLevel.fix method <===
//<============================









//===> AMRHierarchy class ===>
//===========================>
class AMRHierarchy {

  const max_ref_index;

  var ref_indices: [1..2];

  var child_levels:  [ref_indices] AMRLevel;
  var ref_ratios:    [ref_indices] dimension*int;
  
}
//<=== AMRHierarchy class <===
//<===========================






def refine(dom: domain(dimension,stridable=true), ref_ratio: dimension*int) {
 
  //===> Query and refine low/high indices ===>
  var low_indices, high_indices: dimension*int;

  for d in dimensions {
    low_indices(d)  = dom.dim(d).low;
    high_indices(d) = dom.dim(d).high;
  }
  low_indices  *= ref_ratio;
  high_indices *= ref_ratio;
  //<=== Query and refine low/high indices <===


  //===> Set and return new domain ===>
  var range_tuple: dimension*range(stridable=true);

  for d in dimensions do
    range_tuple(d) = low_indices(d) .. high_indices(d) by dom.dim(d).stride;

  var new_dom: dom.type;
  new_dom = range_tuple;
  return new_dom;
  //<=== Set and return new domain <===
  
}


