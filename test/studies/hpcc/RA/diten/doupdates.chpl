use ra;

proc doUpdates(A, nu) {
  var numUps = nu;
  var start = 0;
  param maxTransfer = 1024;

  proc updateBlock(param n, nu: int) {
    var t: n*uint(64);
    //printf("n = " + n + " nu = " + nu + "\n");
    local {
      for i in 1..nu {
        t(i) = A(start+i-1);
      }
    }
    on T.domain.dist.idxToLocale(t(0) & indexMask) {
      var tt = t;
      local {
        for i in 0..#nu {
          T(tt(i) & indexMask) ^= tt(i);
        }
      }
    }
  }

  proc msb(in x: int) {
    // http://aggregate.org/MAGIC
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return(x & ~(x >> 1));
  }

  while numUps != 0 {
    select msb(numUps) {
      when 512 {
        updateBlock(2*512-1, numUps);
        numUps = 0;
      }
      when 256 {
        updateBlock(2*256-1, numUps);
        numUps = 0;
      }
      when 128 {
        updateBlock(2*128-1, numUps);
        numUps = 0;
      }
      when 64 {
        updateBlock(2*64-1, numUps);
        numUps = 0;
      }
      when 32 {
        updateBlock(2*32-1, numUps);
        numUps = 0;
      }
      when 16 {
        updateBlock(2*16-1, numUps);
        numUps = 0;
      }
      when 8 {
        updateBlock(2*8-1, numUps);
        numUps = 0;
      }
      when 4 {
        updateBlock(2*4-1, numUps);
        numUps = 0;
      }
      when 2 {
        updateBlock(2*2-1, numUps);
        numUps = 0;
      }
      when 1 {
        updateBlock(1, 1);
        numUps = 0;
      }
      otherwise {
        updateBlock(maxTransfer, maxTransfer);
        numUps -= maxTransfer;
        start += maxTransfer;
      }
    }
  }
}
/*
proc main {
  var A: [1..10000] int;

  writeln("n is ", 1031);
  doUpdates(0, A, 1031);
  writeln("n is ", 734);
  doUpdates(0, A, 734);
}
*/
