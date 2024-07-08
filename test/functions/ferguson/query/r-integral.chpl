// from issue #11224

record R { var x; }
proc h(arg: R(integral)) {
  writeln(arg.type:string);
}

h(new R(1));
