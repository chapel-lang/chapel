// to run this on linux
// sudo apt-get install mono-complete

// mcs -out:allNumericsBinary.exe allNumericsBinary.cs
// mono allNumericsBinary.exe

 // csharp numeric types
 // sbyte byte short ushort int uint long ulong nint nuint
 // float double decimal

 // ignoring decimal, nint, nuint since Chapel does not have these

using System;

class allNumericsBinary { 
  static void f(sbyte x, sbyte y) {
    Console.WriteLine("f(sbyte,sbyte)");
  }
  static void f(short x, short y) {
    Console.WriteLine("f(short,short)");
  }
  static void f(int x, int y) {
    Console.WriteLine("f(int,int)");
  }
  static void f(long x, long y) {
    Console.WriteLine("f(long,long)");
  }

  static void f(byte x, byte y) {
    Console.WriteLine("f(byte,byte)");
  }
  static void f(ushort x, ushort y) {
    Console.WriteLine("f(ushort,ushort)");
  }
  static void f(uint x, uint y) {
    Console.WriteLine("f(uint,uint)");
  }
  static void f(ulong x, ulong y) {
    Console.WriteLine("f(ulong,ulong)");
  }

  static void f(float x, float y) {
    Console.WriteLine("f(float,float)");
  }
  static void f(double x, double y) {
    Console.WriteLine("f(double,double)");
  }

  static void sec(string arg) {
    Console.Write(" Second actual {0,-12}", arg);
  }

  static void p(string t) {
    Console.Write("               {0,-12} + results in {1}\n", "", t);
  }

