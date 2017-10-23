// Testing coercion & best candidate rules

module RealComplexImag32Foos {

  proc foo(x:real(32)) {
    writeln("    r32 ", x);
  }
  proc foo(x:complex(64)) {
    writeln("    c64 ", x);
  }
  proc foo(x:imag(32)) {
    writeln("    i32 ", x);
  }
}

module RealComplexImag64Foos {
  proc foo(x:real(64)) {
    writeln("    r64 ", x);
  }
  proc foo(x:complex(128)) {
    writeln("    c128 ", x);
  }
  proc foo(x:imag(64)) {
    writeln("    i64 ", x);
  }
}

module Int8Foos {
  proc foo(x:uint(8)) {
    writeln("    u8 ", x);
  }
  proc foo(x:int(8)) {
    writeln("    i8 ", x);
  }
}

module Int16_32Foos {
  proc foo(x:uint(16)) {
    writeln("    u16 ", x);
  }
  proc foo(x:uint(32)) {
    writeln("    u32 ", x);
  }
  proc foo(x:int(16)) {
    writeln("    i16 ", x);
  }
  proc foo(x:int(32)) {
    writeln("    i32 ", x);
  }
}

module Int64Foos {
  proc foo(x:uint(64)) {
    writeln("    u64 ", x);
  }
  proc foo(x:int(64)) {
    writeln("    i64 ", x);
  }
}

module TestGlobals {
  var u8:uint(8);
  var i8:int(8);
  var u16:uint(16);
  var i16:int(16);
  var u32:uint(32);
  var i32:int(32);
  var u64:uint(64);
  var i64:int(64);

  var r32:real(32);
  var r64:real(64);

  var c64:complex(64);
  var c128:complex(128);
}

module Test0 {
  use TestGlobals;
  use RealComplexImag32Foos;

  proc test0() {
    writeln("Test0 uses only RealComplexImag32Foos");

    writeln("foo(u8)");
    foo(u8);
    writeln("foo(i8)");
    foo(i8);
    writeln("foo(u16)");
    foo(u16);
    writeln("foo(i16)");
    foo(i16);
    //writeln("foo(u32)");
    //foo(u32);
    //writeln("foo(i32)");
    //foo(i32);
    //writeln("foo(u64)");
    //foo(u64);
    //writeln("foo(i64)");
    //foo(i64);

    writeln("foo(r32)");
    foo(r32);
    //writeln("foo(r64)");
    //foo(r64);

    writeln("foo(c64)");
    foo(c64);
    //writeln("foo(c128)");
    //foo(c128);

    // Test param coercions
    writeln("foo(1)");
    foo(1);
    writeln("foo(-1)");
    foo(-1);
    writeln("foo(1000)");
    foo(1000);
    writeln("foo(-1000)");
    foo(-1000);
    writeln("foo(100000)");
    foo(100000);
    writeln("foo(-100000)");
    foo(-100000);
    //writeln("foo(10000000000)");
    //foo(10000000000);
    //writeln("foo(-10000000000)");
    //foo(-10000000000);
    
    // These should work, but don't, because the compiler
    // is unwilling to param-convert.
    writeln("foo(0.0)");
    foo(0.0);
    //writeln("foo(0.0+0.0i)"); // TODO: could convert param complexes
    //foo(0.0+0.0i);
  }
}

module Test1 {
  use TestGlobals;
  use RealComplexImag32Foos;
  use RealComplexImag64Foos;
  
  proc test1() {
    writeln("Test1 uses only RealFoos");

    writeln("foo(u8)");
    foo(u8);
    writeln("foo(i8)");
    foo(i8);
    writeln("foo(u16)");
    foo(u16);
    writeln("foo(i16)");
    foo(i16);
    writeln("foo(u32)");
    foo(u32);
    writeln("foo(i32)");
    foo(i32);
    writeln("foo(u64)");
    foo(u64);
    writeln("foo(i64)");
    foo(i64);

    writeln("foo(r32)");
    foo(r32);
    writeln("foo(r64)");
    foo(r64);

    writeln("foo(c64)");
    foo(c64);
    writeln("foo(c128)");
    foo(c128);

    // Test param coercions
    writeln("foo(1)");
    foo(1);
    writeln("foo(-1)");
    foo(-1);
    writeln("foo(1000)");
    foo(1000);
    writeln("foo(-1000)");
    foo(-1000);
    writeln("foo(100000)");
    foo(100000);
    writeln("foo(-100000)");
    foo(-100000);
    writeln("foo(10000000000)");
    foo(10000000000);
    writeln("foo(-10000000000)");
    foo(-10000000000);
 
    writeln("foo(0.0)");
    foo(0.0);
    writeln("foo(0.0+0.0i)");
    foo(0.0+0.0i);
  }
}

