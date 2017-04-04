// Run various basic operations on a DimensionalDist2D/BlockCyclicDim-mapped
// domain and array, from each locale.

use DimensionalDist2D, BlockCycDim;
use u;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);


/////////// distribution

var vdf = new BlockCyclicDim(lowIdx=-100, blockSize=3, numLocales=s1,
                              name="D1");
var sdf = new BlockCyclicDim(lowIdx=-10, blockSize=2, numLocales=s2,
                              name="D2");

hd("new DimensionalDist2D()");
var ddf = new DimensionalDist2D(mylocs, vdf, sdf, "ddf");
tl();

hd("wrapping ddf in dmap");
var ddfm = new dmap(ddf);
tl();

// Wrapping a second time would result in double-deletion of ddf.
hd("skipping: wrapping ddf in another dmap, without assignment");
//new dmap(ddf);
tl();

i2lTest(ddfm, (1,1));
i2lTest(ddfm, (1,2));
i2lTest(ddfm, (2,4));
i2lTest(ddfm, (3,6));


/////////// domain

const dmbase = {1..5,1..7};

fphase(10);
hd("dmdom - creating");
var dmdom: domain(2) dmapped ddfm;
tl();

hd("dmdom - assigning");
dmdom = dmbase;
msg("dmdom = ", dmdom);
tl();

fphase(20);
showDom(dmdom);

hd("dmdom slice 1");
const slicedom1 = dmdom(1..3, 4..5);
msg(slicedom1);
msgserial(slicedom1);
forall i in slicedom1 do msg(i);
tl();

hd("dmdom slice 2");
const slicedom2 = dmdom(1..4 by 2, 2..7 by 3);
msg(slicedom2);
msgserial(slicedom2);
forall i in slicedom2 do msg(i);
tl();


/////////// array

fphase(40);
hd("dmarr - creating");
var dmarr: [dmdom] int;
tl();

var dmhelp: [dmbase] dmarr.eltType;

hd("dmarr - initializing with explicit indexing");
// note this updates only *one* replicated copy
for (i,j) in dmdom {
  dmarr[i,j] = i*10 + j;
}
tl();

hd("dmarr - writeln()");
writeln(dmarr);
tl();

fphase(50);
showArr(dmdom, dmarr, dmhelp);


/////////// subordinate 1-d distributions

fphase(90);
hd("dmarr - dimSpecifier()");
msg(dmarr._value.dimSpecifier(1));
msg(dmarr._value.dimSpecifier(2));
tl();


/////////// privatization

hd("privatization tests");
if manylocs {
  if traceDimensionalDist {
    msg("skipped because traceDimensionalDist = true");
  } else {
    proc test(locIds, arrIdx, expVal) {
      on mylocs(locIds) {
        msg(" mylocs", locIds, "  dmarr", arrIdx, "  expecting ", expVal,
            "  on ", here.id);
        var temp = 999999;
        const idx = arrIdx;
        assert(temp != expVal);
        local {
          temp = dmarr(idx);
        }
        if temp != expVal then
          msg("*** error: expected ", expVal, ", got ", temp);

      }
    }  // proc test

    test((0,0), (1,2), 12);
    test((0,0), (1,3), 13);
    test((0,0), (3,2), 32);

    test((0,1), (2,4), 24);
    test((0,1), (1,5), 15);
    test((0,1), (3,4), 34);

    test((0,2), (2,7), 27);
    test((0,2), (4,6), 46);
    test((0,2), (5,1), 51);
  }
} else {
  msg("skipped because of oversubscribing Locales(0)");
}
tl();

// Does not presently work (see t3.future). Once it does, replace the above.
//
//hd("privatization tests");
//privTest(dmarr, (0,0), (1,2), 12);
//privTest(dmarr, (0,1), (2,3), 23);
//privTest(dmarr, (1,0), (3,1), 0);
//privTest(dmarr, (1,1), (3,3), 0);
//tl();
