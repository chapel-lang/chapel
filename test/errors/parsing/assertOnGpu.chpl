@assertOnGpu
record myRecord {
    @assertOnGpu
    var A = foreach i in 1..100 do i;
}

@assertOnGpu
proc f() {

}
