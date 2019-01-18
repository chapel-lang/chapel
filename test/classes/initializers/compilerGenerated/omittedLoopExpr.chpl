
class ForLoops {
  var Dom: domain(1);

  var A = for i in 1..10 do i;
  var B = for (i, j) in zip(1..10, 0..9) do i*j;
  var C = for 1..10 do 5;
  var D = for i in 1..10 do if i % 2 == 0 then i;
  var E = for (i, j) in zip(1..10, 0..9) do if (i*j) % 2 == 0 then i*j;
  var F = for 1..10 do if Dom.size > 5 then Dom.size;

  var G = for i in (for j in 1..10 do j) do i;

  var Z: [Dom] real = for i in 1..10 do i;
}

var fors = new unmanaged ForLoops({1..10});
writeln("----- ForLoops -----");
writeln(fors);
writeln();
delete fors;

class ForallLoops {
  var Dom: domain(1);

  var A = forall i in 1..10 do i;
  var B = forall (i, j) in zip(1..10, 0..9) do i*j;
  var C = forall 1..10 do 5;
  var D = forall i in 1..10 do if i % 2 == 0 then i;
  var E = forall (i, j) in zip(1..10, 0..9) do if (i*j) % 2 == 0 then i*j;
  var F = forall 1..10 do if Dom.size > 5 then Dom.size;

  var G = forall i in (forall j in 1..10 do j) do i;
  var H = forall (i, j) in zip(1..10, forall k in 0..9 do k) do i*j;

  var Z: [Dom] real = forall i in 1..10 do i;
}

var foralls = new unmanaged ForallLoops({1..10});
writeln("----- ForallLoops -----");
writeln(foralls);
writeln();
delete foralls;

class SquareLoops {
  var Dom: domain(1);

  var A = [i in 1..10] i;
  var B = [(i, j) in zip(1..10, 0..9)] i*j;
  var C = [1..10] 5;
  var D = [i in 1..10] if i % 2 == 0 then i;
  var E = [(i, j) in zip(1..10, 0..9)] if (i*j) % 2 == 0 then i*j;

  var G = [i in ([ j in 1..10] j)] i;
  var H = [(i, j) in zip(1..10, [k in 0..9] k)] i*j;

  var Z: [Dom] real = [i in 1..10] i;
}

var squares = new unmanaged SquareLoops({1..10});
writeln("----- SquareLoops -----");
writeln(squares);
writeln();
delete squares;
