var r = 1:uint..5 by -1;

writeln("r is: ", r);
writeln("for loop over r");
for i in r {
  writeln(i);
}

writeln("forall loop over r");
serial {
  forall i in r {
    writeln(i);
  }
}
