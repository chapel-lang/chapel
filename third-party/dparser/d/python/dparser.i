%module dparser_swig
%{
#include "pydparser.h"
%}

%typemap(python, in) PyObject* {
  $target = $source;
}

%typemap(python, out) PyObject* {
  $target = $source;
}

%include pydparser.h

typedef struct d_loc_t {
  char *pathname;
  int previous_col, col, line;
} d_loc_t;

typedef struct D_ParseNode {
  int			symbol;
  d_loc_t		start_loc;
  D_ParseNode_Globals	*globals;
  user_pyobjects	user;
} D_ParseNode;

D_ParseNode *d_get_child(D_ParseNode *pn, int child);
D_ParseNode *d_find_in_tree(D_ParseNode *pn, int symbol);
int d_get_number_of_children(D_ParseNode *pn);

