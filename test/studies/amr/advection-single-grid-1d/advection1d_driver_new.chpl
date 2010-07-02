module advection1d_driver {

  config const num_cells:        int = 100;
  config const num_output_times: int = 10;
  config const time_initial:     real = 0.0;
  config const time_final:       real = 1.0;
  config const velocity:         real = 2.0;

  use grid_data;
  use clawpack_output_routines;


  def main {

  //---- Initialize the grid ----
    var grid = new Grid(-1.0, 1.0, num_cells, 2);


  //===> Initialize solution ===>
    var x_cell: [grid.physical_cells] real;
    var q:      [grid.all_cells] real;

    for cell in grid.physical_cells do {
      x_cell(cell) = grid.x_low + (cell:real - 0.5)*grid.dx;
    }

    const pi : real = 4.0*atan(1.0);
    q(grid.physical_cells)    = sin(pi*x_cell);
    q(grid.left_ghost_cells)  = q(grid.periodic_image_of_left_ghost_cells);
    q(grid.right_ghost_cells) = q(grid.periodic_image_of_right_ghost_cells);
  //<=== Initialize solution <===



    var time: real = time_initial;
    var output_times: [1..num_output_times] real;
    var dt_output = (time_final - time_initial) / num_output_times;

    for i in output_times.domain do
      output_times(i) = time_initial + i:real*dt_output;


    var dt_target:  real = 0.9*grid.dx/velocity;
    var dt_used:    real;
    var flux: [grid.edges] real;
    var frame_number: int;


  //---- Start time ----
    time = time_initial;

  //---- Write output at first time ----
    frame_number = 0;
    write_output(frame_number, time, q, grid);


  //===> Time-stepping loop ===>
    for time_out in output_times do {

    //===> Step to next output time ===>
      while (time < time_out) do {

        //---- Adjust time step to hit output time if necessary ----
    	  if (time+dt_target > time_out) then
    	    dt_used = time_out-time;
    	  else
    	    dt_used = dt_target;
       
        //---- Define fluxes at each edge ----
    	  for edge in grid.edges do
    	    flux(edge) = velocity * q( grid.cell_left_of(edge) );

        //---- Update solution on each cell ----
    	  for cell in grid.physical_cells do {
    	    q(cell) = q(cell) - dt_used/grid.dx *
    	              (  flux( grid.edge_right_of(cell) )  -  flux( grid.edge_left_of(cell) )  );
    	  }

        //---- Fill in ghost cells for use at next step ----
    	  q(grid.left_ghost_cells)  = q(grid.periodic_image_of_left_ghost_cells);
    	  q(grid.right_ghost_cells) = q(grid.periodic_image_of_right_ghost_cells);

      //---- Update time ----
      time += dt_used;
      }
    //<=== Step to next output time <===

    //---- Write output to file ----
      frame_number += 1;
      write_output(frame_number, time, q, grid);
    }
  //<=== Time-stepping loop <===


  } // end main



}