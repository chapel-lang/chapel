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
    if d == 1 then {
      x_low(d)  = 0.0;
      x_high(d) = 0.4;
    }
    else {
      x_low(d)  = -0.1;
      x_high(d) = 0.4;
    }
  }
  
  level.addGrid(x_low, x_high);
  //<=== First grid <===
  
  
  //===> Second grid ===>
  for d in dimensions {
    if d == 1 then {
      x_low(d)  = -0.4;
      x_high(d) = 0.0;
    }
    else {
      x_low(d)  = -0.4;
      x_high(d) = 0.1;
    }
  }
  
  level.addGrid(x_low, x_high);
  //<=== Second grid <===
  
  
  
  
  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
     f *= exp(-30*x(d)**2);
    return f;
  }
  
  var sol = new LevelSolution(level = level);
  level.initializeSolution(sol, initial_condition);
  //<=== Initialize  solution <===
  
  
  
  //==== Output ====
  level.clawOutput(sol, 0);


}