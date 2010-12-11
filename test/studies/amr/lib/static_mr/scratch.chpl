/* //===> LevelInterface.C2FInteriorInterpolator method ===> */
/* //======================================================> */
/* //--------------------------------------------------------------------- */
/* //  */
/* //  */
/* //  */
/* //--------------------------------------------------------------------- */
/* def LevelInterface.C2FInteriorInterpolator( */
/*   q_coarse: LevelVariable, */
/*   q_fine:   LevelVariable,) */
/* { */

/*   for coarse_grid in coarse_grids { */
/*     for fine_grid in fine_grids { */

/*       var cluster = c2f_clusters(coarse_grid)(fine_grid); */
/*       var q_coarse_grid = q_coarse(coarse_grid); */


/*       //==== Compute differentials on the coarse grid's locale ==== */
/*       var coarse_values = q_coarse_grid(cluster.coarse_cells); */
/*       var coarse_diffs  = minmodDiffs(q_coarse_grid, cluster.coarse_cells); */

/*       // Send coarse_values and coarse_diffs to the fine grid's locale */


/*       //==== Evaluate the interpolant on refined image of each coarse cell ==== */
/*       forall coarse_cell in cluster.coarse_cells { */
/*      for fine_cell in cluster.fine_cells(coarse_cell) { */
          
/*        var fine_displacement: real; */
/*        var fine_value = coarse_values(coarse_cell); */

/*        for d in dimensions { */
/*          //==== Move to coarse indices ==== */
/*          fine_displacement = fine_cell(d):real / ref_ratio(d):real; */

/*          //==== Compute displacement ==== */
/*          fine_displacement -= coarse_cell(d):real; */

/*          //==== Rescale: One cell occupies 2 indices ==== */
/*          fine_displacement /= 2.0; */

/*          //==== Modify fine_value ==== */
/*          fine_value += fine_displacement * coarse_diffs(coarse_cell)(d); */
/*        } */

/*        //==== Store fine_value ==== */
/*        q_fine(fine_grid)(fine_cell) = fine_value; */

/*      } */
/*       } */


/*     } // end for fine_grid in fine_grids */
/*   } // end for coarse_grid in coarse_grids */

/* } */
/* //<=== LevelInterface.C2FInteriorInterpolator method <=== */
/* //<============================================== */



/* // Next up: create minmodDiffs method */



/* //===> LevelInterface.minmodDiffs ===> */
/* //===================================> */
/* //---------------------------------------------------- */
/* // */
/* // */
/* def LevelInterface.minmodDiffs( */
/*   q:     GridVariable,  */
/*   cells: subdomain(coarse_level.cells)) */
/* { */

/*   var differentials: [cells] [dimensions] real; */

/*   forall cell in cells { */

/*     //===> Maximum allowed differential ===> */
/*     var max_rise = 0.0, */
/*       max_drop = 0.0; */

/*     //==== First compute the max and min values in the neighborhood ==== */
/*     for nbr in neighborhood(cell,1) { */
/*       max_rise = max( max_rise, sol.value(nbr) ); */
/*       max_drop = min( max_drop, sol.value(nbr) ); */
/*     } */

/*     //==== Now adjust accordingly to get the diffs ==== */
/*     max_rise = max_rise - sol.value(cell); */
/*     max_drop = sol.value(cell) - max_drop; */
/*     var max_allowed_diff = min(max_rise, max_drop); */
/*     //<=== Maximum allowed differential <=== */


/*     //==== Estimate differentials with centered differences ==== */
/*     for d in dimensions { */
/*       var shift: dimension*int; */
/*       shift(d) = 2; */
/*       differentials(cell)(d) = (  q.value(cell + shift)  */
/*                              - q.value(cell - shift) */
/*                             ) / 2.0; */
/*     } */

/*     //==== Maximum of centered differentials ==== */
/*     var max_centered_diff = 0.0; */
/*     for d in dimensions do  */
/*       max_centered_diff = max(max_centered_diff,  */
/*                               abs(differentials(cell)(d))); */


/*     //==== Compute limiter ==== */
/*     var limiter =  */
/*       if max_centered_diff > max_allowed_diff then  */
/*         max_allowed_diff / max_centered_diff */
/*       else  */
/*      1; */


/*     //==== Limit the estimated differentials ==== */
/*     differentials(cell) *= limiter; */

/*   } */


