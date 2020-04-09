record R {
  type t;
}

proc subtype(type myr) type {
  return myr.t;
}

proc test1() {

  writeln("test1");

  // 3 ways of writing R(_).t

  writeln(R(int).t:string);

  type MyR = R(int);
  writeln(MyR.t:string);

  writeln(subtype(R(int)):string);
}

class X {
  type eltee;
  param strided;
  param rank;
}

proc printer(type T) {
  writeln(T.eltee:string);
  writeln(T.strided);
  writeln(T.rank);
}

proc test2() {
  writeln("test2");

  var A : borrowed X(real, false, 1) = new borrowed X(real, false, 1);
  var B : borrowed X(uint, true, 3) = new borrowed X(uint, true, 3);

  printer(A.type);
  printer(B.type);
}

proc getSize(type t) param {
  return t.size;
}

proc getFirst(type t) type {
  return t(0);
}

proc test3() {
  writeln("test3");

  param sz = getSize( 2*int );
  writeln(sz);

  param sz2 = (2*int).size;
  writeln(sz2);

  type t = getFirst( (string, real, int) );
  writeln(t:string);
}

test1();
test2();
test3();
