//Common Higher-Order Functions

iter reverse(arr) {
  for n in arr.domain by -1 {
    yield arr[n];
  }
}

proc foldLeft(arr, init, op) {
  var ret = init;

  for a in arr {
    ret = op(ret, a);
  }
  
  return ret;
}

proc foldRight(arr, init, op) {
  var ret = init;

  for a in reverse(arr) {
    ret = op(a, ret);
  }
  
  return ret;
}

iter map(arr, op) {
  for a in arr {
    yield(op(a));
  }
}

iter filter(arr, op) {
  for a in arr {
    if (op(a)) {
      yield a;
    }
  }
}

proc array(x...?n) {
  var ret : [1..n] x(1).type;
  for param i in 0..n-1 do
    ret(i+1) = x(i);
  
  return ret;
}

iter take(arr, n) {
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

iter drop(arr, n) {
  var i = 0;
  for a in arr {
    if (i >= n) {
      yield a;
    }
    i += 1;
  }
}

proc splitAt(arr, n) {
  return (take(arr, n), drop(arr, n));
}

iter takeWhile(arr, op) {
  for a in arr {
    if (op(a)) {
      yield a;
    }
    else {
      break;
    }
  }
}
 
iter dropWhile(arr, op) {
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


