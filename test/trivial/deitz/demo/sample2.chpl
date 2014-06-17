use List;

config var phrase : string = "shuffle me please";
const n = (phrase.length);

var encoded = phrase.substring(1..n by 2) + phrase.substring(2..n by 2);

var decoder : list(int);
for i in 1..n/2 do
  decoder.concat(makeList(i, n/2 + n % 2 + i));
if n % 2 == 1 { 
  var tmp : int = n / 2 + 1;
  decoder.concat(makeList(tmp));
}
var decoded : string;
for i in decoder do
  decoded = decoded + encoded.substring(i);

writeln("phrase:   ", phrase);
writeln("encoded:  ", encoded);
writeln("decoder:  ", decoder);
writeln("decoded:  ", decoded);
