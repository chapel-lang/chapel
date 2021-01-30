type arrOfInt = [1..3] int;
type arrOfReal = [1..3] real;
type tupOfArrs = (arrOfInt, arrOfReal);
var A: [1..2] tupOfArrs;
writeln(A);

