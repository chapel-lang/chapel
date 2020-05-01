public use GridVariable_DiffusionBE;
public use GridBC_def;



//===> GridSolution.advanceDiffusionBE method ===>
//=======================================>
//----------------------------------------------------------------------
// Advances a GridSolution to the requested time, stepping forward with
// diffusion via Backward Euler.
//----------------------------------------------------------------------
proc GridSolution.advance_DiffusionBE(
  bc:             unmanaged GridBC,
  diffusivity:    real,
  time_requested: real,
  dt_max:         real)
{

  //==== Safety check ====
  assert(current_time <= time_requested,
	 "error: GridSolution.advance_DiffusionBE\n" +
	 "GridSolution.current_time > time_requested.");


  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - current_time) / dt_max ) : int;
  var dt_target = (time_requested - current_time) / n_steps;
  var dt: real;
   
  while current_time < time_requested {
    //==== Adjust the time step to hit time_requested if necessary ====
    if current_time + dt_max > time_requested then
      dt = time_requested - current_time;
    else
      dt = dt_target;
    writeln("Taking step of size dt=", dt, " to time ", current_time+dt, ".");


    //==== Update solution ====
    step_DiffusionBE(bc, diffusivity, dt, dt/4.0);

  }
  //<=== Time-stepping <===
 
}
//<=== GridSolution.advanceDiffusionBE method <===
//<=======================================







//===> GridSolution.step_DiffusionBE method ===>
//====================================>
proc GridSolution.step_DiffusionBE(
  bc:          unmanaged GridBC,
  diffusivity: real,
  dt:          real, 
  tolerance:   real
){

  //===== Assign names to solution components ====
  var dq    = old_data;  // Same reference with new name for clarity
  var t_new = current_time + dt;


  //==== Storage allocations ====

  var residual_norm_square, old_residual_norm_square: real;
  var alpha, beta, inner_product: real;
  

  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;


  //==== Calculate the rhs for conjugate gradients ====
  //----------------------------------------------------------------------
  // This is calculated by filling q_current's ghost cells with boundary
  // data from the *next* time, and then calculating -dt*flux_divergence.
  // I haven't actually seen this done before, but it makes possible
  // a clean decoupling of the "ghostFill" and "homogeneousGhostFill"
  // methods.
  //----------------------------------------------------------------------
  var rhs = new unmanaged GridVariable(grid);
  bc.apply(current_data, t_new);
  rhs.storeFluxDivergence(current_data, diffusivity);
  rhs(grid.cells) *= -dt;


  //==== Initial guess ====
  dq(grid.cells) = rhs(grid.cells);

  //==== Initialize residual ====
  //------------------------------------------------------------
  // residual = rhs - (dq + dt*flux_divergence(dq))
  //------------------------------------------------------------
  var residual = new unmanaged GridVariable(grid);
  bc.apply_Homogeneous(dq);
  residual.storeBEOperator(dq, diffusivity, dt);
  residual(grid.cells) = rhs(grid.cells) - residual(grid.cells);
  residual_norm_square = +reduce(residual.value(grid.cells)**2);


  //==== Initialize search direction ====
  var search_dir = new unmanaged GridVariable(grid);
  search_dir(grid.cells) = residual(grid.cells);
  

  //==== Initialize residual update direction ====
  //--------------------------------------------------
  // Initializes to homogeneousBEOperator(search_dir)
  //--------------------------------------------------
  var residual_update = new unmanaged GridVariable(grid);
  bc.apply_Homogeneous(search_dir);
  residual_update.storeBEOperator(search_dir, diffusivity, dt);
  
  
  //===> CG iteration ===>
  for iter_no in 1..maxiter {
 
    //==== Update the solution and residual ====
    inner_product = +reduce( residual_update(grid.cells) * search_dir(grid.cells) );
    alpha = residual_norm_square / inner_product;
 
    dq(grid.cells) += alpha * search_dir(grid.cells);
    residual(grid.cells) -= alpha * residual_update(grid.cells);

    //==== Compute norm of residual, and check for convergence ====
    old_residual_norm_square = residual_norm_square;
    residual_norm_square = +reduce(residual(grid.cells)**2);
    writeln("Iteration ", iter_no, ": residual_norm = ", sqrt(residual_norm_square));
    if sqrt(residual_norm_square) < tolerance then break;
    if iter_no==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta = residual_norm_square / old_residual_norm_square;
    search_dir(grid.cells) = residual(grid.cells) + beta * search_dir(grid.cells);

    bc.apply_Homogeneous(search_dir);
    residual_update.storeBEOperator(search_dir, diffusivity, dt);

  }
  //<=== CG iteration <===
  
  
  //==== Update solution ====
  //----------------------------------------------
  // Recalling that dq = q.old_data...
  //----------------------------------------------
  old_data(grid.cells) += current_data(grid.cells);
  old_data <=> current_data;

  old_time = current_time;
  current_time += dt;

  delete residual_update;
  delete search_dir;
  delete residual;
  delete rhs;
}
//<=== Grid.step_DiffusionBE method <===
//<========================================
