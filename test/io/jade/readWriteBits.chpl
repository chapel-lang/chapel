use IO;

config const noisy = false;

proc testio(x:int, numBits:int)
{
  if noisy then writeln("Testing ",x.type:string);
  var f = openTempFile();
  {
    var ch = f.writer(locking=false);
    if noisy then writeln("Writing ", x:string);
    ch.writeBits(x, numBits);
    ch.close();
  }
  {
    var ch = f.reader(locking=false);
    var y = x;
    var z = x;
    if noisy then writeln("Reading element");
    var got = ch.readBits(y, numBits);
    if noisy then writeln("Read ", y:string);
    assert( got );
    assert( y == x );

    // Try reading another item -- should get EOF
    if noisy then writeln("Reading another - should get EOF");
    got = ch.readBits(z, numBits);
    assert( !got );

    ch.close();
  }
  f.close();
}

proc main() {
  testio(0b011011001101000110101101, 24);
}
