use IO;

config type dataType = string;
config const noisy = false;

proc testio(param typ:iokind, style:iostyleInternal, x)
{
  if noisy then writeln("Testing ",typ:int(64)," ",x.type:string," ",style.binary:int(64)," ",style.byteorder:int(64)," ",style.str_style);
  var f = openTempFile();
  {
    var ch = f.writer(typ, style=style);
    if noisy then writeln("Writing ", x:string);
    ch.write(x);
    ch.close();
  }
  {
    var ch = f.reader(typ, style=style);
    var y = x; // assignments here so ioLiteral works
    var z = x;
    if noisy then writeln("Reading element");
    var got = ch.read(y);
    if noisy then writeln("Read ", y:string);
    assert( got );
    assert( y == x );

    // Try reading another item -- should get EOF
    if noisy then writeln("Reading another - should get EOF");
    got = ch.read(z);
    assert( !got );

    ch.close();
  }
  f.close();
}

proc testio(x)
{
  // Set up some I/O styles.
  var styles = ( defaultIOStyleInternal().native(), defaultIOStyleInternal().little(), defaultIOStyleInternal().big(), defaultIOStyleInternal().text() );

  for i in 0..#styles.size {
    var style = styles[i];
    if noisy then writeln("STYLE ", i, " iodynamic");
    testio(iodynamic, style, x);
    if noisy then writeln("STYLE ", i, " ionative");
    testio(ionative, style, x);
    if noisy then writeln("STYLE ", i, " iobig");
    testio(iobig, style, x);
    if noisy then writeln("STYLE ", i, " iolittle");
    testio(iolittle, style, x);
  }
}

proc test_readlines(stripNewline = false)
{

  var f = openTempFile();
  {
    var ch = f.writer();
    ch.writeln("a b");
    ch.writeln("c d");
    ch.flush();
  }

  if noisy then writeln("Testing readlines: channel.readLine(line)");
  // try reading it in a few ways.
  {
    var ch = f.reader();
    var line:dataType;
    var got:bool;
    got = ch.readLine(line);
    if noisy then writeln("got ", got, " line ", line);
    assert( got && line == "a b\n":dataType );
    got = ch.readLine(line);
    if noisy then writeln("got ", got, " line ", line);
    assert( got && line == "c d\n":dataType );
    got = ch.readLine(line);
    if noisy then writeln("got ", got, " line ", line);
    assert( !got );
  }


  if noisy then writeln("Testing readlines: fileReader.lines(stripNewline=", stripNewline, ")");
  {
    proc getTestString(s) do return if stripNewline then s else s + "\n";

    for (line,i) in zip(f.reader().lines(stripNewline),1..) {
      if i == 1 {
        assert(line == getTestString("a b"));
      } else if i == 2 {
        assert(line == getTestString("c d"));
      } else {
        assert(false);
      }
    }
  }
}

proc main() {
  //testio(dynamic, defaultIOStyle().text(), 100:uint(8));
  //testio(dynamic, defaultIOStyle().text(), 57.24e23:real(32));
  //testio(dynamic, defaultIOStyle().text(), 997.89+200.124i);

  testio(true);
  testio(false);

  testio(100:int(8));
  testio(-100:int(8));
  testio(100:uint(8));
  testio(-1000:int(16));
  testio(1000:int(16));
  testio(1000:uint(16));
  testio(-100000:int(32));
  testio(100000:int(32));
  testio(100000:uint(32));
  testio(-12884901888:int(64));
  testio(12884901888:int(64));
  testio(12884901888:uint(64));
  testio(0.002:real(32));
  testio(0.002:real(64));
  testio(-0.002:real(32));
  testio(-0.002:real(64));
  testio(57.24e23:real(32));
  testio(57.24e23:real(64));
  testio(-57.24e23:real(32));
  testio(-57.24e23:real(64));
  testio("teststring");
  testio(997.89+200.124i);
  testio(-997.89-200.124i);
  testio(1.0:imag(32));
  testio(1.0:imag(64));
  //testio(100:uint(8));

  testio(new ioNewline());
  testio(new ioLiteral("test"));
  
  test_readlines();
  test_readlines(true);
}
