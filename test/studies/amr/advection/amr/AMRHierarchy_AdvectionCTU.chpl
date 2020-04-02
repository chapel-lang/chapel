
public use LevelSolution_AdvectionCTU;
public use AMRHierarchy_def;
public use AMRBC_def;

private use IO;



//|\""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.advance_AdvectionCTU    | >
//|/__________________________________________|/
//
//------------------------------------------------------------------
// This advances the AMRHierarchy ahead to time_requested using the
// CTU (corner transport upwind) algorithm for advection.
//
// Mathematical details of the CTU algorithm ultimately fall to the
// method GridSolution.step_AdvectionCTU.
//------------------------------------------------------------------

proc AMRHierarchy.advance_AdvectionCTU(
  bc:             unmanaged AMRBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //---- Safety check ----
  assert(time < time_requested,
         "error: AMRHierarchy.advance_AdvectionCTU\n" +
         "AMRHierarchy.time = " + "%8.4Er".format(time) + ", while time_requested = " +
         "%8.4Er".format(time_requested));
  
  
  //---- Set dt_target ----
  //---------------------------------------------------------------------
  // In each dimension, the maximum time step is determined by the CFL 
  // (Courant-Friedrichs-Lewy) condition.  Since our method only allows 
  // data to propagate one cell at each time step, dt must be chosen so
  // that
  //        dt*velocity <= dx
  // in each dimension.
  //
  // The dx here refers to the coarsest level.  When taking steps on
  // finer levels, dt will have to be refined in conjunction with dx.
  //---------------------------------------------------------------------

  var max_dt_values = levels(1)!.dx / abs(velocity);
  var dt_target = min( (...max_dt_values) );
  dt_target *= 0.95;



  //===> Step ahead to time_requested ===>

  var dt: real;

  while time < time_requested {

    //---- Choose time step ----
    if time + dt_target > time_requested then
      dt = time_requested - time;
    else
      dt = dt_target;
    writeln("Taking step of size dt=", dt, " to time ", time+dt, ".");


    //---- Step solution forwards ----
    //---------------------------------------------------------------------
    // This call invokes the same method on all levels down the hierarchy,
    // so stepping the top level is all that is needed.
    //---------------------------------------------------------------------
    stepLevel_AdvectionCTU(1, bc, velocity, dt);


    //---- Update time ----
    time += dt;
  }
  //<=== Step ahead to time_requeste <===

}
// /|""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.advance_AdvectionCTU    < |
// \|__________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.stepLevel_AdvectionCTU    | >
//|/____________________________________________|/
//
//--------------------------------------------------------------
// This method steps a level of the AMRHierarchy ahead by time
// step dt.  This procedure is recursively applied to all finer
// levels, so in fact the entire hierarchy below the specified
// level is advanced by dt.  (That actually suggests that
// 'stepLevel' may not be the right name...I'll have to think
// about something more suitable.)
//--------------------------------------------------------------

proc AMRHierarchy.stepLevel_AdvectionCTU(
  i_level: int,
  bc:        unmanaged AMRBC,
  velocity:  dimension*real,
  dt:        real)
{

  //===> Step solution on the input level ===>
  
  
  //---- Aliases ----
  const level_solution = level_solutions(i_level)!;
  const t =              level_solution.current_time;


  //---- Apply physical boundary conditions ----
  bc.apply(i_level, level_solution.current_data, t);


  //---- If level is fine, fill its CF ghost region ----
  if i_level > 1 then
    level_solution.current_data.fillCFGhostRegion( cf_ghost_solutions(i_level)!, t );


  //---- Step the LevelSolution ahead ----
  level_solution.step_AdvectionCTU(velocity, dt);
  
  
  //---- Update the regrid counter ----
  regrid_counters(i_level) -= 1;

  //<=== Step solution on the input level <===



  //===> Step solution on the next-finest level ===>

  if i_level < level_indices.high {

    //---- Fill CFGhostSolution for the next level ----
    cf_ghost_solutions(i_level+1)!.fill( level_solution );


    //---- Step the finer solution as many times as necessary ----
    const ref_ratio      = refinementRatio(levels(i_level)!, levels(i_level+1)!);
    const time_ref_ratio = max( (...ref_ratio) );
    const dt_fine        = dt / time_ref_ratio;
    
    for i in 1..time_ref_ratio {
      stepLevel_AdvectionCTU( i_level+1, bc, velocity, dt_fine );
      if regrid_counters(i_level+1)==0 then regrid(i_level);
    }


    //---- Correct current_data on the invalid region ----
    level_solution.current_data.fillInvalidRegion( invalid_regions(i_level)!, 
                                                   level_solutions(i_level+1)!.current_data );

  }

  //<=== Step solution on the next-finest level <===

}
// /|""""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.stepLevel_AdvectionCTU    < |
// \|____________________________________________\|
