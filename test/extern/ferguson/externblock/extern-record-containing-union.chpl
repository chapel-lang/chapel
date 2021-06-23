extern {
  union someUnion {
    char alpha;
    int num;
  };
  struct someStruct {
    union someUnion u;
  };
}

var x: someStruct;
var y: someStruct;
x.u.num = 0;
y = x;
writef("%xn\n", x.u.alpha);
writef("%xn\n", x.u.num);
writef("%xn\n", y.u.alpha);
writef("%xn\n", y.u.num);
