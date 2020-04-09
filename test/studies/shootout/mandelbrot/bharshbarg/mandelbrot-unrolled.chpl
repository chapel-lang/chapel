use DynamicIters, IO;

config const n = 200,
             maxIter = 49,
             limit = 4,
             chunkSize = 1;

proc main() {
  var invN = 2.0 / n;
  var Crb, Cib : [0..(n+6)] real;
  const ydim = 0..n-1;
  const xdim = 0..((n+7)/8)-1;
  var pixels : [ydim][xdim] uint(8);

  forall i in ydim {
    Cib[i] = i * invN - 1.0;
    Crb[i] = i * invN - 1.5;
  }

  forall y in dynamic(ydim, chunkSize) {
    for x in xdim {
      var xm = x*8;
      var res : uint(8);
      for i in 0..7 by 2 {
        var Zr1 = Crb[xm+i];
        var Zi1 = Cib[y];

        var Zr2 = Crb[xm+i+1];
        var Zi2 = Cib[y];
        var b : uint(8);
        for j in 1..maxIter {
          var nZr1 = Zr1*Zr1 - Zi1*Zi1 + Crb[xm+i];
          var nZi1 = Zr1*Zi1 + Zr1*Zi1 + Cib[y];
          Zr1 = nZr1;
          Zi1 = nZi1;

          var nZr2 = Zr2*Zr2 - Zi2*Zi2 + Crb[xm+i+1];
          var nZi2 = Zr2*Zi2 + Zr2*Zi2 + Cib[y];
          Zr2 = nZr2;
          Zi2 = nZi2;

          if Zr1*Zr1 + Zi1*Zi1 > limit {
            b |= 2;
            if b == 3 then break;
          }
          if Zr2*Zr2 + Zi2*Zi2 > limit {
            b |= 1;
            if b == 3 then break;
          }

        }
        res = (res << 2) + b;
      }

      pixels[y][x] = res ^ -1;
    }
  }

  var f = openfd(1);                           // open a stdout file descriptor
  var w = f.writer(iokind.native, locking=false);  // get a lock-free writer

  w.writef("P4\n");                            // write the file header
  w.writef("%i %i\n", n, n);

  w.write(pixels);                              // write out the image
}
