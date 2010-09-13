use amr_base_defs;
use amr_solution_defs;



def LevelInterface.printC2FClusters() {
  for (coarse_grid,fine_grid) in [coarse_grids,fine_grids] {
    var clusters = c2f_clusters(coarse_grid)(fine_grid);
    writeln("Coarse grid:");
    writeln(coarse_grid);
    writeln("Fine grid: ");
    writeln(fine_grid);
    for coarse_cell in clusters.coarse_cells {
	writeln("Coarse cell: ", coarse_cell);
	writeln("Fine cells:  ", clusters.fine_cells(coarse_cell));
    }
    writeln("");

  }
}


def LevelInterface.printF2CCells() {
  for (coarse_grid,fine_grid) in [coarse_grids,fine_grids] {

    var coarse_cells = f2c_coarse_cells(coarse_grid)(fine_grid);

    writeln("Coarse grid: ");
    writeln(coarse_grid);
    writeln("");
    writeln("Fine grid: ");
    writeln(fine_grid);
    writeln("");
    writeln("F2C coarse cells:", coarse_cells);
    writeln("F2C fine cells  :", refine(coarse_cells));
    writeln("\n");

  }
}



def initial_condition ( x: dimension*real ) {
  var f: real = 1.0;
  for d in dimensions do
    f *= exp(-10 * (x(d) + 0.0)**2);
  return f;
}






def main {

  var hierarchy = hierarchyFromInputFile("space.txt");
  var amr_solution = new AMRSolution(hierarchy = hierarchy);
  
  amr_solution.setToFunction(initial_condition, 0.3);
  amr_solution.clawOutput(0);

  //==== Test LevelInterface ====
  var interface = hierarchy.fine_interfaces(hierarchy.top_level);

  // interface.printC2FClusters();
  interface.printF2CCells();

}



/*   //==== Test hierarchy.boundary_ghosts ==== */
/*   var level = hierarchy.bottom_level; */
/*   for grid in level.grids { */
/*     writeln("x_low = ", grid.x_low, ",  x_high = ", grid.x_high); */
/*     writeln("i_low = ", grid.i_low, ",  i_high = ", grid.i_high); */
/*     for d in dimensions { */
/*       writeln("Dimension ", d); */
/*       writeln("-----------"); */
/*       writeln("Low: ",  hierarchy.boundary_ghosts(level)(grid).low(d)); */
/*       writeln("High: ", hierarchy.boundary_ghosts(level)(grid).high(d)); */
/*       writeln(""); */
/*     } */
/*     writeln(""); */
/*   } */