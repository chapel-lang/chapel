/***
*/
module ExternBlock {
  extern {
    #include <assert.h>
    void foo(int64_t (*)(int64_t, int64_t));
    void foo(int64_t (*fn)(int64_t, int64_t)) {
      int n = fn(4, 4);
      assert(n == 8);
    }
  }

  extern proc foo(fn: proc(_: int, _: int): int): void;

  proc test5() {
    // Call 'foo' with our proc literal.
    foo(proc(x: int, y: int) {
      return x + y;
    });

    // Call again but with a variable.
    var fn = proc(x: int, y: int) { return x + y; };
    foo(fn);
  }
  test5();
}

