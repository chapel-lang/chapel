
def neighborhood(cell: dimension*int, k: int) {
  var range_tuple: dimension*range(stridable=true);
  for d in dimensions do range_tuple(d) = -2*k..2*k by 2;
  var shifts = [(...range_tuple)];

  for shift in shifts do
    yield cell + shift;

}


def intVec( x ...? n ) {
  var int_vec: dimension*int;
  for param i in 1..n do
    int_vec(x(i)(2)) = x(i)(1);
  return int_vec;
}







forall cell in D {   // not sure what the domain is yet

  //===> Maximum allowed differential ===>
  var max_rise = 0.0,
    max_drop = 0.0;

  //==== First compute the max and min values in the neighborhood ====
  for nbr in neighborhood(cell,1) {
    max_rise = max( max_rise, sol.value(nbr) );
    max_drop = min( max_drop, sol.value(nbr) );
  }

  //==== Now adjust accordingly to get the diffs ====
  max_rise = max_rise - sol.value(cell);
  max_drop = sol.value(cell) - max_drop;
  var max_allowed_diff = min(max_rise, max_drop);
  //<=== Maximum allowed differential <===


  //==== Estimated differentials ====
  var est_diff: [dimensions] real;
  for d in dimensions {
    var shift: dimension*int;
    shift(d) = 2;
    est_diff(d) = (sol.value(cell + shift) 
		   - sol.value(cell - shift)
		  ) / 2.0;
  }

  //==== Maximum differential based on the estimates in each direction ====
  var max_est_diff = 0.0;
  for d in dimensions do 
    max_est_diff = max(max_est_diff, abs(est_diff(d)));


  //==== Compute limiter ====
  var limiter = 
    if max_est_diff > max_allowed_diff then max_allowed_diff / max_est_diff
    else 1;


  //==== Limit the estimated differentials ====
  est_diff *= limiter;

}
