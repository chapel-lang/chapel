record R {
    proc hash(): int {
        return 0;
    }
}

chpl__defaultHashWrapper(new R());
