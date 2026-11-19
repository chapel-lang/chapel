// The purpose of this test is to use and test the explicit "gpu threadIdx",
// "gpu blockIdx" "gpu blockDim" and "gpu gridDim" primitives.

use CTypes;

param VALS_PER_THREAD=13;

pragma "GPU kernel"
export proc add_nums(dst_ptr: c_ptr(real(32))){
  var tid_x = __primitive("gpu threadIdx x");
  var tid_y = __primitive("gpu threadIdx y");
  var tid_z = __primitive("gpu threadIdx z");
  var bid_x = __primitive("gpu blockIdx x");
  var bid_y = __primitive("gpu blockIdx y");
  var bid_z = __primitive("gpu blockIdx z");
  var blockDim_x = __primitive("gpu blockDim x");
  var blockDim_y = __primitive("gpu blockDim y");
  var blockDim_z = __primitive("gpu blockDim z");
  var gridDim_x  = __primitive("gpu gridDim x");
  var gridDim_y  = __primitive("gpu gridDim y");
  var gridDim_z  = __primitive("gpu gridDim z");

  param VALS_TO_REPORT = 13;
  const idx = VALS_TO_REPORT *
    ((tid_z + blockDim_z * bid_z)  * (gridDim_x * blockDim_x * gridDim_y * blockDim_y) +
     (tid_y + blockDim_y * bid_y)  * (gridDim_x * blockDim_x) +
     (tid_x + blockDim_x * bid_x));

  dst_ptr[idx]    = idx;
  dst_ptr[idx+1]  = tid_x;
  dst_ptr[idx+2]  = tid_y;
  dst_ptr[idx+3]  = tid_z;
  dst_ptr[idx+4]  = bid_x;
  dst_ptr[idx+5]  = bid_y;
  dst_ptr[idx+6]  = bid_z;
  dst_ptr[idx+7]  = blockDim_x;
  dst_ptr[idx+8]  = blockDim_y;
  dst_ptr[idx+9]  = blockDim_z;
  dst_ptr[idx+10] = gridDim_x;
  dst_ptr[idx+11] = gridDim_y;
  dst_ptr[idx+12] = gridDim_z;
}

// gdim = grid dimension; bdim = block dimension
proc runExample(gdimX, gdimY, gdimZ, bdimX, bdimY, bdimZ) {
  writeln(" Grid size: ", gdimX, " x ", gdimY, " x ", gdimZ);
  writeln("Block size: ", bdimX, " x ", bdimY, " x ", bdimZ);

  const N = gdimX * gdimY * gdimZ * bdimX * bdimY * bdimZ * VALS_PER_THREAD;

  var X : [0..<N] real(32);

  var cfg = __primitive("gpu init kernel cfg 3d",
                        /*fn*/ "add_nums":chpl_c_string,
                        /*grd_dims*/ gdimX, gdimY, gdimZ,
                        /*blk_dims*/ bdimX, bdimY, bdimZ,
                        /*args*/1,
                        /*pids*/0,
                        /*reductions*/0,
                        /*hostRegVars*/0);


  // 1 is an enum value that says: "pass the address of this to the
  //   kernel_params, while not offloading anything". I am not entirely sure why
  //   we need to do that for C pointers
  __primitive("gpu arg", cfg, c_ptrTo(X), 1);

  __primitive("gpu kernel launch", cfg);

  writef("     |--- thread idx --|--- block idx ---|--- block size --|--- grid size --|\n");
  writef("  idx tid_x tid_y tid_z bid_x bid_y bid_z bdm_x bdm_y bdm_z gdm_x gdm_y gdm_z\n");

  for i in 0..<N/VALS_PER_THREAD {
    for j in 0..< VALS_PER_THREAD {
      writef("%5.0dr ", X[i*VALS_PER_THREAD + j]);
    }
    writef("\n");
  }

  writeln();
}

proc main() {
  on here.gpus[0] {
    var dummy = [1,2,3]; // to ensure that the CUDA context is attached to the
                         // thread

    runExample(1,1,1, 1,1,1);

    runExample(1,1,1, 2,2,2);
    runExample(1,1,1, 2,3,4);

    runExample(2,2,2, 1,1,1);
    runExample(2,3,4, 1,1,1);

    runExample(2,2,2, 2,2,2);
    runExample(2,3,4, 2,3,4);
  }
}
