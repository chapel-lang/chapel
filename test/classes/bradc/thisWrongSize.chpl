class C {
  type idxtype;

  var dom: domain(1) = {1..2};
  var data: [dom] D(idxtype);

  proc C(type idxtype) {
    for i in dom do
      data(i) = new D(idxtype);
  }

  proc this(i: idxtype) ref {
    /* This gets the first i into the right size
    var i32 = i: int(32);
    return data(i32)(i);
    */
    return data(i)(i);
    /*          ^ This is the wrong size, but error message doesn't help */
  }
}

class D {
  type idxtype;
  var x: real;

  proc this(i: idxtype) ref {
    return x;
  }    
}

var myC = new C(int(64));

var i: int(64) = 1;

writeln("myC(i) = ", myC(i));

