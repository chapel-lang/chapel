use List;
class C { var x: string; }

var l = new list(owned C?);

on here.gpus[0] {
  l.pushBack(new owned C("10"));
}

writeln(l[0]);
