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
    var myOwned = new owned somedata( int, 10);

    something = myOwned;

    for e in objs do
       e.jam( 99, something!);
  }
}


proc main () {
  var ownA = new owned aclass(int);
  var a : borrowed aclass(int)  = ownA.borrow();
  var ownB = new owned bclass(bool);
  var b : borrowed bclass(bool) = ownB.borrow();
  var ownC = new owned contain();
  var c : borrowed contain      = ownC.borrow();

  c.objs.append(b);
  c.objs.append(a);
  c.xxx();
}
