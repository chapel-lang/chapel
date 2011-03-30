use StaticMRSolution_def;


//|\"""""""""""""""""""""""""|\
//| >    StaticMRBC class    | >
//|/_________________________|/
class StaticMRBC {

  const hierarchy: StaticMRHierarchy;

  proc apply(level_idx: int, q: LevelVariable, t: real) {}
  proc apply_Homogeneous(level_idx: int, q: LevelVariable) {}

}
// /|"""""""""""""""""""""""""/|
//< |    StaticMRBC class    < |
// \|_________________________\|