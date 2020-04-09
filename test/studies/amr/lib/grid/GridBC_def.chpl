public use Grid_def;
public use GridSolution_def;



//|""""""""""""""""""""\
//|===> GridBC class ===>
//|____________________/
class GridBC {

  const grid: unmanaged Grid;

  //==== Dummy routines to be provided in derived classes ====
  proc apply(q: unmanaged GridVariable, t: real) {}
  proc apply_Homogeneous(q: unmanaged GridVariable) {}
  
}
// /""""""""""""""""""""|
//<=== GridBC Class <===|
// \____________________|







//===> PeriodicGridBC class ===>
//=============================>
class PeriodicGridBC: GridBC {


  override proc apply(q: unmanaged GridVariable, t: real) {
    //==== Periodic BCs are homogeneous ====
    apply_Homogeneous(q);
  }


  override proc apply_Homogeneous(q: unmanaged GridVariable) {

    for ghost_domain in grid.ghost_domains {
      var loc = grid.relativeLocation(ghost_domain);
      var shift: dimension*int;

      for d in dimensions {
        if loc(d) == loc1d.below:int then
          shift(d) = 2*grid.n_cells(d);
        else if loc(d) == loc1d.above:int then
          shift(d) = -2*grid.n_cells(d);
      }

      forall cell in ghost_domain do
        q.value(cell) = q.value(cell+shift);

    }

  }


}
//<=============================
//<=== PeriodicGridBC class <===
