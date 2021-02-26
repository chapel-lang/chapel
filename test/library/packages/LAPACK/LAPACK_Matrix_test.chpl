use TestHelpers;

config var verbose_test: bool;

proc putInto2D( arrayOfArrays: [?D1] ?t, twoArray: [?D2] ?o): void{
  // no check. Cannot have ragged array literals
  assert( D1.rank == 1 && D2.rank == 2  );
  var formed: domain(2) = {D1.dim(0), arrayOfArrays[D1.dim(0).low].domain.dim(0)};
  assert( formed.dim(0).size == D2.dim(0).size && formed.dim(1).size == D2.dim(1).size );
  var trans = ( D2.dim(0).low - formed.dim(0).low, D2.dim(1).low - formed.dim(1).low ); 
  
  for (i,j) in D2 {
    twoArray[i,j] = arrayOfArrays[i+trans[0]][j+trans[1]];
  }

}

writeln( "1D row input => row" );
var input_1_row: [1..4] real = 
  [ 3.0, 1.0, 
    4.0, 2.0 ];
var A_1_row = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.row_major, input_array = input_1_row  ); 
writeln( A_1_row.data, "\n", A_1_row.toString(), "\n" );

writeln( "1D col input => col" );
var input_1_col: [1..4] real  = 
  [ 3.0, 4.0,   // tranpose of [ 3.0, 1.0,
    1.0, 2.0 ]; //                 4.0, 2.0 ];
var A_1_col = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.column_major, input_array = input_1_col  );  
writeln( A_1_col.data, "\n", A_1_col.toString(), "\n" );

writeln( "1D col input => row" );
var A_1_row_f_col = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.row_major, input_array = input_1_col,  input_array_order = lapack_memory_order.column_major ); 
writeln( A_1_row_f_col.data, "\n", A_1_row_f_col.toString(), "\n" );

writeln( "1D row input => col" );
var A_1_col_f_row = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.column_major, input_array = input_1_row, input_array_order = lapack_memory_order.row_major );  
writeln( A_1_col_f_row.data, "\n", A_1_col_f_row.toString(), "\n" );

// 1.5D data
var input_1_5_row1: [1..2] real = [3.0, 1.0];
var input_1_5_row2: [1..2] real = [4.0, 2.0];
var input_1_5_row: [1..2][1..2] real =
  [ input_1_5_row1,
    input_1_5_row2 ];
//var A_1_5_row = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.row_major, input_array = input_1_5_row  ); 

var input_1_5_col1: [1..2] real = [3.0, 4.0];
var input_1_5_col2: [1..2] real = [1.0, 2.0];
var input_1_5_col: [1..2][1..2] real =
  [ input_1_5_col1,
    input_1_5_col2 ];
//var A_1_5_col = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.column_major, input_array = input_1_5_col  ); 


// 2D data
writeln( "2D row input => row" );
var input_2_row: [1..2,1..2] real;
putInto2D( input_1_5_row, input_2_row );
var A_2_row = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.row_major, input_array = input_2_row ); 
writeln( A_2_row.data, "\n", A_2_row.toString(), "\n" );

writeln( "2D col input => col" );
var input_2_col: [1..2,1..2] real;
putInto2D( input_1_5_col, input_2_col );
var A_2_col = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.column_major, input_array = input_2_col ); 
writeln( A_2_col.data, "\n", A_2_col.toString(), "\n" );

writeln( "2D col input => row" );
putInto2D( input_1_5_col, input_2_row );
var A_2_row_f_col = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.row_major, input_array = input_2_row, input_array_order = lapack_memory_order.column_major ); 
writeln( A_2_row_f_col.data, "\n", A_2_row_f_col.toString(), "\n" );

writeln( "2d row input => col" );
putInto2D( input_1_5_row, input_2_col );
var A_2_col_f_row = new owned LAPACK_Matrix( real, 2, 2, lapack_memory_order.column_major, input_array = input_2_col, input_array_order = lapack_memory_order.row_major  ); 
writeln( A_2_col_f_row.data, "\n", A_2_col_f_row.toString(), "\n" );


writeln( A_1_row == A_1_col &&
         A_1_col == A_1_row_f_col &&
         A_1_row_f_col == A_1_col_f_row &&
         A_1_col_f_row  == A_2_row &&
         A_2_row == A_2_col &&
         A_2_col == A_2_row_f_col &&
         A_2_row_f_col == A_2_col_f_row );
