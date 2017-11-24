// This test aims to check whether userGlobal, A, D are deinitialized
// in this order. They used to be deinitialized in the reverse order,
// causing invalid memory accesses.
// Best tested with valgrind.

var D = {1..3,1..3};

var A: [D][D] int;

record UserRecord {
  proc deinit() {
    writeln(D);
    writeln(A);
  }
}

var userGlobal: UserRecord;
