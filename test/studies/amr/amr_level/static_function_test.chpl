use amr_level_defs;



def main {
  
  var x_low, x_high: dimension*real,
    n_cells, n_ghost: dimension*int;


  [d in dimensions] {
    x_low(d)    = -0.4;
    x_high(d)   =  0.4;
    n_cells(d)  = 40;
    n_ghost(d)  = 2;
  }

  var level = new AMRLevel(x_low, x_high, n_cells, n_ghost);


  var i_low, i_high: dimension*int;
  // var x_low, x_high: dimension*real;
  
  //===> First grid ===>
  for d in dimensions {
    if d == 1 then {
      i_low(d)  = 40;      
      i_high(d) = 80;
      
      x_low(d)  = 0.0;
      x_high(d) = 0.4;
    }
    else {
      i_low(d)  = 20;
      i_high(d) = 80;
      
      x_low(d)  = -0.1;
      x_high(d) = 0.4;
    }
  }
  
  level.add_grid(x_low, x_high);
  //<=== First grid <===
  
  
  //===> Second grid ===>
  for d in dimensions {
    if d == 1 then {
      i_low(d)  = 0;
      i_high(d) = 40;
      
      x_low(d)  = -0.4;
      x_high(d) = 0.0;
    }
    else {
      i_low(d)  = 0;
      i_high(d) = 60;
      
      x_low(d)  = -0.4;
      x_high(d) = 0.1;
    }
  }
  
  level.add_grid(x_low, x_high);
  //<=== Second grid <===
  



  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*x(d)**2);
    return f;
  }

  var q = level.make_level_function(initial_condition);
  //<=== Initialize  solution <===



  //==== Output ====
  level.claw_output(q, 0);



  //==== Print grids ====
  var grid_num = 0;
  for grid in level.grids {
    grid_num += 1;
    writeln("");
    writeln("Grid number ", grid_num);
    writeln("==============");
    writeln("x_low     = ", grid.x_low);
    writeln("x_high    = ", grid.x_high);
    writeln("i_low     = ", grid.i_low);
    writeln("i_high    = ", grid.i_high);    
    writeln("n_cells       = ", grid.n_cells);
    writeln("n_ghost_cells = ", grid.n_ghost_cells);
  }


}