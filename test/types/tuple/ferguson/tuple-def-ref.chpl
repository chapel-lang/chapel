record R {
  var x:int;
}


proc refy(ref tup:(R, R))
{
  tup[0].x = 11;
  tup[1].x = 22;
}

proc run() {
  var tup:(R,R);

  tup[0].x = 1;
  tup[1].x = 2;

  refy(tup);

  writeln(tup);
}

run();