  static void Main() {
    sbyte sbyte_ = 0;
    short short_ = 0;
    int int_ = 0;
    long long_ = 0;

    byte byte_ = 0;
    ushort ushort_ = 0;
    uint uint_ = 0;
    ulong ulong_ = 0;

    float float_ = 0;
    double double_ = 0;

    Console.WriteLine("First actual sbyte");
    Console.WriteLine();
    sec("sbyte"); f(sbyte_, sbyte_); p((sbyte_ + sbyte_).GetType().ToString());
    sec("short"); f(sbyte_, short_); p((sbyte_ + short_).GetType().ToString());
    sec("int");   f(sbyte_, int_);   p((sbyte_ + int_).GetType().ToString());
    sec("long");  f(sbyte_, long_);  p((sbyte_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(sbyte_, byte_);  p((sbyte_ + byte_).GetType().ToString());
    sec("ushort"); f(sbyte_, ushort_);p((sbyte_ + ushort_).GetType().ToString());
    sec("uint");   f(sbyte_, uint_);  p((sbyte_ + uint_).GetType().ToString());
    sec("ulong");  f(sbyte_, ulong_); //p((sbyte_ + //ulong_).GetType().ToString()); //err
    Console.WriteLine();

    sec("float");   f(sbyte_, float_); p((sbyte_ + float_).GetType().ToString());
    sec("double");  f(sbyte_, double_);p((sbyte_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual short");
    Console.WriteLine();
    sec("sbyte"); f(short_, sbyte_); p((short_ + sbyte_).GetType().ToString());
    sec("short"); f(short_, short_); p((short_ + short_).GetType().ToString());
    sec("int");   f(short_, int_);   p((short_ + int_).GetType().ToString());
    sec("long");  f(short_, long_);  p((short_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(short_, byte_);  p((short_ + byte_).GetType().ToString());
    sec("ushort"); f(short_, ushort_);p((short_ + ushort_).GetType().ToString());
    sec("uint");   f(short_, uint_);  p((short_ + uint_).GetType().ToString());
    sec("ulong");  f(short_, ulong_); //p((short_ + //ulong_).GetType().ToString()); //err
    Console.WriteLine();

    sec("float");   f(short_, float_); p((short_ + float_).GetType().ToString());
    sec("double");  f(short_, double_);p((short_ + double_).GetType().ToString());

    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual int");
    Console.WriteLine();
    sec("sbyte"); f(int_, sbyte_); p((int_ + sbyte_).GetType().ToString());
    sec("short"); f(int_, short_); p((int_ + short_).GetType().ToString());
    sec("int");   f(int_, int_);   p((int_ + int_).GetType().ToString());
    sec("long");  f(int_, long_);  p((int_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(int_, byte_);  p((int_ + byte_).GetType().ToString());
    sec("ushort"); f(int_, ushort_);p((int_ + ushort_).GetType().ToString());
    sec("uint");   f(int_, uint_);  p((int_ + uint_).GetType().ToString());
    sec("ulong");  f(int_, ulong_); //p((int_ + //ulong_).GetType().ToString()); err
    Console.WriteLine();

    sec("float");   f(int_, float_); p((int_ + float_).GetType().ToString());
    sec("double");  f(int_, double_);p((int_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual long");
    Console.WriteLine();
    sec("sbyte"); f(long_, sbyte_); p((long_ + sbyte_).GetType().ToString());
    sec("short"); f(long_, short_); p((long_ + short_).GetType().ToString());
    sec("int");   f(long_, int_);   p((long_ + int_).GetType().ToString());
    sec("long");  f(long_, long_);  p((long_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(long_, byte_);  p((long_ + byte_).GetType().ToString());
    sec("ushort"); f(long_, ushort_);p((long_ + ushort_).GetType().ToString());
    sec("uint");   f(long_, uint_);  p((long_ + uint_).GetType().ToString());
    sec("ulong");  f(long_, ulong_); //p((long_ + //ulong_).GetType().ToString()); err
    Console.WriteLine();

    sec("float");   f(long_, float_); p((long_ + float_).GetType().ToString());
    sec("double");  f(long_, double_);p((long_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual byte");
    Console.WriteLine();
    sec("sbyte"); f(byte_, sbyte_); p((byte_ + sbyte_).GetType().ToString());
    sec("short"); f(byte_, short_); p((byte_ + short_).GetType().ToString());
    sec("int");   f(byte_, int_);   p((byte_ + int_).GetType().ToString());
    sec("long");  f(byte_, long_);  p((byte_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(byte_, byte_);  p((byte_ + byte_).GetType().ToString());
    sec("ushort"); f(byte_, ushort_);p((byte_ + ushort_).GetType().ToString());
    sec("uint");   f(byte_, uint_);  p((byte_ + uint_).GetType().ToString());
    sec("ulong");  f(byte_, ulong_); p((byte_ + ulong_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(byte_, float_); p((byte_ + float_).GetType().ToString());
    sec("double");  f(byte_, double_);p((byte_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual ushort");
    Console.WriteLine();
    sec("sbyte"); f(ushort_, sbyte_); p((ushort_ + sbyte_).GetType().ToString());
    sec("short"); f(ushort_, short_); p((ushort_ + short_).GetType().ToString());
    sec("int");   f(ushort_, int_);   p((ushort_ + int_).GetType().ToString());
    sec("long");  f(ushort_, long_);  p((ushort_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(ushort_, byte_);  p((ushort_ + byte_).GetType().ToString());
    sec("ushort"); f(ushort_, ushort_);p((ushort_ + ushort_).GetType().ToString());
    sec("uint");   f(ushort_, uint_);  p((ushort_ + uint_).GetType().ToString());
    sec("ulong");  f(ushort_, ulong_); p((ushort_ + ulong_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(ushort_, float_); p((ushort_ + float_).GetType().ToString());
    sec("double");  f(ushort_, double_);p((ushort_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual uint");
    Console.WriteLine();
    sec("sbyte"); f(uint_, sbyte_); p((uint_ + sbyte_).GetType().ToString());
    sec("short"); f(uint_, short_); p((uint_ + short_).GetType().ToString());
    sec("int");   f(uint_, int_);   p((uint_ + int_).GetType().ToString());
    sec("long");  f(uint_, long_);  p((uint_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(uint_, byte_);  p((uint_ + byte_).GetType().ToString());
    sec("ushort"); f(uint_, ushort_);p((uint_ + ushort_).GetType().ToString());
    sec("uint");   f(uint_, uint_);  p((uint_ + uint_).GetType().ToString());
    sec("ulong");  f(uint_, ulong_); p((uint_ + ulong_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(uint_, float_); p((uint_ + float_).GetType().ToString());
    sec("double");  f(uint_, double_);p((uint_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual ulong");
    Console.WriteLine();
    sec("sbyte"); f(ulong_, sbyte_); //p((ulong_ + //sbyte_).GetType().ToString()); err
    sec("short"); f(ulong_, short_); //p((ulong_ + //short_).GetType().ToString()); err
    sec("int");   f(ulong_, int_);   //p((ulong_ + //int_).GetType().ToString()); err
    sec("long");  f(ulong_, long_);  //p((ulong_ + //long_).GetType().ToString()); err
    Console.WriteLine();

    sec("byte");   f(ulong_, byte_);  p((ulong_ + byte_).GetType().ToString());
    sec("ushort"); f(ulong_, ushort_);p((ulong_ + ushort_).GetType().ToString());
    sec("uint");   f(ulong_, uint_);  p((ulong_ + uint_).GetType().ToString());
    sec("ulong");  f(ulong_, ulong_); p((ulong_ + ulong_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(ulong_, float_); p((ulong_ + float_).GetType().ToString());
    sec("double");  f(ulong_, double_);p((ulong_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual float");
    Console.WriteLine();
    sec("sbyte"); f(float_, sbyte_); p((float_ + sbyte_).GetType().ToString());
    sec("short"); f(float_, short_); p((float_ + short_).GetType().ToString());
    sec("int");   f(float_, int_);   p((float_ + int_).GetType().ToString());
    sec("long");  f(float_, long_);  p((float_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(float_, byte_);  p((float_ + byte_).GetType().ToString());
    sec("ushort"); f(float_, ushort_);p((float_ + ushort_).GetType().ToString());
    sec("uint");   f(float_, uint_);  p((float_ + uint_).GetType().ToString());
    sec("ulong");  f(float_, ulong_); p((float_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(float_, float_); p((float_ + float_).GetType().ToString());
    sec("double");  f(float_, double_);p((float_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();


    Console.WriteLine("First actual double");
    Console.WriteLine();
    sec("sbyte"); f(double_, sbyte_); p((double_ + sbyte_).GetType().ToString());
    sec("short"); f(double_, short_); p((double_ + short_).GetType().ToString());
    sec("int");   f(double_, int_);   p((double_ + int_).GetType().ToString());
    sec("long");  f(double_, long_);  p((double_ + long_).GetType().ToString());
    Console.WriteLine();

    sec("byte");   f(double_, byte_);  p((double_ + byte_).GetType().ToString());
    sec("ushort"); f(double_, ushort_);p((double_ + ushort_).GetType().ToString());
    sec("uint");   f(double_, uint_);  p((double_ + uint_).GetType().ToString());
    sec("ulong");  f(double_, ulong_); p((double_ + ulong_).GetType().ToString());
    Console.WriteLine();

    sec("float");   f(double_, float_); p((double_ + float_).GetType().ToString());
    sec("double");  f(double_, double_);p((double_ + double_).GetType().ToString());
    Console.WriteLine();

    Console.WriteLine();
    Console.WriteLine();
  }
}
