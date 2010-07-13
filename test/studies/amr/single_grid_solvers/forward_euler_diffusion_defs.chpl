use grid_base_defs;



//===> Forward Euler advancement of a Grid Function ===>
//=====================================================>
def RectangularGrid.forward_euler_diffusion(q:              GridFunction,
                                             diffusivity:    real,
                                             time_requested: real) {
                                             
   //---- Make sure q can validly be updated ----
   assert(q.parent_grid == this  &&  q.time <= time_requested);

   //===> Time-stepping ===>
   var dt_max: real;
   var dt_stab: [dimensions] real;
   [d in dimensions] dt_stab(d) = dx(d)**2 / 2.0 / diffusivity;   
   (dt_max,) = minloc reduce(dt_stab,dimensions);

   var dt_used: real;
   
   
   while (q.time < time_requested) do {
     //---- Adjust the time step to hit time_requested if necessary ----
     if (q.time + dt_max > time_requested) then
       dt_used = time_requested - q.time;
     else
       dt_used = dt_max;
     writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");


     //---- Update solution ----
     q.value(physical_cells) = forward_euler_diffusion_operator(q.value, diffusivity, dt_used);


     //---- Update time ----
     q.time += dt_used;

   }
   //<=== Time-stepping <===
 
}
//<=== Forward Euler advancement of a Grid Function <===
//<=====================================================



//===> Forward Euler diffusion operator, L(u) = u + dt*diffusivity*laplacian(u) ===>
//=================================================================================>
def RectangularGrid.forward_euler_diffusion_operator(u: [all_cells] real,
                                                     diffusivity:   real,
                                                     dt:            real) {
  
  var Lu: [physical_cells] real;
  
  //---- Fill in ghost cells for periodic BCs ----
  for d in dimensions do {
    u(lower_ghost_cells(d)) = u(periodic_image_of_lower_ghost_cells(d));
    u(upper_ghost_cells(d)) = u(periodic_image_of_upper_ghost_cells(d));
  }
  
  
  //---- Compute operator ----
  forall cell_pretuple in physical_cells {

    //===> If dimension==1, the cell index must be tuple-ized ===>
    var cell: dimension*int;
    if cell_pretuple.type == int then
      cell(1) = cell_pretuple;
    else
      cell = cell_pretuple;
    //<=== If dimension==1, the cell index must be tuple-ized <===


    Lu(cell) = 0.0;
    for d in dimensions {
      Lu(cell) += (u(lower_cell(cell,d)) - 2.0*u(cell) + u(upper_cell(cell,d))) / dx(d)**2;
    }
    Lu(cell) *= dt*diffusivity;
    Lu(cell) += u(cell);
  }
  
  return Lu;
  
}
//<=================================================================================
//<=== Forward Euler diffusion operator, L(u) = u + dt*diffusivity*laplacian(u) <===