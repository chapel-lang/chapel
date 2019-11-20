//use BlockDist;
use IO;

config const size = 200;
config const iterations = 50;
config const limit = 4.0;

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

  forall (im,re8) in D {
    set(im,re8) = in_set8(im, re8);
  }

  var f = openfd(1);
  var w = f.writer(kind=iokind.native, locking=false);
  w.writef("P4\n%i %i\n", size, size);
 
  for (im,re8) in D {
    w.write(set(im,re8));
  }
}
