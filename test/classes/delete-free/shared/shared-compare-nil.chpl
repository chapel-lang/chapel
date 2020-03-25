class MyClass {
  var field:int;
}

proc test() {
  var a:shared MyClass?;
  var b:shared MyClass?;

  assert( (a == b) == true );
  assert( (a == nil) == true );
  assert( (nil == b) == true );
  assert( (a != b) == false );
  assert( (a != nil) == false );
  assert( (nil != b) == false );

  var c = new shared MyClass(1);
  assert( (a == c) == false);
  assert( (c == nil) == false);
  assert( (nil == c) == false);
  assert( (a != c) == true );
  assert( (c != nil) == true );
  assert( (nil != c) == true );
}

test();
