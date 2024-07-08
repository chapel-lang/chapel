use IO, Set;

const ropeLength = 10;

iter readCommands() {
    var dir: string, amt: int;
    while readf("%s %i\n", dir, amt) {
        yield (dir, amt);
    }
}

var rope : [1..ropeLength] ([0..<2] int),
    visited = new set([0..<2] int);

for (dir, amt) in readCommands() {
    for 0..#amt {
        // move the head
        select dir {
            when "U" do rope[1][0] += 1;
            when "D" do rope[1][0] -= 1;
            when "L" do rope[1][1] -= 1;
            when "R" do rope[1][1] += 1;
        }

        // compute the movements of the other segments
        for i in 2..ropeLength {
            // compute the separation of H and T
            const delta = rope[i-1] - rope[i];

            // move the tail if needed
            if abs(delta[0]) > 1 {
                rope[i][0] += sgn(delta[0]);
                if abs(delta[1]) > 0 then rope[i][1] += sgn(delta[1]);
            }
            else if abs(delta[1]) > 1 {
                rope[i][1] += sgn(delta[1]);
                if abs(delta[0]) > 0 then rope[i][0] += sgn(delta[0]);
            }
        }

        visited.add(rope[ropeLength]);
    }
}

writeln(visited.size);
