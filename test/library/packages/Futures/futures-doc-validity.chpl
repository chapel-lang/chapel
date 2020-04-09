use Futures;

// F is a valid future
var F = async(lambda(x: int) { return 99; }, 13);
const f = F.get();

// G is currently an invalid future
var G: Future(int);

// ...other things can happen here...

// G is now a valid future
G = async(lambda(x: int) { return 42 + x; }, 23);
const g = G.get();
