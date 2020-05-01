//Array with empty/queried domain can only be used as a formal argument
//type -> a has to be defined in terms of n.

//Array aliasing requires a new variable declaration ->
//wrap cannot be implemented.

use Math;

/*
A very, very basic & crude implementation of the tensor/matrix/vector
classes used by MADNESS.  This to enable a standalone and pure Python
version of the 1d example code.
*/

type elemType = real(64);

class Vector {
  var n : int;
  var d = {1..n};
  var a : [d] elemType;

  //
  // TODO: At present, the compiler-generated constructor for classes
  // with array fields like this requires the actual that's passed to
  // it to be a default rectangular array.  This is stricter than if
  // the user wrote the equivalent initializer in which the actual to
  // have the same formal type because we treat formal array arguments
  // as being flexible if they don't name an explicit domain map.
  // This causes issues once array operations like slices are not
  // stored in closed form.  See issue #5289 for details.
  //
  // Once this issue is resolved and the compiler is generating
  // similarly generic initializers, the changes that introduced this
  // comment can/should be reverted (the introduction of the 'd' field
  // above and the three routines that follow).
  //
  proc init(initN: int, initA: [1..initN] elemType) {
    n = initN;
    d = {1..n};
    a = initA;
  }

  proc init(initN: int) {
    n = initN;
    d = {1..n};
  }

  override proc writeThis(f) throws {
    f.write("{n = ", n, ", a = ", a, "}");
  }

  //error: internal failure SYM1167 chpl Version 0.5
  //due to default constructor semantics i think?
  /*
  proc Vector(arg) {
    if (arg.type == int) then {
      n = arg;
      a = 0.0;
    }
    else {
      n = arg.size;
      a = arg;
    }
  }
  */

  proc normf() {
    return sqrt(+ reduce a**2);
  }

  proc inner(other) {
    return + reduce (a*other.a);
  }

  proc gaxpy(alpha,other,beta) {
    a = alpha*a + beta*other.a;
    return _to_unmanaged(this);
  }

  proc scale(s) {
    a = a*s;
    return _to_unmanaged(this);
  }

  proc emul(other) {
    a = a*other.a;
    return _to_unmanaged(this);
  }

  proc getitem(ind) {
    return a(ind);
  }

  proc setitem(ind,value) {
    a(ind) = value;
  }

  proc getslice(lo,hi) {
    ref aslice = a(lo..hi);
    return new unmanaged Vector(aslice.size,aslice);
  }

  proc setslice(lo,hi,value) {
    a(lo..hi) = value;
  }

  proc len() {
    return n;
  }

  proc add(other) {
    return gaxpy(1.0,other,1.0);
  }
}

class Matrix {
  var n,m : int;
  var a : [1..n,1..m] elemType;

  //error: internal failure SYM1167 chpl Version 0.5
  //due to default constructor semantics i think?
  /*
  proc Matrix(arg0,arg1=-1) {
    if (arg1 == -1)
    then {
      n = arg0.domain.dim(0).high;
      m = arg0.domain.dim(1).high;
      a = arg0;
    }
    else {
      n = arg0;
      m = arg1;
      a = 0.0;
    }
  }
  */

  //error: internal failure SYM1167 chpl Version 0.5
  //due to default constructor semantics i think?
  /*
  proc Matrix(arg0) {
    n = arg0.domain.dim(0).high;
    m = arg0.domain.dim(1).high;
    a = arg0;
  }

  proc Matrix(arg0,arg1) {
    n = arg0;
    m = arg1;
    a = 0.0;
  }
  */

  proc dims() {
    return (n,m);
  }

  proc getitem(i,j) {
    return a(i,j);
  }

  proc setitem(i,j,value) {
    a(i,j) = value;
  }

  // does (nxm)*(mx1)
  proc mul(v) {
    var r = new unmanaged Vector(n);

    //Case 1 -- works fine if v is of type [1..m].
    //tested by m4.mul(v3.a) from main(); v3.a is of type [1..m].
    //[i in 1..n] r.setitem(i, + reduce a(i,1..m)*v);

    //Case 2 -- if v is of type Vector, then compiler throws the error:
    //"type mismatch in assignment from _ic_+ to real".
    //tested by m4.mul(v3) from main().
    //Note that v3, of type Vector, wraps an array of type [1..m].
    //Comment: Here [1..m] is wrapped inside a Vector, while case 1 above
    //passes [1..m] in directly. Otherwise, the two are identical.
    [i in 1..n] r.setitem(i, + reduce (a(i,1..m)*v.a));

    //Case 3 -- if v is of type Vector, then this gives a tensor product.
    //tested by m4.mul(v3) from main().
    //Note that v3, of type Vector, wraps an array of type [1..m].
    //Comment: This was to debug case 2. I expected a zipper product here.
    //[i in 1..n] writeln(a(i,1..m)*v.a);

    return r;
  }

