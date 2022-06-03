use IO, Map;

var line: string;

enum wires {a, b, c, d, e, f, g};
enum leds {A, B, C, D, E, F, G};

var wireSpace: domain(wires);
for w in wires do wireSpace += w;
var counts: [wireSpace] int;

proc resetCounts() {
  counts = 0;
}

proc canonical(s: string, training = false) {
  var result: string;
  for ch in wires do
    if s.find(ch:string) != -1 {
      result += ch:string;
      if training then
        counts[ch] += 1;
    }
  return result;
}

proc contains(teststring, seekstring) {
  for ch in seekstring do
    if teststring.find(ch) == -1 then
      return false;
  return true;
}

iter getDigitsOutputs() {
  while readLine(line) {
    //  writeln("Got: ", line);
    var parts: [1..2] string = line.split(" | ");
    //  writeln("Got: ", (parts[1], parts[2]));
    resetCounts();
    var digits: [0..9] string = canonical(parts[1].strip().split(" "), true);
    writeln(counts);
    var outdigits: [1..4] string = canonical(parts[2].strip().split(" "));

    var key: map(string, int);
    var seven, five: string;
    var E: wires;
    // find four numbers by length
    for digit in digits {
      const len = digit.size;
      if len == 2 then
        key.add(digit, 1);
      if len == 3 {
        key.add(digit, 7);
        seven = digit;
      }
      if len == 4 then
        key.add(digit, 4);
      if len == 7 {
        key.add(digit, 8);
      }
    }
    // find 3
    for digit in digits {
      const len = digit.size;
      if len == 5 && contains(digit, seven) then
        key.add(digit, 3);
    }
    // find E
    for w in wires do
      if counts[w] == 4 then
        E = w;
    // find 2
    for digit in digits do
      if digit.size == 5 && contains(digit, E:string) then
        key.add(digit, 2);
    // find 5
    for digit in digits do
      if digit.size == 5 && !key.contains(digit) then {
        key.add(digit, 5);
        five = digit;
      }
    // find 6
    for digit in digits do
      if digit.size == 6 && contains(digit, five) {
        if contains(digit, E:string) then
          key.add(digit, 6);
        else
          key.add(digit, 9);
      }
    // find 0
    for digit in digits do
      if !key.contains(digit) then
        key.add(digit, 0);
    
    writeln(key);

    // solve
    var answer: int;
    for digit in outdigits {
      answer *= 10;
      answer += key.getValue(digit);
    }
    yield answer;
  }
}

writeln(+ reduce getDigitsOutputs());
