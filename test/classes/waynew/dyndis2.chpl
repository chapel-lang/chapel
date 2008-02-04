use List;

class somedata {
  type elt_type;
  var  data: elt_type;
  var  number: int;
}


class base {
  def jam( x: int, d: somedata(int)) {
    writeln( "base ", " : ", d);
  }
}

class aclass: base {
  type dtype;
  var data: dtype;
  def jam( x: int, d: somedata(int)) {
    writeln( "aclass ", data, " ", x, " : ", d);
  }
}

class bclass: base {
  type dtype;
  var y: dtype;
  def jam( x:int, d: somedata(int)) {
    writeln( "bclass ", y, " ", x, " : ", d);
  }
}

class contain {
  var objs: list(base);

  def xxx() {
    var something: somedata(int);
    something = new somedata( int, 10);
    for e in objs do
       e.jam( 99, something);
  }
}


def main () {
  var a : aclass(int) = new aclass(int);
  var b : bclass(bool) = new bclass(bool);

  var c: contain = new contain();
  c.objs.append(b);
  c.objs.append(a);
  c.xxx();
}

