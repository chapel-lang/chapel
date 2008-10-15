class EC {
  var i: sync int(64) = 0;
  var b: sync bool = true;
  def ~EC() {
    delete i;
    delete b;
  }
}

def up(e: EC) {
  var i = e.i;
  if i == 0 then
    e.b.reset();
  e.i = i + 1;
}

def down(e: EC) {
  var i = e.i;
  if i == 1 then
    e.b = true;
  e.i = i - 1;
}

def allocate() {
  return new EC();
}

def wait(e: EC) {
  e.b;
  e.i; // wait until down is finished setting i
  delete e;
}

config var n = 1000;

def main {
  var a = 0;
  for i in 1..n {
    //var unused = allocate();
    var e = allocate();
    up(e);
    begin {
      a += 1;
      down(e);
    }
    wait(e);
  }
  writeln(a == n);
}
