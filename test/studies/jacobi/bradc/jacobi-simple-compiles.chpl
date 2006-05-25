// configuration constants
config const n = 10;
config const epsilon = 0.0001;

const BigD: domain(2) = [0..n+1, 0..n+1];
const D: domain(2) = [1..n, 1..n];
//const D: subdomain(BigD) = [1..n, 1..n];

const north = (-1,  0);
const south = ( 1,  0);
const east  = ( 0,  1);
const west  = ( 0, -1);

fun main() {
  var A: [BigD] float = 0.0, 
      B: [D] float;

  var South: domain(2) = D.exterior(south);

  [ij in South] A(ij) = 1.0;

  var bigdiff: float;

  do {

    [ij in D] B(ij) = (A(ij+north) + A(ij+south) + A(ij+east) + A(ij+west))/4.0;

    //    bigdiff = max(float) reduce [ij in D] (A(ij) - B(ij));
    bigdiff = max(float) reduce (A-B);

    [ij in D] B(ij) = A(ij);
  } while (bigdiff > epsilon);
}
