var x: int; 

def main() {
  writeln("In main before foo x = ", x);
  atomic {
    foo();
  }
  writeln("In main after foo x = ", x);
}

def foo() {
  bar();
}

def bar() {
  x += 1;
  if x < 5 then bar();
}