typedef struct user_pyobjects {
  PyObject *t;
  PyObject *s;
  int inced_global_state;
} user_pyobjects;

#ifndef SWIG
#define D_ParseNode_User user_pyobjects
#define D_ParseNode_Globals PyObject
#include "../d.h"
#endif

void my_d_loc_t_s_set(d_loc_t *dlt, D_Parser *dp, int val);
int my_d_loc_t_s_get(d_loc_t *dlt, D_Parser *dp);
void my_D_ParseNode_end_set(D_ParseNode *dpn, D_Parser *dp, int val);
void my_D_ParseNode_end_skip_set(D_ParseNode *dpn, D_Parser *dp, int val);
int my_D_ParseNode_end_get(D_ParseNode *dpn, D_Parser *dp);
int my_D_ParseNode_end_skip_get(D_ParseNode *dpn, D_Parser *dp);
PyObject *my_D_ParseNode_symbol_get(D_ParseNode *dpn, D_Parser *dp);

void remove_parse_tree_viewer(D_Parser* dp);
void add_parse_tree_viewer(D_Parser* dp);
void del_parser(D_Parser *dp);
D_Parser *make_parser(int idpt,
		      PyObject *self,
		      PyObject *reject,
		      PyObject *make_token,
		      PyObject *loc_type,
		      PyObject *node_info_type,
		      PyObject *actions,
		      PyObject *initial_white_space_fn,
		      PyObject *syntax_error_fn,
		      PyObject *ambiguity_fn,
		      int dont_fixup_internal_productions,
		      int dont_merge_epsilon_trees,
		      int commit_actions_interval,
		      int error_recovery,
		      int print_debug_info,
		      int partial_parses,
		      int dont_compare_stacks,
		      int dont_use_eagerness_for_disambiguation,
		      int dont_use_height_for_disambiguation,
		      char *start_state);
PyObject *run_parser(D_Parser *dp, PyObject* string, int buf_idx);
int make_tables(char *grammar_string, char *grammar_pathname);
int load_parser_tables(char *tables_name);
