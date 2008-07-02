class C {
  def ~foo () {writeln("inside ~foo");}
  def ~C (x) {writeln("inside ~C");}
}

record R {
  def ~R () {writeln("inside ~R");}
}

def ~bar () {writeln("inside ~bar");}

var c: C;
var r: R;

delete c;
delete r;
