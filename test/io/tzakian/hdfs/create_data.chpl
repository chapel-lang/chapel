// This is designed to create data for the tests in histogram_reduction/ and parBeer/
// read_test/
use Random;
var real_stream = new RandomStream();
var int_stream  = new Rand(); 

class Rand {
  const nSeed = 5323;
   proc next() {
     nSeed = (8253729 * nSeed + 2396403);
     return abs(nSeed) % 32767;
   }
}
writeln(real_stream.getNext());
writeln(int_stream.next());
writeln(int_stream.next());
writeln(int_stream.next());


var fl = open("output.txt", iomode.cw);

config const num_records = 80;

var ch = fl.writer();

for i in 1..num_records {
  ch.write("beer/name: dummy_name\n"+
           "beer/beerId: ", int_stream.next(), "\n" +
           "beer/brewerId: ", int_stream.next(),"\n" +
           "beer/ABV: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" +
           "beer/style: dummy_style\n" + 
           "review/appearance: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" +
           "review/aroma: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" +
           "review/palate: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" + 
           "review/taste: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" + 
           "review/overall: ", (int_stream.next() % 5) + real_stream.getNext(), "\n" + 
           "review/time: ", int_stream.next(), "\n" + 
           "review/profileName: dummy_profile_name\n" + 
           "review/text: This is some dummy test\n\n");
}

delete real_stream;
delete int_stream;

ch.close();
fl.close();

