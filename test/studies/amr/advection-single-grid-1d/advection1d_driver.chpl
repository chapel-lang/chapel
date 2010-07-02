module advection1d_driver {

  config const num_cells:        int = 100;
  config const num_output_times: int = 10;
  config const time_initial:     real = 0.0;
  config const time_final:       real = 1.0;
  config const velocity:         real = 2.0;

  use grid_data;


  def main {

    //===> Initialize the grid and solution ===>
    var grid = new OneDimensionalGrid(-1.0, 1.0, num_cells, 2);
    var q    = new GridFunction(grid);

    const pi : real = 4.0*atan(1.0);
    q.value(grid.physical_cells) = sin(pi * grid.cell_centers());
    //<=== Initialize the grid and solution <===


    //===> Initializations for output ===>
    var output_times: [1..num_output_times] real;
    var dt_output = (time_final - time_initial) / num_output_times;

    for i in output_times.domain do
      output_times(i) = time_initial + i:real * dt_output;

    var frame_number: int;
    //<=== Initializations for output <===


    //===> Write output at first time ===>
    q.time = time_initial;
    frame_number = 0;
    grid.output_in_clawpack_format(q, frame_number);
    //<=== Write output at first time <===


    //===> Time-stepping loop ===>
    for output_time in output_times do {

      //---- Advance q to output time ----
      grid.constant_advection_upwind(q, output_time, velocity);

      //---- Write output to file ----
      frame_number += 1;
      grid.output_in_clawpack_format(q, frame_number);

    }
    //<=== Time-stepping loop <===


  } // end main



}