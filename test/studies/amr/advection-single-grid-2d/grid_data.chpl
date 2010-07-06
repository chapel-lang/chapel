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
    
    var lower_ghost_cells: [dimensions] domain(dimension),
        upper_ghost_cells: [dimensions] domain(dimension);
    
    
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
             

      //---- Orientation-dependent domains ----
      var size: dimension*int;
    
      for orientation in dimensions do {
        //---- Cell-to-cell interfaces ----
        [d in dimensions] size(d) = num_cells(d) + 2*num_ghost_cells;
        size(orientation) -= 1;
        interfaces(orientation) = all_cells.interior(size);
        
        //---- Ghost cells ----
        [d in dimensions] size(d) = 0;
        size(orientation) = -num_ghost_cells;
        lower_ghost_cells(orientation) = physical_cells.exterior(size);
        size(orientation) = num_ghost_cells;
        upper_ghost_cells(orientation) = physical_cells.exterior(size);        
      }                            
    }
    //<=== TwoDimensionalGrid constructor <===
    //<=======================================



    //=============================================>
    //===> Generate coordinates of cell centers ===>
    def cell_center_coordinates (cells: subdomain(physical_cells)) {
    
      var coordinates: [cells] dimension*real;
    
      forall (cell,d) in [cells,dimensions] do
        coordinates(cell)(d) = lower_corner(d) + (cell(d):real - 0.5)*dx(d);

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
    var G = new TwoDimensionalGrid((0.0, 0.0),  (1.0, 1.0),  (8, 10),  2);

    writeln("lower_corner = ", G.lower_corner);
    writeln("upper_corner = ", G.upper_corner);
    writeln("num_cells    = ", G.num_cells);
    writeln("dx           = ", G.dx);
    writeln(" ");
    writeln("physical_cells = ", G.physical_cells);
    writeln("all_cells      = ", G.all_cells);
    writeln("interfaces(1)  = ", G.interfaces(1));
    writeln("interfaces(2)  = ", G.interfaces(2));
    writeln(" ");
    writeln("lower_ghost_cells(1) = ", G.lower_ghost_cells(1));
    writeln("upper_ghost_cells(1) = ", G.upper_ghost_cells(1));
    writeln("lower_ghost_cells(2) = ", G.lower_ghost_cells(2));
    writeln("upper_ghost_cells(2) = ", G.upper_ghost_cells(2));



  } // end main
  

} // end module grid_data
