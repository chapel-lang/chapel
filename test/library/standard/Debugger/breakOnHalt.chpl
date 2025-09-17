proc foo(x) {
  if x > 0 {
    halt("x is greater than 0");
  }
}

proc main() {

  foo(10);

}
