use IO, Set;

iter readCommands() {
    var dir: string, amt: int;
    while readf("%s %i\n", dir, amt) {
        yield (dir, amt);
    }
}

var head, tail = [0, 0],
    visited = new set(head.type);

param X = 0, Y = 1;

for (dir, amt) in readCommands() {
    for 0..#amt {
        select dir {
            when "U" do head += [0, 1];
            when "D" do head -= [0, 1];
            when "L" do head -= [1, 0];
            when "R" do head += [1, 0];
        }

        const delta = head - tail;

        if abs(delta[X]) > 1 && delta[Y] == 0
            then tail[X] += sgn(delta[X]);
        else if abs(delta[Y]) > 1 && delta[X] == 0
            then tail[Y] += sgn(delta[Y]);
        else if abs(delta[0]) + abs(delta[1]) > 2
            then tail += sgn(delta);

        visited.add(tail);
    }
}

writeln(visited.size);
