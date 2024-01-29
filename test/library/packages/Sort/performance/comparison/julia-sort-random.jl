# run it with
#   julia julia-sort-random.jl

function testSort(n)
  v = rand(UInt64, n)
  @timed sort!(v);
end

n = 128*1024*1024
t = testSort(n)
println(8*n / t.time / 1024.0 / 1024.0, " MiB/s")
println(n / t.time / 1000.0 / 1000.0, " million elements sorted per second")
