config const buffersize = 10;
config const numToProduce = buffersize*2;



// ‘sync’ types store full/empty state along with value
var buff: [0..#buffersize] sync real;

// start the producer and consumer
begin producer();
consumer();

proc producer() {
  var i = 0;
  var count = 0;
  while count < numToProduce {
    i = (i+1) % buffersize;
    buff[i].writeEF(i);         // wait for empty, write, leave full
    writeln("producer: wrote ",i," into buff[",i,"]");
    count += 1;
  }
}

proc consumer() {
  var i = 0;
  var count = 0;
  while count < numToProduce {
    i = (i+1) % buffersize;
    // wait for full, read, leave empty
    writeln("consumer: buff[",i,"].readFE() ", buff[i].readFE());

    count += 1;
  }
}

