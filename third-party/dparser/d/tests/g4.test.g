{
extern char *ops;
extern void *ops_cache;
int ops_scan(char *ops, void *ops_cache, char **as, 
    int *col, int *line, unsigned char *op_assoc, int *op_priority);
}

X: '1' (${scan ops_scan(ops, ops_cache)} '2')*;
