use GridBC_def;



//===> ZeroInflowAdvectionGridBC class ===>
//========================================>
class ZeroInflowAdvectionBC: GridBC {
  
  
  proc apply(q: GridVariable, t: real) {
    //==== This type of BC is homogeneous ====
    apply_Homogeneous(q);
  }
  
  
  proc apply_Homogeneous(q: GridVariable) {

    for ghost_domain in grid.ghost_multidomain {
      forall cell in ghost_domain do
        q.value(cell) = 0.0;
    }

/*     for loc in ghost_locations { */
/*       forall cell in grid.ghost_multidomain(loc) do */
/* 	      q.value(cell) = 0.0; */
/*     } */
    
  }
  
}
//<=== ZeroInflowAdvectionGridBC class <===
//<========================================