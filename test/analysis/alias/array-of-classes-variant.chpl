module mymodule {

  config const i = 1;
  config const j = 1;
  config const n = 2;

  class MyClass {
    var x: int;
  }

  proc makeArray(instance: borrowed MyClass) {
    var AA = [instance, instance];
    return AA;
  }

  proc main() {
    var instance = new borrowed MyClass(0);
    var A = makeArray(instance);
    var B = makeArray(instance);

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
}
