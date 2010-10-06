use LevelArray_AdvectionCTU;
use LevelSolution_def;
use LevelBC_def;



//|"""""""""""""""""""""""""""""""""""""""""""""""""\
//|===> LevelSolution.advance_AdvectionCTU method ===>
//|_________________________________________________/
def LevelSolution.advance_AdvectionCTU(
  bc:             LevelBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(current_time <= time_requested);


  //===> Initialize ===>
  var cfl:       [dimensions] real;
  var dt_target: real;
  var dt:        real;

  [d in dimensions] cfl(d) = level.dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;
  //<=== Initialize <===
  

  
  //===> Time-stepping loop ===>
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
// /"""""""""""""""""""""""""""""""""""""""""""""""""/
//<=== LevelSolution.advance_AdvectionCTU method <==<
// \_________________________________________________\




//|""""""""""""""""""""""""""""""""""""""""""""""\
//|===> LevelSolution.step_AdvectionCTU method ===>
//|______________________________________________/
def LevelSolution.step_AdvectionCTU(
  velocity: dimension*real, 
  dt:       real)
{

  //==== Fill shared ghost cells ====
  current_data.fillSharedGhosts();
  
  //==== Update old storage with new data ====
  old_data.storeCTUOperator(current_data, velocity, dt);
    
  //==== Update ====
  old_time = current_time;
  current_time += dt;
  old_data <=> current_data;

}
// /""""""""""""""""""""""""""""""""""""""""""""""/
//<=== LevelSolution.step_AdvectionCTU method <==<
// \______________________________________________\
