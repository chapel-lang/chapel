proc f(x: Hashable) {}

record R {
    proc hash(): uint {
        return 0;
    }
}

f(new R());
