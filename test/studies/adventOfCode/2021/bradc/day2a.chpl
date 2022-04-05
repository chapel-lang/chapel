use IO;

enum direction {forward, backward, down, up};
use direction;

var horizontal = 0, depth = 0, aim = 0;

var dir: direction, distance: int;
while read(dir, distance) {
  select dir {
    when forward {
      horizontal += distance;
      depth += aim*distance;
    }
    when backward {
      horizontal -= distance;
      depth -= aim*distance;
    }
    when up do
      aim -= distance;
    when down do
      aim += distance;
  }
}
writeln(horizontal*depth);
