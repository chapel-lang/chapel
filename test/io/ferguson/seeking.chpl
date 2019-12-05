use Random, IO, SysCTypes;

config const path = "binary-output.bin";
config const maxbyte = 255;
config const maxint = 32*1024;
config const seed = SeedGenerator.oddCurrentTime;

config const bufsz = 0;
extern var qbytes_iobuf_size:size_t;

if bufsz > 0 {
  qbytes_iobuf_size = bufsz:size_t;
}

proc test1() {
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);
    w.write(1:uint(8));

    w.seek(16);
    w.write(2:uint(8));

    w.seek(32);
    w.write(3:uint(8));
    
    w.seek(64);
    w.write(4:uint(8));
  }

  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    var b:uint(8);
    var got:bool;
    
    got = r.read(b);
    assert(got && b == 1);

    r.seek(16);
    got = r.read(b);
    assert(got && b == 2);

    r.seek(32);
    got = r.read(b);
    assert(got && b == 3);
    
    r.seek(64);
    got = r.read(b);
    assert(got && b == 4);
  }
}

proc test2() {
  // Create a file by writing in reverse.
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for i in 0..maxbyte by -1 {
      w.seek(i);
      w.write(i:uint(8));
    }
  }

  // Check the data
  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for i in 0..maxbyte {
      var b:uint(8);
      var got:bool;

      got = r.read(b);
      assert(got && b == i:uint(8));
    }
  }
}

proc test3() {
  // Create a file by writing forward and read it in reverse
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for i in 0..maxbyte {
      w.write(i:uint(8));
    }
  }

  // Check the data
  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for i in 0..maxbyte by -1 {
      var b:uint(8);
      var got:bool;

      r.seek(i);
      got = r.read(b);
      assert(got && b == i:uint(8));
    }
  }
}

proc test4() {
  // Create a file by writing in reverse.
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for i in 0..maxint by -1 {
      w.seek(i*numBytes(int));
      w.write(i);
    }
  }

  // Check the data
  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for i in 0..maxint {
      var b:int;
      var got:bool;

      got = r.read(b);
      assert(got && b == i);
    }
  }
}

proc test5() {
  // Create a file by writing forward and read it in reverse
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for i in 0..maxint {
      w.write(i);
    }
  }

  // Check the data
  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for i in 0..maxint by -1 {
      var b:int;
      var got:bool;

      r.seek(i*numBytes(int));
      got = r.read(b);
      assert(got && b == i);
    }
  }
}

proc test6() {
  // Write 0..maxint using a permutation and then read it normally
  var A:[0..maxint] int;
  Random.permutation(A, seed=seed);

  // Write to the permutation
  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for a in A {
      w.seek(a*numBytes(int));
      w.write(a);
    }
  }

  // Check the data
  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for i in 0..maxint {
      var b:int;
      var got:bool;

      got = r.read(b);
      assert(got && b == i);
    }
  }
}

proc test7() {
  // Write 0..maxint and then read with a permutation
  var A:[0..maxint] int;
  Random.permutation(A, seed=seed);

  {
    var f = open(path, iomode.cw, style = new iostyle(binary=1));
    var w = f.writer(locking=false);

    for i in 0..maxint {
      w.write(i);
    }
  }

  {
    var f = open(path, iomode.r, style = new iostyle(binary=1));
    var r = f.reader(locking=false);
    for a in A {
      var b:int;
      var got:bool;

      r.seek(a*numBytes(int));
      got = r.read(b);
      assert(got && b == a);
    }
  }
}



test1();
test2();
test3();
test4();
test5();
test6();
test7();
