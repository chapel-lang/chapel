// try a rank-change cast
var X = [1, 2, 3, 4];
var Y = X: [1..2, 1..2] int(32);

// try casts between non-rectangular arrays
var D = {1..3, 1..3};
var DA: domain(int) = {200, 100, 300};
var DS: sparse subdomain(D);
var AA: [DA] real;
var AS: [DS] real;

var A = [1.2, 3.4, 5.6]: [DA] real(32);
var B = [1.2, 3.4, 5.6]: [DS] real(32);
var C = AA: [0..<AA.size] real(32);
var E = AS: [0..<AS.size] real(32);
var F = AA: [DS] real(32);
var G = AS: [DA] real(32);
