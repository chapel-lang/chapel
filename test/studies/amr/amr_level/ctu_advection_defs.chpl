//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a CTU
// (corner transport upwind) solver.
//
//<=== Description <===


use grid_base_defs;


//===> CTu update of a GridFunction ===>
//=====================================>
def RectangularGrid.advance_ctu_advection(q:              GridFunction,
                                          time_requested: real,
                                          velocity:       dimension*real) {

  //---- Make sure q can validly be updated ----
  assert(q.parent_grid == this  &&  q.time <= time_requested);



  //===> Set up periodic images of ghost cells ===>
  var periodic_image_of_lower_ghost_cells: [dimensions] subdomain(all_cells),
      periodic_image_of_upper_ghost_cells: [dimensions] subdomain(all_cells);

  var size: dimension*int;
  for d in dimensions do {
    [d_temp in dimensions] size(d_temp) = 0;
          
    size(d) = 2*num_cells(d);
    periodic_image_of_lower_ghost_cells(d) = lower_ghost_cells(d).translate(size);
    
    size(d) = -2*num_cells(d);
    periodic_image_of_upper_ghost_cells(d) = upper_ghost_cells(d).translate(size);
  }   
  //<=== Set up periodic images of ghost cells <===



  //===> Initialize ===>
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  var old_value: [all_cells] real;

  [d in dimensions] cfl(d) = dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;
  //<=== Initialize <===

  
  
  //===> Time-stepping loop ===>
  while (q.time < time_requested) do {
    //---- Adjust the time step to hit time_requested if necessary ----
    if (q.time + dt_target > time_requested) then
      dt_used = time_requested - q.time;
    else
      dt_used = dt_target;
    writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");

  
    //---- Record q at old time level ----
    old_value = q.value;


    //---- Fill in ghost cells ----
    for d in dimensions do {
      old_value(lower_ghost_cells(d)) = old_value(periodic_image_of_lower_ghost_cells(d));
      old_value(upper_ghost_cells(d)) = old_value(periodic_image_of_upper_ghost_cells(d));
    }
    
    //-----------------------------------------------------------
    //---- Domain of alignments is [0..1, 0..1, ..., 0..1].
    //---- In each dimension, alignment 1 indicates that it's
    //---- aligned with the cel, and alignment 0 indicates that
    //---- it's upwind.  (What is "it"?)
    //-----------------------------------------------------------
    var alignments: domain(dimension);
    alignments = alignments.expand(1);
  
    //===> Update solution on each cell ===>
    forall cell_pretuple in physical_cells {

      //===> If dimension==1, the cell index must be tuple-ized ===>
      var cell: dimension*int;
      if cell_pretuple.type == int then
        cell(1) = cell_pretuple;
      else
        cell = cell_pretuple;
      //<=== If dimension==1, the cell index must be tuple-ized <===

      q.value(cell) = 0.0;
      var volume_fraction: real;
      var upwind_cell: dimension*int;
      for alignment_pretuple in alignments {
        //===> If dimension==1, the alignment must be tuple-ized ===>
        var alignment: dimension*int;
        if alignment_pretuple.type == int then
          alignment = tuple(alignment_pretuple);
        else
          alignment = alignment_pretuple;
        //<=== If dimension==1, the alignment must be tuple-ized <===
        
        volume_fraction = 1.0;
        for d in dimensions {
          //---- For each alignment, set the volume fraction and index
          //---- of the upwind cell. ---------------------------------
          if alignment(d) == 0 then {
            volume_fraction *= abs(velocity(d))*dt_used / dx(d);
            if velocity(d) < 0.0 then
              upwind_cell(d) = cell(d)+2;
            else // the case velocity(d)==0 can refer to any cell
              upwind_cell(d) = cell(d)-2;
          }
          else {
            volume_fraction *= 1.0 - abs(velocity(d))*dt_used / dx(d);
            upwind_cell(d) = cell(d);
          }
          
        }

          // writeln("");
          // writeln("dt = ", dt_used);
          // writeln("velocity = ", velocity);
          // writeln("cell = ", cell);
          // writeln("alignment = ", alignment);
          // writeln("upwind_cell = ", upwind_cell);
          // writeln("volume fraction = ", volume_fraction);
          
          
        //---- Update cell value ----
        q.value(cell) += volume_fraction * old_value(upwind_cell);
  
      }
    
    }
    //<=== Update solution on each cell <===
    
  
  
    //---- Update time ----
    q.time += dt_used;
          
  }
  //<=== Time-stepping loop <===


}
//<=== CTU update of a GridFunction <===
//<=====================================

