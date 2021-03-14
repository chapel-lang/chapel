public use LevelVariable_DiffusionBE;
public use LevelSolution_def;
public use LevelBC_def;

//|\"""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.advance_DiffusionBE method    | >
//|/_________________________________________________|/

proc LevelSolution.advance_DiffusionBE(
  bc:             unmanaged LevelBC,
  diffusivity:    real,
  time_requested: real,
  dt_max:         real)
{

  //==== Safety checks ====
  assert(current_time <= time_requested);


  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - current_time) / dt_max ) : int;
  var dt = (time_requested - current_time) / n_steps;
   
   
  while (current_time < time_requested) {
    //==== Adjust the time step to hit time_requested if necessary ====
    if (current_time + dt_max > time_requested) then
      dt = time_requested - current_time;
    else
      dt = dt_max;
    writeln("Taking step of size dt=", dt, " to time ", current_time+dt, ".");


    //==== Update solution ====
    step_DiffusionBE(bc, diffusivity, dt, dt/4.0);

  }
  //<=== Time-stepping <===
 
}
// /|"""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.advance_DiffusionBE method    < |
// \|_________________________________________________\|





//|\""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.step_DiffusionBE method    | >
//|/______________________________________________|/

proc LevelSolution.step_DiffusionBE(
  bc:           unmanaged LevelBC,
  diffusivity:  real,
  dt:           real,
  tolerance:    real)
{

  //==== Set rhs ====
  var rhs = new unmanaged LevelVariable(level = level);
  bc.apply(current_data, current_time+dt);
  rhs.storeFluxDivergence(current_data, diffusivity);
  for grid in level.grids do rhs(grid,grid.cells) *= -dt;


  //==== Make initial guess, and solve for change in solution ====
  for grid in level.grids do
    old_data(grid,grid.cells) = rhs(grid,grid.cells);
  old_data.storeCGSolution(rhs, bc, diffusivity, dt, tolerance);


  //===> Update solution ===>
  for grid in level.grids do 
    old_data(grid,grid.cells) += current_data(grid,grid.cells);
  old_data <=> current_data;

  old_time = current_time;
  current_time += dt;
  
  delete rhs;
}
// /|""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.step_DiffusionBE method    < |
// \|______________________________________________\|


