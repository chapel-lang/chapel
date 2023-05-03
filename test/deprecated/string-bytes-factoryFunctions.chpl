// deprecated by jade in 1.31
use CTypes;

var helloBytes: [1..6] uint(8) = [104, 101, 108, 108, 111, 0]:uint(8);

writeln("string factory functions");
{
  var s: string = "hello";
  var s_ = createStringWithBorrowedBuffer(s);
  writeln(s_);
}
{
  var s: c_string = "hello";
  var s_ = createStringWithBorrowedBuffer(s);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = createStringWithBorrowedBuffer(c_ptrTo(b), length=5, size=6);
  writeln(s_);
}
{
  var ptr = c_malloc(uint(8), helloBytes.size);
  for (idx, char) in zip(0..<helloBytes.size, helloBytes) do
    ptr[idx] = char;
  var s = ptr: c_string;
  var s_ = createStringWithOwnedBuffer(s);
  writeln(s_);
}
{
  var ptr = c_malloc(uint(8), helloBytes.size);
  for (idx, char) in zip(0..<helloBytes.size, helloBytes) do
    ptr[idx] = char;
  var s_ = createStringWithOwnedBuffer(ptr, length=helloBytes.size-1, size=helloBytes.size);
  writeln(s_);
}
{
  var s: string = "hello";
  var s_ = createStringWithNewBuffer(s);
  writeln(s_);
}
{
  var s: c_string = c"hello";
  var s_ = createStringWithNewBuffer(s);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = createStringWithNewBuffer(c_ptrTo(b), length=helloBytes.size-1, size=helloBytes.size);
  writeln(s_);
}

writeln("bytes factory functions");
{
  var b: bytes = "hello";
  var s_ = createBytesWithBorrowedBuffer(b);
  writeln(s_);
}
{
  var s: c_string = "hello";
  var s_ = createBytesWithBorrowedBuffer(s);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = createBytesWithBorrowedBuffer(c_ptrTo(b), length=5, size=6);
  writeln(s_);
}
{
  var ptr = c_malloc(uint(8), helloBytes.size);
  for (idx, char) in zip(0..<helloBytes.size, helloBytes) do
    ptr[idx] = char;
  var s = ptr: c_string;
  var s_ = createBytesWithOwnedBuffer(s);
  writeln(s_);
}
{
  var ptr = c_malloc(uint(8), helloBytes.size);
  for (idx, char) in zip(0..<helloBytes.size, helloBytes) do
    ptr[idx] = char;
  var s_ = createBytesWithOwnedBuffer(ptr, length=helloBytes.size-1, size=helloBytes.size);
  writeln(s_);
}
{
  var b: bytes = "hello";
  var s_ = createBytesWithNewBuffer(b);
  writeln(s_);
}
{
  var s: c_string = c"hello";
  var s_ = createBytesWithNewBuffer(s);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = createBytesWithNewBuffer(c_ptrTo(b), length=helloBytes.size-1, size=helloBytes.size);
  writeln(s_);
}


// inline proc createBytesWithBorrowedBuffer(x: bytes)
//   inline proc createBytesWithBorrowedBuffer(x: c_string,
//                                             length=x.size) 
//   inline proc createBytesWithBorrowedBuffer(x: c_ptr(?t), length: int,
//                                             size: int)
//   inline proc createBytesWithOwnedBuffer(x: c_string, length=x.size) 
//   inline proc createBytesWithOwnedBuffer(x: c_ptr(?t), length: int,
//                                          size: int)
// inline proc createBytesWithNewBuffer(x: bytes) 
//   inline proc createBytesWithNewBuffer(x: c_string, length=x.size)
//   inline proc createBytesWithNewBuffer(x: c_ptr(?t), length: int,
//                                        size=length+1)
