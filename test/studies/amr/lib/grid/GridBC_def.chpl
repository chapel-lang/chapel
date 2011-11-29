use Grid_def;
use GridSolution_def;



//|""""""""""""""""""""\
//|===> GridBC class ===>
//|____________________/
class GridBC {

  const grid: Grid;

  //==== Dummy routines to be provided in derived classes ====
  proc apply(q: GridVariable, t: real) {}
  proc apply_Homogeneous(q: GridVariable) {}
  
}
// /""""""""""""""""""""|
//<=== GridBC Class <===|
// \____________________|







//===> PeriodicGridBC class ===>
//=============================>
class PeriodicGridBC: GridBC {


  proc apply(q: GridVariable, t: real) {
    //==== Periodic BCs are homogeneous ====
    apply_Homogeneous(q);
  }


  proc apply_Homogeneous(q: GridVariable) {

    for ghost_domain in grid.ghost_domains {
      var loc = grid.relativeLocation(ghost_domain);
      var shift: dimension*int;

      for d in dimensions {
        if loc(d) == loc1d.below then
          shift(d) = 2*grid.n_cells(d);
        else if loc(d) == loc1d.above then
          shift(d) = -2*grid.n_cells(d);
      }

      forall cell in ghost_domain do
        q.value(cell) = q.value(cell+shift);

    }

  }


}
//<=============================
//<=== PeriodicGridBC class <===