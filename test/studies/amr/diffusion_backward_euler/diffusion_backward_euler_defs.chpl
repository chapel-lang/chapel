//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a bacward Euler
// diffusion solver.
//
//<=== Description <===


use grid_base_defs;



//===> Backward Euler/conjugate gradient advancement of a Grid Function ===>
//=========================================================================>
def RectangularGrid.advance_diffusion_backward_euler(q:              GridFunction,
						     diffusivity:    real,
						     time_requested: real,
						     dt_max:         real) {
                                             
  //==== Make sure q can validly be updated ====
  assert(q.parent_grid == this  &&  q.time <= time_requested);



  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - q.time) / dt_max );
  var dt_used = (time_requested - q.time) / n_steps;
  var rhs: [physical_cells] real;
   
   
  while (q.time < time_requested) do {
    //==== Adjust the time step to hit time_requested if necessary ====
    if (q.time + dt_max > time_requested) then
      dt_used = time_requested - q.time;
    else
      dt_used = dt_max;
    writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");


    //==== Record q at old time level ====
    q_old = copy_grid_function(q);


    //==== Update solution ====
    q = conjugate_gradient(q_old, diffusivity, dt_used, dt_used/4.0);


    //==== Update time ====
    q.time += dt_used;

  }
  //<=== Time-stepping <===
 
}
//<=== Backward Euler/conjugate gradient advancement of a Grid Function <===
//<=========================================================================





//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------
// L(u) = u - dt*diffusivity*laplacian(u) 
//----------------------------------------
def RectangularGrid.backward_euler_diffusion_operator(u:           GridFunction,
                                                      diffusivity: real,
                                                      dt:          real
						     ) {
  
  var Lu: [physical_cells] real;
  

  //==== Fill in ghost cells ====
  u.boundary_manager.fill_ghost_cells(u);

  
  //===> Compute operator ===>
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
      var cell_shift: dimension*int;
      cell_shift(d) = 2;
      Lu(cell) += (u(cell-cell_shift) - 2.0*u(cell) + u(cell+cell_shift)) / dx(d)**2;
    }
    Lu(cell) *= -dt*diffusivity;
    Lu(cell) += u(cell);
  }
  //<=== Compute operator <===
  
  return Lu;
  
}
//<==========================================
//<=== Backward Euler diffusion operator <===




//===> Conjugate gradient algorithm ===>
//=====================================>
def RectangularGrid.conjugate_gradient(rhs:         GridFunction,
				       diffusivity: real,
				       dt:          real, 
				       tolerance:   real
				      ) {

  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;
  

  //==== Initialize solution ====
  var u: [all_cells] real;
  u(physical_cells) = rhs;


  //==== Initialize residual ====
  var residual: [physical_cells] real;
  residual = rhs - backward_euler_diffusion_operator(u, diffusivity, dt);


  //==== Initialize search direction ====
  var search_direction: [all_cells] real;
  search_direction(physical_cells) = residual;


  //==== Initialize residual update direction ====
  var residual_update_direction: [physical_cells] real;
  residual_update_direction = backward_euler_diffusion_operator(search_direction, diffusivity, dt);


  //==== Initialize scalars ====
  var residual_norm = +reduce(residual*residual);
  var alpha, beta, residual_norm_old: real;
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {
    //==== Update the solution and residual ====
    alpha     = +reduce( residual*residual )  /  +reduce( residual_update_direction*search_direction(physical_cells) );
    u        += alpha*search_direction;
    residual -= alpha*residual_update_direction;


    //==== Compute norm of residual, and check for convergence ====
    residual_norm_old = residual_norm;
    residual_norm     = +reduce(residual * residual);
    if residual_norm < tolerance then break;
    if iter==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta                             = residual_norm / residual_norm_old;
    search_direction(physical_cells) = residual + beta*search_direction(physical_cells);
    residual_update_direction        = backward_euler_diffusion_operator(search_direction, diffusivity, dt);
    
  }
  //<=== CG iteration <===
  
  return u;
  
}
//<=====================================
//<=== Conjugate gradient algorithm <===