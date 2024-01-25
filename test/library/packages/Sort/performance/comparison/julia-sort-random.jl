# run it with
#   julia julia-sort-random.jl

function testSort(n)
  v = rand(Int, n)
  @timed sort!(v);
end

n = 1024*1024*128
t = testSort(n)
println(8*n / t.time / 1024.0 / 1024.0, " MiB/s")
println(8*n / t.time / 1000.0 / 1000.0, " MB/s")
