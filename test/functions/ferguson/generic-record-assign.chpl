record R_with_1_param {
   param p : int = 1;
}

var qq1 = new R_with_1_param();
var qq2 = new R_with_1_param();
qq1 = qq2; // this one works

record R_with_2_params {
   param p : int = 1;
   param s : int = 4;
}

var rr1 = new R_with_2_params();
var rr2 = new R_with_2_params();
rr1 = rr2; // this one does not.

