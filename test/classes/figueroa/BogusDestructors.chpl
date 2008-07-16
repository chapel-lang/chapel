class C {
  def ~foo () {writeln("inside ~foo");}
  def ~C () {writeln("inside ~C");}
  def ~C () {writeln("inside ~C");}
}

record R {
  def ~R () {writeln("inside ~R");}
}

var c: C;
var r: R;

delete c;
delete r;
