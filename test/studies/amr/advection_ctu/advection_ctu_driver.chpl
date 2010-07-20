//===> Description ===>
//
// Driver for an CTU advection example.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl,
// and may be set to 1, 2, or 3.
//
//<=== Description <===


use ctu_advection_defs;


//==== Initial condition method ===>
//------------------------------------------
// When first-class functions are available,
// an "evaluate" method that sets q.value to
// an analytical function would be nice.
//------------------------------------------
def RectangularGrid.initial_condition() {

  def f_initial ( coords: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*coords(d)**2);
    return f;
  }

  var q = new GridFunction(this);

  if dimension==1 then
    forall cell in physical_cells do
      q.value(cell) = f_initial(coordinates(tuple(cell)));
  else
    forall cell in physical_cells do
      q.value(cell) = f_initial(coordinates(cell));

  return q;
}
//<=== Initial condition method <===


def main {
  //==== Advection velocity ====
  //-------------------------------
  // velocity = (1, 3/4, 5/8, ...)
  //-------------------------------
  var velocity: dimension*real;
  [d in dimensions] velocity(d) = 0.5 + 0.5**d;



  //===> Initialize grid and solution ===>
  var low_coord, high_coord:  dimension*real,
      low_index:              dimension*int,
      n_cells, n_ghost_cells: dimension*int;

  var N: int;
  if dimension<3 then N=100;
  else N = 50;

  for d in dimensions do {
    low_coord(d)     = -1.0;
    high_coord(d)    = 1.0;
    low_index(d)     = 0;
    n_cells(d)       = N;
    n_ghost_cells(d) = 2;
  }

  var G = new RectangularGrid(low_coord, high_coord,
			      low_index,
			      n_cells, n_ghost_cells);

  var q = G.initial_condition();
  //<=== Initialize grid and solution <===



  //===> Initializations for output ===>
  var time_initial: real = 0.0,
    time_final:     real = 3.0,
    n_output:       int  = 30,
    output_times:   [1..n_output] real,
    dt_output:      real = (time_final - time_initial) / n_output,
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
    G.advance_ctu_advection(q, output_time, velocity);

    //---- Write output to file ----
    frame_number += 1;
    G.clawpack_output(q, frame_number);
  }
  //<=== Generate output <===
  

} // end main