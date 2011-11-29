use Containers;

writeln("Vector with default initial capacity");
var v = new Vector(int);
writeln("Size = ", v.size);
writeln("Capacity = ", v.capacity);

writeln("push 1 through 100 ");
serial true {
  v.push(1..100);
}
writeln("Size = ", v.size);
writeln("Capacity = ", v.capacity);

writeln("Empty the vector");
while !v.empty {
    v.pop();
}
writeln("Size = ", v.size);
writeln("Capacity = ", v.capacity);

writeln("---------------------------------------------");

writeln("Vector with initial capacity of 100");
var v2 = new Vector(int, 100);
writeln("Size = ", v2.size);
writeln("Capacity = ", v2.capacity);

writeln("push 1 through 100 ");
serial true {
  v2.push(1..100);
}
writeln("Size = ", v2.size);
writeln("Capacity = ", v2.capacity);

