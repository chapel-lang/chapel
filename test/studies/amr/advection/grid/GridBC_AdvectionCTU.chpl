use GridBC_def;



//===> ZeroInflowAdvectionGridBC class ===>
//========================================>
class ZeroInflowAdvectionBC: GridBC {
  
  
  def apply(q: GridArray, t: real) {
    //==== This type of BC is homogeneous ====
    apply_Homogeneous(q);
  }
  
  
  def apply_Homogeneous(q: GridArray) {

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