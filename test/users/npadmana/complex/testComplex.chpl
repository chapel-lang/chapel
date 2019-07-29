var big = 3.14e160;
var small = 2.78e-160;
var simple = 231.42 + 229.0i;
var unit = simple/abs(simple);
var mag = abs(simple);

{
  var tmp = 1.0/big:complex;
  if diff(tmp.re, 1.0/big) then writeln("FAILED-1 : ",tmp.re);
}

{
  var tmp = 1.0/small:complex;
  if diff(tmp.re,1.0/small) then writeln("FAILED-2 : ",tmp.re);
}

{
  var tmp = 1.0/(simple*big);
  if diff(tmp.re,unit.re/(big*mag)) then writeln("FAILED-3 re : ",tmp.re);
  if diff(tmp.im,-unit.im/(big*mag)) then writeln("FAILED-3 im : ",tmp.re);
}


{
  var tmp = 1.0/(simple*small);
  if diff(tmp.re,unit.re/(small*mag)) then writeln("FAILED-4 re : ",tmp.re);
  if diff(tmp.im,-unit.im/(small*mag)) then writeln("FAILED-4 im : ",tmp.re);
}

{
  var tmp = 1.0i/big:complex;
  if diff(tmp.im, 1.0/big) then writeln("FAILED-5 : ",tmp.re);
}

{
  var tmp = 1.0i/small:complex;
  if diff(tmp.im,1.0/small) then writeln("FAILED-6 : ",tmp.re);
}

{
  var tmp = 1.0i/(simple*big);
  if diff(tmp.re,unit.im/(big*mag)) then writeln("FAILED-5 re : ",tmp.re);
  if diff(tmp.im,unit.re/(big*mag)) then writeln("FAILED-5 im : ",tmp.re);
}


{
  var tmp = 1.0i/(simple*small);
  if diff(tmp.re,unit.im/(small*mag)) then writeln("FAILED-6 re : ",tmp.re);
  if diff(tmp.im,unit.re/(small*mag)) then writeln("FAILED-6 im : ",tmp.re);
}

{
  var tmp = 1.0:complex/(simple*big);
  if diff(tmp.re,unit.re/(big*mag)) then writeln("FAILED-7 re : ",tmp.re);
  if diff(tmp.im,-unit.im/(big*mag)) then writeln("FAILED-7 im : ",tmp.re);
}


{
  var tmp = 1.0:complex/(simple*small);
  if diff(tmp.re,unit.re/(small*mag)) then writeln("FAILED-8 re : ",tmp.re);
  if diff(tmp.im,-unit.im/(small*mag)) then writeln("FAILED-8 im : ",tmp.re);
}


proc diff(actual : real, expected : real) : bool {
  return abs((actual-expected)/expected) > 1.0e-14;
}