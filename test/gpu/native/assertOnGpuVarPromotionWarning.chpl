on here.gpus[0] {
    var Source = [1,2,3,4,5];

    @assertOnGpu
    var A = Source + 1;

    @assertOnGpu
    var B = (foreach i in 1..5 do i) + 1;
}
