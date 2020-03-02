use Map;
use List;

var m: map(string, list(int));

m['a'] = new list(int);

writeln(m);
// {a: []}

m['a'].append(1);
writeln(m);
// {a: [1]}

m['a'][0] = 2;
writeln(m);
// {a: [1]}
// but I expected:
// {a: [2]}
