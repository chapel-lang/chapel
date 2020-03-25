{ // bool
  var t = true;
  var f = false;

  print(t:bytes);
  print(f:bytes);

  var tb = b"true";
  var fb = b"false";

  var a = 10;

  print(tb:bool);
  print(fb:bool);
}

{ // int
  var i8: uint(8) = 8;
  var i16: uint(16) = 16;
  var i32: uint(32) = 32;
  var i64: uint(64) = 64;

  print(i8:bytes);
  print(i16:bytes);
  print(i32:bytes);
  print(i64:bytes);

  var b8 = b"8";
  var b16 = b"16";
  var b32 = b"32";
  var b64 = b"64";

  print(b8:int(8));
  print(b16:int(16));
  print(b32:int(32));
  print(b64:int(64));
}

{ // real & img
  var r = 3.14;
  var i = 3i;

  print(r:bytes);
  print(i:bytes);

  var rb = b"3.14";
  var ib = b"3i";

  print(r:real);
  print(i:imag);
}

{ // complex

  var c = 3+2i;

  print(c:bytes);

  var cb = b"3+2i";

  print(cb:complex);

}

{ // string
  var s = "some string";
  print(s:bytes);
}

{ // enum
  enum E { Foo, Bar };

  print(E.Foo:bytes);
  print(E.Bar:bytes);

  print((b"Foo"):E);
  print((b"Bar"):E);
  print((b"E.Foo"):E);
  print((b"E.Bar"):E);

}

proc print(val) {
  writeln("Value = ", val, " Type = ", val.type:string);
}
