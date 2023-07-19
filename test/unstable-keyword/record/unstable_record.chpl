//record + quadratic formula
//shouldn't be unstable

@unstable("The record 'UnstableRecordTest' is unstable")
record UnstableRecordTest {
    var a, b, c: real;
}

//uses the record
var x: UnstableRecordTest; //trig
var y: UnstableRecordTest; //trig
x = new UnstableRecordTest(1,6,9); //trig
writeln("x's test: ", x);
writeln((-x.b - (sqrt(x.b**2-(4*x.a*x.c))))/(2*x.a));
writeln((-x.b + (sqrt(x.b**2-(4*x.a*x.c))))/(2*x.a));

y = new UnstableRecordTest(1,8,16); //trig
writeln("y's test: ", y);
writeln((-y.b - (sqrt(y.b**2-(4*y.a*y.c))))/(2*y.a));
writeln((-y.b + (sqrt(y.b**2-(4*y.a*y.c))))/(2*y.a));

