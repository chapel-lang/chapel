use Level_def;



//|\"""""""""""""""""""""""""""""|\
//| >    LevelDomainSet class    | >
//|/_____________________________|/
class LevelDomainSet {

  const level: Level;
  
  var domain_sets: [level.grids] DomainSet(rank=dimension,stridable=true);
  
  def this(grid: Grid) var {
    return domain_sets(grid);
  }
  
}
// /|"""""""""""""""""""""""""""""/|
//< |    LevelDomainSet class    < |
// \|_____________________________\|



//|\""""""""""""""""""""""|\
//| >    LevelArraySet    | >
//|/______________________|/
class LevelArraySet {

  const level: Level;
  var array_sets: [level.grids] ArraySet(dimension,true,real);
  
  def initialize() {
    for grid in level.grids do
      array_sets(grid) = new ArraySet(dimension,true,real);
  }
  
  
  def allocate(level_domain_set: LevelDomainSet) {
    for grid in level.grids do
      array_sets(grid).allocate(level_domain_set(grid));
    
  }

  def this(grid: Grid) {
    return array_sets(grid);
  }
}
// /|""""""""""""""""""""""/|
//< |    LevelArraySet    < |
// \|______________________\|




def main {

  var level = readLevel("input_level.txt");
  writeln(level);
  writeln("");

  var level_domain_set = new LevelDomainSet(level);
  for grid in level.grids do
    level_domain_set(grid) = grid.ghost_domain_set;


  var level_array_set = new LevelArraySet(level);
  level_array_set.allocate(level_domain_set);

  for grid in level.grids {
    writeln("Grid:");
    writeln(grid);
    
    for array in level_array_set(grid) {
      writeln(array.dom);
      writeln(array.value);
    }
  }


}