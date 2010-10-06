//Common Higher-Order Functions

def reverse(arr) {
  for n in arr.domain by -1 {
    yield arr[n];
  }
}

def foldLeft(arr, init, op) {
  var ret = init;

  for a in arr {
    ret = op(ret, a);
  }
  
  return ret;
}

def foldRight(arr, init, op) {
  var ret = init;

  for a in reverse(arr) {
    ret = op(a, ret);
  }
  
  return ret;
}

def map(arr, op) {
  for a in arr {
    yield(op(a));
  }
}

def filter(arr, op) {
  for a in arr {
    if (op(a)) {
      yield a;
    }
  }
}

def array(x...?n) {
  var ret : [1..n] x(1).type;
  for param i in 1..n do
    ret(i) = x(i);
  
  return ret;
}

def take(arr, n) {
  var i = 0;
  for a in arr {
    if (i < n) {
      yield a;
      i += 1;
    }
    else {
      break;
    }
  }
}

def drop(arr, n) {
  var i = 0;
  for a in arr {
    if (i >= n) {
      yield a;
    }
    i += 1;
  }
}

def splitAt(arr, n) {
  return (take(arr, n), drop(arr, n));
}

def takeWhile(arr, op) {
  for a in arr {
    if (op(a)) {
      yield a;
    }
    else {
      break;
    }
  }
}
 
def dropWhile(arr, op) {
  var keepDropping = true;
  for a in arr {
    if (keepDropping) {
      if (!op(a)) {
	keepDropping = false;
      }
      else {
	continue;
      }
    }

    yield a;
  }
}


