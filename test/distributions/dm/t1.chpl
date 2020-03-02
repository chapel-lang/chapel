// Run various basic operations on a DimensionalDist2D-mapped
// domain and array, from each locale.

use DimensionalDist2D, ReplicatedDim, BlockDim;
use u;

config const s1 = 2;
config const s2 = 2;
setupLocales(s1, s2);


/////////// distribution

var vdf = new ReplicatedDim(s1);
var sdf = new BlockDim(s2, 1, 3);

hd("new DimensionalDist2D()");
var ddf = new unmanaged DimensionalDist2D(mylocs!, vdf, sdf, "ddf");
tl();

hd("wrapping ddf in dmap");
var ddfm = new dmap(ddf);
tl();

hd("wrapping ddf in another dmap, without assignment");
//new dmap(ddf);
tl();

i2lTest(ddfm, (1,2));
i2lTest(ddfm, (2,3));


/////////// domain

const dmbase = {1..3,1..4};

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
privTest(dmarr, (0,1), (2,3), 23);
privTest(dmarr, (1,0), (3,1), 0);
privTest(dmarr, (1,1), (3,3), 0);
tl();
