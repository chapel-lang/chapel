use Time, Random;

// radix_sort
// ----------
// Values is the 0-based array to be sorted.
// radix and nbits toggles how many keys are generated and processed per pass.

// assert indices for Values starts at 0
proc radix_sort(Values, Permute, radix:int(64), nbits:int(64)): void {

    var nelem:int(64) = Values.size;
    var nbuckets:int(64) = 1 << radix;   // Number of keys in counting sort

    var D1 = {0..(nelem-1)};
    var D2 = {0..(nbuckets-1)};

    var Permute_old: [D1] int(64) = Permute; // Old permutation array

    var sortTime: real;                           // overall timing
    const sortStartTime = getCurrentTime();       // capture the start time

    var npasses:int(64) = (nbits / radix);
    if ( (npasses * radix) < nbits ) then npasses = npasses + 1;

    for pass in (0..(npasses-1)) {
      var Index: [D1] int(64);           // Index
      var Offsets: [D2] atomic int(64);    // Starting offset of 
                                         // histogrammed values
      var Offsets_old: [D2] int(64);     // Old offsets
      var Histogram: [D2] atomic int(64);  // Histogram for counting sort
      var All_counts: [D1] int(64);      // Storage for counting sort tallies

      var phaseTime: real;                        // timing of pass
      const phaseStartTime = getCurrentTime();    // capture the pass start

      // Mask of bits sorted on this pass
      var mask:int(64) = (nbuckets - 1) << (pass * radix);

      Histogram.write(0);
      forall i in D1 {
        var r:int(64) = ((mask & Values[Permute_old[i]]) >> (pass * radix));
        Histogram[r].add(1);
      }

// write this loop with an aggregate array operation
      Offsets[0].write(0);
      for i in (1..(nbuckets-1)) {
        Offsets[i].write(Offsets[(i-1)].read() + Histogram[(i-1)].read());
      }
      Offsets_old = Offsets.read();

      forall i in D1 {
        var r:int(64) = ((mask & Values[Permute_old[i]]) >> (pass * radix));
        var loc = Offsets[r].fetchAdd(1);
        Permute[loc] = Permute_old[i];
        Index[loc] = i;
      }

      for b in D2 {
        var base:int(64) = Offsets_old[b];
        var Count: [base..(nelem-1)] atomic int(64);
        for i in base..(nelem-1) do
          Count[i].write(All_counts[i]);

        forall i in (0..(Histogram[b].read()-1)) {
          Count[(i+base)].write(0);
          Permute_old[(i+base)] = Permute[(i+base)];
        }

        for i in (1..(Histogram[b].read()-1)) {
          forall j in (0..(i-1)) {
            if (Index[(i+base)] < Index[(j+base)]) {
              Count[(j+base)].add(1);
            } else {
              Count[(i+base)].add(1);
            }
          }
        }

        forall i in (0..(Histogram[b].read()-1)) {
          Permute[(Count[(i+base)].read()+base)] = Permute_old[(i+base)];
        }
      }

      var Tmp: [D1] int(64) = Permute_old;
      Permute_old = Permute;
      Permute = Tmp;

      phaseTime = getCurrentTime() - phaseStartTime; // store the elapsed pass
//      writeln("Completed pass ", pass, " in ", phaseTime, " sec    mask=", mask);

    }

    Permute = Permute_old;

    sortTime = getCurrentTime() - sortStartTime;  // store the elapsed time
//    writeln(npasses, " passes in ", sortTime, " secs");

    return;
}

config const npoints:int(64) = 8;
writeln("Using ", npoints, " points");

var D = {0..(npoints-1)};
var F: [D] real(64);
var F_prime: [D] int(64);
var G: [D] int(64);
var Permute_F: [D] int(64);
var Permute_G: [D] int(64);

var rngTime: real;
writeln("Generating random numbers...");
const rngStartTime = getCurrentTime();
fillRandom(F, 65535, algorithm=RNG.NPB);
F_prime = (F * 9223372036854775808):int(64);
rngTime = getCurrentTime() - rngStartTime;
//writeln("Finished generating numbers in ", rngTime, " sec");

forall i in D {
  Permute_F[i] = i;
  Permute_G[i] = i;
  G[i] = i % 10;
}

var mwTime: real;
const mwStartTime = getCurrentTime();

radix_sort(F_prime, Permute_F, 16, 64);
radix_sort(G, Permute_G, 5, 5);

mwTime = getCurrentTime() - mwStartTime;
//writeln("----- Total Time = ", mwTime, " sec -----------------------------------------");

var selfcheckTime: real;
const selfcheckStartTime = getCurrentTime();
var nerr_f: int(64) = 0;
var nerr_g: int(64) = 0;

writeln("0: ", G[Permute_G[0]], " - ", F_prime[Permute_F[0]]);
for i in (1..(npoints-1)) {
  writeln(i, ": ", G[Permute_G[i]], " - ", F_prime[Permute_F[i]]);
  if (F_prime[Permute_F[i]] <= F_prime[Permute_F[(i-1)]]) then nerr_f = nerr_f + 1;
  if (G[Permute_G[i]] < G[Permute_G[(i-1)]]) then nerr_g = nerr_g + 1;
}

selfcheckTime = getCurrentTime() - selfcheckStartTime;
writeln("Number of errors: f=", nerr_f, "  g=", nerr_g);
//writeln("Checked in ", selfcheckTime, " sec");
