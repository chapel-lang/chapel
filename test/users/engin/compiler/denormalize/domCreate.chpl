use LayoutCSR;

var dom = {0..10};
writeln(dom);

var dom2 = {0..10, 0..10};
writeln(dom2);

config const N = 10;
const dom3= {N/2..N-1, N/2..N-1};
var sd: sparse subdomain(dom3) dmapped CSR();
writeln(dom3);
