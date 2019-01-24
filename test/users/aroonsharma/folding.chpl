/*use CyclicZipOpt;*/
use CyclicDist;
use BlockDist;
use BlockCycDist;

use Time;
use CommDiagnostics;

config var n=16;
config var iterations=2;
config var dist='C';
config var correct=false;
config var messages=false;
config var timeit=false;
config var volume=false;
config var bsize = 16;

var mydom = {1..n};
if dist=='NONE' {
  var mydist = mydom;
  dobench(mydist, mydom);
/*} else if dist=='CM' {
  var mydist = mydom dmapped CyclicZipOpt(startIdx=mydom.low);
  //totalcomm2=volume;
  dobench(mydist, mydom);  */
} else if dist=='C' {
  var mydist = mydom dmapped Cyclic(startIdx=mydom.low);
  dobench(mydist, mydom);  
} else if dist=='B' {
  var mydist = mydom dmapped Block(boundingBox=mydom);
  dobench(mydist, mydom);
} /*else if dist == 'BC' {
  //won't work for this benchmark because of strided access patterns
  var mydist = mydom dmapped BlockCyclic(startIdx=mydom.low, blocksize=bsize);
  dobench(mydist, mydom);
} else if dist == "BCM" {
  //won't work for this benchmark because of strided access patterns
  var mydist = mydom dmapped MyBlockCyclic(startIdx=mydom.low, blocksize=bsize);
  dobench(mydist, mydom);
}*/

proc dobench(mydist, mydom) {
  var a:[mydist]int = {1..n};
  var b:[mydist]int;

  var at:[mydom]int = {1..n};
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

  for i in 1..iterations{
//     forall j in 1..n/2 {
//       b[j]=a[j*2]+a[j*2-1];
//     }
    forall (bj, aj2, aj21) in zip(b[1..n/2], a[2..n by 2], a[1..n by 2]) {
      bj=aj2+aj21;
    }
  
//     forall j in 1..n/2 {
//       a[j]=b[j];
//       a[j+n/2]=b[j];
//     }
    forall (bj, aj, ajn2) in zip(b[1..n/2], a[1..n/2], a[n/2+1..n]) {
      aj=bj;
      ajn2=bj;
    }
  }

  if timeit {
    timer.stop();
  }

  if messages {
    stopCommDiagnostics();
  }

  if correct {
    for i in 1..iterations {
      for j in 1..n/2 {
        bt[j]=at[j*2]+at[j*2-1];
      }

      for j in 1..n/2 {
        at[j]=bt[j];
        at[j+n/2]=bt[j];
      }  
    }
  
    for j in mydom {
      still_correct &&= (at[j]==a[j]) && (bt[j]==b[j]);
    }
  }

  //var seconds=floor(stopTime-startTime);
  if timeit then writeln("took ", timer.elapsed(), " (s)");
  //writeln(stopTime:real(64));
  //writeln(startTime);
  //writeln(startTime.type:string);

  if correct then writeln("it is correct? ", still_correct);

  if correct then {
    if n<20 {
      writeln('last row is ', b);
    } else {
      writeln('first element is: ', b[1]);
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
      total+=total_communication_counts2(i+1);
    }
    //writeln('totals count=', total_communication_counts+total_communication_counts2);
    writeln('message volume=', total);
  }*/
}
