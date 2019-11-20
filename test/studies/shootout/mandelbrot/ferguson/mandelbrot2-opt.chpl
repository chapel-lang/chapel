//use BlockDist;
use IO;

config const size = 200;
config const iterations = 50;
config const limit = 4.0;

config const outfile = "mandelbrot.pbm";

const lower = -1.5 - 1.0i;
const upper = 0.5 + 1.0i;

// imag, real
const D = {0..#size, 0..#size by 8};
//const D: domain(2) dmapped Block(boundingBox=space) = space;

proc in_set8(ipart: int, rpart8: int):uint(8) {

  var c_im:real = (upper.im - lower.im) * ipart / size + lower.im;
  
  var c8: 8*complex;

  for i in 0..#8 {
    var rpart = rpart8 + i;
    c8[i].re = (upper.re - lower.re) * rpart / size + lower.re;
    c8[i].im = c_im;
  }

  var z8: 8*complex;

  var mask:int = 0xff;

  for it in 1..iterations {
    for param i in 0..7 {
      z8[i] = z8[i] * z8[i] + c8[i];
      var diverged:int = (z8[i].re*z8[i].re + z8[i].im*z8[i].im) > limit;
      var clearbit:int = (diverged << (7-i));
      mask &= ~clearbit;
    }
    if mask == 0 then break;
  }
 
  return mask:uint(8);
}

proc main() {
  var set: [D] uint(8);

  //var values: [D] complex = for (re,im) in space do compute(get_point(i));

  forall (im,re8) in D {
    set(im,re8) = in_set8(im, re8);
  }

  var datastart:int(64);
  var f = open(outfile, iomode.cwr);
  var hwriter = f.writer();
  hwriter.writeln("P4");
  hwriter.writeln(size, " ", size);
  datastart = hwriter.offset();
  hwriter.close();

  var writer = f.writer(kind=iokind.native, locking=false, start=datastart);
  for (im,re8) in D {
    writer.write(set(im,re8));
  }
  writer.close();
}
