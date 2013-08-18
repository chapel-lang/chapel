use DistIO;
use BlockDist;
use Time;

config const n = 30;
config const block = 3;
config const pdebug = false;
//const Space = [1..n,1..n,1..n,1..n,1..n,1..n];
const Space = {1..n};

var timer: Timer;

const BDom = Space dmapped new dmap(new Block(boundingBox=Space));
var RB: [BDom] int;

var s=sizeof(int):int;

timer.start();
forall i in BDom {
  RB[i] = 200+i:int;
}
timer.stop();
writeln("init Array RB:",timer.elapsed(TimeUnits.seconds)," bw: ",(n*s)/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");

var Dist = new dmap(new DistIO(startIdx=tuple(1), blocksize=tuple(block)));
var CDom: domain(1) dmapped Dist = Space;

var CA: [CDom] int;
CA._value.setFilename("test6.bor");

timer.clear();
timer.start();
CA=RB;
timer.stop();

    writeln("write Array :",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",(n*s)/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");

//if pdebug then writeln("After CA-------------------------",CA);

//writeln("Representacion Dist:",Dist);
//writeln("Type Dist:",typeToString(Dist.type));
//writeln("Type val Dist:",typeToString(Dist._value.type));
//writeln("Type CA:",typeToString(CA.type));
//writeln("Type val CA:",typeToString(CA._value.type));
//writeln("Representacion:",CDom);
