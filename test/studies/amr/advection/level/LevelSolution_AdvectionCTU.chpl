public use LevelVariable_AdvectionCTU;
public use LevelSolution_def;
public use LevelBC_def;



//|\""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.advance_AdvectionCTU method    | >
//|/__________________________________________________|/

//-----------------------------------------------------------
// Advances the LevelSolution to time_requested by advection
// with the specified velocity, taking time steps with the
// CTU (corner transport upwind) method.
//
// The mathematical content of CTU is in the method
// GridVariable.storeCTUOperator.
//-----------------------------------------------------------

proc LevelSolution.advance_AdvectionCTU(
  bc:             unmanaged LevelBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(current_time <= time_requested);


  //===> Calculate dt_target via CFL condition ===>
  var max_dt_values = level.dx / abs(velocity);
  var dt_target     = min( (...max_dt_values) );
  dt_target *= 0.95;
  //<=== Calculate dt_target via CFL condition <===


  //===> Time-stepping loop ===>
  var dt: real;
  
  while current_time < time_requested {

    //==== Adjust the time step to hit time_requested if necessary ====
    if current_time + dt_target > time_requested then
      dt = time_requested - current_time;
    else
      dt = dt_target;
    writeln("Taking step of size dt=", dt, " to time ", current_time + dt, ".");


    //==== Update solution ====
    bc.apply(current_data, current_time);
    step_AdvectionCTU(velocity, dt);
          
  }
  //<=== Time-stepping loop <===

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.advance_AdvectionCTU method    < |
// \|__________________________________________________\|




//|\"""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.step_AdvectionCTU method    | >
//|/_______________________________________________|/

//----------------------------------------------------
// Takes a time step of length dt via the CTU method.
//----------------------------------------------------

proc LevelSolution.step_AdvectionCTU(
  velocity: dimension*real, 
  dt:       real)
{

  //==== Fill shared ghost cells ====
  current_data.fillOverlaps();
  
  //==== Update old storage with new data ====
  old_data.storeCTUOperator(current_data, velocity, dt);
    
  //==== Update ====
  old_time = current_time;
  current_time += dt;
  old_data <=> current_data;

}
// /|""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.step_AdvectionCTU method   < |
// \|______________________________________________\|
