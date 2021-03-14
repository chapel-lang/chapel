use CPtr;
proc test(type t, v1:t, v2:t) {
  var x:t = v1;
  var y:t = v2;

  writeln("test(", t:string, ",", v1, ",", v2, ")");

  // test c_sizeof on the type
  assert( numBytes(x.type) == c_sizeof(x.type) );
  assert( numBytes(t) == c_sizeof(t) );

  // test isAnyCPtr on the type
  assert( isAnyCPtr(c_ptrTo(x).type) );
  assert( isAnyCPtr(c_void_ptr) );

  // test memset on the pointer to the type
  c_memset( c_ptrTo(x), 0, c_sizeof(x.type) );
  // and on void pointer
  c_memset( c_ptrTo(y):c_void_ptr, 0, c_sizeof(x.type) );

  assert( x != v1 );
  assert( x == y );

  x = v1;
  y = v2;

  // test memmove on the pointer to the type
  c_memmove( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type) );
  assert( x == y );

  x = v1;
  y = v2;

  // test memmove on c_void_ptr
  c_memmove( c_ptrTo(x):c_void_ptr, c_ptrTo(y):c_void_ptr, c_sizeof(x.type) );
  assert( x == y );

  x = v1;
  y = v2;

  // test memcpy on the pointer to the type
  c_memcpy( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type) );
  assert( x == y );

  x = v1;
  y = v2;

  // test memcpy on c_void_ptr
  c_memcpy( c_ptrTo(x):c_void_ptr, c_ptrTo(y):c_void_ptr, c_sizeof(x.type) );
  assert( x == y );

  x = v1;
  y = v2;

  // don't assume v1 memcmps less than v2, but do check less than
  // and greater than are different.

  var lt = c_memcmp( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type));
  // test memcmp on pointer to the type
  assert( lt != 0 );

  // test memcmp on c_void_ptr
  assert( c_memcmp( c_ptrTo(x):c_void_ptr, c_ptrTo(y):c_void_ptr, c_sizeof(x.type)) == lt );

  x = v2;
  y = v1;

  // test memcmp on pointer to the type
  var gt = c_memcmp( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type));
  assert( gt != 0 );

  // test memcmp on c_void_ptr
  assert( c_memcmp( c_ptrTo(x):c_void_ptr, c_ptrTo(y):c_void_ptr, c_sizeof(x.type)) == gt );

  assert( lt != gt );

  x = v1;
  y = v1;

  // test memcmp on pointer to the type
  assert( c_memcmp( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type)) == 0 );

  // test memcmp on c_void_ptr
  assert( c_memcmp( c_ptrTo(x):c_void_ptr, c_ptrTo(y):c_void_ptr, c_sizeof(x.type)) == 0 );
}

test(int, 1, 2);
test(uint, 1, 2);
test(int(8), 1, 2);
test(int(16), 1, 2);
test(int(32), 1, 2);
test(int(64), 1, 2);
test(uint(8), 1, 2);
test(uint(16), 1, 2);
test(uint(32), 1, 2);
test(uint(64), 1, 2);
test(real(32), 1.0:real(32), 2.0:real(32));
test(real(64), 1.0:real(64), 2.0:real(64));
test(real, 1.0:real, 2.0:real);

// check memcmp returns < 0, 0, or > 0 for byte comparison
// (no issues of endianness, memory representation)
{
  var x = 1:uint(8);
  var y = 2:uint(8);
  var z = 1:uint(8);
  assert( c_memcmp( c_ptrTo(x), c_ptrTo(y), c_sizeof(x.type)) < 0 );
  assert( c_memcmp( c_ptrTo(y), c_ptrTo(x), c_sizeof(x.type)) > 0 );
  assert( c_memcmp( c_ptrTo(x), c_ptrTo(z), c_sizeof(x.type)) == 0 );
}

writeln("OK");
