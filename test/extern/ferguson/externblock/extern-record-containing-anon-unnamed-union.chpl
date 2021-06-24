extern {
  typedef struct someStruct {
    union {
      char alpha;
      int num;
    };
  } someStruct;
}

var x: someStruct;
var y: someStruct;
x.num = 0;
y = x;
writef("%xn\n", x.alpha);
writef("%xn\n", x.num);
writef("%xn\n", y.alpha);
writef("%xn\n", y.num);
