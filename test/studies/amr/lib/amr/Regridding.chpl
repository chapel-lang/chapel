
use AMRHierarchy_def;
use AMRSolution_def;



//|\""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.regrid    | >
//|/____________________________|/
//----------------------------------------
// Regrid all levels finer than base_idx.
//----------------------------------------
def AMRHierarchy.regrid(
  base_idx:     int,
  amr_solution: AMRSolution)
{
  
  var new_level_indices = base_idx+1 .. n_levels;
  
  //==== Remove old CFBoundaries ====
  for i in new_level_indices {
    coarse_boundaries(i).clear();
    delete coarse_boundaries(i);
    coarse_boundaries(i) = nil;
  }
  
  for i in new_level_indices by -1 do
    regridLevel(i, amr_solution);
    
}
// /|""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.regrid    < |
// \|____________________________\|



//|\"""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.regridLevel    | >
//|/_________________________________|/
//--------------------------------------------------------
// Regrids the ith level.  This is done by flagging cells
// on level i-1.
//--------------------------------------------------------
def regridLevel(
  i:            int
  amr_solution: AMRSolution)
{
  const ref_ratio_above = refinementRatio(levels(i-1), levels(i));
  
  //==== Flag level above ====
  var flags: [levels(i-1).cells] bool;
  amr_solution.setFlags(i-1,flags);
  
  //==== Add flags for level below if needed ====
  if i < n_levels {    
    const ref_ratio_below = refinementRatio(levels(i), levels(i+1));
    
    for grid_below in levels(i+1).grids {
      var cells_to_flag = coarsen(nested_grid.cells, ref_ratio_below);
      cells_to_flag = coarsen(cells_to_flag, ref_ratio_over);
      flags(cells_to_flag) = true;
    }
  }
  
  //==== Partition ====
  var grid_domains = partitionFlags(flags); // returns DomainSet
  
  //==== Ensure nesting ====
  for grid_above in levels(i-1).grids do
    grid_domains.subtract(grid_above.cells);
  
  //===> Create new level ===>
  var new_level = new Level(x_low   = this.x_low,
                            x_high  = this.x_high,
                            n_cells = levels(i-1).n_cells * ref_ratio_above,
                            n_ghost_cells = this.n_ghost_cells);
                            
  for grid_domain_above in grid_domains do
    new_level.addGrid( refine(grid_domain_above, ref_ratio_above) ); //# need to add this method to the level
  //<=== Create new level <===
  
  
  //==== Move solution data to new level ====
  amr_solution.transferData(i, new_level);
  
  //==== Migrate new level to level list ====
  levels(i).clear();
  delete levels(i);
  levels(i) = new_level;
  
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.regridLevel    < |
// \|_________________________________\|




//|\"""""""""""""""""""""""""""""""""|\
//| >    AMRSolution.transferData    | >
//|/_________________________________|/
//-----------------------------------------------
// Transfers the data from level i to new_level.
//-----------------------------------------------
def AMRSolution.transferData(
  i:         int,
  new_level: Level)
{
  
  //==== Safety check ====
  assert( abs(level_solutions(i).current_time 
              - level_solutions(i-1).current_time) < 1.0e-8 );
  
  //===> Set up new LevelSolution ===>
  var new_level_solution = new LevelSolution(new_level);
  
  new_level_solution.current_data.fill(level_solutions(i).current_data,
                                       level_solutions(i-1).current_data);

  new_level_soluton.current_time = level_solutions(i).current_time;

  // old_data doesn't hold any relevant information to start.  
  // Hopefully this is a good enough indicator?
  new_level_solution.old_time = new_level_solution.current_time;
  //<=== Set up new LevelSolution <===
  
  
  //==== Remove old LevelSolution, and insert the new one ====
  level_solutions(i).clear();
  delete level_solutions(i);
  level_solutions(i) = new_level_solution;

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    AMRSolution.transferData    < |
// \|_________________________________\|




//|\""""""""""""""""""""""""|\
//| >    LevelArray.fill    | >
//|/________________________|/
def LevelArray.fill(
  q_old:     LevelArray,
  q_coarse:  LevelArray)
{
  //==== Safety check ====
  assert(this.level == q_old.level);
  
  //==== Refinement ratio ====
  const ref_ratio = refinementRatio(q_coarse.level, this.level);
  
  //===> Fill grid-by-grid ===>
  for grid in level.grids {
    //==== Initialize unfilled cell blocks ====
    var unfilled_cell_blocks = new DomainSet(dimension, true);
    unfilled_cell_blocks.add(grid.cells);

    //===> Copy from q_old where possible ===>
    for old_grid in q_old.level.grids {
      
      var overlap = grid.cells( old_grid.cells );

      if overlap.numIndices > 0 {
        this(grid,overlap) = q_old(old_grid, overlap);
        unfilled_cell_blocks.subtract(overlap);
      }
    }
    //<=== Copy from q_old where possible <===
  
    //===> Interpolate from q_coarse everywhere else ===>
    for coarse_grid in q_coarse.level.grids {
      
      var full_overlap = grid.cells( refine(coarse_grid.cells, ref_ratio) );
      
      if full_overlap.numIndices > 0 {
        
        var overlap_set = unfilled_cell_blocks( full_overlap );

        for overlap in overlap_set {
          this(grid,overlap) = q_coarse(coarse_grid).coarsen_Linear(overlap);
          unfilled_cell_blocks.subtract(overlap);
        }

        delete overlap_set;
      }
      
    }
    //<=== Interpolate from q_coarse everywhere else <===

  }
  //<=== Fill grid-by-grd <===
}
// /|""""""""""""""""""""""""/|
//< |    LevelArray.fill    < |
// \|________________________\|