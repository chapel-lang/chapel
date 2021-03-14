module locality_info_strided {

  proc my_local_cyclic_data_strided 
    ( A_domain, A_grid_domain, processor : 2*int ) {

    // general code, for arbitrary (unrelated) row and column index ranges 

    const (r, c)           = A_grid_domain.high + (1, 1);

    const my_processor_row = processor (0);
    const my_processor_col = processor (1);

    const row_stride       = A_domain.dim (0).stride;
    const col_stride       = A_domain.dim (1).stride;

    // this processor owns data with indices in the tensor product
    // [ my_rows, my_cols ];

    const my_rows = A_domain.dim(0).low + my_processor_row*row_stride .. 
                    A_domain.dim(0).high  by  r*row_stride;
    const my_cols = A_domain.dim(1).low + my_processor_col*col_stride .. 
                    A_domain.dim(1).high  by  c*col_stride;

    // rows it computes of L21

    const my_L21_rows_to_compute =
      A_domain.dim(0).low  +  
             my_processor_row*row_stride  +  r*row_stride*my_processor_col .. 
      A_domain.dim(0).high  by  r*c*row_stride;

    //    writeln ( processor, "  my_rows:", my_rows, "\n" +
    //	      "     my_cols: ", my_cols,  "\n" +
    //	      "     my_L21_rows_to_compute: ", my_L21_rows_to_compute );
    
    return ( my_rows, my_cols, my_L21_rows_to_compute );
  }
}