/* } */
/* //<=== LevelInterface.minmodDiffs <=== */
/* //<=================================== */



/* def neighborhood(cell: dimension*int, k: int) { */
/*   var ranges: dimension*range(stridable=true); */
/*   for d in dimensions do ranges(d) = -2*k..2*k by 2; */
/*   var shifts = [(...ranges)]; */

/*   for shift in shifts do */
/*     yield cell + shift; */

/* } */


/* def intVec( x ...? n ) { */
/*   var int_vec: dimension*int; */
/*   for param i in 1..n do */
/*     int_vec(x(i)(2)) = x(i)(1); */
/*   return int_vec; */
/* } */








/* //|""""""""""""""""""""""""""""""""""""""""""""""""""\ */
/* //|===> LevelInterface.setF2CReceivingCells method ===> */
/* //|__________________________________________________/ */
/* //--------------------------------------------------------------------- */
/* // Identifies cells (as arithmetic domains) that will be involved in */
/* // fine-to-coarse averaging on the way up an AMR cycle. */
/* //--------------------------------------------------------------------- */
/* def LevelInterface.setF2CReceivingCells() { */

/*   for coarse_grid in coarse_grids { */
/*     for fine_grid in fine_grids { */

/*       f2c_receiving_cells(coarse_grid)(fine_grid) */
/*         = coarse_grid.cells( coarsen(fine_grid.cells) ); */
/*     } */
/*   } */

/* } */
/* // /""""""""""""""""""""""""""""""""""""""""""""""""""| */
/* //<=== LevelInterface.setF2CReceivingCells method <===| */
/* // \__________________________________________________| */






/* //|""""""""""""""""""""""""""""""""""""""""""""""""""\ */
/* //|===> LevelInterface.interpolateC2FLinear method ===> */
/* //|__________________________________________________/ */
/* def LevelInterface.interpolateC2FLinear(q_coarse: LevelVariable) */
/* { */

/*   //===> Fill coarse ghost cells for interpolation ===> */
/*   //----------------------------------------------------------------------- */
/*   // Shared ghost cells are brought up to date at this point, and boundary */
/*   // ghosts are filled by linear extrapolation from the interior.  This */
/*   // avoids reasoning about where one-sided differences are needed when */
/*   // computing the coarse differentials later on. */
/*   //----------------------------------------------------------------------- */
/*   coarse_level.fillSharedGhosts(q_coarse); */
/*   coforall grid in coarse_level.grids { */

/*     forall cell in coarse_level.boundary_ghosts(grid) { */
/*       var inward_shift: dimension*int; */

/*       //==== Compute the inward shift direction ==== */
/*       for d in dimensions { */
/*        if cell(d) < grid.i_low(d) then */
/*          inward_shift(d) = 2; */
/*        else if cell(d) > grid.i_high(d) then */
/*          inward_shift(d) = -2; */
/*       } */

/*       //==== Extrapolate the boundary cell value ==== */
/*       q_coarse(grid).value(cell) = 2.0*q_coarse(grid).value(cell + inward_shift) */
/*                                      - q_coarse(grid).value(cell + 2*inward_shift); */

/*     } */

/*   } */
/*   //<=== Fill coarse ghost cells for interpolation <=== */


/*   for coarse_grid in coarse_grids { */
/*     for fine_grid in fine_grids { */

/*       var clusters = c2f_clusters(coarse_grid)(fine_grid); */
/*       var q_coarse_grid = q_coarse(coarse_grid); */

/*       //===> Fill in coarse data for interpolants ===> */
/*       // LOCALE: COARSE GRID */
/*       //------------------------------------------------------------------ */
/*       // The coarse differential in each dimension is the minimum of each */
/*       // one-sided difference and the centered difference.  If the signs */
/*       // of the one-sided differences disagree, it's set to zero. */
/*       //------------------------------------------------------------------ */
/*       var coarse_values = q_coarse_grid(clusters.coarse_cells); */
/*       var coarse_diffs: [clusters.coarse_cells] [dimensions] real; */

/*       for cell in clusters.coarse_cells { */
/* 	var diff_mag, diff_sign, diff_low, diff_high, diff_cen: real; */

/* 	for d in dimensions { */
/* 	  var shift: dimension*int; */
/* 	  shift(d) = 2; */
          
