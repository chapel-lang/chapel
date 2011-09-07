use IO;

config const noisy = false;

proc testio(param typ:iokind, style:iostyle, x)
{
  if noisy then writeln("Testing ",typ:int(64)," ",typeToString(x.type)," ",style.binary:int(64)," ",style.byteorder:int(64)," ",style.str_style);
  var f = opentmp();
  {
    var ch = f.writer(typ, style=style);
    if noisy then writeln("Writing ", x);
    ch.write(x);
    ch.close();
  }
  {
    var ch = f.reader(typ, style=style);
    var y = x;
    var z = x;
    var got = ch.read(y);
    if noisy then writeln("Read ", y);
    assert( got );
    assert( y == x );

    // Try reading another item -- should get EOF
    got = ch.read(z);
    assert( !got );

    ch.close();
  }
  f.close();
}

/*
proc testeof()
{
  var f = opentmp(style=defaultStyle().native());
  {
    var ch = f.writer();
    ch.write(5,6);
  }
  {
    var ch = f.reader();
    var x:int;
    var y:int;
    var z:int;
    var got:bool;
    got = ch.read(x);
    writeln("Got ",x);
    assert(got);
    assert(x==5);
    got = ch.read(y);
    writeln("Got ",y);
    assert(got);
    assert(y==6);
    got = ch.read(z);
    writeln("Got ",z);
    assert(!got);
  }
}*/

proc testio(x)
{
  // Set up some I/O styles.
  var styles = ( defaultStyle().native(), defaultStyle().little(), defaultStyle().big(), defaultStyle().text() );

  for i in 1..styles.size {
    var style = styles[i];
    testio(dynamic, style, x);
    testio(native, style, x);
    testio(big, style, x);
    testio(little, style, x);
  }
}

proc test_readlines()
{

  var f = opentmp();
  {
    var ch = f.writer();
    ch.writeln("a b");
    ch.writeln("c d");
    ch.flush();
  }

  if noisy then writeln("Testing readlines: channel.readline(line)");
  // try reading it in a few ways.
  {
    var ch = f.reader();
    var line:string;
    var got:bool;
    got = ch.readline(line);
    assert( got && line == "a b" );
    got = ch.readline(line);
    assert( got && line == "c d" );
    got = ch.readline(line);
    assert( !got );
  }

  if noisy then writeln("Testing readlines: itemReader");
  {
    var style = defaultStyle();
    style.string_format = QIO_STRING_FORMAT_TOEND;
    style.string_end = 0x0a;
    var ch = f.reader(style=style);
    for (line,i) in (ch.itemReader(string),1..) {
      if i == 1 {
        assert(line == "a b");
      } else if i == 2 {
        assert(line == "c d");
      } else {
        assert(false);
      }
    }
  }


  if noisy then writeln("Testing readlines: file.lines()");
  {
    for (line,i) in (f.lines(),1..) {
      if i == 1 {
        assert(line == "a b");
      } else if i == 2 {
        assert(line == "c d");
      } else {
        assert(false);
      }
    }
  }
}

proc main() {
  //testio(dynamic, defaultStyle().text(), 100:uint(8));
  //testio(dynamic, defaultStyle().text(), 57.24e23:real(32));
  //testio(dynamic, defaultStyle().text(), 997.89+200.124i);

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

  test_readlines();
}

