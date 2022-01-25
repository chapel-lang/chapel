use IO;

enum direction {forward, backward, down, up};
use direction;

var horizontal = 0, depth = 0;

do {
  var dir: direction, distance: int;
  const success = read(dir, distance);
  select dir {
    when forward do
      horizontal += distance;
    when backward do
      horizontal -= distance;
    when up do
      depth -= distance;
    when down do
      depth += distance;
  }
} while (success);
writeln(horizontal*depth);