  // does (1xn)*(nxm)
  proc rmul(v) {
    var r = new unmanaged Vector(m);

    //Case 1 -- works fine if v is of type [1..n].
    //tested by m4.rmul(v4.a) from main(); v4.a is of type [1..n].
    //[i in 1..m] r.setitem(i, + reduce v*a(1..n,i));

    //Case 2 -- if v is of type Vector, then compiler throws the error:
    //"type mismatch in assignment from _ic_+ to real".
    //tested by m4.rmul(v4) from main().
    //Note that v4, of type Vector, wraps an array of type [1..n].
    //Comment: Here [1..n] is wrapped inside a Vector, while case 1 above
    //passes [1..n] in directly. Otherwise, the two are identical.
    [i in 1..m] r.setitem(i, + reduce (v.a*a(1..n,i)));

    //Case 3 -- if v is of type Vector, then this gives a tensor product.
    //tested by m4.rmul(v4) from main().
    //Note that v4, of type Vector, wraps an array of type [1..n].
    //Comment: This was to debug case 2. I expected a zipper product here.
    //[i in 1..m] writeln(v.a*a(1..n,i));

    return r;
  }
}

proc main() {
  const v1 = new unmanaged Vector(6);
  writeln("v1 = ",v1);

  var a2 : [1..6] elemType = [i in 1..6] i;
  var v2 = new unmanaged Vector(a2.size,a2);
  writeln("v2 = ",v2);

  var a3 : [1..6] elemType = [1..6] 10.0;
  var v3 = new unmanaged Vector(a3.size,a3);
  writeln("v3 = ",v3);

  writeln("v1.normf() = ",v1.normf());
  writeln("v2.normf() = ",v2.normf());
  writeln("v3.normf() = ",v3.normf());

  writeln("v1.inner(v2) = ",v1.inner(v2));
  writeln("v1.inner(v3) = ",v1.inner(v3));
  writeln("v2.inner(v3) = ",v2.inner(v3));

  v2 = v2.gaxpy(2,v3,10.0);
  writeln("v2.gaxpy(2,v3,10.0) = ",v2);

  v2 = v2.scale(3.0);
  writeln("v2.scale(3.0) = ",v2);

  v2 = v2.emul(v3);
  writeln("v2.emul(v3) = ",v2);

  writeln("v2.getitem(5) = ",v2.getitem(5));

  v2.setitem(5,1.5);
  writeln("v2.setitem(5,1.5) = ",v2);

  var v4 = v2.getslice(1,3);
  writeln("v4 = v2.getslice(1,3) = ",v4);

  v2.setslice(1,3,9);
  writeln("v2.setslice(1,3,9) = ",v2);
  writeln("v4 = ",v4);

  v2 = v2.add(v3);
  writeln("v2.add(v3) = ",v2);
  writeln("v3 = ",v3);

  var m1 = new unmanaged Matrix(2,3);
  writeln("m1 = ",m1);

  var a4 : [1..3,1..6] elemType = [(i,j) in {1..3, 1..6}] i+j;
  var m4 = new unmanaged Matrix(a4.domain.dim(0).high,a4.domain.dim(1).high,a4);
  writeln("m4 = ",m4);

  writeln("m1.dims() = ",m1.dims());
  writeln("m4.dims() = ",m4.dims());

  writeln("m4.getitem(2,2) = ",m4.getitem(2,2));

  m4.setitem(2,2,100);
  writeln("m4.setitem(2,2,100) = ",m4);


  var v5 = m4.mul(v3);
  var v6 = m4.rmul(v4);

  writeln("m4.mul(v3) = ",  v5);
  writeln("m4.rmul(v4) = ", v6);

  delete m1;
  delete m4;

  delete v6;
  delete v5;
  delete v4;
  delete v3;
  delete v2;
  delete v1;
}
