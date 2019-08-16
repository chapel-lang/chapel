use Buffers;

config const msg1 = "Good morning, Dave.";
config const msg2 = "I'm sorry.";
config const msg3 = "I'm afraid I can't do that.";

var buf: buffer;
buf.append(new byteBuffer(3*numBytes(int) + msg1.numBytes +
                          msg2.numBytes + msg3.numBytes));

{
  var cur = buf.start();
  cur = buf.copyin(cur, msg1);
  cur = buf.copyin(cur, msg2);
  cur = buf.copyin(cur, msg3);
}

{
  var cur = buf.start();
  var str: string;
  cur = buf.copyout(cur, str);
  writeln("Original: ", msg1);
  writeln("    Copy: ", str);
  cur = buf.copyout(cur, str);
  writeln("Original: ", msg2);
  writeln("    Copy: ", str);
  cur = buf.copyout(cur, str);
  writeln("Original: ", msg3);
  writeln("    Copy: ", str);
}
