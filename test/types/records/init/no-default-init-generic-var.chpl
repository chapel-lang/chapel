record R { var genericField: integral; }

var x: R(int); // ok
writeln(x, " : ", x.type:string);

