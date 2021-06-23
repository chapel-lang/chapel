extern {
  typedef struct someStruct {
    union {
      char alpha;
      int num;
    } u;
  } someStruct;
}

var x: someStruct;
var y: someStruct;
x.u.num = 0;
y = x;
writef("%xn\n", x.u.alpha);
writef("%xn\n", x.u.num);
writef("%xn\n", y.u.alpha);
writef("%xn\n", y.u.num);
