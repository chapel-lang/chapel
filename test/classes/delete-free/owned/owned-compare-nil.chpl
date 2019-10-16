class MyClass {
  var field:int;
}

proc test() {
  var a:owned MyClass?;
  var b:owned MyClass?;

  assert( (a == b) == true );
  assert( (a == nil) == true );
  assert( (nil == b) == true );
  assert( (a != b) == false );
  assert( (a != nil) == false );
  assert( (nil != b) == false );

  var c = new owned MyClass(1);
  assert( (a == c) == false);
  assert( (c == nil) == false);
  assert( (nil == c) == false);
  assert( (a != c) == true );
  assert( (c != nil) == true );
  assert( (nil != c) == true );
}

test();
