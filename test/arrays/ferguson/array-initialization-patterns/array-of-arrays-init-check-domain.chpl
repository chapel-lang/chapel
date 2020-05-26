// simple test
proc test1() {
  writeln("test1");
  var A:[0..1] real = [1.0, 2.0];
  var AA: [1..2][1..2] real = [ A, A ];
  writeln(AA[1].domain); // this should be {1..2}
  writeln(AA[2].domain); // this should be {1..2}
}
test1();

proc test2() {
  writeln("test2");
  var AA: [1..2][1..2] real = [ [1.0, 2.0], [3.0, 4.0] ];
  writeln(AA[1].domain); // this should be {1..2}
  writeln(AA[2].domain); // this should be {1..2}
}
test2();

proc test3() {
  writeln("test3");
  var B:[0..1] real = [1.0, 2.0];
  var BB: [1..2][0..1] real = [ B, B ];
  var AA: [1..2][1..2] real = BB; // check copy eliding
  writeln(AA[1].domain); // this should be {1..2}
  writeln(AA[2].domain); // this should be {1..2}
}
test3();

proc test3a() {
  writeln("test3a");
  var B:[0..1] real = [1.0, 2.0];
  var BB: [1..2][0..1] real = [ B, B ];
  var AA: [1..2][1..2] real = BB; // check not copy eliding
  writeln(AA[1].domain); // this should be {1..2}
  writeln(AA[2].domain); // this should be {1..2}
  BB; // not copy eliding
}
test3a();

proc makeBB() {
  var B:[0..1] real = [1.0, 2.0];
  var BB: [1..2][0..1] real = [ B, B ];
  return BB;
}

proc test4() {
  writeln("test4");
  var AA: [1..2][1..2] real = makeBB(); // check copy eliding
  writeln(AA[1].domain); // this should be {1..2}
  writeln(AA[2].domain); // this should be {1..2}
}
test4();



// The below is a reproducer for part of LAPACK_Matrix_test.
writeln("reproducer");

proc putInto2D( arrayOfArrays: [?D1] ?t, twoArray: [?D2] ?o): void{
  writeln("in putInto2D");
  writeln("D1 is ", D1);
  writeln("arrayOfArrays is ", arrayOfArrays);
  writeln("arrayOfArrays[D1.dim(0).low].domain is ", arrayOfArrays[D1.dim(0).low].domain);
  writeln("arrayOfArrays[D1.dim(0).low] is ", arrayOfArrays[D1.dim(0).low]);
  writeln("arrayOfArrays[D1.dim(0).high].domain is ", arrayOfArrays[D1.dim(0).high].domain);
  writeln("arrayOfArrays[D1.dim(0).high] is ", arrayOfArrays[D1.dim(0).high]);
  writeln("D2 is ", D2);
  writeln("twoArray is ", twoArray);
  // no check. Cannot have ragged array literals
  assert( D1.rank == 1 && D2.rank == 2  );
  var formed: domain(2) = {D1.dim(0), arrayOfArrays[D1.dim(0).low].domain.dim(0)};
  writeln("formed is ", formed);
  assert( formed.dim(0).size == D2.dim(0).size && formed.dim(1).size == D2.dim(1).size );
  var trans = ( D2.dim(0).low - formed.dim(0).low, D2.dim(1).low - formed.dim(1).low ); 
  writeln("trans is ", trans);

  for (i,j) in D2 {
    writeln("assigning ", (i,j)); 
    const ref rhs1 = arrayOfArrays[i+trans[0]];
    var rhs2 = rhs1[j+trans[1]];
    ref lhs = twoArray[i,j];
    lhs = rhs2;
  }

}

writeln( "1D row input => row" );
var input_1_row: [1..4] real = 
  [ 3.0, 1.0, 
    4.0, 2.0 ];

writeln( "1D col input => col" );
var input_1_col: [1..4] real  = 
  [ 3.0, 4.0,   // tranpose of [ 3.0, 1.0,
    1.0, 2.0 ]; //                 4.0, 2.0 ];

// 1.5D data
var input_1_5_row: [1..2][1..2] real =
  [ [3.0, 1.0], 
    [4.0, 2.0] ];

var input_1_5_col: [1..2][1..2] real =
  [ [3.0, 4.0], 
    [1.0, 2.0] ];


// 2D data
writeln( "2D row input => row" );
var input_2_row: [1..2,1..2] real;
putInto2D( input_1_5_row, input_2_row );
writeln(input_2_row);