/* 	  var diff_low  = q_coarse_grid.value(cell) - q_coarse_grid.value(cell-shift); */
/* 	  var diff_high = q_coarse_grid.value(cell+shift) - q_coarse_grid.value(cell); */
/* 	  var diff_cen  = (diff_high - diff_low) / 2.0; */

/* 	  if diff_low*diff_high > 0 { */
/* 	    diff_sign = diff_low / abs(diff_low); */
/* 	    diff_mag = min( abs(diff_low), abs(diff_high), abs(diff_cen) ); */
/* 	    coarse_diffs(cell)(d) = diff_sign * diff_mag; */
/* 	  } */
/* 	  else */
/* 	    coarse_diffs(cell)(d) = 0.0; */
/* 	} */
/*       } */
/*       //<=== Fill in coarse data for interpolants <=== */


/*       //===> Interpolate on each fine cell ===> */
/*       // LOCALE: FINE GRID */
/*       //----------------------------------------------------------------------- */
/*       // Compute in batches, one for each coarse-fine cluster. */
/*       // On each cell, the fine value is initialized to the coarse value. */
/*       // Then, for each dimension, and update is applied based on displacement */
/*       // from the coarse cell-center. */
/*       //----------------------------------------------------------------------- */
/*       for coarse_cell in clusters.coarse_cells { */
        
/*      var fine_cells = clusters.fine_cells(coarse_cell); */
/*      var fine_values: [fine_cells] real; */

/*      for fine_cell in fine_cells { */
          
/*        var fine_displacement: real; */
/*        fine_values(fine_cell) = coarse_values(coarse_cell); */

/*        for d in dimensions { */
/*          //==== Move to coarse indices ==== */
/*          fine_displacement = fine_cell(d):real / ref_ratio(d):real; */

/*          //==== Compute displacement ==== */
/*          fine_displacement -= coarse_cell(d):real; */

/*          //==== Rescale: One cell occupies 2 indices ==== */
/*          fine_displacement /= 2.0; */

/*          //==== Modify fine_value ==== */
/*          fine_values(fine_cell) += fine_displacement * coarse_diffs(coarse_cell)(d); */
/*        } */
/*      } */

/*      q_fine(fine_grid).value(fine_cells) = fine_values; */

/*       } */
/*       //<=== Interpolate fine values <=== */

/*     } // end for coarse_grid in coarse_grids */
/*   } // end for fine_grid in fine_grids */

/* } */
/* // /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| */
/* //<=== LevelInterface.LinearBoundaryInterpolator method <=== */
/* // \_______________________________________________________| */











/* //|""""""""""""""""""""""""""""""""""""""""""""\ */
/* //|===> LevelInterface.interpolateF2C method ===> */
/* //|____________________________________________/ */
/* //------------------------------------------------------------------------- */
/* // Conservatively averages data from the fine level onto the coarse level. */
/* //------------------------------------------------------------------------- */
/* def LevelInterface.interpolateF2C( */
/*   q_coarse: LevelVariable, */
/*   q_fine:   LevelVariable */
/* ) { */

/*   //==== Volume fraction is 1/product(ref_ratio) ==== */
/*   var volume_fraction: real = 1.0; */
/*   for d in dimensions do */
/*     volume_fraction /= ref_ratio(d):real; */


/*   for coarse_grid in coarse_grids { */
/*     for fine_grid in fine_grids { */

/*       //==== Compute coarse averages ==== */
/*       // LOCALE: FINE GRID */
/*       var coarse_cells = f2c_coarse_cells(coarse_grid)(fine_grid); */
/*       var coarse_values: [coarse_cells] real; // init to 0 */

/*       forall coarse_cell in coarse_cells { */
/*         var fine_cells = refine(coarse_cell); */
/*         for fine_cell in fine_cells do */
/*           coarse_values(coarse_cell) += q_fine(fine_grid).value(fine_cell); */
/*         coarse_values(coarse_cell) *= volume_fraction; */
/*       } */

/*       // Transfer coarse_values to coarse grid locale */

/*       //==== Update coarse grid values ==== */
/*       // LOCALE: COARSE GRID */
/*       q_coarse(coarse_grid).value(coarse_cells) = coarse_values; */

/*     } // end for fine_grid in fine_grids */
/*   } // end for coarse_grid in coarse_grids */

/* } */
/* // /""""""""""""""""""""""""""""""""""""""""""""| */
/* //<=== LevelInterface.interpolateF2C method <===| */
/* // \____________________________________________| */





















