//
// parameter and type declarations: these are set at compile time;
// numdims can be overridden on the compiler's command-line
//
config param numdims = 2;          // the number of dimensions for our points

type coord = numdims*real;         // use numdims-tuples of reals as coords


//
// configuration constants: these can be overridden at execution time
//
config const n = 10,               // the number of points
             k = 3,                // the number of centers
             thresh = 1e-4;        // the threshold for termination

config const debug = false;        // controls debug printing

config const printTiming = false;  // unused, but should be eventually


//
// make sure k is positive because we divide by it
//
assert(k > 0, "k must be positive");



//
// the index space and array for the data elements
//
const dataSpace = {1..n};
var data: [dataSpace] coord = [i in dataSpace] initData(i);


//
// the index space and array for the cluster centers
//
const centerSpace = {1..k};
var centers: [centerSpace] coord = initCenters(data);


//
// Print initial locations
//
writeln("data points are at: ", data);
writeln("initial centers are: ", centers);


//
// the main computation
//
var prevError = 0.0,      // the error from the previous iteration
    numIters = 0;         // the number of iterations required

do {
  if debug then writeln("centers are: ", centers);

  //
  // compute the error of the current solution and the offsets for the
  // new solution
  //
  var (error, offsets) = kmeans reduce [d in data] 
                                  (findBestCenter(d, centers), d, centers);

  centers += offsets;   // update the centers using the computed offsets

  if debug then writeln("error is: ", error);

  prevError <=> error;  // swap the error and previous error for the next iter

  numIters += 1;        // increment the number of iterations
} while (abs(error - prevError) > thresh);


//
// Print the result
//
writeln("converged to < ", thresh, " in ", numIters, " iterations");
writeln("final centers are: ", centers);



//
// helper function to find the best center for a given coordinate
//
proc findBestCenter(loc: coord, centers: [] coord) {
  return minloc reduce zip(dist(loc, centers), centerSpace);
}


//
// helper function to compute the distance between two coordinates
//
proc dist(x: coord, y:coord) {
  return sqrt(+ reduce [d in 0..#numdims] ((x(d) - y(d))**2));
}


//
// User-defined reduction class for computing updated center locations
//

record Accumulation {
  var error: real;                     // accumulated error
  var clusterSize: [centerSpace] int;  // histogram of data->center counts
  var offset: [centerSpace] coord;     // offset for each center
}

class kmeans: ReduceScanOp {
  type eltType;                        // required field
  var state: Accumulation;

  proc identity {
    return new Accumulation();
  }

  //
  // the accumulator takes a distance, center, and data coordinate and
  // updates the error, the appropriate clusterSize, and the offset
  // for that center
  //
  proc accumulateOntoState(ref state, ((dist, ctr), datum, centers)) {
    state.error            += dist;    
    state.clusterSize(ctr) += 1;
    state.offset(ctr)      += datum - centers(ctr);
  }

  proc accumulate(value) {
    accumulateOntoState(state, value);
  }

  proc accumulate(other: Accumulation) {
    state.error       += other.error;
    state.clusterSize += other.clusterSize;
    state.offset      += other.offset;
  }

  //
  // the combine function takes two kmeans classes and combines
  // them
  //
  proc combine(other: borrowed kmeans) {
    accumulate(other.state);
  }

  //
  // the generate function updates the centers and returns the error
  //
  proc generate() {
    // this is a bug workaround
    state.offset = state.offset / state.clusterSize;
    return (state.error, state.offset);
    // the original was
    //return (error, offset / clusterSize);
    // but that seems to return an iterator record and then causes
    // a type mismatch compilation error on the arrays branch.
  }

  proc clone() {
    return new unmanaged kmeans(eltType=eltType);
  }
}


//
// a helper function that, for now, initializes data element i to be
// (i, 0.0, 0.0, 0.0, ...)
//
proc initData(i) {
  var loc: coord;
  loc(0) = i;
  return loc;
}


//
// compute the initial centers by taking every ceil(n/k)-th element from data[]
//
proc initCenters(data) {
  return data[dataSpace by intCeilDiv(n,k)];
}


//
// a helper function to divide a coordinate by a floating point value
//
proc /(x: coord, y: real) {
  var result: coord;
  for param d in 0..numdims-1 do
    result(d) = x(d) / y;
  return result;
}


//
// a helper function that computes the integer ceiling of x/y
//
proc intCeilDiv(x: integral, y: integral) {
  return (x + (y-1)) / y;
}


