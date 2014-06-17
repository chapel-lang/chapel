module LibGL {
  // Hidden in the bowels of some huge but useful Chapel library like LibGL.
  proc list.append(x: int) {
    halt("I Got you!");
  }
}

proc main {
  foo();
  bar();
}

proc foo() {
  var li = new list(real);
  li.append(3);
  li.append(4);
  writeln(li);
}

proc bar() {
  use LibGL;
  var li = new list(real);
  li.append(3);
}