/* //|"""""""""""""""""""""""""""""""""""""""""""\ */
/* //|===> LevelInterface.setC2FReceivingCells ===> */
/* //|___________________________________________/ */
/* //----------------------------------------------------------------------- */
/* // For each fine grid/coarse grid pair, if the intersection is nonempty, */
/* // stores the set of fine ghost cells that will receive data from */
/* // the coarse grid. */
/* //----------------------------------------------------------------------- */
/* def LevelInterface.setC2FReceivingCells() { */
    

/*   for fine_grid in fine_grids { */
/*     for coarse_grid in coarse_grids { */

/*       var ghost_cells = new GhostCells(fine_grid); */
/*       var refined_coarse_cells = refine(coarse_grid.cells); */
/*       var intersection: domain(dimension, stridable=true); */
/*       var has_intersection = false; */

/*       for loc in ghost_locations { */
/*         if loc != interior_loc { */
/*           intersection = ghost_cells(loc)(refined_coarse_cells); */

/*           if intersection.numIndices > 0 then */
/*             has_intersection = true; */

/*           ghost_cells(loc) = intersection; */
/*         } */
/*       } */

/*       if has_intersection then */
/*         c2f_receiving_cells(fine_grid)(coarse_grid) = ghost_cells; */
      
/*     } // end for coarse_grid in coarse_grids */
/*   } // end for fine_grid in fine_grids */


/* } */
/* // /"""""""""""""""""""""""""""""""""""""""""""| */
/* //<=== LevelInterface.setC2FReceivingCells <===| */
/* // \___________________________________________| */





/* //|""""""""""""""""""""""""""""""""""\ */
/* //|===> CoarseGridSenderInfo class ===> */
/* //|__________________________________/ */
/* class CoarseGridSenderInfo { */

/*   var fine_grids:       domain(Grid); */
/*   var fine_ghost_cells: [fine_grids] GhostCells; */

/*   //===> Constructor ===> */
/*   def CoarseGridSenderInfo( */
/*     level_interface: LevelInterface, */
/*     coarse_grid:     Grid) */
/*   { */

/*     for fine_grid in level_interface.fine_grids { */

/*       var ghost_cells = new GhostCells(fine_grid); */
/*       var refined_coarse_cells = level_interface.refine(coarse_grid.cells); */
/*       var intersection: domain(dimension, stridable=true); */
/*       var has_intersection = false; */

/*       //==== Interior location doesn't require special treatment ==== */
/*       for loc in ghost_locations { */
/* 	intersection = ghost_cells(loc)(refined_coarse_cells); */

/* 	if intersection.numIndices > 0 then */
/* 	  has_intersection = true; */

/* 	ghost_cells(loc) = intersection; */
/*       } */
      
/*       if has_intersection { */
/* 	this.fine_grids.add(fine_grid); */
/* 	fine_ghost_cells(fine_grid) = ghost_cells; */
/*       } */
      
/*     } // end for fine_grid in level_interface.fine_grids */
/*   } */
/*   //<=== Constructor <=== */

/* } */
/* // /""""""""""""""""""""""""""""""""""| */
/* //<=== CoarseGridSenderInfo class <===| */
/* // \__________________________________| */





/* //|"""""""""""""""""""""""""""""""""\ */
/* //|===> FineGridSenderInfo method ===> */
/* //|_________________________________/ */
/* class FineGridSenderInfo { */
  
/*   var coarse_grids: domain(Grid); */
/*   var coarse_cells: [coarse_grids] domain(dimension, stridable=true); */

/*   //===> Constructor ===> */
/*   def FineGridSenderInfo( */
/*     interface: LevelInterface, */
/*     fine_grid: Grid) */
/*   { */
    
/*     var coarsened_fine_cells = coarsen( fine_grid.cells ); */
/*     var intersection: domain(dimension, stridable=true); */

/*     for coarse_grid in interface.coarse_grids { */
/*       intersection = coarsened_fine_cells( coarse_grid.cells ); */

/*       if intersection.numIndices > 0 { */
/* 	coarse_grids.add(coarse_grid); */
/* 	coarse_cells(coarse_grid) = intersection; */
/*       } */
/*     } */
/*   } */
/*   //<=== Constructor <=== */

/* } */
/* // /""""""""""""""""""""""""""""""""| */
/* //<=== FineGridSenderInfo class <===| */
/* // \________________________________| */