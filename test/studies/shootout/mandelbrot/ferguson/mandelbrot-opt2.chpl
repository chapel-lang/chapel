use BlockDist;
use IO;

config const size = 200;
config const iterations = 50;
config const limit = 4.0;
config const outfile = "mandelbrot.pbm";

const lower = -1.5 - 1.0i;
const upper = 0.5 + 1.0i;

const cols = size / 8;
//const row_space = [0..#cols];
//const row_domain: domain(1) dmapped Block(boundingBox=row_space) = row_space;

proc main() {

  var datastart:int(64);
  var f = open(outfile, iomode.cwr);
  var writer = f.writer();
  writer.writeln("P4");
  writer.writeln(size, " ", size);
  datastart = writer.offset();
  writer.close();

  forall ipart in 0..#size {
    var c_im:real = (upper.im - lower.im) * ipart / size + lower.im;
    var writer = f.writer(kind=iokind.native, locking=false,
                          start=datastart+ipart*cols,
                          end=datastart+(ipart+1)*cols);

    for rstart in 0..#cols {

      var rpart8 = 8*rstart;

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
    
      writer.write(mask:uint(8));
    }

    writer.close();
  }
}
