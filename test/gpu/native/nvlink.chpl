use CTypes, IO, Time, Sort, Subprocess, ChplConfig;

config const N = 1024 * 1024 * 1024; // Setting to 8 GB causes an error?
config const NUM_TRIALS = 3;         // Setting to 16 causes an error?
config const p2p = true;
config const verbose = false;
config const runChecksForTest = false;
config const printTimes = true;
const gNumDevs = here.gpus.size;
const gGpuRange = 0..<here.gpus.size;

proc bw(time) { return (N : real) / (time * (1.0 * (1 << 30))); }

proc measureP2P(dev1 : integral, dev2 : integral) {
  var timer : stopwatch;

  const locDev1 = here.gpus[dev1];
  on here.gpus[dev2] {
    var dev2Buf : [0..<N] uint(8);
    on locDev1 {
      var dev1Buf : [0..<N] uint(8);
      timer.start();
      dev2Buf = dev1Buf;
      timer.stop();
    }
  }
  return timer.elapsed();
}

proc medianTime(times, i, j) {
  var arr = times[i, j, ..];
  const n = arr.size;
  sort(arr);
  if (n % 2 == 0) {
    return (arr[n/2-1] + arr[n/2]) / 2.0;
  } else {
    return arr[n/2];
  }
}

proc doPrintTimes(times) {
  writef("Times (GiB/s):\n");

  write(b"┏");      for i in 0..< gNumDevs * 9 + 5 do write(b"━");        write(b"┓\n");
  write(b"┃    │"); for i in 0..< gNumDevs         do writef("%-8i ", i); write(b"┃\n");
  write(b"┃━━━━│"); for i in 0..< gNumDevs * 9     do write(b"━");        write(b"┃\n");

  for i in gGpuRange {
    writef(b"┃ %2i │", i);
    for j in gGpuRange {
      writef("%8.1dr ", bw(medianTime(times, i, j)));
    }
    write("┃\n");
  }

  write("┗"); for i in 0..<gNumDevs * 9 + 5 do write("━"); write("┛\n");
}

proc turnOnPeerAccess() {
  extern proc chpl_gpu_can_access_peer(i : c_int, j : c_int) : bool;
  extern proc chpl_gpu_set_peer_access(
    i : c_int, j : c_int, shouldEnable : bool) : bool;
  var n = 0;

  for (i,j) in {gGpuRange, gGpuRange} {
    if chpl_gpu_can_access_peer(i : c_int, j : c_int) {
      n += 1;
      if verbose then writef("ENABLE ACCESS: %i -> %i\n", i, j);
      chpl_gpu_set_peer_access(i : c_int, j : c_int, true);
    }
  }
  return n;
}

proc measureTimes(ref times) {
  for i in gGpuRange {
    for j in gGpuRange {
      if verbose then stderr.writef("%i -> %i: ", i, j);
      for k in 0..<NUM_TRIALS {
        if verbose { stderr.write("."); stderr.flush(); }
        times[i,j,k] = measureP2P(i,j);
      }
      if verbose then stderr.writeln();
    }
  }
}

proc checkNumAccessEnabled_cuda(valFromChpl) {
  // The "nvidia-smi topo -p2p w" command will print a table showing what pairs
  // of GPUs can enable peer access, each pair is marked with "OK" so we could
  // the number of "OKs" output by the command. 
  var sub = spawn(["nvidia-smi", "topo", "-p2p", "w"], stdout=pipeStyle.pipe);
  var line : string;
  var count = 0;
  while sub.stdout.readLine(line) do
    count += line.count("OK");
  count -= 1; // Remove one for "OK" in legend

  assert(count == valFromChpl,
    "peer accesses enabled by Chapel does not match number possible ",
    "according to nvidia-smi");
}

proc checkNumAccessEnabled_rocm(valFromChpl) {
  // The "rocm-smi --showtopotype" command will print a table showing how pairs
  // of GPUs are connected to one another.   The behavior I've seen with ROCM
  // is that GPUs connected by PCIe can have peer access enabled.  It's very
  // likely that this test will have to be updated to account for other link
  // types in the future. Unfortunatly, I'm not aware of a subcommand in
  // `rocm-smi` that would let me directly see which GPUs can enable peer
  // access.  
  var sub = spawn(["rocm-smi", "--showtopotype"], stdout=pipeStyle.pipe);
  var line : string;
  var count = 0;
  while sub.stdout.readLine(line) do
    count += line.count("PCIE");

  assert(count == valFromChpl,
    "peer accesses enabled by Chapel does not match number of PCIE ",
    "connections found uising rocm-smi");
}

proc checkNumAccessEnabled(valFromChpl) {
  // For the time being I'm going to run this as a CUDA only test as we I
  // believe `rocm-smi --showtopotype` could be used to detect pairs of AMD
  // GPUs that can have peer access enabled but I have not yet explored this.
  select ChplConfig.CHPL_GPU_CODEGEN {
    when "cuda" do checkNumAccessEnabled_cuda(valFromChpl);
    when "rocm" do checkNumAccessEnabled_rocm(valFromChpl);
    otherwise do compilerError("Update test to check for new runtime lib");
  }
}

proc main() {
  var times : [gGpuRange, gGpuRange, 0..<NUM_TRIALS] real;
  var numAccessEnabled = 0;
  if p2p then numAccessEnabled = turnOnPeerAccess();
  measureTimes(times);
  if printTimes then doPrintTimes(times);

  if(runChecksForTest) {
    checkNumAccessEnabled(numAccessEnabled);
  }
}
