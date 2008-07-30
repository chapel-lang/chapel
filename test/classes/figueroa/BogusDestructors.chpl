class C {
  def ~C () {writeln("inside ~C");}
}

def C.~C () {writeln("inside C.~C");}

record R {
  def ~R () {writeln("inside ~R");}
}

var c: C;
var r: R;

delete c;
delete r;
