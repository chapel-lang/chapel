use Buffers;

proc testassign() {
  var b = new byteBuffer(50);
  {
    // test assignment overload.
    var b_tmp:byteBuffer = new byteBuffer();
    var b_tmp2 = new byteBuffer();
    b_tmp2 = b;
    b_tmp = b;
  }
  {
    // test copy-init
    var b_tmp3 = b;
  }
}

proc main() {
  testassign();

  // allocate 50 bytes
  var b = new byteBuffer(50);
  {
    // test assignment overload.
    var b_tmp:byteBuffer;
    var b_tmp2 = new byteBuffer();
    b_tmp2 = b;
    b_tmp = b;
  }

  // put our bytes into a buffer.
  var buf = new buffer();
  buf.append(b);
  var num:int(32) = 5;
  var num2:int(32) = 900;
  var got:int(32);

  // now write some data to it.
  var start = buf.start();
  var next = buf.start();
  var end = buf.start();
  buf.advance(next, 4);
  buf.advance(end, 8);

  var r = buf.all();
  buf.copyin(start, num);
  buf.copyin(next, num2);

  buf.copyout(start, got);
  assert( got == num );
  buf.copyout(next, got);
  assert( got == num2 );

  var all = buf.all();
  var buf_bytes = buf.flatten(all);

  var buf_buf_bytes = new buffer();
  buf_buf_bytes.append(buf_bytes);

  buf.copyout(buf_buf_bytes.start(), got);
  assert(got == num);

  var buf2 = new buffer();
  //writeln("Appending to new buffer");
  buf2.append(buf);

  var s2 = buf2.start();
  //writeln("Copying out");
  buf.copyout(start, got);
  //writeln("checking");
  assert( got == num );
}
