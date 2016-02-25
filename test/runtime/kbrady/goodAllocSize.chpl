extern proc chpl_mem_good_alloc_size(minSize:size_t): size_t;

writeln(chpl_mem_good_alloc_size(1));
writeln(chpl_mem_good_alloc_size(16));
writeln(chpl_mem_good_alloc_size(17));
writeln(chpl_mem_good_alloc_size(1000));
