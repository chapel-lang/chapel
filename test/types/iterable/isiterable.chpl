// Some types
youAreNotIterable(bool);
youAreNotIterable(int);
youAreNotIterable(uint);
youAreNotIterable(real);
youAreNotIterable(imag);
youAreNotIterable(complex);
youAreIterable(string);
youAreIterable(domain(1));

// Some values
enum alphabet {A, B, C};
youAreNotIterable(alphabet);

var a = [1,2,3,4];
youAreIterable(a);

var t = (1,2,3,4);
youAreIterable(t);

record R { iter these() { yield 1; } }
youAreIterable(R);

record R2 { }
youAreNotIterable(R2);

// Range
youAreIterable(1..10);


// Associatve array
var d: domain(string);
var aa: [d] int;
youAreIterable(aa);


inline proc youAreNotIterable(type t) {
  var x: t;
  youAreNotIterable(x);
}

inline proc youAreIterable(type t) {
  var x: t;
  youAreIterable(x);
}

inline proc youAreNotIterable(x) { assert(!isIterable(x)); }
inline proc youAreIterable(x) { assert(isIterable(x)); }
