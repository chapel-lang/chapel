config const i = 1;
config const j = 1;
config const n = 2;

class MyClass {
  var x: int;
}

proc setupArrays(A, B, instance) lifetime A < instance, B < instance, B < A {
  A[1] = instance;
  A[2] = A[1];
  B[1] = A[1];
  B[2] = B[1];
}

proc main() {
  var A:[1..n] MyClass;
  var B:[1..n] MyClass;
 
  var instance = new borrowed MyClass(0);
  setupArrays(A, B, instance);

  var sum = 0; 
  for k in 1..n {
    sum += A[i].x + A[j].x;
    A[i].x += 1;
    // We shouldn't conclude that A[i].x doesn't alias A[j].x
  }
  writeln(sum);

  instance.x = 0;
  sum = acceptsArrays(A, B);
  writeln(sum);
  
  instance.x = 0;
  sum = acceptsClasses(A[i], A[j]);
  writeln(sum);

  instance.x = 0;
  sum = acceptsIntRefs(A[i].x, A[j].x);
  writeln(sum);
}

proc acceptsArrays(a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[i].x + b[j].x;
    a[i].x += 1;
  }
  return sum;
}

proc acceptsClasses(a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a.x + b.x;
    a.x += 1;
  }
  return sum;
}

proc acceptsIntRefs(ref a, ref b) {
  var sum = 0;
  b += 0;
  for k in 1..n {
    sum += a + b;
    a += 1;
  }
  return sum;
}
