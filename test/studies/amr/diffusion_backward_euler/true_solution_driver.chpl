//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use diffusion_backward_euler_defs;


class Gaussian: TrueSolution {

  const pi = 4.0*atan(1.0);
  var diffusivity = 0.1;
  var t0 = 0.1;

  def qTrue(x: dimension*real, t: real) {

    //==== Calculate norm squared of x ====
    var norm_squared_x = 0.0;
    for d in dimensions do norm_squared_x += x(d)**2;

    return (4.0 * pi * diffusivity * (t + t0))**(-dimension:real / 2.0)
      * exp(-norm_squared_x / (4.0 * diffusivity * (t + t0)));

  }

}



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
    x_low(d)         = -0.1*d;
    x_high(d)        = x_low(d) + 1.0;
    i_low(d)         = 0;
    n_cells(d)       = N;
    n_ghost_cells(d) = 1;
  }

  var G = new RectangularGrid(x_low         = x_low,
			      x_high        = x_high,
			      i_low         = i_low,
			      n_cells       = n_cells, 
			      n_ghost_cells = n_ghost_cells);
  //<=== Initialize grid <===



  //===> Initialize boundary conditions ===>
  var true_solution = new Gaussian(diffusivity = diffusivity,
				   t0 = 0.6);

  var bc = new TrueSolutionDirichletGridBC(grid          = G,
					   true_solution = true_solution);
  //<=== Initialize boundary conditions <===



  //==== Initialize solution ====
  var q = new GridSolution(grid = G);
  forall precell in G.cells {
    var cell = tuplify(precell);
    q.value(cell) = true_solution.qTrue(G.xValue(cell), 0.0);
  }



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