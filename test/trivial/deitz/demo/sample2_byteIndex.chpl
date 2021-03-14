use LinkedLists;

config var phrase : string = "shuffle me please";
const n = (phrase.numBytes);

var encoded = phrase[0:byteIndex..(n-1):byteIndex by 2] + phrase[1:byteIndex..(n-1):byteIndex by 2];

var decoder : LinkedList(int);

for i in 0..#(n/2) {
  var l = makeList(i, n/2 + n % 2 + i);

  decoder.concat(l);

  l.destroy();
}

if n % 2 == 1 {
  var tmp : int = n / 2;
  var l         = makeList(tmp);

  decoder.concat(l);

  l.destroy();
}

var decoded : string;

for i in decoder do
  decoded = decoded + encoded[i:byteIndex];

writeln("phrase:   ", phrase);
writeln("encoded:  ", encoded);
writeln("decoder:  ", decoder);
writeln("decoded:  ", decoded);

decoder.destroy();
