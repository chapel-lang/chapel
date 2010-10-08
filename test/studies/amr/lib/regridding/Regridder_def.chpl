use FlaggedDomain_def;
use LevelSolution_def;


//|\""""""""""""""""""""""""|\
//| >    Regridder class    | >
//|/________________________|/
class Regridder {
  var target_efficiency: real;
  var min_n_cells: dimension*int;
  var candidate_stack: Stack(FlaggedDomain);
  var finished_stack:  Stack(domain(dimension,stridable=true));
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def Regridder(
    target_efficiency: real,
    min_n_cells: dimension*int)
  {
    this.target_efficiency = target_efficiency;
    this.min_n_cells = min_n_cells;
    
    candidate_stack = new Stack(FlaggedDomain);
    finished_stack  = new Stack(domain(dimension,stridable=true));
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|


  //|\'''''''''''''''''|\
  //| >    setFlags    | >
  //|/.................|/
  //----------------------------------------------------------
  // Creates an array of flags based on the behavior of a
  // LevelSolution.  This is meant to be overridden by the 
  // user in a derived class of Regridder.
  //----------------------------------------------------------
  def setFlags(
    solution:  LevelSolution)
  {}
  // /|'''''''''''''''''/|
  //< |    setFlags    < |
  // \|.................\|
}
// /|""""""""""""""""""""""""/|
//< |    Regridder class    < |
// \|________________________\|




//|\"""""""""""""""""""""""""|\
//| >    Regridder.regrid    | >
//|/_________________________|/
def Regridder.regrid(solution: LevelSolution) {
  
  var flags = setFlags(solution);
  var candidate = new FlaggedDomain(flags);
  candidate_stack.push(candidate);
  
  while !candidate_stack.isEmpty() {
    candidate = candidate_stack.pop();
    
    if candidate.efficiency() < target_efficiency {
      var new_candidates: 2*FlaggedDomain = candidate.split();
      
      if new_candidates==(candidate,nil) {
        writeln("Warning: Regridder.regrid\n" +
                "Could not meet target efficiency for all grids.");
        finished_stack.push(candidate.D);
      }
      else {
        candidate_stack.push(new_candidates(1));
        candidate_stack.push(new_candidates(2));
      }
      
      delete candidate();
    }
  }
  
}
// /|"""""""""""""""""""""""""/|
//< |    Regridder.regrid    < |
// \|_________________________\|