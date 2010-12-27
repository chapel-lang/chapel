
use LevelSolution_def;
use RefiningTransferVariable_def;



//|\""""""""""""""""""""""""""""""""""""|\
//| >    GridVariable.coarsen_Linear    | >
//|/____________________________________|/

//-------------------------------------------------------------------
// Returns values from the GridVariable linearly interpolated to the 
// input domain coarse_cells.  This is akin to accessing the
// GridVariable's values on a coarser index space.
//-------------------------------------------------------------------

def GridVariable.coarsen_Linear(
  coarse_cells: domain(dimension,stridable=true),
  ref_ratio:    dimension*int)
{

  //==== Volume fraction is 1/product(ref_ratio) ====
  var volume_fraction: real = 1.0;
  for d in dimensions do
    volume_fraction /= ref_ratio(d):real;


  //==== Compute coarse averages ====
  var coarse_values: [coarse_cells] real;

  forall coarse_cell in coarse_cells {
    var fine_cells = refine(coarse_cell, ref_ratio);
    for fine_cell in fine_cells do
      coarse_values(coarse_cell) += value(fine_cell);
    coarse_values(coarse_cell) *= volume_fraction;
  }

  return coarse_values;

}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    GridVariable.coarsen_Linear    < |
// \|____________________________________\|




//|\"""""""""""""""""""""""""""""""""""|\
//| >    GridVariable.refine_Linear    | >
//|/___________________________________|/
//----------------------------------------------------------------
// Returns values from the GridVariable linearly interpolated to the 
// input domain fine_cells.  This is akin to accessing the
// GridVariable's values on a finer index space.
//----------------------------------------------------------------
def GridVariable.refine_Linear(
  fine_cells: domain(dimension,stridable=true),
  ref_ratio:  dimension*int)
{

  var coarse_cells  = grid.cells( coarsen(fine_cells, ref_ratio) );

  var coarse_values = value(coarse_cells);
  var coarse_diffs: [coarse_cells] dimension*real;


  //===> Form interpolant data (values and differentials ===>
  forall cell in coarse_cells {
    var diff_mag, diff_sign, diff_low, diff_high, diff_cen: real;
    var shift: dimension*int;

    for d in dimensions {
      shift *= 0;
      shift(d) = 2;

      diff_low  = value(cell) - value(cell-shift);
      diff_high = value(cell+shift) - value(cell);
      diff_cen  = (diff_high - diff_low) / 2.0;

      if diff_low*diff_high > 0 {
        diff_sign = diff_low / abs(diff_low);
        diff_mag = min( abs(diff_low), abs(diff_high), abs(diff_cen) );
        coarse_diffs(cell)(d) = diff_sign * diff_mag;
      }
      else
        coarse_diffs(cell)(d) = 0.0;
    }
  }
  //<=== Form interpolant data (values and differentials) <===


  //===> Evaluate interpolant on fine cells ===>
  var fine_values: [fine_cells] real;

  forall fine_cell in fine_cells {
    var coarse_cell = coarsen(fine_cell, ref_ratio);
    fine_values(fine_cell) = coarse_values(coarse_cell);

    var fine_displacement: real = 0.0;

    for d in dimensions {
      //==== Move to coarse indices ====
      fine_displacement = fine_cell(d):real / ref_ratio(d):real;

      //==== Compute displacement ====
      fine_displacement -= coarse_cell(d):real;

      //==== Rescale: One cell occupies 2 indices ====
      fine_displacement /= 2.0;

      //==== Modify fine_value ====
      fine_values(fine_cell) += fine_displacement * coarse_diffs(coarse_cell)(d);
    }      

  }
  //<=== Evaluate interpolant on fine cells <===


  return fine_values;

}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    GridVariable.refine_Linear    < |
// \|___________________________________\|




//|\""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.getFineValues_Linear   | >
//|/__________________________________________|/
def LevelVariable.getFineValues_Linear(
  q_fine:      LevelVariable,
  cf_boundary: CFBoundary)
{

  //==== Safety check ====
  assert(this.level == cf_boundary.coarse_level);
  assert(q_fine.level == cf_boundary.fine_level);
  
  //==== Refinement ratio ====
  const ref_ratio = refinementRatio(this.level, q_fine.level);

  //==== Each grid obtains coarsened values from fine neighbors ====
  for grid in this.level.grids {
    for (fine_nbr,overlap) in cf_boundary.fine_overlaps(grid) do
      this(grid,overlap) = q_fine(fine_nbr).coarsen_Linear(overlap,ref_ratio);
  }

}
// /|"""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.getFineValues_Linear    < |
// \|___________________________________________\|





//|\""""""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    RefiningTransferVariable.getCoarseValues_Linear    | >
//|/________________________________________________________|/
def RefiningTransferVariable.getCoarseValues_Linear(
  q_coarse:  LevelVariable)
{
  //==== Safety check ====
  assert(q_coarse.level == cf_boundary.coarse_level);

  //==== Pull refinement ratio ====
  const ref_ratio = cf_boundary.ref_ratio;

  //==== Prepare ghost data of q_coarse ====
  q_coarse.extrapolateGhostData();
  q_coarse.fillOverlaps();

  //===> Interpolate on each fine grid ===>
  for fine_grid in cf_boundary.fine_level.grids {
    
    const multiarray     = multiarrays(fine_grid);
    const coarse_overlap = cf_boundary.coarse_overlaps(fine_grid);
    
    for (nbr, multidomain, subrange) in coarse_overlap {
      
      var i_nbr = 0;
      for i in subrange {
        // const array = multiarray.arrays(i);
        var array => multiarray(i);

        //==== Safety check; potentially fragile iteration ====
        i_nbr += 1;
        assert(array.domain == multidomain.domains(i_nbr));
        
        //==== Interpolate ====
        // array.value = q_coarse(nbr).refine_Linear(array.Domain,ref_ratio);
        array = q_coarse(nbr).refine_Linear(array.domain,ref_ratio);
        
      }

    }
    
  }
  //<=== Interpolate on each fine grid <===

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    RefiningTransferVariable.getCoarseValues_Linear    < |
// \|________________________________________________________\|






//|\"""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.correct_Linear    | >
//|/_____________________________________|/

def LevelSolution.correct_Linear(
  fine_solution: LevelSolution,
  cf_boundary:   CFBoundary)
{
  //==== Safety check ====
  assert(this.level == cf_boundary.coarse_level);
  assert(fine_solution.level == cf_boundary.fine_level);
  assert( abs(this.current_time - fine_solution.current_time) < 1.0e-8);
  
  //==== Correct ====
  current_data.getFineValues_Linear(fine_solution.current_data, cf_boundary);
}
// /|"""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.correct_Linear    < |
// \|_____________________________________\|