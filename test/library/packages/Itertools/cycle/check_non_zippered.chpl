use Itertools;

const testArray = [1, 2, 3, 4];
const testString = 'ABC';
const testTuple = (5, 6, 7);
const testRange = 1..6;

for el in cycle(testArray, 4) do write(el, ' ');
writeln();

for el in cycle(testString, 4) do write(el, ' ');
writeln();

for el in cycle(testTuple, 4) do write(el, ' ');
writeln();

for el in cycle(testRange, 4) do write(el, ' ');
writeln();
