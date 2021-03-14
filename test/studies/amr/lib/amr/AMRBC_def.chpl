public use AMRHierarchy_def;


//|"""""""""""""""""""\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {

  const hierarchy: unmanaged AMRHierarchy;

  proc apply(level_idx: int, q: unmanaged LevelVariable, t: real) {}
  proc apply_Homogeneous(level_idx: int, q: unmanaged LevelVariable) {}

}
// /"""""""""""""""""""|
//<=== AMRBC class <===|
// \___________________|
