{ // bool
  var t = true;
  var f = false;

  print(t:_bytes);
  print(f:_bytes);

  var tb = new _bytes("true");
  var fb = new _bytes("false");

  var a = 10;

  print(tb:bool);
  print(fb:bool);
}

{ // int
  var i8: uint(8) = 8;
  var i16: uint(16) = 16;
  var i32: uint(32) = 32;
  var i64: uint(64) = 64;

  print(i8:_bytes);
  print(i16:_bytes);
  print(i32:_bytes);
  print(i64:_bytes);

  var b8 = new _bytes("8");
  var b16 = new _bytes("16");
  var b32 = new _bytes("32");
  var b64 = new _bytes("64");

  print(b8:int(8));
  print(b16:int(16));
  print(b32:int(32));
  print(b64:int(64));
}

{ // real & img
  var r = 3.14;
  var i = 3i;

  print(r:_bytes);
  print(i:_bytes);

  var rb = new _bytes("3.14");
  var ib = new _bytes("3i");

  print(r:real);
  print(i:imag);
}

{ // complex

  var c = 3+2i;

  print(c:_bytes);

  var cb = "3+2i";

  print(cb:complex);

}

{ // string
  var s = "some string";
  print(s:_bytes);

  var b = "some bytes":_bytes;
  print(b:string);
}

{ // enum
  enum E { Foo, Bar };

  print(E.Foo:_bytes);
  print(E.Bar:_bytes);

}

proc print(val) {
  writeln("Value = ", val, " Type = ", val.type:string);
}
