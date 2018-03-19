class Base {
  proc init() {
    complete();
  }
}

class Child1 {
  type  t1;
  param p1;

  proc init(type _t1, param _p1) {
    t1 = _t1;
    p1 = _p1;

    complete();
  }
}

class Child2 : Child1 {
  type  t2;

  proc init(type _t1, param _p1, type _t2) {
    super.init(_t1, _p1);

    t2 = _t2;

    complete();
  }
}





proc main() {
  testLevel1();
  writeln();

  testLevel2();
}


proc testLevel1() {
  var c11 : Child1(int, 10);
  var c12 : Child1(p1 = 10, t1 = real);

  writeln('c11.type ', c11.type : string);
  writeln('c12.type ', c12.type : string);
  writeln();

  writeln('Are the types the same? ', c11.type == c12.type);
  writeln();

  c11 = new Child1(int, 10);

  writeln(c11);

  delete c11;
}


proc testLevel2() {
  var c21 : Child2(real, 20, int);
  var c22 : Child2(t2 = int, t1 = real, p1 = 20);

  writeln('c21.type ', c21.type : string);
  writeln('c22.type ', c22.type : string);
  writeln();

  writeln('Are the types the same? ', c21.type == c22.type);
  writeln();

  c21 = new Child2(real, 20, int);

  writeln(c21);

  delete c21;
}



