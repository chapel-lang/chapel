var A: [1..0] int;
A.push_back(1);
A.push_back(2);
A.push_back(3);
writeln((A, A.numElements));
A.clear();
writeln((A, A.numElements));
