//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use diffusion_backward_euler_defs;



//===> Backward Euler/conjugate gradient advancement of a GridSolution ===>
//========================================================================>
def RectangularGrid.advanceDiffusionBE(q:              GridSolution,
                                       bc:             GridBC,
                                       diffusivity:    real,
                                       time_requested: real,
                                       dt_max:         real) {
                                             
  //==== Make sure q can validly be updated ====
  assert(q.grid == this  &&  q.time <= time_requested);


  //==== Auxiliary space ====
  var dq = new GridSolution(grid = this, time = q.time);
  var rhs: [cells] real;


  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - q.time) / dt_max ) : int;
  var dt_used = (time_requested - q.time) / n_steps;
   
   
  while (q.time < time_requested) do {
    //==== Adjust the time step to hit time_requested if necessary ====
    if (q.time + dt_max > time_requested) then
      dt_used = time_requested - q.time;
    else
      dt_used = dt_max;
    writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");


    //==== Update solution ====
    grid.invHomBEOperator(q, bc, diffusivity, dt_used, dt_used/4.0);


  }
  //<=== Time-stepping <===
 
}
//<=== Backward Euler/conjugate gradient advancement of a GridSolution <===
//<========================================================================




def main {

  //==== Diffusivity ====
  const diffusivity = 0.1;


  //===> Initialize grid ===>
  //---------------------------------------------------
  // The "_init" on each variable name is unnecessary,
  // but I find the constructor call to be unsettling
  // without.
  //---------------------------------------------------
  var x_low, x_high:           dimension*real,
      i_low:                   dimension*int,
      n_cells, n_ghost_cells:  dimension*int;

  var N: int;
  if dimension<3 then N=100;
  else N = 50;

  for d in dimensions do {
    x_low(d)         = -1.0;
    x_high(d)        = 1.0;
    i_low(d)         = 0;
    n_cells(d)       = N;
    n_ghost_cells(d) = 2;
  }

  var G = new RectangularGrid(x_low         = x_low,
			      x_high        = x_high,
			      i_low         = i_low,
			      n_cells       = n_cells, 
			      n_ghost_cells = n_ghost_cells);
  //<=== Initialize grid <===



  //==== Initialize boundary conditions ====
  var bc = new PeriodicGridBC(grid = G);



  //===> Initialize solution ===>
  def initial_condition (x: dimension*real) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30*(x(d)+0.5)**2);
    return f;
  }

  var q = G.initializeGridSolution(initial_condition);
  //<=== Initialize  solution <===



  //===> Initialize output ===>
  var time_initial: real = 0.0,
    time_final:     real = 1.0,
    num_output:     int  = 20,
    output_times:   [1..num_output] real,
    dt_output:      real = (time_final - time_initial) / num_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = time_initial + i * dt_output;
  //<=== Initialize output <===



  //===> Generate output ===>
  //==== Initial time ====
  q.time = time_initial;
  G.clawOutput(q, frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance q to output time ====
    G.advanceDiffusionBE(q, bc, diffusivity, output_time, 0.05);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    G.clawOutput(q, frame_number);
  }
  //<=== Generate output <===
  

} // end main