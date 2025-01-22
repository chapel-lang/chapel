use Set;

var s = new set(string);

s.add("my");
s.add("great");
s.add("aunt");
s.add("sally");
s.add("told");
s.add("me");
s.add("to");
s.add("sleep");
s.add("in");
s.add("tomorrow");

// Check the behavior of the contents iterator when used in a serial context
for val in s.contents() {
  writeln(val);
}
