use TestHelpers, LAPACK, LAPACK.ClassicLAPACK;

config var verbose_test: bool;
config const epsilon = 10e-6;
extern var LAPACK_D_SELECT2_LESS_THAN: LAPACK_D_SELECT2;

proc LAPACK_dgees_no_sort_test(){
  if verbose_test then
    writeln( "LAPACK_dgees_no_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
 
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
  
  var jobvs : c_char = "V".toByte();
  var sort : c_char = "N".toByte();
  var chlapack_select : LAPACK_D_SELECT2;
  var n : c_int = A.order : c_int;
  var a = A.data;
  var lda : c_int = A.leadingDimension : c_int;
  var sdim: c_int;
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order );
  var vs = VS.data;
  var ldvs : c_int = VS.leadingDimension : c_int;
  var lwork : c_int = 3*n : c_int;
  var work : [1..#lwork] real(64);
  var bwork : [1..#n] c_int;
  var info : c_int;
  
  
  LAPACK_dgees(jobvs,
               sort, 
               chlapack_select,
               n, 
               a, 
               lda,
               sdim,
               wr,
               wi,
               vs,
               ldvs,
               work,
               lwork,
               bwork,
               info);
  
  var A_result = new LAPACK_Matrix( real(64), A.rows, A.columns, A.storage_order, epsilon );
  A_result.populateFromArray( a, lapack_memory_order.column_major );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ 6.60555,   4.49073, 0.826322, 
                                      0.0, -0.605551, -1.07263, 
                                      0.0,       0.0,     -1.0 ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ 6.60555 : real(64), -0.605551 : real(64), -1.0 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_result = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  VS_result.populateFromArray( vs, lapack_memory_order.column_major );
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ -0.498574,  -0.764694,  0.408248, 
                                   -0.574052, -0.0616275, -0.816497, 
                                    -0.64953,   0.641439,  0.408248 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS_result.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS_result ;
  
  return ( info == 0 && isSame, info, "" );
}

proc LAPACK_dgees_sort_test(){
  if verbose_test then
    writeln( "LAPACK_dgees_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
 
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
  
  var jobvs : c_char = "V".toByte();
  var sort : c_char = "S".toByte();
  var chlapack_select : LAPACK_D_SELECT2 = LAPACK_D_SELECT2_LESS_THAN;
  var n : c_int = A.order : c_int;
  var a = A.data;
  var lda : c_int = A.leadingDimension : c_int;
  var sdim: c_int;
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order );
  var vs = VS.data;
  var ldvs : c_int = VS.leadingDimension : c_int;
  var lwork : c_int = 3*n : c_int;
  var work : [1..#lwork] real(64);
  var bwork : [1..#n] c_int;
  var info : c_int;
  
  
  LAPACK_dgees(jobvs,
               sort, 
               chlapack_select,
               n, 
               a, 
               lda,
               sdim,
               wr,
               wi,
               vs,
               ldvs,
               work,
               lwork,
               bwork,
               info);
  
  var A_result = new LAPACK_Matrix( real(64), A.rows, A.columns, A.storage_order, epsilon );
  A_result.populateFromArray( a, lapack_memory_order.column_major );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ -0.605551, -1.26776,  4.51384, 
                                        0.0,     -1.0, 0.134409, 
                                        0.0,      0.0, 6.60555   ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ -0.605551 : real(64), -1.0 : real(64), 6.60555 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_result = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  VS_result.populateFromArray( vs, lapack_memory_order.column_major );
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ 0.385555, -0.422805, -0.820111, 
                                  -0.251146, 0.807183, -0.534211, 
                                  -0.887847, -0.411935, -0.205028 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS_result.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS_result ;
  
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_dgees_row_major_no_sort_test(){
  if verbose_test then
    writeln( "LAPACKE_dgees_row_major_no_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_dgees( A.storage_order, // matrix_order
                   "V", // jobvs
                   "N", //sort
                   lapack_select, // select
                   A.order : c_int, //n
                   A_result.data, // a
                   A.leadingDimension : c_int, // lda
                   sdim, //sdim
                   wr, // wr
                   wi, // wi
                   VS.data, // vs
                   VS.leadingDimension : c_int // ldv
                 );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ 6.60555,   4.49073, 0.826322, 
                                      0.0, -0.605551, -1.07263, 
                                      0.0,       0.0,     -1.0 ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ 6.60555 : real(64), -0.605551 : real(64), -1.0 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ -0.498574,  -0.764694,  0.408248, 
                                   -0.574052, -0.0616275, -0.816497, 
                                    -0.64953,   0.641439,  0.408248 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_dgees_col_major_no_sort_test(){
  if verbose_test then
    writeln( "LAPACKE_dgees_col_major_no_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_dgees( A.storage_order, // matrix_order
                   "V", // jobvs
                   "N", //sort
                   lapack_select, // select
                   A.order : c_int, //n
                   A_result.data, // a
                   A.leadingDimension : c_int, // lda
                   sdim, //sdim
                   wr, // wr
                   wi, // wi
                   VS.data, // vs
                   VS.leadingDimension : c_int // ldv
                 );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ 6.60555,   4.49073, 0.826322, 
                                      0.0, -0.605551, -1.07263, 
                                      0.0,       0.0,     -1.0 ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ 6.60555 : real(64), -0.605551 : real(64), -1.0 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ -0.498574,  -0.764694,  0.408248, 
                                   -0.574052, -0.0616275, -0.816497, 
                                    -0.64953,   0.641439,  0.408248 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_dgees_row_major_sort_test(){
  if verbose_test then
    writeln( "LAPACKE_dgees_row_major_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2 = LAPACK_D_SELECT2_LESS_THAN;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_dgees( A.storage_order, // matrix_order
                   "V", // jobvs
                   "S", //sort
                   lapack_select, // select
                   A.order : c_int, //n
                   A_result.data, // a
                   A.leadingDimension : c_int, // lda
                   sdim, //sdim
                   wr, // wr
                   wi, // wi
                   VS.data, // vs
                   VS.leadingDimension : c_int // ldv
                 );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ -0.605551, -1.26776,  4.51384, 
                                        0.0,     -1.0, 0.134409, 
                                        0.0,      0.0, 6.60555   ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ -0.605551 : real(64), -1.0 : real(64), 6.60555 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ 0.385555, -0.422805, -0.820111, 
                                  -0.251146, 0.807183, -0.534211, 
                                  -0.887847, -0.411935, -0.205028 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc LAPACKE_dgees_col_major_sort_test(){
  if verbose_test then
    writeln( "LAPACKE_dgees_col_major_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2 = LAPACK_D_SELECT2_LESS_THAN;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
    LAPACKE_dgees( A.storage_order, // matrix_order
                   "V", // jobvs
                   "S", //sort
                   lapack_select, // select
                   A.order : c_int, //n
                   A_result.data, // a
                   A.leadingDimension : c_int, // lda
                   sdim, //sdim
                   wr, // wr
                   wi, // wi
                   VS.data, // vs
                   VS.leadingDimension : c_int // ldv
                 );
  
  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ -0.605551, -1.26776,  4.51384, 
                                        0.0,     -1.0, 0.134409, 
                                        0.0,      0.0, 6.60555   ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ -0.605551 : real(64), -1.0 : real(64), 6.60555 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ 0.385555, -0.422805, -0.820111, 
                                  -0.251146, 0.807183, -0.534211, 
                                  -0.887847, -0.411935, -0.205028 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc gees_row_major_no_sort_test(){
  if verbose_test then
    writeln( "gees_row_major_no_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0 ],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gees( A.storage_order, // matrix_order
           "V", // jobvs
           "N", //sort
           lapack_select, // select
           A_result.data, // a
           sdim, //sdim
           wr, // wr
           wi, // wi
           VS.data // vs
         );

  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ 6.60555,   4.49073, 0.826322, 
                                      0.0, -0.605551, -1.07263, 
                                      0.0,       0.0,     -1.0 ],
                                lapack_memory_order.row_major );
  
  var wr_expected : [wr.domain] real(64) = [ 6.60555 : real(64), -0.605551 : real(64), -1.0 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ -0.498574,  -0.764694,  0.408248, 
                                   -0.574052, -0.0616275, -0.816497, 
                                    -0.64953,   0.641439,  0.408248 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc gees_col_major_no_sort_test(){
  if verbose_test then
    writeln( "gees_col_major_no_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0 ],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gees( A.storage_order, // matrix_order
           "V", // jobvs
           "N", //sort
           lapack_select, // select
           A_result.data, // a
           sdim, //sdim
           wr, // wr
           wi, // wi
           VS.data // vs
         );

  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ 6.60555,   4.49073, 0.826322, 
                                      0.0, -0.605551, -1.07263, 
                                      0.0,       0.0,     -1.0 ],
                                lapack_memory_order.row_major );
  
  var wr_expected : [wr.domain] real(64) = [ 6.60555 : real(64), -0.605551 : real(64), -1.0 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ -0.498574,  -0.764694,  0.408248, 
                                   -0.574052, -0.0616275, -0.816497, 
                                    -0.64953,   0.641439,  0.408248 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc gees_row_major_sort_test(){
  if verbose_test then
    writeln( "gees_row_major_sort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0 ],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2 = LAPACK_D_SELECT2_LESS_THAN;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gees( A.storage_order, // matrix_order
           "V", // jobvs
           "S", //sort
           lapack_select, // select
           A_result.data, // a
           sdim, //sdim
           wr, // wr
           wi, // wi
           VS.data // vs
         );

  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ -0.605551, -1.26776,  4.51384, 
                                        0.0,     -1.0, 0.134409, 
                                        0.0,      0.0, 6.60555   ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ -0.605551 : real(64), -1.0 : real(64), 6.60555 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ 0.385555, -0.422805, -0.820111, 
                                  -0.251146, 0.807183, -0.534211, 
                                  -0.887847, -0.411935, -0.205028 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc gees_col_major_sort_test(){
  if verbose_test then
    writeln( "gees_col_majorsort\n===============================" );

  var A = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  
  A.populateFromArray( 
                      [ 3.0, 2.0, 1.0,
                        4.0, 2.0, 1.0,
                        4.0, 4.0, 0.0 ],
                        lapack_memory_order.row_major
                     );
  
  var A_result = new LAPACK_Matrix( A );
  var wr: [0..#A.rows] real(64);
  var wi: [0..#A.rows] real(64);
  var VS = new LAPACK_Matrix( real(64), A.rows, A.rows, A.storage_order, epsilon );
  
  var sdim: c_int;
  
  var lapack_select: LAPACK_D_SELECT2 = LAPACK_D_SELECT2_LESS_THAN;
  
  if verbose_test then
    writeln(  "A\n", A.toString() );
  
  if verbose_test then
    writeln( "===============================" );
    
  var info = 
     gees( A.storage_order, // matrix_order
           "V", // jobvs
           "S", //sort
           lapack_select, // select
           A_result.data, // a
           sdim, //sdim
           wr, // wr
           wi, // wi
           VS.data // vs
         );

  var A_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.column_major, epsilon );
  A_expected.populateFromArray( [ -0.605551, -1.26776,  4.51384, 
                                        0.0,     -1.0, 0.134409, 
                                        0.0,      0.0, 6.60555   ],
                                lapack_memory_order.row_major );
                                
  var wr_expected : [wr.domain] real(64) = [ -0.605551 : real(64), -1.0 : real(64), 6.60555 : real(64) ];                      
  var wi_expected : [wi.domain] real(64) = [ 0.0 : real(64), 0.0 : real(64), 0.0 : real(64) ];
  
  var VS_expected = new LAPACK_Matrix( real(64), 3, 3, lapack_memory_order.row_major, epsilon );
  VS_expected.populateFromArray( [ 0.385555, -0.422805, -0.820111, 
                                  -0.251146, 0.807183, -0.534211, 
                                  -0.887847, -0.411935, -0.205028 ],
                                 lapack_memory_order.row_major );
  
  if verbose_test {
    writeln( "A\n", A_result.toString() );
    writeln( "wr\n", wr );
    writeln( "wi\n", wi );
    writeln( "VS\n", VS.toString() );
  }
  
  var isSame =   epsilonEquals( wr_expected, wr, epsilon )
              && epsilonEquals( wi_expected, wi, epsilon )
              && A_expected == A_result
              && VS_expected == VS ;
  
  return ( info == 0 && isSame, info, "" );
}

proc main(){
  var LAPACK_dgees_no_sort_result = LAPACK_dgees_no_sort_test();
  var LAPACK_dgees_sort_result = LAPACK_dgees_sort_test();
  var LAPACKE_dgees_row_major_no_sort_result = LAPACKE_dgees_row_major_no_sort_test();
  var LAPACKE_dgees_col_major_no_sort_result = LAPACKE_dgees_col_major_no_sort_test();
  var LAPACKE_dgees_row_major_sort_result = LAPACKE_dgees_row_major_sort_test();
  var LAPACKE_dgees_col_major_sort_result = LAPACKE_dgees_col_major_sort_test();
  var gees_row_major_no_sort_result = gees_row_major_no_sort_test();
  var gees_col_major_no_sort_result = gees_col_major_no_sort_test();
  var gees_row_major_sort_result = gees_row_major_sort_test();
  var gees_col_major_sort_result = gees_col_major_sort_test();
  
  
  writeln( "LAPACK_dgees_no_sort ", if LAPACK_dgees_no_sort_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACK_dgees_sort ", if LAPACK_dgees_sort_result[0] then "passed" else "FAILED!" );    
  writeln( "LAPACKE_dgees_row_major_no_sort ", if LAPACKE_dgees_row_major_no_sort_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_dgees_col_major_no_sort ", if LAPACKE_dgees_col_major_no_sort_result[0] then "passed" else "FAILED!" );  
  writeln( "LAPACKE_dgees_row_major_sort ", if LAPACKE_dgees_row_major_sort_result[0] then "passed" else "FAILED!" );
  writeln( "LAPACKE_dgees_col_major_sort ", if LAPACKE_dgees_col_major_sort_result[0] then "passed" else "FAILED!" );  
  writeln( "gees_row_major_no_sort ", if gees_row_major_no_sort_result[0] then "passed" else "FAILED!" );
  writeln( "gees_col_major_no_sort ", if gees_col_major_no_sort_result[0] then "passed" else "FAILED!" );  
  writeln( "gees_row_major_sort ", if gees_row_major_sort_result[0] then "passed" else "FAILED!" );
  writeln( "gees_col_major_sort ", if gees_col_major_sort_result[0] then "passed" else "FAILED!" );  
}
