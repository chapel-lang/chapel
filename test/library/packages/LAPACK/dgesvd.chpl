use TestHelpers, LAPACK, LAPACK.ClassicLAPACK;

config var verbose_test: bool;

proc LAPACKE_dgesvd_row_major_test(){
  if verbose_test then
    writeln( "LAPACKE_dgesvd_row_major_test\n===============================" );
    
  var order: lapack_memory_order = lapack_memory_order.row_major;
  var A = new owned LAPACK_Matrix( real(64), 4, 5, order );
  var U = new owned LAPACK_Matrix( real(64), 4, 4, order ); 
  var S = new owned LAPACK_Matrix( real(64), 4, 5, order ); 
  var V = new owned LAPACK_Matrix( real(64), 5, 5, order );
  
  A.populateFromArray( 
                      [ 1.0, 0.0, 0.0, 0.0, 2.0,
                        0.0, 0.0, 3.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 4.0, 0.0, 0.0, 0.0 ],
                      lapack_memory_order.row_major 
                     );
    
  U.populateFromArray( 
                      [ 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, -1.0, 
                        1.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  S.populateFromArray( 
                      [ 4.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 3.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, sqrt(5), 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  V.populateFromArray( 
                      [ 0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        sqrt(0.2), 0.0, 0.0, 0.0, sqrt(0.8), 
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        -sqrt(.2), 0.0, 0.0, 0.0, sqrt(0.2)],
                       lapack_memory_order.row_major
                      );

  var A_work = new owned LAPACK_Matrix( A );
  var U_work = new owned LAPACK_Matrix( U );
  var S_work = new owned LAPACK_Matrix( S );
  var V_work = new owned LAPACK_Matrix( V );
  
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );
           
  var ipiv: [0..#A.rows] c_int;
  var superb: [1..#(min(A.rows,A.columns)-1)] real(64);
  
  if verbose_test then
    writeln( "===============================" );
  var info = 
    LAPACKE_dgesvd( A_work.storage_order, // major_order
                    "A",   // jobu
                    "A",   // jobvt
                    A_work.rows : c_int, // m
                    A_work.columns : c_int, // n
                    A_work.data, // a
                    A_work.leadingDimension : c_int, // lda
                    S_work.data, // s
                    U_work.data, // u
                    U_work.leadingDimension : c_int, // ldu
                    V_work.data, // Vt
                    V_work.leadingDimension : c_int, // ldvt
                    superb // superb
                  );
    
    
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );

  var isSame = U == U_work;
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_dgesvd_col_major_test(){
  if verbose_test then
    writeln( "LAPACKE_dgesvd_col_major_test\n===============================" );
    
  var order: lapack_memory_order = lapack_memory_order.column_major;
  var A = new owned LAPACK_Matrix( real(64), 4, 5, order );
  var U = new owned LAPACK_Matrix( real(64), 4, 4, order ); 
  var S = new owned LAPACK_Matrix( real(64), 4, 5, order ); 
  var V = new owned LAPACK_Matrix( real(64), 5, 5, order );
  
  A.populateFromArray( 
                      [ 1.0, 0.0, 0.0, 0.0, 2.0,
                        0.0, 0.0, 3.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 4.0, 0.0, 0.0, 0.0 ],
                      lapack_memory_order.row_major
                     );
    
  U.populateFromArray( 
                      [ 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, -1.0, 
                        1.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  S.populateFromArray( 
                      [ 4.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 3.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, sqrt(5), 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  V.populateFromArray( 
                      [ 0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        sqrt(0.2), 0.0, 0.0, 0.0, sqrt(0.8), 
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        -sqrt(.2), 0.0, 0.0, 0.0, sqrt(0.2)],
                       lapack_memory_order.row_major
                      );

  var A_work = new owned LAPACK_Matrix( A );
  var U_work = new owned LAPACK_Matrix( U );
  var S_work = new owned LAPACK_Matrix( S );
  var V_work = new owned LAPACK_Matrix( V );
  
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );
           
  var ipiv: [0..#A.rows] c_int;
  var superb: [1..#(min(A.rows,A.columns)-1)] real(64);
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_dgesvd( A_work.storage_order, // major_order
                    "A",   // jobu
                    "A",   // jobvt
                    A_work.rows : c_int, // m
                    A_work.columns : c_int, // n
                    A_work.data, // a
                    A_work.leadingDimension : c_int, // lda
                    S_work.data, // s
                    U_work.data, // u
                    U_work.leadingDimension : c_int, // ldu
                    V_work.data, // Vt
                    V_work.leadingDimension : c_int, // ldvt
                    superb // superb
                  );
    
    
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );

  var isSame = U == U_work;
  return ( info == 0 && isSame, info, "" );
}

proc gesvd_row_major_test(){
  if verbose_test then
    writeln( "gesvd_row_major_test\n===============================" );
    
  var order: lapack_memory_order = lapack_memory_order.row_major;
  var A = new owned LAPACK_Matrix( real(64), 4, 5, order );
  var U = new owned LAPACK_Matrix( real(64), 4, 4, order ); 
  var S = new owned LAPACK_Matrix( real(64), 4, 5, order ); 
  var V = new owned LAPACK_Matrix( real(64), 5, 5, order );
  
  A.populateFromArray( 
                      [ 1.0, 0.0, 0.0, 0.0, 2.0,
                        0.0, 0.0, 3.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 4.0, 0.0, 0.0, 0.0 ],
                      lapack_memory_order.row_major 
                     );
    
  U.populateFromArray( 
                      [ 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, -1.0, 
                        1.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  S.populateFromArray( 
                      [ 4.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 3.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, sqrt(5), 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  V.populateFromArray( 
                      [ 0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        sqrt(0.2), 0.0, 0.0, 0.0, sqrt(0.8), 
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        -sqrt(.2), 0.0, 0.0, 0.0, sqrt(0.2)],
                       lapack_memory_order.row_major
                      );

  var A_work = new owned LAPACK_Matrix( A );
  var U_work = new owned LAPACK_Matrix( U );
  var S_work = new owned LAPACK_Matrix( S );
  var V_work = new owned LAPACK_Matrix( V );
  
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );
           
  var ipiv: [0..#A.rows] c_int;
  var superb: [1..#(min(A.rows,A.columns)-1)] real(64);
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    gesvd( A_work.storage_order, // major_order
           "A",   // jobu
           "A",   // jobvt
           A_work.data, // a
           S_work.data, // s
           U_work.data, // u
           V_work.data, // Vt
           superb // superb
         );
    
    
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );

  var isSame = U == U_work;
  return ( info == 0 && isSame, info, "" );
}

proc gesvd_col_major_test(){
  if verbose_test then
    writeln( "gesvd_col_major_test\n===============================" );
    
  var order: lapack_memory_order = lapack_memory_order.column_major;
  var A = new owned LAPACK_Matrix( real(64), 4, 5, order );
  var U = new owned LAPACK_Matrix( real(64), 4, 4, order ); 
  var S = new owned LAPACK_Matrix( real(64), 4, 5, order ); 
  var V = new owned LAPACK_Matrix( real(64), 5, 5, order );
  
  A.populateFromArray( 
                      [ 1.0, 0.0, 0.0, 0.0, 2.0,
                        0.0, 0.0, 3.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 4.0, 0.0, 0.0, 0.0 ],
                      lapack_memory_order.row_major
                     );
    
  U.populateFromArray( 
                      [ 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, -1.0, 
                        1.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  S.populateFromArray( 
                      [ 4.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 3.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, sqrt(5), 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0 ],
                       lapack_memory_order.row_major
                      );
                      
  V.populateFromArray( 
                      [ 0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        sqrt(0.2), 0.0, 0.0, 0.0, sqrt(0.8), 
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        -sqrt(.2), 0.0, 0.0, 0.0, sqrt(0.2)],
                       lapack_memory_order.row_major
                      );

  var A_work = new owned LAPACK_Matrix( A );
  var U_work = new owned LAPACK_Matrix( U );
  var S_work = new owned LAPACK_Matrix( S );
  var V_work = new owned LAPACK_Matrix( V );
  
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );
           
  var ipiv: [0..#A.rows] c_int;
  var superb: [1..#(min(A.rows,A.columns)-1)] real(64);
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    gesvd( A_work.storage_order, // major_order
           "A",   // jobu
           "A",   // jobvt
           A_work.data, // a
           S_work.data, // s
           U_work.data, // u
           V_work.data, // Vt
           superb // superb
         );
    
    
  if verbose_test then
    writeln(  "A\n", A_work.toString(), 
            "\nU\n", U_work.toString(), 
            "\nS\n", S_work.toString(),
            "\nV\n", V_work.toString()
           );

  var isSame = U == U_work;
  return ( info == 0 && isSame, info, "" );
}

proc main(){
  var LAPACKE_dgesvd_row_major_result = LAPACKE_dgesvd_row_major_test();
  var LAPACKE_dgesvd_col_major_result = LAPACKE_dgesvd_col_major_test();
  var gesvd_row_major_result = gesvd_row_major_test();
  var gesvd_col_major_result = gesvd_col_major_test();
  
  writeln( "LAPACKE_dgesvd_row_major ", if LAPACKE_dgesvd_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_dgesvd_col_major ", if LAPACKE_dgesvd_col_major_result[0] then "passed" else "FAILED!" );  
  writeln( "gesvd_row_major ", if gesvd_row_major_result[0] then "passed" else "FAILED!" );
  writeln( "gesvd_col_major ", if gesvd_col_major_result[0] then "passed" else "FAILED!" ); 
  
}
