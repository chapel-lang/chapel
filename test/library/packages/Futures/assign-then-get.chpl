use Futures;

var G: Future(int);

// G is currently an invalid future
assert(G.isValid() == false);

// G is now a valid future
G = async(lambda(x: int) { return 10 + x; }, 1);
const g = G.get();
writeln(g);
