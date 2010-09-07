use amr_base_defs;


class AMRSolution {

  const hierarchy: AMRHierarchy;
  var level_solutions: [parent_hierarchy.ref_indices] AMRLevelSolution;

}

class AMRLevelSolution: LevelSolution {
  const level:  AMRLevel;


}