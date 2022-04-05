use IO, Set;

config const days = 80, debug = false;

class Fish {
  var age: int;
}

var school: set(owned Fish);

var age: int;
while read(age) {
  school.add(new Fish(age));
  try {
    readf(",");
  } catch {
    break;
  }
}

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
  if debug then
    writeln("After ", i, " day(s), school is ", school);
  writeln("After ", i, " day(s), school size is ", school.size);
}