module Test2 {
  use TestGlobals;
  use RealComplexImag32Foos;
  use RealComplexImag64Foos;
  use Int16_32Foos;
  
  proc test2() {
    writeln("Test2 uses only RealFoos");

    writeln("foo(u8)");
    foo(u8);
    writeln("foo(i8)");
    foo(i8);
    writeln("foo(u16)");
    foo(u16);
    writeln("foo(i16)");
    foo(i16);
    writeln("foo(u32)");
    foo(u32);
    writeln("foo(i32)");
    foo(i32);
    writeln("foo(u64)");
    foo(u64);
    writeln("foo(i64)");
    foo(i64);

    writeln("foo(r32)");
    foo(r32);
    writeln("foo(r64)");
    foo(r64);

    writeln("foo(c64)");
    foo(c64);
    writeln("foo(c128)");
    foo(c128);

    // Test param coercions
    writeln("foo(1)");
    foo(1);
    writeln("foo(-1)");
    foo(-1);
    writeln("foo(1000)");
    foo(1000);
    writeln("foo(-1000)");
    foo(-1000);
    writeln("foo(100000)");
    foo(100000);
    writeln("foo(-100000)");
    foo(-100000);
    writeln("foo(10000000000)");
    foo(10000000000);
    writeln("foo(-10000000000)");
    foo(-10000000000);
 
    writeln("foo(0.0)");
    foo(0.0);
    writeln("foo(0.0+0.0i)");
    foo(0.0+0.0i);
  }
}


module Test3 {
  use TestGlobals;
  use RealComplexImag32Foos;
  use RealComplexImag64Foos;
  use Int8Foos;
  use Int16_32Foos;
  
  proc test3() {
    writeln("Test3 uses only RealFoos Int8Foos and Int16_32Foos");

    writeln("foo(u8)");
    foo(u8);
    writeln("foo(i8)");
    foo(i8);
    writeln("foo(u16)");
    foo(u16);
    writeln("foo(i16)");
    foo(i16);
    writeln("foo(u32)");
    foo(u32);
    writeln("foo(i32)");
    foo(i32);
    writeln("foo(u64)");
    foo(u64);
    writeln("foo(i64)");
    foo(i64);

    writeln("foo(r32)");
    foo(r32);
    writeln("foo(r64)");
    foo(r64);

    writeln("foo(c64)");
    foo(c64);
    writeln("foo(c128)");
    foo(c128);

    // Test param coercions
    writeln("foo(1)");
    foo(1);
    writeln("foo(-1)");
    foo(-1);
    writeln("foo(1000)");
    foo(1000);
    writeln("foo(-1000)");
    foo(-1000);
    writeln("foo(100000)");
    foo(100000);
    writeln("foo(-100000)");
    foo(-100000);
    writeln("foo(10000000000)");
    foo(10000000000);
    writeln("foo(-10000000000)");
    foo(-10000000000);

    writeln("foo(0.0)");
    foo(0.0);
    writeln("foo(0.0+0.0i)");
    foo(0.0+0.0i);
  }
}

module Test4 {
  use TestGlobals;
  use RealComplexImag32Foos;
  use RealComplexImag64Foos;
  use Int8Foos;
  use Int16_32Foos;
  use Int64Foos;
  
  proc test4() {
    writeln("Test4 uses only RealFoos Int8Foos Int16_32Foos and Int64Foos");

    writeln("foo(u8)");
    foo(u8);
    writeln("foo(i8)");
    foo(i8);
    writeln("foo(u16)");
    foo(u16);
    writeln("foo(i16)");
    foo(i16);
    writeln("foo(u32)");
    foo(u32);
    writeln("foo(i32)");
    foo(i32);
    writeln("foo(u64)");
    foo(u64);
    writeln("foo(i64)");
    foo(i64);

    writeln("foo(r32)");
    foo(r32);
    writeln("foo(r64)");
    foo(r64);

    writeln("foo(c64)");
    foo(c64);
    writeln("foo(c128)");
    foo(c128);

    // Test param coercions
    writeln("foo(1)");
    foo(1);
    writeln("foo(-1)");
    foo(-1);
    writeln("foo(1000)");
    foo(1000);
    writeln("foo(-1000)");
    foo(-1000);
    writeln("foo(100000)");
    foo(100000);
    writeln("foo(-100000)");
    foo(-100000);
    writeln("foo(10000000000)");
    foo(10000000000);
    writeln("foo(-10000000000)");
    foo(-10000000000);
   
    writeln("foo(0.0)");
    foo(0.0);
    writeln("foo(0.0+0.0i)");
    foo(0.0+0.0i);
  }
}

module Main {
  use Test0, Test1, Test2, Test3, Test4;

  proc main() {
    test0();
    test1();
    test2();
    test3();
    test4();
    writeln("DONE");
  }
}
