use Time;

config var filename = "Arr.dat";

config const arrSize = 5;
var A: [0..#arrSize] int;
for i in [0..#arrSize] {
  A[i]=i;
}
  var timer: Timer;


var outfile = open(filename, iomode.cwr);
var ch = outfile.writer(style=defaultIOStyle().native(),locking=false);

timer.start();
  for a in A do {
    ch.write(a);
  }
  timer.stop();

    writeln("write Array element by element :",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",arrSize/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");

ch.close();

  timer.clear();
filename = "Whole_Arr.dat";

outfile = open(filename, iomode.cwr);
ch = outfile.writer(style=defaultIOStyle().native(),locking=false);

timer.start();
var err:syserr;
    writeln("write Array using chunks ");
for i in 0..#arrSize by 655360 {
	var sz = if i + 655360 < arrSize then 655360 else arrSize - i;
	err = qio_channel_write_amt(false, ch._channel_internal, A[i], 8*sz);
	assert(!err);
}

//ch.write(A);
timer.stop();
    writeln("write Array with locking=false and chunks:",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",arrSize/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");


ch.close();


