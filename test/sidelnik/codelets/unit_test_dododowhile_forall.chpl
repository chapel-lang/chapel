use CodeletDist;

proc main {

  var x = 0;
  var y = 0;
  var z = 0;
  var N = [0..3] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  do {
    do {
      do {
        writeln("Before forall loop :", x, " out of 5");
        forall j in N do {
          writeln("inside forall loop: ", j, " out of 3");
          A(j) = x + j + y;
        }
        writeln("AFter forall loop :", x, " out of 9" );
        x = x + 1;
      } while x < 6;
      writeln("performing outer do-while loop : Y = ", y, " < 5");
      if y == 3 then {
        y = 5;
      }
      y = y + 1;
    } while y < 5;
    z = z + 1;
    writeln("performing outer outer do-while loop: Z = ", z, " < 2");
  } while z < 2;

  writeln("finished do-do-do loop");

  if y < 10 then
    writeln(A(1));
  else
    writeln(A(2));
}
