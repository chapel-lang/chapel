use TestHelpers, LAPACK, LAPACK.ClassicLAPACK;

config var verbose_test: bool;

proc LAPACK_cgesv_test(){
  if verbose_test then
    writeln( "LAPACK_cgesv\n===============================" );

  var A = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 6.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 );
  var X = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 4.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 ); 
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
  
  LAPACK_cgesv( n, nrhs, a, lda, ipiv, b, ldb, info );
  
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

proc LAPACKE_cgesv_row_major_test(){
  if verbose_test then
    writeln( "LAPACKE_cgesv_row_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.row_major, input_array = [ ( 5.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 6.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6  );
  var X = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.row_major, input_array = [ ( 3.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 4.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6  ); 
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
    LAPACKE_cgesv( A_work.storage_order, // matrix_order
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

proc LAPACKE_cgesv_col_major_test(){
  if verbose_test then
    writeln( "LAPACKE_cgesv_col_major\n===============================" );

  var A = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 6.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 );
  var X = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 4.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 ); 
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
    LAPACKE_cgesv( A_work.storage_order, // matrix_order
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

  var A = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.row_major, input_array = [ ( 5.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 6.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6  );
  var X = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.row_major, input_array = [ ( 3.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 4.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6  ); 
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

  var A = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 5.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 6.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 );
  var X = new owned LAPACK_Matrix( complex(64), 2, 2, lapack_memory_order.column_major, input_array = [ ( 3.0 + 1.0i ) : complex(64), ( 1.0 + 1.0i ) : complex(64), ( 4.0 + 1.0i ) : complex(64), ( 2.0 + 1.0i ) : complex(64) ], input_array_order = lapack_memory_order.row_major, error = 10e-6 ); 
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
  
  var LAPACK_cgesv_result = LAPACK_cgesv_test();
  var LAPACKE_cgesv_row_major_result = LAPACKE_cgesv_row_major_test();
  var LAPACKE_cgesv_col_major_result = LAPACKE_cgesv_col_major_test();
  var gesv_row_major_result = gesv_row_major_test();
  var gesv_col_major_result = gesv_col_major_test();
  
  writeln( "LAPACK_cgesv ", if LAPACK_cgesv_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_cgesv_row_major ", if LAPACKE_cgesv_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_cgesv_col_major ", if LAPACKE_cgesv_col_major_result[0] then "passed" else "FAILED!" );
  writeln( "gesv_row_major ", if gesv_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "gesv_col_major ", if gesv_col_major_result[0] then "passed" else "FAILED!" );

}
