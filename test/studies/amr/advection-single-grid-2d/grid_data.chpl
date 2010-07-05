module grid_data {
  
  param dimension = 2;

  //===> Description ===>
  //
  // Class definition for a rectangular grid of arbitrary dimension.
  //
  //<=== Description <===


  //===============================================>
  //===> Definition of class TwoDimensionalGrid ===>
  //===============================================>
  class TwoDimensionalGrid {
    
    const dimensions: domain(1) = [1..dimension];
    
    // var domain2_2tuple = ([1..0,1..0], [1..0,1..0]);
    
    var lower_corner: [dimensions] real,
        upper_corner: [dimensions] real,
        num_cells:    [dimensions] int,
        num_ghost_cells: int;

    var dx: [dimensions] real;
            
    var all_cells:      domain(dimension),
        physical_cells: subdomain(all_cells);
    
    var interfaces: [dimensions] domain(dimension);
    // 
    // var lower_ghost_cells = domain2_2tuple,
    //     upper_ghost_cells = domain2_2tuple;
    // 
    // 
    //     left_ghost_cells:                     subdomain(all_cells),
    //     right_ghost_cells:                    subdomain(all_cells),
    //     bottom_ghost_cells:                   subdomain(all_cells),
    //     top_ghost_cells:                      subdomain(all_cells),
    //     periodic_image_of_left_ghost_cells:   subdomain(all_cells),
    //     periodic_image_of_right_ghost_cells:  subdomain(all_cells),
    //     periodic_image_of_bottom_ghost_cells: subdomain(all_cells),
    //     periodic_image_of_top_ghost_cells:    subdomain(all_cells);
    //   
    // var x_interfaces: domain(dimension),
    //     y_interfaces: domain(dimension);

    //=======================================>
    //===> TwoDimensionalGrid constructor ===>
    def TwoDimensionalGrid(lower_corner_input: dimension*real,
                           upper_corner_input: dimension*real,
                           num_cells_input:    dimension*int,
                           num_ghost_cells_input:        int)
    {
      //---- Core parameters ----
      lower_corner    = lower_corner_input;
      upper_corner    = upper_corner_input;
      num_cells       = num_cells_input;
      num_ghost_cells = num_ghost_cells_input;
      dx = (upper_corner - lower_corner) / num_cells;
      
      //---- Cell domains ----
      physical_cells = [1..#num_cells(1), 1..#num_cells(2)];
      all_cells      = physical_cells.expand((num_ghost_cells, num_ghost_cells));
             
      // ---- Interface domains ----
      interfaces(1) = [all_cells.dim(1).low + 1  ..  all_cells.dim(1).high,
                       all_cells.dim(2)];
                       
      interfaces(2) = [all_cells.dim(1),
                       all_cells.dim(2).low + 1  ..  all_cells.dim(2).high];
      
      var size_array: [dimensions] int,
          size_tuple: dimension*int;
    
      for orientation in dimensions do {
        size_array = num_cells + 2*num_ghost_cells;
        size_array(orientation) -= 1;
        [d in dimensions] size_tuple(d) = size_array(d);
        interfaces(orientation) = all_cells.interior(size_tuple);
       }
      
      // //---- Ghost cells and related domains ----
      // lower_ghost_cells(1) = [all_cells.dim(1).low .. #num_ghost_cells,
      //                         physical_cells.dim(2)];
      //                     
      // upper_ghost_cells(1)  = [physical_cells.dim(1).high + 1  ..  #num_ghost_cells,
      //                          physical_cells.dim(2)];
      // 
      // lower_ghost_cells(2) = [physical_cells.dim(1),
      //                         all_cells.dim(2).low .. #num_ghost_cells];
      //                       
      // upper_ghost_cells(2) = [physical_cells.dim(1),
      //                         physical_cells.dim(2).high + 1  ..  #num_ghost_cells];
                            
      // periodic_image_of_left_ghost_cells   = [physical_cells.dim(1).high - num_ghost_cells + 1  ..  #num_ghost_cells,
      //                                         physical_cells.dim(2)];
      // 
      // periodic_image_of_right_ghost_cells  = [physical_cells.dim(1).low  ..  #num_ghost_cells,
      //                                         physical_cells.dim(2)];
      //                                        
      // periodic_image_of_bottom_ghost_cells = [physical_cells.dim(1),
      //                                         physical_cells.dim(2).high - num_ghost_cells + 1  ..  #num_ghost_cells];
      //                                         
      // periodic_image_of_top_ghost_cells    = [physical_cells.dim(1),
      //                                         physical_cells.dim(2).low  ..  #num_ghost_cells];
    }
    //<=== TwoDimensionalGrid constructor <===
    //<=======================================



    // //=============================================>
    // //===> Generate coordinates of cell centers ===>
    // def cell_center_coordinates (cells: subdomain(physical_cells)) {
    // 
    //   var coordinates: [cells] 2*real;
    //   var real_cell: 2*real;
    // 
    //   for cell in cells do {
    //     // for d in dimensions do
    //     //   real_cell(d) = cell(d):real - 0.5;
    //       
    //     coordinates(cell) = lower_corner;
    //     // Ideally: coordinates(cell) = lower_corner + (cell:real - 0.5)*dx
    //   }
    //   
    //   return coordinates;
    // }
    // //<=== Generate coordinates of cell centers <===
    // //<=============================================

  }
  //<===============================================
  //<=== Definition of class OneDimensionalGrid <===
  //<===============================================
  
  
  
  
  //=========================================>
  //===> Definition of class GridFunction ===>
  //=========================================>
  class GridFunction {
    var parent_grid: TwoDimensionalGrid;
    var value: [parent_grid.all_cells] real;
    var time: real;
  }
  //<=========================================
  //<=== Definition of class GridFunction ====
  //<=========================================
  
  
  
  def main {
    

    
    var G = new TwoDimensionalGrid((0.0, 0.0), 
                                   (1.0, 1.0),
                                   (8, 10), 
                                   2);

    writeln("lower_corner = ", G.lower_corner);
    writeln("upper_corner = ", G.upper_corner);
    writeln("num_cells    = ", G.num_cells);
    writeln("dx           = ", G.dx);
    
    writeln("physical_cells = ", G.physical_cells);
    writeln("all_cells      = ", G.all_cells);
    writeln("interfaces(1)  = ", G.interfaces(1));
    writeln("interfaces(2)  = ", G.interfaces(2));
    // 
    // var D = G.interfaces(1);
    // writeln(D);
    // writeln(G.interfaces(1)(-10..2,1..1));
    
    // var coordinates = G.cell_center_coordinates(G.physical_cells);
    // 
    // writeln(coordinates);


  } // end main
  


} // end module grid_data


  // //=========================================>
  // //===> Definition of class GridFunction ===>
  // //=========================================>
  // class GridFunction {
  //   var parent_grid: Grid;
  //   var value: [parent_grid.all_cells] real;
  //   var time: real;
  // }
  // //<=========================================
  // //<=== Definition of class GridFunction ====
  // //<=========================================
