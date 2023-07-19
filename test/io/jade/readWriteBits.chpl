use IO;

config const noisy = false;

proc testio(param typ:iokind, style:iostyleInternal, x:int, numBits:int)
{
  if noisy then writeln("Testing ",typ:int(64)," ",x.type:string," ",style.binary:int(64)," ",style.byteorder:int(64)," ",style.str_style);
  var f = openTempFile();
  {
    var ch = f.writer(typ, style=style);
    if noisy then writeln("Writing ", x:string);
    ch.writeBits(x, numBits);
    ch.close();
  }
  {
    var ch = f.reader(typ, style=style);
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

proc testio(x:int, numBits:int)
{
  // Set up some I/O styles.
  var styles = ( defaultIOStyleInternal().native(), defaultIOStyleInternal().little(), defaultIOStyleInternal().big(), defaultIOStyleInternal().text() );

  for i in 0..#styles.size {
    var style = styles[i];
    if noisy then writeln("STYLE ", i, " iodynamic");
    testio(iodynamic, style, x, numBits);
    if noisy then writeln("STYLE ", i, " ionative");
    testio(ionative, style, x, numBits);
    if noisy then writeln("STYLE ", i, " iobig");
    testio(iobig, style, x, numBits);
    if noisy then writeln("STYLE ", i, " iolittle");
    testio(iolittle, style, x, numBits);
  }
}

proc main() {
  testio(0b011011001101000110101101, 24);
}
