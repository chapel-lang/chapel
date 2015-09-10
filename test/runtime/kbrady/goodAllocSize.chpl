extern proc chpl_mem_goodAllocSize(minSize:size_t): size_t;

writeln(chpl_mem_goodAllocSize(1));
writeln(chpl_mem_goodAllocSize(16));
writeln(chpl_mem_goodAllocSize(17));
writeln(chpl_mem_goodAllocSize(1000));
