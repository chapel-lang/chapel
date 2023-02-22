module Library {
  proc callFoo(type t) {
    return foo;
  }
}

module Application {
  use Library;

  proc foo { return 1; }

  proc main() {
    callFoo(int);
  }
}
