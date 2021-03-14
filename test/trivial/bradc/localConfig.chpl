config var x = 1;

proc main() {
  config var y = 2;

  for i in 1..10 {
    config var z = 3;

    writeln((x,y,z,i));
  }
}
