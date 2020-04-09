use LinkedLists;

config var phrase : string = "shuffle me please";
const n = (phrase.size);

var encoded = phrase[1..n by 2] + phrase[2..n by 2];

var decoder : LinkedList(int);

for i in 1..n/2 {
  var l = makeList(i, n/2 + n % 2 + i);

  decoder.concat(l);

  l.destroy();
}

if n % 2 == 1 {
  var tmp : int = n / 2 + 1;
  var l         = makeList(tmp);

  decoder.concat(l);

  l.destroy();
}

var decoded : string;

for i in decoder do
  decoded = decoded + encoded[i];

writeln("phrase:   ", phrase);
writeln("encoded:  ", encoded);
writeln("decoder:  ", decoder);
writeln("decoded:  ", decoded);

decoder.destroy();
