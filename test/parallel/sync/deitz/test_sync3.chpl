// From "Shet, Aniruddha G." <shetag@ornl.gov>
// Bug submitted 7/17/2007

class C {
  var a,b : int;
}

var l : sync unmanaged C? = new unmanaged C(1,2);
var m : unmanaged C? = l.readFE(); //compiles without any error

var n : [1..5] sync unmanaged C?;
n[1].writeEF(new unmanaged C(3,4));
var o : unmanaged C? = n[1].readFE();
writeln(o);

delete m;
delete o;
