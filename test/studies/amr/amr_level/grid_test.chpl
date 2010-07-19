
  //===> Description ===>
  //
  // Runs a series of simple I/O tests to verify basic
  // functionality of the RectangularGrid data structure.
  //
  //<=== Description <===


use grid_base_defs;



def main {

  var lower_corner, upper_corner: dimension*real,
    low_index:                    dimension*int,
    num_cells, num_ghost_cells: dimension*int;

  for d in [1..dimension] {
    lower_corner(d) = 0.0;
    upper_corner(d) = 1.0;
    low_index(d) = 2*d;
    num_cells(d)    = 5; //10 * 2**(d-1);
    num_ghost_cells(d) = 2;
  }

			
  var AG: [1..3] RectangularGrid;
  
  AG(1) = new RectangularGrid(lower_corner, upper_corner, low_index,
    			                    num_cells, num_ghost_cells);
  
    
  var coordinates: [AG(1).physical_cells] dimension*real;
  
  writeln("");
  writeln("Core parameters:");
  writeln("----------------");
  writeln("low_coord       = ", AG(1).low_coord);
  writeln("high_coord      = ", AG(1).high_coord);
  writeln("low_index       = ", AG(1).low_index);
  writeln("high_index      = ", AG(1).high_index);    
  writeln("num_cells       = ", AG(1).num_cells);
  writeln("num_ghost_cells = ", AG(1).num_ghost_cells);
  writeln("dx              = ", AG(1).dx);
    

  
    // 
    // 
    // writeln("");
    // writeln("Cell domains:");
    // writeln("-------------");
    // writeln("physical_cells = ", AG(1).physical_cells);
    // writeln("all_cells      = ", AG(1).all_cells);
    // 
    // 
    // writeln("");
    // writeln("Interface domains:");
    // writeln("------------------");
    // for d in dimensions do
    //   writeln("interfaces(", d, ") = ", AG(1).interfaces(d));
    // 
    // 
    // writeln("");
    // writeln("Ghost cells:");
    // writeln("-------------------");
    // for d in dimensions do {
    //   writeln("lower_ghost_cells(", d, ") = ", AG(1).lower_ghost_cells(d));
    //   writeln("upper_ghost_cells(", d, ") = ", AG(1).upper_ghost_cells(d));
    // }
    // 
    // writeln("");
    // writeln("Periodic images of ghost cells:");
    // writeln("-------------------");
    // for d in dimensions do {
    //   writeln("periodic_image_of_lower_ghost_cells(", d, ") = ", AG(1).periodic_image_of_lower_ghost_cells(d));
    //   writeln("periodic_image_of_upper_ghost_cells(", d, ") = ", AG(1).periodic_image_of_upper_ghost_cells(d));
    // }


/*     writeln(""); */
/*     writeln("Cell indices in column-major order:"); */
/*     writeln("-----------------------------------"); */
/*     var physical_cells_transposed: domain(dimension), */
/*         shape: dimension*int; */
      
/*     [d in dimensions] shape(d) = AG(1).num_cells(1 + dimension - d); */
/*     physical_cells_transposed = physical_cells_transposed.exterior(shape); */

/*     if dimension == 1 then { */
/*       for cell in AG(1).physical_cells do */
/*       writeln(cell); */
/*     } */
/*     else { */
/*       var cell: dimension*int; */
/*       for cell_transposed in physical_cells_transposed do { */
/*         [d in dimensions] cell(d) = cell_transposed(1 + dimension - d); */
/*         writeln(cell); */
/*       } */
/*     } */


    // writeln("");
    // writeln("Cell center coordinates:");
    // writeln("------------------------");
    // coordinates = AG(1).cell_center_coordinates(AG(1).physical_cells);
    // writeln(coordinates);



/*     writeln(""); */
/*     writeln("Testing function evaluation:"); */
/*     writeln("----------------------------"); */
/*     def fcn1( (x): 1*real) { */
/*       return 2.0*x; */
/*     } */
/*     def fcn2( (x,y): 2*real) { */
/*       return 2.0*x + y; */
/*     } */
/*     var f = fcn2; */
/*     coordinates = AG(1).cell_center_coordinates(AG(1).physical_cells); */
/*     var q = AG(1).evaluate(f); */
/*     writeln(q.value); */


    

    
/*     AG(1).clawpack_output(q, 0); */


  //     def tupleize(dom: domain(dimension)) {
  //       var el_tuple: dimension*int;
  // 
  //       for el in dom {
  // if dom.rank == 1 then {
  //   el_tuple(1) = el;
  //   yield el_tuple;
  // }
  // else 
  //   yield el;
  //       }
  //     }

    
/*     writeln(""); */
/*     writeln("Testing cell-->interface location:"); */
/*     writeln("----------------------------------"); */
/*     for cell in tupleize(AG(1).physical_cells) do { */
/*       for d in dimensions do { */
/* 	write("lower_interface(", cell, ",", d, ") = "); */
/* 	write(AG(1).lower_interface(cell,d), "\n"); */
/* 	write("upper_interface(", cell, ",", d, ") = "); */
/* 	write(AG(1).upper_interface(cell,d), "\n"); */
/*       } */
/*       write("\n"); */
/*     } */

/*     writeln(""); */
/*     writeln("Testing interface-->cell location:"); */
/*     writeln("----------------------------------"); */
/*     for d in dimensions do { */
/*       for interface in tupleize(AG(1).interfaces(d)) do { */
/* 	for d_cell in dimensions do { */
/* 	write("lower_cell(", interface, ",", d, ") = "); */
/* 	write(AG(1).lower_cell(interface,d), "\n"); */
/* 	write("upper_cell(", interface, ",", d, ") = "); */
/* 	write(AG(1).upper_cell(interface,d), "\n"); */
/* 	} */
/* 	write("\n"); */
/*       } */
/*     } */



} // end main


