
extern {
  struct aligned32 {
    int x;
    int a __attribute__((aligned (32)));
  };
};

record R1 {
  var field1: aligned32;
}

class C2 {
  var field2: aligned32;
}

class C3 {
  var field3: aligned32;
}

writeln(new R1());
writeln(new C2());
writeln(new C3());
