var values = [(0,0), (0, 1), (1, 0)];

proc test(type t1, type t2) {
  var x: t1;
  var y: t2;

  for tup in values {
    x = tup(0):t1;
    y = tup(1):t2;
    var mn = min(x,y);
    var mx = max(x,y);
    writeln("min(", x, ":", x.type:string, ",", y, ":", y.type:string, ") = ",
            mn, " : ", mn.type:string);
    writeln("max(", x, ":", x.type:string, ",", y, ":", y.type:string, ") = ",
            mx, " : ", mx.type:string);
  }
}

proc gettype(param p) type {
  if p == 1 then
    return int(64);
  else if p == 2 then
    return int(32);
  else if p == 3 then
    return int(16);
  else if p == 4 then
    return int(8);
  else if p == 5 then
    return uint(64);
  else if p == 6 then
    return uint(32);
  else if p == 7 then
    return uint(16);
  else if p == 8 then
    return uint(8);
}

proc main() {
  for param i in 1..8 {
    for param j in 1..8 {
      test(gettype(i), gettype(j));
    }
  }
}
