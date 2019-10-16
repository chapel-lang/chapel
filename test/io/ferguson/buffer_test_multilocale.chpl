use Buffers;

proc main() {
  // allocate 50 bytes
  var b = new byteBuffer(50);

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

  on Locales[1] {
    // Read the data in three ways
    var local_got:int(32);

    // 1: copyout on remote buffer
    buf.copyout(buf_buf_bytes.start(), local_got);
    assert(local_got == num);

    // 2: flatten remote buffer to local bytes,
    // append to local buffer, copy out of local buffer
    var local_bytes = buf.flatten(all);
    var buf_local_bytes = new buffer();
    buf_local_bytes.append(local_bytes);
    buf_local_bytes.copyout(buf_local_bytes.start(), local_got);
    assert(local_got == num);

    // 3: copy remote bytes locally
    var local_bytes2 = new byteBuffer();
    local_bytes2 = buf_bytes;
    var buf_local_bytes2 = new buffer();
    buf_local_bytes2.append(local_bytes2);
    buf_local_bytes2.copyout(buf_local_bytes2.start(), local_got);
    assert(local_got == num);
  }
}
