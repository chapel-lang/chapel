use CTypes;

var x: c_ptr(int);
var y: c_ptr(uint) = x;

var x_const: c_ptrConst(int);
var y_const: c_ptrConst(uint) = x_const;
