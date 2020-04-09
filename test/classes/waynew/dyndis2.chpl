use LinkedLists;

class somedata {
  type elt_type;
  var  data: elt_type;
  var  number: int;
}


class base {
  proc jam( x: int, d: borrowed somedata(int)) {
    writeln( "base ", " : ", d);
  }
}

class aclass: base {
  type dtype;
  var data: dtype;
  override proc jam( x: int, d: borrowed somedata(int)) {
    writeln( "aclass ", data, " ", x, " : ", d);
  }
}

class bclass: base {
  type dtype;
  var y: dtype;
  override proc jam( x:int, d: borrowed somedata(int)) {
    writeln( "bclass ", y, " ", x, " : ", d);
  }
}

class contain {
  var objs: LinkedList(borrowed base);

  proc deinit() {
    objs.destroy();
  }

  proc xxx() {
    var something: borrowed somedata(int)?;

    something = new owned somedata( int, 10);

    for e in objs do
       e.jam( 99, something!);
  }
}


proc main () {
  var a : borrowed aclass(int)  = new borrowed aclass(int);
  var b : borrowed bclass(bool) = new borrowed bclass(bool);
  var c : borrowed contain      = new borrowed contain();

  c.objs.append(b);
  c.objs.append(a);
  c.xxx();
}

