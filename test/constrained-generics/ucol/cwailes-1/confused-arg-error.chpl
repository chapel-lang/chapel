
interface I(T) {
  proc foo(T):T;
}

// There is a matching implementation of foo() below,
// so this is correct, right?
implements I(int);

proc foo(x:int):int {
  return x + 1;
}
