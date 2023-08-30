proc f(x: hashable) {}

record R {
    proc hash(): uint {
        return 0;
    }
}

f(new R());
