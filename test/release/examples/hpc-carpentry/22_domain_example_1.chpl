var domain1to10: domain(1) = {1..10};         // 1D domain from 1 to 10
                                              // defined using the range 1..10
var twoDimensions: domain(2) = {-2..2,0..2};  // 2D domain over a product of 
                                              // two ranges
var thirdDim: range = 1..16;                  // a range
var treeDims: domain(3) = {thridDim, 1..10, 5..10}; // 3D domain over a product
                                                    // of three ranges
for idx in twoDimensions do // Cycle through all points in a 2D domain
  write(idx, ", ");
writeln();
for (x,y) in twoDimensions { // can also cycle using explicit tuples (x,y)
  write("(", x, ", ", y, ")", ", ");
}
