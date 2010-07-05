module grid_data {

  //===> Description ===>
  //
  // Class definition for a rectangular grid of arbitrary dimension.
  //
  //<=== Description <===


  //===============================================>
  //===> Definition of class TwoDimensionalGrid ===>
  //===============================================>
  class TwoDimensionalGrid {
    
    
    const dimensions: domain(1) = [1..2];
    
    var lower_corner: [dimensions] real,
        upper_corner: [dimensions] real,
        num_cells:    [dimensions] int,
        num_ghost_cells:           int;

    var dx: [dimensions] real;
        
    var all_cells:                            domain(2),
        physical_cells:                       subdomain(all_cells),
        left_ghost_cells:                     subdomain(all_cells),
        right_ghost_cells:                    subdomain(all_cells),
        bottom_ghost_cells:                   subdomain(all_cells),
        top_ghost_cells:                      subdomain(all_cells),
        periodic_image_of_left_ghost_cells:   subdomain(all_cells),
        periodic_image_of_right_ghost_cells:  subdomain(all_cells),
        periodic_image_of_bottom_ghost_cells: subdomain(all_cells),
        periodic_image_of_top_ghost_cells:    subdomain(all_cells);
      
    var x_interfaces: domain(2),
        y_interfaces: domain(2);

    //=======================================>
    //===> TwoDimensionalGrid constructor ===>
    def TwoDimensionalGrid(lower_corner_input: [dimensions] real,
                           upper_corner_input: [dimensions] real,
                           num_cells_input:    [dimensions] int,
                           num_ghost_cells_input:           int)
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
      
      //---- Interface domains ----
      x_interfaces = [all_cells.dim(1).low + 1  ..  all_cells.dim(1).high,
                      all_cells.dim(2)];
                      
      y_interfaces = [all_cells.dim(1),
                      all_cells.dim(2).low + 1  ..  all_cells.dim(2).high];

      //---- Ghost cells and related domains ----
      left_ghost_cells   = [all_cells.dim(1).low .. #num_ghost_cells,
                            physical_cells.dim(2)];
                          
      right_ghost_cells  = [physical_cells.dim(1).high + 1  ..  #num_ghost_cells,
                            physical_cells.dim(2)];
                           
      bottom_ghost_cells = [physical_cells.dim(1),
                            all_cells.dim(2).low .. #num_ghost_cells];
                            
      top_ghost_cells    = [physical_cells.dim(1),
                            physical_cells.dim(2).high + 1  ..  #num_ghost_cells];
                            
      periodic_image_of_left_ghost_cells   = [physical_cells.dim(1).high - num_ghost_cells + 1  ..  #num_ghost_cells,
                                              physical_cells.dim(2)];

      periodic_image_of_right_ghost_cells  = [physical_cells.dim(1).low  ..  #num_ghost_cells,
                                              physical_cells.dim(2)];
                                             
      periodic_image_of_bottom_ghost_cells = [physical_cells.dim(1),
                                              physical_cells.dim(2).high - num_ghost_cells + 1  ..  #num_ghost_cells];
                                              
      periodic_image_of_top_ghost_cells    = [physical_cells.dim(1),
                                              physical_cells.dim(2).low  ..  #num_ghost_cells];
    }
    //<=== TwoDimensionalGrid constructor <===
    //<=======================================



    //=============================================>
    //===> Generate coordinates of cell centers ===>
    def cell_center_coordinates (cells: subdomain(physical_cells)) {

      var coordinates: [cells] 2*real;
      var real_cell: 2*real;

      for cell in cells do {
        // for d in dimensions do
        //   real_cell(d) = cell(d):real - 0.5;
          
        coordinates(cell) = lower_corner;
        // Ideally: coordinates(cell) = lower_corner + (cell:real - 0.5)*dx
      }
      
      return coordinates;
    }
    //<=== Generate coordinates of cell centers <===
    //<=============================================

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
    

    var lower_corner: [1..2] real = (0.0, 0.0),
        upper_corner: [1..2] real = (1.0, 1.0),
        num_cells:    [1..2] int  = (8, 10);
    
    var G = new TwoDimensionalGrid(lower_corner, upper_corner, num_cells, 1);
    
    
    var coordinates = G.cell_center_coordinates(G.physical_cells);
    
    writeln(coordinates);


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
