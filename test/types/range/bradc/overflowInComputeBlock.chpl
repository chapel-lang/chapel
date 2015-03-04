type tp = int(16);
config const n = max(tp)-1;
writeln( + reduce [1:tp..n:tp] (1:tp));
