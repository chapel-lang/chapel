// node nodejs-sort-random.js

function test() {
  const arr = [];
  const n = 64*1024*1024; // 128 Mi elements crashes due to array memory limit
  for (let i = 0; i < n; i++) {
    const a = Math.floor(Math.random() * 0xffff);
    const b = Math.floor(Math.random() * 0xffff);
    const c = Math.floor(Math.random() * 0xffff);
    const d = Math.floor(Math.random() * 0xffff);
    arr.push((a << 48) | (b << 32) || (c << 16) || d)
  }

  console.log("Begin sorting");
  const start = performance.now();
  arr.sort()
  const end = performance.now();
  const elapsed = (end - start)/1000.0; // in seconds
  console.log("Time: " + elapsed + " s");
  console.log("MiB/s: " + 8*n/elapsed/1024.0/1024.0);
  console.log("million elements sorted per second: " + n/elapsed/1000.0/1000.0);
}

test()
