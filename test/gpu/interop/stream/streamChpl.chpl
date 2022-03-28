use CTypes;


param N = 10;

extern proc vector_add(
    a : c_ptr(c_float),
    b : c_ptr(c_float),
    c : c_ptr(c_float),
    alpha : c_float,
    n : c_int);

var a : c_array(c_float, N); 
var b : c_array(c_float, N);
var c : c_array(c_float, N);
for i in 0..<N {
    a[i] = 0;
    b[i] = i : c_float;
    c[i] = 2;
}
const alpha = 3.0 : c_float;

vector_add(a,b,c,alpha,N);

writeln(a);
