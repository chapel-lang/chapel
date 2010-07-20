//===> Description ===>
//
// Driver for an upwind advection example.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl,
// and may be set to 1, 2, or 3.
//
//<=== Description <===


use forward_euler_diffusion_defs;


def main {
  //---- Diffusivity ----
  var diffusivity = 0.1;


  //===> Initial condition ===>
  const pi = 4.0*atan(1.0);
  def initial_condition( coords: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*coords(d)**2);
    return f;
  }
  //<=== Initial condition <===


  //===> Initialize grid and solution ===>
  var lower_corner, upper_corner: dimension*real,
    num_cells, num_ghost_cells: dimension*int;

  var N: int;
  if dimension<3 then N=100;
  else N = 50;

  for d in dimensions do {
    lower_corner(d)    = -1.0;
    upper_corner(d)    = 1.0;
    num_cells(d)       = N;
    num_ghost_cells(d) = 2;
  }

  var G = new RectangularGrid(lower_corner, upper_corner,
			num_cells, num_ghost_cells);

  var q = G.evaluate(initial_condition);
  //<=== Initialize grid and solution <===



  //===> Initializations for output ===>
  var time_initial: real = 0.0,
    time_final:     real = 1.0,
    num_output:     int  = 10,
    output_times:   [1..num_output] real,
    dt_output:      real = (time_final - time_initial) / num_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = time_initial + i * dt_output;
  //<=== Initializations for output <===



  //===> Generate output ===>
  //---- Initial time ----
  q.time = time_initial;
  G.clawpack_output(q, frame_number);

  //---- Subsequent times ---- 
  for output_time in output_times do {
    //---- Advance q to output time ---- 
    G.forward_euler_diffusion(q, diffusivity, output_time); 

    //---- Write output to file ---- 
    frame_number += 1; 
    G.clawpack_output(q, frame_number); 
  }
  //<=== Generate output <===
  

} // end main