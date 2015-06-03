//use CyclicZipOpt;
//use MyBlockCyclic;
use CyclicDist;
use BlockCycDist;
use BlockDist;

use Time;
use CommDiagnostics;

config var n1=999;
config var n2=1002;
config var dist='BC';
config var correct=false;
config var messages=false;
config var timeit=false;
config var volume=false;
config var blocksize=4;

var mydom = {1..n2};
if dist=='NONE' {
	var mydist = mydom;
	dobench(mydist, mydom);
} /*else if dist=='BCM' {
	var mydist = mydom dmapped MyBlockCyclic(startIdx=mydom.low, blocksize=blocksize);
	//totalcomm3=volume;
	dobench(mydist, mydom);
} else if dist=='CM' {
	var mydist = mydom dmapped CyclicZipOpt(startIdx=mydom.low);
	//totalcomm2=volume;
	dobench(mydist, mydom);	
} */else if dist=='C' {
	var mydist = mydom dmapped Cyclic(startIdx=mydom.low);
	dobench(mydist, mydom);	
} else if dist=='BC' {
	var mydist = mydom dmapped BlockCyclic(startIdx=mydom.low, blocksize=blocksize);
	dobench(mydist, mydom);
} else if dist=='B' {
	var mydist = mydom dmapped Block(boundingBox=mydom);
	dobench(mydist, mydom);
}

proc dobench(mydist, mydom) {

var a={1..n2}:[mydist]int;
var b:[mydist]int;

var at={1..n2}:[mydom]int;
var bt:[mydom]int;
var still_correct = true;

var timer:Timer;

if messages {
	resetCommDiagnostics();
	startCommDiagnostics();
}

if timeit {
	timer = new Timer();
	timer.start();
}

for i in n1..n2-1 {
	b[1]=1;
	b[i+1]=1;
//	b[2..i]=a[1..i-1];
	forall (bp,ap) in zip(b[2..i], a[1..i-1]) do bp=ap;

	a[1..i]=b[1..i]+b[2..i+1];
	if correct {
		bt[1]=1;
		bt[i+1]=1;
		
		bt[2..i]=at[1..i-1];
		at[1..i]=bt[1..i]+bt[2..i+1];
		
		for j in mydom {
			still_correct &&= (at[j]==a[j]) && (bt[j]==b[j]);
		}
	}
}

if timeit {
	timer.stop();
}

if messages {
	stopCommDiagnostics();
}


//var seconds=floor(stopTime-startTime);
if timeit then writeln("took ", timer.elapsed(), " (s)");
//writeln(stopTime:real(64));
//writeln(startTime);
//writeln(typeToString(startTime.type));

if correct then writeln("it is correct? ", still_correct);

if correct then {
	if n2<20 {
		writeln('last row is ', b);
	} else {
		writeln('middle element is: ', b[n2/2]);
	}
}

// write comms
if messages {
	var messages=0;
	var coms=getCommDiagnostics();
	for i in 0..numLocales-1 {
		//writeln(coms(i));
		messages+=coms(i).get:int;
		messages+=coms(i).put:int;
	}
	writeln('message count=', messages);
}

/*if volume {
	var total=0;
	for i in 0..numLocales-1 {
		total+=total_communication_counts(i+1)+total_communication_counts2(i+1);
	}
	//writeln('totals count=', total_communication_counts+total_communication_counts2);
	writeln('message volume=', total);
}*/
}
