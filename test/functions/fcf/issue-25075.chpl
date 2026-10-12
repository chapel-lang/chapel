use CTypes;

type D = [1..3] real(64);

proc normalize(ref ret: D, const ref input: D): void {
  for i in 1..3 {
    ret[i] = input[i];
  }
}

var f = c_ptrTo(normalize);

var A, B: D;
A = [1.0, 2.0, 3.0];

extern {
  static void call(void (*f)(void*, void*), void* ret, void* input) {
    f(ret, input);
  }
}
writeln(B);
call(f, c_addrOf(B), c_addrOf(A));
writeln(B);
