record R { var genericField; }

var x: R(int); // error
writeln(x, " : ", x.type:string);

