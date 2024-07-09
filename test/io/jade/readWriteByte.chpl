use IO;

config const noisy = false;

proc testio(x:uint(8))
{
  if noisy then writeln("Testing ",x.type:string);
  var f = openTempFile();
  {
    var ch = f.writer(locking=false);
    if noisy then writeln("Writing ", x:string);
    ch.writeByte(x);
    ch.close();
  }
  {
    var ch = f.reader(locking=false);
    var y = x;
    var z = x;
    if noisy then writeln("Reading element");
    var got = ch.readByte(y);
    if noisy then writeln("Read ", y:string);
    assert( got );
    assert( y == x );

    // Try reading another item -- should get EOF
    if noisy then writeln("Reading another - should get EOF");
    got = ch.readByte(z);
    assert( !got );

    ch.close();
  }
  f.close();
}

proc main() {
  testio(97);
}
