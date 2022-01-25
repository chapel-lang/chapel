use IO, Set;

config const days = 80, debug = false;

class Fish {
  var age: int;
}

var school: set(owned Fish);

do {
  var age: int;
  var success = read(age);
  if success {
    school.add(new Fish(age));
    try { readf(","); } catch { success = false; }
  }
} while success;

if debug then writeln(school);

for i in 1..days {
  var newfish: atomic int;
  forall fish in school {
    if fish.age == 0 {
      fish.age = 6;
      newfish.add(1);
    } else {
      fish.age -= 1;
    }
  }
  for i in 1..newfish.read() do
    school.add(new Fish(age=8));
//  writeln("After ", i, " day(s), school is ", school);
  writeln("After ", i, " day(s), school size is ", school.size);
}
