module LibGL {
  // Hidden in the bowels of some huge but useful Chapel library like LibGL.
  def list.append(x: int) {
    halt("I Got you!");
  }
}

def main {
  foo();
  bar();
}

def foo() {
  var li = new list(int(64));
  li.append(3);
  li.append(4);
  writeln(li);
}

def bar() {
  use LibGL;
  var li = new list(int(64));
  li.append(3);
}
