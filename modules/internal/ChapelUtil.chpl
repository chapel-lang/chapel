// Internal data structures module

param _INIT_STACK_SIZE = 8;

class _stack {
  type eltType;
  var  size: int;
  var  top: int;
  var  data: _ddata(eltType);
  
  def initialize() {
    top = 0;
    size = _INIT_STACK_SIZE;
    data = new _ddata(eltType);
    data.init(8);
  }

  def push( e: eltType) {
    if (top == size-1) {  // supersize as necessary
      size *= 2;
      var supersize = new _ddata(eltType);
      supersize.init(size);
      [i in 0..(size/2)-1] supersize[i] = data[i];
      data = supersize;
    }
    data[top] = e;
    top += 1;
  }

  def pop() {
    var e: eltType;
    if top>0 then {
      top -= 1;
      e = data[top];
    } else {
      halt( "pop() on empty stack");
    }
    return e;
  }

  def empty() {
    top = 0;
  }

  def length {
    return top;
  }

  def writeThis(f: Writer) {
    for i in 0..top-1 do f.write(" ", data[i]);
  }
}

// true iff an overflow would occur for a + b
//
// This can be written as:
//   return if a < 0 then b < min(t) - a else b > max(t) - a;
// but I found it easier to convince myself it was right as is.
//
def addOverflowsType(a: ?t, b: t) {
  if a < 0 {
    if b > 0 {
      return false;
    } else {
      if b < min(t) - a {
        return true;
      } else {
        return false;
      }
    }
  } else {
    if b < 0 {
      return false;
    } else {
      if b > max(t) - a {
        return true;
      } else {
        return false;
      }
    }
  }
}

// true iff an overflow would occur for a - b
//
// This can be written as:
//   return if a > 0 then b < min(t) + a else b > max(t) + a;
// but I found it easier to convince myself it was right as is.
//
def subUnderflowsType(a: ?t, b: t) {
  if a > 0 {
    if b > 0 {
      return false;
    } else {
      if b < min(t) + a {
        return true;
      } else {
        return false;
      }
    }
  } else {
    if b < 0 {
      return false;
    } else {
      if b > max(t) + a {
        return true;
      } else {
        return false;
      }
    }
  }
}
