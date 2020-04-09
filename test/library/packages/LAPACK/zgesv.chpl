use TestHelpers, LAPACK, LAPACK.ClassicLAPACK;

config var verbose_test: bool;

proc LAPACK_zgesv_test(){
  if verbose_test then
    writeln( "LAPACK_zgesv\n===============================" );

  var A = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 6.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7 );
  var X = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 4.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7 ); 
  var B = A*X;
  
  var A_work = new owned LAPACK_Matrix( A );
  var B_work = new owned LAPACK_Matrix( B );
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nX\n", X.toString(), 
            "\nB\n", B_work.toString()
           );
         
  var ipiv: [0..#A.rows] c_int;
  
  if verbose_test then
    writeln( "===============================" );
  
  var n: c_int = A_work.order : c_int;
  var nrhs: c_int = B_work.columns : c_int;
  var a = A_work.data;
  var lda: c_int = A_work.leadingDimension : c_int;
  var b = B_work.data;
  var ldb: c_int = B_work.leadingDimension : c_int;
  var info: c_int = 0 : c_int;
  
  LAPACK_zgesv( n, nrhs, a, lda, ipiv, b, ldb, info );
  
  A_work.data = a;
  B_work.data = b;
  
  var B_test = A*B_work;
  if verbose_test then
    writeln(  "A\n", A.toString(), 
            "\nX\n", B_work.toString(),
            "\nB\n", B_test.toString()
           );
         
  
  var isSame = X == B_work && B == B_test ;
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_zgesv_row_major_test(){
  if verbose_test then
    writeln( "LAPACKE_zgesv_row_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.row_major, input_array = [ ( 5.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 6.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7  );
  var X = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.row_major, input_array = [ ( 3.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 4.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7  ); 
  var B = A*X;
  
  var A_work = new owned LAPACK_Matrix( A );
  var B_work = new owned LAPACK_Matrix( B );
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nX\n", X.toString(), 
            "\nB\n", B_work.toString()
           );
         
  var ipiv: [0..#A.rows] c_int;
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_zgesv( A_work.storage_order, // matrix_order
                   A_work.order : c_int, // N
                   B_work.columns : c_int, // NRHS
                   A_work.data, // A
                   A_work.leadingDimension : c_int, // LDA
                   ipiv, // IPIV
                   B_work.data, // B
                   B_work.leadingDimension : c_int // LDB
                 );

  var B_test = A*B_work;
  if verbose_test then
    writeln(  "A\n", A.toString(), 
            "\nX\n", B_work.toString(),
            "\nB\n", B_test.toString()
           );
         
  
  var isSame = X == B_work && B == B_test ;
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_zgesv_col_major_test(){
  if verbose_test then
    writeln( "LAPACKE_zgesv_col_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 6.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7 );
  var X = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 4.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major , error = 1e-7); 
  var B = A*X;
  
  var A_work = new owned LAPACK_Matrix( A );
  var B_work = new owned LAPACK_Matrix( B );
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nX\n", X.toString(), 
            "\nB\n", B_work.toString()
           );
         
  var ipiv: [0..#A.rows] c_int;
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_zgesv( A_work.storage_order, // matrix_order
                   A_work.order : c_int, // N
                   B_work.columns : c_int, // NRHS
                   A_work.data, // A
                   A_work.leadingDimension : c_int, // LDA
                   ipiv, // IPIV
                   B_work.data, // B
                   B_work.leadingDimension : c_int // LDB
                 );

  var B_test = A*B_work;
  if verbose_test then
    writeln(  "A\n", A.toString(), 
            "\nX\n", B_work.toString(),
            "\nB\n", B_test.toString()
           );
         
  
  var isSame = X == B_work && B == B_test ;
  return ( info == 0 && isSame, info, "" );
}

proc gesv_row_major_test(){
  if verbose_test then
    writeln( "gesv_row_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.row_major, input_array = [ ( 5.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 6.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7  );
  var X = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.row_major, input_array = [ ( 3.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 4.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7  ); 
  var B = A*X;
  
  var A_work = new owned LAPACK_Matrix( A );
  var B_work = new owned LAPACK_Matrix( B );
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nX\n", X.toString(), 
            "\nB\n", B_work.toString()
           );
         
  var ipiv: [0..#A.rows] c_int;
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gesv( A_work.storage_order, // matrix_order
           A_work.data, // A
           ipiv, // IPIV
           B_work.data // B
         );

  var B_test = A*B_work;
  if verbose_test then
    writeln(  "A\n", A.toString(), 
            "\nX\n", B_work.toString(),
            "\nB\n", B_test.toString()
           );
         
  
  var isSame = X == B_work && B == B_test ;
  return ( info == 0 && isSame, info, "" );
}

proc gesv_col_major_test(){
  if verbose_test then
    writeln( "gesv_col_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 6.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7 );
  var X = new owned LAPACK_Matrix( complex(128), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(128), ( 1.0 + 1.0i ) : complex(128), ( 4.0 + 1.0i ) : complex(128), ( 2.0 + 1.0i ) : complex(128) ], input_array_order = lapack_memory_order.row_major, error = 1e-7 ); 
  var B = A*X;
  
  var A_work = new owned LAPACK_Matrix( A );
  var B_work = new owned LAPACK_Matrix( B );
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nX\n", X.toString(), 
            "\nB\n", B_work.toString()
           );
         
  var ipiv: [0..#A.rows] c_int;
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gesv( A_work.storage_order, // matrix_order
           A_work.data, // A
           ipiv, // IPIV
           B_work.data // B
         );

  var B_test = A*B_work;
  if verbose_test then
    writeln(  "A\n", A.toString(), 
            "\nX\n", B_work.toString(),
            "\nB\n", B_test.toString()
           );
         
  
  var isSame = X == B_work && B == B_test ;
  return ( info == 0 && isSame, info, "" );
}

proc main(){
  
  var LAPACK_zgesv_result = LAPACK_zgesv_test();
  var LAPACKE_zgesv_row_major_result = LAPACKE_zgesv_row_major_test();
  var LAPACKE_zgesv_col_major_result = LAPACKE_zgesv_col_major_test();
  var gesv_row_major_result = gesv_row_major_test();
  var gesv_col_major_result = gesv_col_major_test();
  
  writeln( "LAPACK_zgesv ", if LAPACK_zgesv_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_zgesv_row_major ", if LAPACKE_zgesv_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_zgesv_col_major ", if LAPACKE_zgesv_col_major_result[0] then "passed" else "FAILED!" );
  writeln( "gesv_row_major ", if gesv_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "gesv_col_major ", if gesv_col_major_result[0] then "passed" else "FAILED!" );

}
