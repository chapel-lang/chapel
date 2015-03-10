extern proc chpl_mem_allocMany(number, size, description, lineno, filename);

chpl_mem_allocMany(max(int(32)), max(int(32)), 0, 3, "allocBig.chpl":c_string);
