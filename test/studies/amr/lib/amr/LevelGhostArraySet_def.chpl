use ArraySet_def;
use Level_def;



class GhostArraySet {
  const grid: Grid;
  var array_set: ArraySet(dimension,true,real);

  def GhostArraySet(grid: Grid) {
    this.grid = grid;
    array_set = new ArraySet(dimension,true,real);
    array_set.allocate( grid.ghost_domain_set );
  }

  def these() {
    for array in array_set do
      yield array;
  }

}


class LevelGhostArraySet {
  const level: Level;
  var ghost_array_sets: [level.grids] GhostArraySet;

  def initialize() {
    for grid in level.grids do
      ghost_array_sets(grid) = new GhostArraySet(grid);
  }

  def this(grid: Grid) {
    return ghost_array_sets(grid);
  }

}