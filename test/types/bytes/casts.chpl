{ // bool
  var t = true;
  var f = false;

  print(t:bytes);
  print(f:bytes);

  var tb = new bytes("true");
  var fb = new bytes("false");

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

  var b8 = new bytes("8");
  var b16 = new bytes("16");
  var b32 = new bytes("32");
  var b64 = new bytes("64");

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

  var rb = new bytes("3.14");
  var ib = new bytes("3i");

  print(r:real);
  print(i:imag);
}

{ // complex

  var c = 3+2i;

  print(c:bytes);

  var cb = "3+2i";

  print(cb:complex);

}

{ // string
  var s = "some string";
  print(s:bytes);

  var b = "some bytes":bytes;
  print(b:string);
}

{ // enum
  enum E { Foo, Bar };

  print(E.Foo:bytes);
  print(E.Bar:bytes);

}

proc print(val) {
  writeln("Value = ", val, " Type = ", val.type:string);
}
