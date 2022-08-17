//record + quadratic formula
//shouldn't be unstable

@unstable "The record 'Test3' is unstable"
record Test3 {
    var a, b, c: real;
}

//uses the record
var x: Test3; //trig
var y: Test3; //trig
x = new Test3(1,6,9); //trig
writeln("x's test: ", x);
writeln((-x.b - (sqrt(x.b**2-(4*x.a*x.c))))/(2*x.a));
writeln((-x.b + (sqrt(x.b**2-(4*x.a*x.c))))/(2*x.a));

y = new Test3(1,8,16); //trig
writeln("y's test: ", y);
writeln((-y.b - (sqrt(y.b**2-(4*y.a*y.c))))/(2*y.a));
writeln((-y.b + (sqrt(y.b**2-(4*y.a*y.c))))/(2*y.a));

