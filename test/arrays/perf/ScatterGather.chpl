use CyclicDist;
use BlockDist;
use BlockCycDist;
use Time;

config const N = 1024 * 1024;
var arrSpace = {1..N};
var scatterArr : [arrSpace] int;
var gatherArr : [arrSpace] int;
var cyclicDom = arrSpace dmapped Cyclic(startIdx=1);
var blockDom = arrSpace dmapped Block(boundingBox=arrSpace);
var blockCycDom = arrSpace dmapped BlockCyclic(startIdx=1, blocksize = N / (here.maxTaskPar * numLocales * 32));
var cyclicArr : [cyclicDom] int;
var blockArr : [blockDom] int;
var blockCycArr : [blockCycDom] int;

var timer = new Timer();
timer.start();
scatterArr = 1..N;
timer.stop();
writeln("Assign Range into DefaultRectangular: ", timer.elapsed());
timer.clear();

timer.start();
gatherArr = scatterArr;
timer.stop();
writeln("Assign DefaultRectangular into DefaultRectangular: ", timer.elapsed());
timer.clear();


timer.start();
cyclicArr = 1..N;
timer.stop();
writeln("Scatter Range into CyclicDist: ", timer.elapsed());
timer.clear();

timer.start();
cyclicArr = scatterArr;
timer.stop();
writeln("Scatter DefaultRectangular into CyclicDist: ", timer.elapsed());
timer.clear();

timer.start();
gatherArr = cyclicArr;
timer.stop();
writeln("Gather CyclicDist into DefaultRectangular: ", timer.elapsed());
timer.clear();

timer.start();
blockArr = 1..N;
timer.stop();
writeln("Scatter Range into BlockDist: ", timer.elapsed());
timer.clear();

timer.start();
gatherArr = blockArr;
timer.stop();
writeln("Gather BlockDist into DefaultRectangular: ", timer.elapsed());
timer.clear();

timer.start();
blockArr = scatterArr;
timer.stop();
writeln("Scatter DefaultRectangular into BlockDist: ", timer.elapsed());
timer.clear();

timer.start();
blockCycArr = 1..N;
timer.stop();
writeln("Scatter Range into BlockCycDist: ", timer.elapsed());
timer.clear();

timer.start();
gatherArr = blockCycArr;
timer.stop();
writeln("Gather BlockCycDist into DefaultRectangular: ", timer.elapsed());
timer.clear();

timer.start();
blockCycArr = scatterArr;
timer.stop();
writeln("Scatter DefaultRectangular into BlockCycDist: ", timer.elapsed());
timer.clear();

// Unimplemented
/*timer.start();
blockCycArr = cyclicArr;
timer.stop();
writeln("GatherScatter CyclicDist into BlockCycDist: ", timer.elapsed());
timer.clear();*/

// Unimplemented
/*timer.start();
cyclicArr = blockCycArr;
timer.stop();
writeln("GatherScatter BlockCycDist into CyclicDist: ", timer.elapsed());
timer.clear();*/

timer.start();
blockArr = blockCycArr;
timer.stop();
writeln("GatherScatter BlockCycDist into BlockDist: ", timer.elapsed());
timer.clear();

timer.start();
blockCycArr = blockArr;
timer.stop();
writeln("GatherScatter BlockDist into BlockCycDist: ", timer.elapsed());
timer.clear();

timer.start();
blockArr = cyclicArr;
timer.stop();
writeln("GatherScatter CyclicDist into BlockDist: ", timer.elapsed());
timer.clear();

timer.start();
cyclicArr = blockArr;
timer.stop();
writeln("GatherScatter BlockDist into CyclicDist: ", timer.elapsed());
timer.clear();
