record R { var genericField: integral; }

var x: R(int); // error
writeln(x, " : ", x.type:string);

