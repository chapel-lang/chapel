@gpu.blockSize(128)
record myRecord {
    @gpu.blockSize(128)
    var A = foreach i in 1..100 do i;
}

@gpu.blockSize(128)
proc f() {

}
