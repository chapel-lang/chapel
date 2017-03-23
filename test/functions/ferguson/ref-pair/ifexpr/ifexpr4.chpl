proc ifexpr(a, b, c) {
   return if a[1] == 1 then b else c;
}

const A:[1..3] int = 1;
const B:[1..3] int = 2;
const C:[1..3] int = 3;

const D = ifexpr(A, B, C);

writeln(D);

