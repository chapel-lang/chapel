testit(64);
testit(128);

proc testit(param numbits) {
  var x: complex(numbits) = (1.1, 1.2):complex(numbits);
  var y: complex(numbits) = (1.3, 1.4): complex(numbits);
  var r: real(numbits/2) = 1.5: real(numbits/2);
  var i: imag(numbits/2) = 1.6i: imag(numbits/2);

  {
    var z = -x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x + y;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = r + i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i + r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i + x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = r + x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x + r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x + i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x - y;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = r - i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i - r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = r - x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x - r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i - x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x - i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x * y;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = r * x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x * r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i * x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x * i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x / y;
    writef("z is %.4z and requires %i bits\n", z, numBits(z.type));
  }
  
  {
    var z = r / x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x / r;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = i / x;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
  
  {
    var z = x / i;
    writeln("z is ", z, " and requires ", numBits(z.type), " bits");
  }
}
