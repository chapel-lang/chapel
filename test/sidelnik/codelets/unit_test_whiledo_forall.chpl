use CodeletDist;

proc main {

  var x = 0;
  var y = 0;
  var N = [0..3] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  while y < 5 {
    do {
      writeln("Before forall loop :", x, " out of 9");
      forall j in N do {
        writeln("inside forall loop: ", j, " out of 3");
        A(j) = x + j + y;
      }
      writeln("AFter forall loop :", x, " out of 9" );
      x = x + 1;
    } while x < 10;
    writeln("performing outer do-while loop : Y = ", y, " < 5");
    if y == 3 then
      y = 5;
    y = y + 1;
  }

  if y < 10 then
    writeln(A(1));
  else
    writeln(A(2));
}
