_extern def chpl_malloc(number, size, description, userCode, lineno, filename);

chpl_malloc(max(int(32)), max(int(32)), "test big malloc", true, 3, "allocBig.chpl");
