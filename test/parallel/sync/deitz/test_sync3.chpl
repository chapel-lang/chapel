// From "Shet, Aniruddha G." <shetag@ornl.gov>
// Bug submitted 7/17/2007

class C {
  var a,b : int;
}

var l : sync C = new C(1,2);
var m : C = l; //compiles without any error

var n : [1..5] sync C;
n[1] = new C(3,4);
var o : C = n[1];
writeln(o);

delete m;
delete o;
