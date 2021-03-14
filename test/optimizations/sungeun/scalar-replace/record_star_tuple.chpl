config param s = 20;

record my_record {
  var my_tuple: s*real;
};

var r: my_record;

for i in 1..s do r.my_tuple[i-1] = i*i;

writeln(r.my_tuple);

