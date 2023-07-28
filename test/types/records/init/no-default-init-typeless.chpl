record R { var genericField; }

var x: R(int); // ok
writeln(x, " : ", x.type:string);

