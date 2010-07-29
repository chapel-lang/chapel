def reverse(arr) {
  for n in arr.domain by -1 {
    yield arr[n];
  }
}

def fold_left(arr, init, op) {
  var ret = init;

  for a in arr {
    ret = op(ret, a);
  }
  
  return ret;
}

def fold_right(arr, init, op) {
  var ret = init;

  for a in reverse(arr) {
    ret = op(a, ret);
  }
  
  return ret;
}

def map(arr, op) {
  return op(arr);
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

def even(x:int) {
  if ((x % 2) == 0) {
    return true;
  }
  else {
    return false;
  }
}

def odd(x:int) {
  return !even(x);
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

/*
def reverse(a, l...?n) {
  var i = a.domain.dim(n+1).length;
  
  if ((n+1) == a.domain.rank) {
    while (i >= 1) {
      //we're at the deepest dimension, so start yielding values at this position
      yield a((...l), i); 
      i -= 1;
    }
  }
  else {
    while (i >= 1) {
      //create a call with arity+1 so that we can see the next dimension
      for k in reverse(a, (...l), i){
	yield k;
      }
      i -= 1;
    }
  }
}

def reverse(a) {
  //if have more than one dimension, instead start a cursor and recurse to see the other dimensions
  if (a.domain.rank > 1) {
    var i = a.domain.dim(1).length;
    while (i >= 1) {
      for j in reverse(a, i) {
	yield j;
      }
      
      i -= 1;
    }
  }
  else {
    //This is only a 1D domain, don't recurse just use the cursor as our position
    var i = a.domain.dim(1).length;
    while (i >= 1) {
      yield a(i);
      i -= 1;
    }
  }
}    
*/

var a : [1..3, 1..3, 1..4] int;
var b : [1..3, 1..3] int;
var c : [1..3] int;

for i in (1..3) {
  for j in (1..3) {
    for k in (1..4) {
      a(i,j,k) = i * 100 + j * 10 + k;
    }
    b(i,j) = i * 10 + j;
  }
  c(i) = i;
}

writeln(fold_left(c, 0, lambda(x:int,y:int) { return x-y; }));
writeln(fold_right(c, 0, lambda(x:int,y:int) { return x-y; }));
writeln(filter(c, odd));

writeln(map(a, lambda(x:int) { return x+1; }));
writeln(lambda(x:int) { return x+2; }(a));
writeln(lambda(x:int) { return x+3; }( array(1, 2, 3, 4) ));

writeln(drop(array(1, 2, 3, 4), 2));
writeln(take(array(1, 2, 3, 4), 2));
writeln(splitAt(array(1, 2, 3, 4), 2));

writeln(dropWhile(array(1, 2, 3, 4), lambda(x:int) { return x <= 2; }));
writeln(takeWhile(array(1, 2, 3, 4), lambda(x:int) { return x <= 2; }));

/*
writeln("3-D array");
writeln(a);
for n in reverse(a) {
  writeln(n);
}

writeln("2-D array");
writeln(b);
for n in reverse(b) {
  writeln(n);
}

writeln("1-D array");
writeln(c);
for n in reverse(c) {
  writeln(n);
}

for n in a.domain by -1 {
  writeln(a[n]);
}
*/

	
	