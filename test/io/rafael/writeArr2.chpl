use Time;

config var filename = "Arr.dat";
config param lock = false;
config var chunk = 65536;
config var element =false;

config const arrSize = 5;
var A: [0..#arrSize] int;
for i in [0..#arrSize] {
  A[i]=i;
}
  var timer: Timer;


var outfile = open(filename, iomode.cwr);
var ch = outfile.writer(style=defaultIOStyle().native(),locking=lock);
if element then {

timer.start();
  for a in A do {
    ch.write(a);
  }
  timer.stop();

    writeln("write Array lock ",lock," element by element :",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",arrSize/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");

}
ch.close();
  timer.clear();
filename = "Whole_Arr.dat";

outfile = open(filename, iomode.cwr);
ch = outfile.writer(style=defaultIOStyle().native(),locking=lock);

timer.start();
var err:syserr;
    writeln("write Array using chunks of size ",chunk);
for i in 0..#arrSize by chunk {
	var sz = if i + chunk < arrSize then chunk else arrSize - i;
	err = qio_channel_write_amt(false, ch._channel_internal, A[i], 8*sz);
	assert(!err);
}

//ch.write(A);
timer.stop();
    writeln("write Array with locking= ",lock," and chunks ",chunk," ",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",arrSize/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");


ch.close();


