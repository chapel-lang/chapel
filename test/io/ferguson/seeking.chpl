use Random, IO, CTypes;

config const path = "binary-output.bin";
config const maxbyte = 255;
config const maxint = 1024;

config const bufsz = 0;
extern var qbytes_iobuf_size:c_size_t;

if bufsz > 0 {
  qbytes_iobuf_size = bufsz:c_size_t;
}

proc writer() {
  var f = open(path, ioMode.cw);
  var w = f.writer(locking=false, serializer=new binarySerializer());
  return w;
}

proc reader() {
  var f = open(path, ioMode.r);
  var r = f.reader(locking=false, deserializer=new binaryDeserializer());
  return r;
}

proc test1() {
  {
    var w = writer();
    w.write(1:uint(8));

    w.seek(16..);
    w.write(2:uint(8));

    w.seek(32..);
    w.write(3:uint(8));
    
    w.seek(64..);
    w.write(4:uint(8));
  }

  {
    var r = reader();
    var b:uint(8);
    var got:bool;
    
    got = r.read(b);
    assert(got && b == 1);

    r.seek(16..);
    got = r.read(b);
    assert(got && b == 2);

    r.seek(32..);
    got = r.read(b);
    assert(got && b == 3);
    
    r.seek(64..);
    got = r.read(b);
    assert(got && b == 4);
  }
}

proc test2() {
  // Create a file by writing in reverse.
  {
    var w = writer();

    for i in 0..maxbyte by -1 {
      w.seek(i..);
      w.write(i:uint(8));
    }
  }

  // Check the data
  {
    var r = reader();
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
    var w = writer();

    for i in 0..maxbyte {
      w.write(i:uint(8));
    }
  }

  // Check the data
  {
    var r = reader();
    for i in 0..maxbyte by -1 {
      var b:uint(8);
      var got:bool;

      r.seek(i..);
      got = r.read(b);
      assert(got && b == i:uint(8));
    }
  }
}

proc test4() {
  // Create a file by writing in reverse.
  {
    var w = writer();

    for i in 0..maxint by -1 {
      w.seek(i*numBytes(int)..);
      w.write(i);
    }
  }

  // Check the data
  {
    var r = reader();
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
    var w = writer();

    for i in 0..maxint {
      w.write(i);
    }
  }

  // Check the data
  {
    var r = reader();
    for i in 0..maxint by -1 {
      var b:int;
      var got:bool;

      r.seek(i*numBytes(int)..);
      got = r.read(b);
      assert(got && b == i);
    }
  }
}

proc test6() {
  // Write 0..maxint using a permutation and then read it normally
  var A = Random.permute(0..maxint);

  // Write to the permutation
  {
    var w = writer();

    for a in A {
      w.seek(a*numBytes(int)..);
      w.write(a);
    }
  }

  // Check the data
  {
    var r = reader();
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
  var A = Random.permute(0..maxint);

  {
    var w = writer();

    for i in 0..maxint {
      w.write(i);
    }
  }

  {
    var r = reader();
    for a in A {
      var b:int;
      var got:bool;

      r.seek(a*numBytes(int)..);
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
