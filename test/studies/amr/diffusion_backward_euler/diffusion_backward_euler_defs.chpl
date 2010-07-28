//===> Description ===>
//
// Initializes grid_base_defs and creates a method for a bacward Euler
// diffusion solver.
//
//<=== Description <===


use grid_base_defs;



//===> Backward Euler/conjugate gradient advancement of a GridSolution ===>
//========================================================================>
def RectangularGrid.advanceDiffusionBE(q:              GridSolution,
				       bc:             GridBC,
				       diffusivity:    real,
				       time_requested: real,
		                       dt_max:         real) {
                                             
  //==== Make sure q can validly be updated ====
  assert(q.grid == this  &&  q.time <= time_requested);


  //==== Auxiliary space ====
  var dq = new GridSolution(grid = this, time = q.time);
  var rhs: [cells] real;


  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - q.time) / dt_max ) : int;
  var dt_used = (time_requested - q.time) / n_steps;
   
   
  while (q.time < time_requested) do {
    //==== Adjust the time step to hit time_requested if necessary ====
    if (q.time + dt_max > time_requested) then
      dt_used = time_requested - q.time;
    else
      dt_used = dt_max;
    writeln("Taking step of size dt=", dt_used, " to time ", q.time+dt_used, ".");


    //==== Form rhs for conjugate gradients ====
    q.time += dt_used;  // Use boundary data at t+dt to set flux divergence
    bc.ghostFill(q);
    rhs = -dt_used * fluxDivergence(q, diffusivity);
    // writeln("rhs formed");


    //==== Update solution ====
    conjugateGradient(dq, bc, rhs, diffusivity, dt_used, dt_used/4.0);
    q.value(cells) += dq.value(cells);


  }
  //<=== Time-stepping <===
 
}
//<=== Backward Euler/conjugate gradient advancement of a GridSolution <===
//<========================================================================




//===> fluxDivergence method ===>
//===============================>
//-----------------------------------------------------------
// Calculates the flux divergence of q, and returns an array
// of values on [cells].  Ghost cells must be filled BEFORE
// CALLING THIS ROUTINE.  Rationale: calculating the flux
// divergence requires the specification of boundary data,
// which is done through ghost cells.
//----------------------------------------------------------
def RectangularGrid.fluxDivergence(q:           GridSolution,
				   diffusivity: real
				  ){

  //writeln("fluxDivergence called");

  var flux_divergence: [cells] real;  // initializes to 0


  //===> Compute operator ===>
  forall cell_pretuple in cells {

    //===> If dimension==1, the cell index must be tuple-ized ===>
    var cell: dimension*int;
    if cell_pretuple.type == int then
      cell(1) = cell_pretuple;
    else
      cell = cell_pretuple;
    //<=== If dimension==1, the cell index must be tuple-ized <===

    
    //==== Apply contribution in each dimension ====
    for d in dimensions {
      var cell_shift: dimension*int; // initializes to 0
      cell_shift(d) = 2;
      flux_divergence(cell) += (     -q.value(cell-cell_shift) 
				+ 2.0*q.value(cell) 
				-     q.value(cell+cell_shift)
			       ) / dx(d)**2;
    }

  }

  flux_divergence *= diffusivity;
  //<=== Compute operator <===


  return flux_divergence;

}
//<==============================
//<=== fluxDivergence method <===




//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
def RectangularGrid.backwardEulerOperator(q:           GridSolution,
                                          diffusivity: real,
                                          dt:          real
					 ) {
  
  var Lq: [cells] real;
  Lq = q.value(cells) + dt * fluxDivergence(q, diffusivity);
  return Lq;

}
//<==========================================
//<=== Backward Euler diffusion operator <===




//===> Conjugate gradient algorithm ===>
//=====================================>
def RectangularGrid.conjugateGradient(dq:          GridSolution,
				      bc:          GridBC,
				      rhs:         [cells] real,
				      diffusivity: real,
				      dt:          real, 
				      tolerance:   real
				     ) {

  //==== Maximum number of iterations before giving up ====
  var maxiter = 100;
  

  //==== Initialize solution ====
  dq.value(cells) = rhs;


  //==== Initialize residual ====
  //------------------------------------------------------------
  // residual = rhs - (solution + dt*flux_divergence(dq))
  //------------------------------------------------------------
  bc.homogeneousGhostFill(dq);
  var residual: [cells] real;
  residual = rhs - backwardEulerOperator(dq, diffusivity, dt);


  //==== Initialize search direction ====
  //--------------------------------------------------------------
  // It's not sharp to initialize this by copying dq, but it's
  // convenient.  Mostly, this needs access to q's homogeneous
  // ghost fill.  Perhaps the flux divergence could take the cell
  // filler as an argument?
  //--------------------------------------------------------------
  // writeln("Initializing search direction");
  var search_dir = new GridSolution(grid = this);
  search_dir.value(cells) = residual;


  //==== Initialize residual update direction ====
  //-------------------------------------------
  // Initializes to Operator(search_dir)
  //-------------------------------------------
  // writeln("Initializing residual update direction");
  bc.homogeneousGhostFill(search_dir);
  var residual_update_dir = backwardEulerOperator(search_dir, diffusivity, dt);


  //==== Initialize scalars ====
  // writeln("Initializing scalars");
  var residual_norm = +reduce(residual*residual);
  var alpha, beta, residual_norm_old: real;
  
  
  //===> CG iteration ===>
  for iter in [1..maxiter] {
 
    //==== Update the solution and residual ====
    // writeln("Updating solution and residual");
    alpha = +reduce( residual * residual )  
             / +reduce( residual_update_dir * search_dir.value(cells) );
 
    dq.value += alpha * search_dir.value;
 
    residual -= alpha * residual_update_dir;


    //==== Compute norm of residual, and check for convergence ====
    residual_norm_old = residual_norm;
    residual_norm     = +reduce(residual * residual);
    writeln("Iteration ", iter, ": residual_norm = ", residual_norm);
    if residual_norm < tolerance then break;
    if iter==maxiter then writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    // writeln("Updating directions");
    beta                      = residual_norm / residual_norm_old;
    search_dir.value(cells)   = residual + beta * search_dir.value(cells);

    bc.homogeneousGhostFill(search_dir);
    residual_update_dir = backwardEulerOperator(search_dir, diffusivity, dt);
    
  }
  //<=== CG iteration <===
    
}
//<=====================================
//<=== Conjugate gradient algorithm <===