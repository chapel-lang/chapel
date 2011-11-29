use AMRHierarchy_def;


//|"""""""""""""""""""\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {

  const hierarchy: AMRHierarchy;

  proc apply(level_idx: int, q: LevelVariable, t: real) {}
  proc apply_Homogeneous(level_idx: int, q: LevelVariable) {}

}
// /"""""""""""""""""""|
//<=== AMRBC class <===|
// \___________________|