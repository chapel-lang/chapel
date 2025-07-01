config const n = 1024;
var count1: atomic int = 0;
var count2: atomic int = 0;

proc test1() {
  const p1 = proc(): void { count1.fetchAdd(1); };
  const p2 = proc() { writeln('Hello from locale: ', here.id); };
  const p3 = proc() { writeln('Hello from locale: ', here.id); };
  const p4 = proc() { writeln('Hello from locale: ', here.id); };
  const p5 = proc() { writeln('Hello from locale: ', here.id); };
  const p6 = proc() { writeln('Hello from locale: ', here.id); };
  const p7 = p2;
  const p8 = p3;
  var arr = [
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); }),
    (proc(): void { count2.fetchAdd(1); })
  ];

  for loc in Locales do on loc {
    for i in 0..#n do p1();
    p2();
    p3();
    p4();
    p5();
    p6();
    p7();
    p8();
    for prc in arr do prc();
  }

  writeln(count1.read());
  writeln(count2.read());

  assert(count1.read() == n * numLocales);
  assert(count2.read() == arr.size * numLocales);

  count1.exchange(0);
  forall loc in Locales do on loc do
    forall i in 0..#n do p1();

  writeln(count1.read());
  assert(count1.read() == n * numLocales);
}

var count3: atomic int = 0;

proc test2() {
  forall loc in Locales do on loc {
    forall i in 0..#n do {
      const p1 = proc(): void { count3.fetchAdd(1); };
      const p2 = proc(): void { count3.fetchAdd(1); };
      const p3 = proc(): void { count3.fetchAdd(1); };
      const p4 = proc(): void { count3.fetchAdd(1); };
      const p5 = proc(): void { count3.fetchAdd(1); };
      const p6 = proc(): void { count3.fetchAdd(1); };
      const p7 = proc(): void { count3.fetchAdd(1); };
      const p8 = proc(): void { count3.fetchAdd(1); };
      const p9 = proc(): void { count3.fetchAdd(1); };
      const p10 = proc(): void { count3.fetchAdd(1); };
      const p11 = proc(): void { count3.fetchAdd(1); };
      const p12 = proc(): void { count3.fetchAdd(1); };
      const p13 = proc(): void { count3.fetchAdd(1); };
      const p14 = proc(): void { count3.fetchAdd(1); };
      const p15 = proc(): void { count3.fetchAdd(1); };
      const p16 = proc(): void { count3.fetchAdd(1); };
      const p17 = proc(): void { count3.fetchAdd(1); };
      const p18 = proc(): void { count3.fetchAdd(1); };
      const p19 = proc(): void { count3.fetchAdd(1); };
      const p20 = proc(): void { count3.fetchAdd(1); };
      const p21 = proc(): void { count3.fetchAdd(1); };
      const p22 = proc(): void { count3.fetchAdd(1); };
      const p23 = proc(): void { count3.fetchAdd(1); };
      const p24 = proc(): void { count3.fetchAdd(1); };
      const p25 = proc(): void { count3.fetchAdd(1); };
      const p26 = proc(): void { count3.fetchAdd(1); };
      const p27 = proc(): void { count3.fetchAdd(1); };
      const p28 = proc(): void { count3.fetchAdd(1); };
      const p29 = proc(): void { count3.fetchAdd(1); };
      const p30 = proc(): void { count3.fetchAdd(1); };
      const p31 = proc(): void { count3.fetchAdd(1); };
      const p32 = proc(): void { count3.fetchAdd(1); };
      p1();
      p2();
      p3();
      p4();
      p5();
      p6();
      p7();
      p8();
      p9();
      p10(); 
      p11(); 
      p12(); 
      p13(); 
      p14(); 
      p15(); 
      p16(); 
      p17(); 
      p18(); 
      p19(); 
      p20(); 
      p21(); 
      p22(); 
      p23(); 
      p24(); 
      p25(); 
      p26(); 
      p27(); 
      p28(); 
      p29(); 
      p30(); 
      p31(); 
      p32(); 
    }
  }
  writeln(count3.read());
  assert(count3.read() == (numLocales * n * 32));
}

extern {
  #include <stdio.h>

  #define PADDING_SIZE 16
  typedef struct test3_r_ {
    int x;
    double y;
    int padding[PADDING_SIZE];
  } test3_r;

  test3_r test3_r_create(int x, double y);
  void test3_r_print(test3_r x);

  test3_r test3_r_create(int x, double y) {
    test3_r ret;
    ret.x = x;
    ret.y = y;
    for (int i = 0; i < PADDING_SIZE; i++) ret.padding[i] = i;
    test3_r_print(ret);
    return ret;
  }

  void test3_r_print(test3_r x) {
    printf("%d, %lf - ", x.x, x.y);
    for (int i = 0; i < PADDING_SIZE; i++) printf("%d ", x.padding[i]);
    printf("!\n");
  }
}

proc test3() {
  const create = test3_r_create;
  const print = test3_r_print;

  const n = (here.id : int(32)) + 1;
  var x = create(n, n*0.2);
  writeln(x.x);
  writeln(x.y);
  print(x);
}

proc test4() {
  writeln('--- LOOP 1: Static Calls ---');
  for loc in Locales do on loc {
    const n = (loc.id : int(32)) + 1;
    var x = test3_r_create(n, n*0.2);
    writeln(x.x);
    writeln(x.y);
    test3_r_print(x);
    writeln('---');
  }

  const create = test3_r_create;
  const print = test3_r_print;

  writeln('--- LOOP 2: Indirect Calls ---');
  for loc in Locales do on loc {
    const n = (loc.id : int(32)) + 1;
    var x = create(n, n*0.2);
    writeln(x.x);
    writeln(x.y);
    print(x);
    writeln('---');
  }

  writeln('--- LOOP 3: Interchange ---');
  for loc in Locales do on loc {
    const n = (loc.id : int(32)) + 1;
    var x1 = test3_r_create(n, n*0.2);
    var x2 = create(n, n*0.2);
    writeln(x1.x, ' == ', x2.x);
    writeln(x1.y, ' == ', x2.y);
    print(x1);
    test3_r_print(x2);
    writeln('---');
  }
}

proc main() {
  test1();
  test2();
  test3();
  test4();
}
