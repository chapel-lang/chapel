var x, y: int = 0;
pragma "omit else from atomic" var inAtomic: bool;

proc main () {
  atomic {
    if inAtomic {
      x += 1;
    } else {
      y += 1;
    }
  }
  writeln("x = ", x);
  writeln("y = ", y);
}