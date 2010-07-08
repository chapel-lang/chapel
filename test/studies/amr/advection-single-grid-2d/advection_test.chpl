module advection_test {


  use grid_class;


  def main {
    //---- Advection velocity ----
    var velocity: dimension*real;
    [d in dimensions] velocity(d) = (dimension-d+1):real;


    //---- Initial condition ----
    const pi = 4.0*atan(1.0);
    def initial_condition( coords: dimension*real ) {
      var f: real = 1.0;
      for d in dimensions do
	f *= sin(d*pi*coords(d));
      return f;
    }


    //---- Initialize grid and solution ----
    var lower_corner, upper_corner: dimension*real,
      num_cells, num_ghost_cells: dimension*int;

    for d in dimensions do {
      lower_corner(d)    = -1.0;
      upper_corner(d)    = 1.0;
      num_cells(d)       = 40;
      num_ghost_cells(d) = 1;
    }

    var G = new RectangularGrid(lower_corner, upper_corner,
				num_cells, num_ghost_cells);

    var q = G.evaluate(initial_condition);



    //---- Initializations for output ----
    var time_initial: real = 0.0,
      time_final:     real = 1.0,
      num_output:     int = 10,
      output_times:   [1..num_output] real,
      dt_output:      real = (time_final - time_initial) / num_output,
      frame_number:   int = 0;

    for i in output_times.domain do
      output_times(i) = time_initial + i * dt_output;



    //---- Output initial condition ----
    q.time = time_initial;
    G.clawpack_output(q, frame_number);



/*     //---- Output-generation loop ---- */
/*     for output_time in output_times do { */
      
/*       //---- Advance q to output time ---- */
/*       G.constant_advection_upwind(q, output_time, velocity); */

/*       //---- Write output to file ---- */
/*       frame_number += 1; */
/*       grid.clawpack_output(q, frame_number); */
/*     } */



  } // end main
}