/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

int write_c_tables(Grammar *g);
int write_binary_tables(Grammar *g);
int write_binary_tables_to_file(Grammar *g, FILE *fp);
int write_binary_tables_to_string(Grammar *g, 
				  unsigned char **str, unsigned int *str_len);
