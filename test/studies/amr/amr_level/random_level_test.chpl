use amr_level_defs;
use Random;


def main {
  
  var x_low, x_high: dimension*real,
    n_cells, n_ghost: dimension*int;

  [d in dimensions] {
    x_low(d)  = 0.0;
    x_high(d) = 1.0;
    n_cells(d)  = 20 * 2**d;
    n_ghost(d)  = 2;
  }

  var level = new AMRLevel(x_low, x_high, n_cells, n_ghost);


  var random_real: [1..2] real;
  var random_int:  [1..2] int;
  var i_low, i_high: dimension*int;



  //===> Create random grids on level ===>
  var n_grids = 10;

  for n in [1..n_grids] {

    //===> Generate random grid indices ===>
    for d in dimensions {
      //==== Generate random ints for grid index bounds ===
      fillRandom(random_real);
      for i in [1..2] do 
	random_int(i) = 2*round(random_real(i) / level.dx(d)):int;

      //==== Space out the ints if they're equal ====
      if random_int(1) == random_int(2) {
	if random_int(1) == 0 then
	  random_int(2) += 2;
	else
	  random_int(1) -= 2;
      }

      //==== Assign i_low and i_high ====
      if random_int(1) < random_int(2) {
	i_low(d)  = random_int(1);
	i_high(d) = random_int(2);
      }
      else {
	i_low(d)  = random_int(2);
	i_high(d) = random_int(1);
      }
      //<=== Generate random grid indices <===

    }

    //==== Add grid to level's list ====
    level.add_grid(i_low, i_high);

  }
  //<=== Create random grids on level



  var grid_num = 0;
  for grid in level.grids {
    grid_num += 1;
    writeln("");
    writeln("Grid number ", grid_num);
    writeln("==============");
    writeln("x_low     = ", grid.x_low);
    writeln("x_high    = ", grid.x_high);
    writeln("n_cells       = ", grid.n_cells);
    writeln("n_ghost_cells = ", grid.n_ghost_cells);
  }


}