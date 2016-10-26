record R {
  var x:int;
}


proc refy(ref tup:(R, R))
{
  tup[1].x = 11;
  tup[2].x = 22;
}

proc run() {
  var tup:(R,R);

  tup[1].x = 1;
  tup[2].x = 2;

  refy(tup);

  writeln(tup);
}

run();

