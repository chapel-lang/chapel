var D = {1..100};
var sparseD1: sparse subdomain(D); // ok
var sparseD2: sparse subdomain(); // bad
var sparseD3: sparse subdomain(D, D); // bad
var sparseD4: sparse domain(); // bad
