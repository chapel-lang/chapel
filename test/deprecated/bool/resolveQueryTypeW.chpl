proc bar(const ref x: bool(?w)) {
  writeln("In bool(", w, ") version of bar()");
}

var b: bool;
bar(b);
