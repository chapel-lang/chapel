class Matrix {
  type element_t;
  var dom;
  var init: element_t;

  var data: [dom] element_t = init;
} 

var m = new Matrix(real, {1..10, 1..10}, 1.0);
