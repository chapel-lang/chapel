config param s = 20;

var my_tuple: s*real;

for i in 1..s do my_tuple[i-1] = i*i;

writeln(my_tuple);

