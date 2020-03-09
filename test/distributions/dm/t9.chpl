// Run various basic operations on a DimensionalDist2D/BlockCyclicDim-mapped
// domain and array, from each locale.

use DimensionalDist2D, BlockCycDim;
use u;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);


/////////// distribution

var vdf = new BlockCyclicDim(lowIdx=-100, blockSize=3, numLocales=s1);
var sdf = new BlockCyclicDim(lowIdx=-10, blockSize=2, numLocales=s2);

hd("new DimensionalDist2D()");
var ddf = new unmanaged DimensionalDist2D(mylocs!, vdf, sdf, "ddf");
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

fphase(91);
hd("privatization tests");

privTest(dmarr, (0,0), (1,2), 12);
privTest(dmarr, (0,0), (1,3), 13);
privTest(dmarr, (0,0), (3,2), 32);

privTest(dmarr, (0,1), (2,4), 24);
privTest(dmarr, (0,1), (1,5), 15);
privTest(dmarr, (0,1), (3,4), 34);

privTest(dmarr, (0,2), (2,7), 27);
privTest(dmarr, (0,2), (4,6), 46);
privTest(dmarr, (0,2), (5,1), 51);

tl();
