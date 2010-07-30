use level_base_defs;



def main {
  
  var x_low, x_high: dimension*real,
    n_cells, n_ghost: dimension*int;


  [d in dimensions] {
    x_low(d)    = -0.4;
    x_high(d)   =  0.4;
    n_cells(d)  = 40;
    n_ghost(d)  = 2;
  }

  var level = new BaseLevel(x_low               = x_low,
		            x_high              = x_high,
			    n_cells             = n_cells,
			    n_child_ghost_cells = n_ghost);


  //===> First grid ===>
  for d in dimensions {
    x_low(d)  = -0.1;
    x_high(d) = 0.4;
  }
  
  level.addGrid(x_low, x_high);
  //<=== First grid <===
  
  
  //===> Second grid ===>
  for d in dimensions {
    x_low(d)  = -0.4;
    x_high(d) = 0.1;
  }
  
  level.addGrid(x_low, x_high);
  //<=== Second grid <===


  //==== Fix the level ====
  level.fix();


  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
     f *= exp(-30*x(d)**2);
    return f;
  }
  
  var q = new LevelSolution(level = level);
  level.initializeSolution(q, initial_condition);
  //<=== Initialize solution <===
  


  //===> Test copyFromNeighbors ===>
  var grid_number = 0;
  for g in level.child_grids {

    //==== Only modify the first grid ====
    grid_number += 1;
    if grid_number > 1 then break;

    
    //==== Set q.value to 0 on this grid ====
    var q_grid = q.child_sols(g);
    q_grid.value = 0.0;
    

    //==== Copy in from other grid ====
    g.copyFromNeighbors(q_grid);

    
    //==== Output this grid only ====
    g.clawOutput(q_grid, 0);
 
  }
  //<=== Test copyFromNeighbors <===

  


}