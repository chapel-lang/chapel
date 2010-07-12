module grid_test {

  //===> Description ===>
  //
  // Runs a series of simple I/O tests to verify basic
  // functionality of the RectangularGrid data structure.
  //
  //<=== Description <===


  use grid_class;



  def main {

    var lower_corner, upper_corner: dimension*real,
      num_cells, num_ghost_cells: dimension*int;

    for d in [1..dimension] {
      lower_corner(d) = 0.0;
      upper_corner(d) = 1.0;
      num_cells(d)    = 5; //10 * 2**(d-1);
      num_ghost_cells(d) = 2;
    }

    var G = new RectangularGrid(lower_corner, upper_corner,
				num_cells, num_ghost_cells);

    var coordinates: [G.physical_cells] dimension*real;

    writeln("");
    writeln("Core parameters:");
    writeln("----------------");
    writeln("lower_corner    = ", G.lower_corner);
    writeln("upper_corner    = ", G.upper_corner);
    writeln("num_cells       = ", G.num_cells);
    writeln("num_ghost_cells = ", G.num_ghost_cells);
    writeln("dx              = ", G.dx);


    writeln("");
    writeln("Cell domains:");
    writeln("-------------");
    writeln("physical_cells = ", G.physical_cells);
    writeln("all_cells      = ", G.all_cells);


    writeln("");
    writeln("Interface domains:");
    writeln("------------------");
    for d in dimensions do
      writeln("interfaces(", d, ") = ", G.interfaces(d));


    writeln("");
    writeln("Ghost cells:");
    writeln("-------------------");
    for d in dimensions do {
      writeln("lower_ghost_cells(", d, ") = ", G.lower_ghost_cells(d));
      writeln("upper_ghost_cells(", d, ") = ", G.upper_ghost_cells(d));
    }

    writeln("");
    writeln("Periodic images of ghost cells:");
    writeln("-------------------");
    for d in dimensions do {
      writeln("periodic_image_of_lower_ghost_cells(", d, ") = ", G.periodic_image_of_lower_ghost_cells(d));
      writeln("periodic_image_of_upper_ghost_cells(", d, ") = ", G.periodic_image_of_upper_ghost_cells(d));
    }


/*     writeln(""); */
/*     writeln("Cell indices in column-major order:"); */
/*     writeln("-----------------------------------"); */
/*     var physical_cells_transposed: domain(dimension), */
/*         shape: dimension*int; */
      
/*     [d in dimensions] shape(d) = G.num_cells(1 + dimension - d); */
/*     physical_cells_transposed = physical_cells_transposed.exterior(shape); */

/*     if dimension == 1 then { */
/*       for cell in G.physical_cells do */
/*       writeln(cell); */
/*     } */
/*     else { */
/*       var cell: dimension*int; */
/*       for cell_transposed in physical_cells_transposed do { */
/*         [d in dimensions] cell(d) = cell_transposed(1 + dimension - d); */
/*         writeln(cell); */
/*       } */
/*     } */


    writeln("");
    writeln("Cell center coordinates:");
    writeln("------------------------");
    coordinates = G.cell_center_coordinates(G.physical_cells);
    writeln(coordinates);



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
/*     coordinates = G.cell_center_coordinates(G.physical_cells); */
/*     var q = G.evaluate(f); */
/*     writeln(q.value); */


    

    
/*     G.clawpack_output(q, 0); */


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
/*     for cell in tupleize(G.physical_cells) do { */
/*       for d in dimensions do { */
/* 	write("lower_interface(", cell, ",", d, ") = "); */
/* 	write(G.lower_interface(cell,d), "\n"); */
/* 	write("upper_interface(", cell, ",", d, ") = "); */
/* 	write(G.upper_interface(cell,d), "\n"); */
/*       } */
/*       write("\n"); */
/*     } */

/*     writeln(""); */
/*     writeln("Testing interface-->cell location:"); */
/*     writeln("----------------------------------"); */
/*     for d in dimensions do { */
/*       for interface in tupleize(G.interfaces(d)) do { */
/* 	for d_cell in dimensions do { */
/* 	write("lower_cell(", interface, ",", d, ") = "); */
/* 	write(G.lower_cell(interface,d), "\n"); */
/* 	write("upper_cell(", interface, ",", d, ") = "); */
/* 	write(G.upper_cell(interface,d), "\n"); */
/* 	} */
/* 	write("\n"); */
/*       } */
/*     } */



  } // end main


}