
extern {
  #define ADD(x,y) ((x)+(y)) // NOT SUPPORTED
}
var x = ADD(1,2); // ERROR - ADD not defined in Chapel.

