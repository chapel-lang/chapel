use Buffers;

proc main() {
  // allocate 50 bytes
  var b = new bytes(50);


  // put our bytes into a buffer.
  var buf = new buffer();
  buf.append(b);
  var num:int(32) = 5;
  var got:int(32);


  // now write some data to it.
  //writeln(numBytes(num.type));
  var start = buf.start();
  //writeln("start");
  //start.debug_print();
  var r = buf.all();
  //writeln("r.start");
  //r.start.debug_print();
  //writeln("r.end");
  //r.end.debug_print();
  //writeln("Copying in");
  buf.copyin(start, num);

  //writeln("Copying out");
  buf.copyout(start, got);
  //writeln("checking");
  assert( got == num );


  var buf2 = new buffer();
  //writeln("Appending to new buffer");
  buf2.append(buf);

  var s2 = buf2.start();
  //writeln("Copying out");
  buf.copyout(start, got);
  //writeln("checking");
  assert( got == num );
 

}
