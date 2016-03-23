writeln(([i in 1..4] i**2) + ([i in 1..4] i**3));
writeln((forall i in 1..4 do i**2) + (forall i in 1..4 do i**3));
writeln((for i in 1..4 do i**2) + (for i in 1..4 do i**3));
writeln(for i in 1..4 do i**2 + for i in 1..4 do i**3);
