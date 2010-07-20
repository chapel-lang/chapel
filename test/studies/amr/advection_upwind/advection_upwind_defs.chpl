//===> Description ===>
//
// Initializes grid_base_defs and creates a method for an upwind solver.
//
//<=== Description <===


use grid_base_defs;


//===> Upwind update of a GridFunction ===>
//========================================>
def RectangularGrid.constant_advection_upwind(q:              GridFunction,
                              time_requested: real,
                              velocity:       dimension*real) {

  //---- Make sure q can validly be updated ----
  assert(q.parent_grid == this  &&  q.time <= time_requested);


  //---- Initialize ----
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  var val_old: [all_cells] real;

  [d in dimensions] cfl(d) = dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.45;

  
  
  //===> Time-stepping loop ===>
  while (q.time < time_requested) do {
    //---- Adjust the time step to hit time_requested if necessary ----
    if (q.time + dt_target > time_requested) then
      dt_used = time_requested - q.time;
    else
      dt_used = dt_target;
    writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");

  
    //---- Record q at old time level ----
    val_old = q.value;


    //---- Fill in ghost cells ----
    for d in dimensions do {
      val_old(lower_ghost_cells(d)) = val_old(periodic_image_of_lower_ghost_cells(d));
      val_old(upper_ghost_cells(d)) = val_old(periodic_image_of_upper_ghost_cells(d));
    }
  
  
    //===> Update solution on each cell; each dimension is independent ===>
    forall cell_pretuple in physical_cells {
      for d in dimensions {
    
        //===> If dimension==1, the cell index must be tuple-ized ===>
        var cell: dimension*int;
        if cell_pretuple.type == int then
          cell(1) = cell_pretuple;
        else
          cell = cell_pretuple;
        //<=== If dimension==1, the cell index must be tuple-ized <===
        
        
        //===> Upwind update in this dimension ===>
        var flux_lower, flux_upper: real;          

        if velocity(d) < 0.0 then {
          flux_lower = velocity(d) * val_old(cell);
          flux_upper = velocity(d) * val_old(upper_cell(cell,d));
        }
        else {
          flux_lower = velocity(d) * val_old(lower_cell(cell,d));
          flux_upper = velocity(d) * val_old(cell);
        }

        q.value(cell) -= dt_used/dx(d) * (flux_upper - flux_lower);
        //<=== Upwind update in this dimension <===

      }
    }
    //<=== Update solution on each cell; each dimension is independent <===
    
  
  
    //---- Update time ----
    q.time += dt_used;
          
  }
  //<=== Time-stepping loop <===


}
//<=== Upwind update of a GridFunction <===
//<========================================