proc intToRealSimpler(x:int(?w)) {
  if w == 64 || w == 32 {
    writeln("OK");

    var r : real(w);

    writeln(r);

  } else {
    writeln("Not good");
  }
}

intToRealSimpler(0:int(32));
intToRealSimpler(0:int( 8));
