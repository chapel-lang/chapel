public use GridVariable_def;


//===> Backward Euler diffusion operator ===>
//==========================================>
//----------------------------------------------------------------
//      L(q) = q + dt*flux_divergence(q), values on [cells]
//
// As with FluxDivergence, ghost cells must be filled beforehand.
//----------------------------------------------------------------
proc GridVariable.storeBEOperator(
  q:           unmanaged GridVariable,
  diffusivity: real,
  dt:          real
){

  storeFluxDivergence(q, diffusivity);
  value(grid.cells) = q.value(grid.cells) + dt * value(grid.cells);

}
//<==========================================
//<=== Backward Euler diffusion operator <===




//===> storeFluxDivergence method ===>
//===============================>
//-----------------------------------------------------------
// Calculates the flux divergence of q.  Note that the ghost
// cells of q must be filled beforehand
//-----------------------------------------------------------
proc GridVariable.storeFluxDivergence(
  q:               unmanaged GridVariable,
  diffusivity:     real)
{

  forall cell_pretuple in grid.cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(cell_pretuple);


    //==== Initialize to zero ====
    value(cell) = 0.0;

    
    //==== Apply contribution in each dimension ====
    for d in dimensions {
      var cell_shift: dimension*int; // initializes to 0
      cell_shift(d) = 2;
      value(cell) += (     -q.value(cell-cell_shift) 
                      + 2.0*q.value(cell) 
                      -     q.value(cell+cell_shift)
                     ) / grid.dx(d)**2;
    }

    value(cell) *= diffusivity;
  }

}
//<==============================
//<=== GridVariable.storeFluxDivergence method <===
