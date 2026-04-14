use CTypes;


extern {
  #include "chpltypes.h"
  #include <stdio.h>

  typedef struct holder {
    void** global_serialize_table;
    void** globals_registry;
    char** mem_descs;
    void** private_broadcast_table;
  } holder;

  // Pass in the values materialized from the addresses through Chapel.
  void getter_chpl_global_serialize_table(void** x);
  void getter_chpl_globals_registry(void** x);
  void getter_chpl_mem_descs(char** x);
  void getter_chpl_private_broadcast_table(void** x);
  void get_from_holder(holder* x);

  // Realize the addresses in C.
  extern void* const chpl_global_serialize_table[];
  extern ptr_wide_ptr_t chpl_globals_registry[];
  extern const char* chpl_mem_descs[];
  extern void* const chpl_private_broadcast_table[];

  #define PRINTER(in__, name__) do {                        \
    void* ptr1 = ((void*) in__);                            \
    void* ptr2 = ((void*) name__);                          \
    printf("%s\n", __FUNCTION__);                           \
    const char* msg = (ptr1 == ptr2) ? "GOOD" : "FAIL";     \
    printf("%s\n", msg );                                   \
  } while (0)
    
  void getter_chpl_global_serialize_table(void** x) {
    PRINTER(x, chpl_global_serialize_table);
  }

  void getter_chpl_globals_registry(void** x) {
    PRINTER(x, chpl_globals_registry);
  }

  void getter_chpl_mem_descs(char** x) {
    PRINTER(x, chpl_mem_descs);
  }

  void getter_chpl_private_broadcast_table(void** x) {
    PRINTER(x, chpl_private_broadcast_table);
  }

  void get_from_holder(holder* x) {
    printf("-- %s --\n", __FUNCTION__);
    getter_chpl_global_serialize_table(x->global_serialize_table);
    getter_chpl_globals_registry(x->globals_registry);
    getter_chpl_mem_descs(x->mem_descs);
    getter_chpl_private_broadcast_table(x->private_broadcast_table);
  }

  #undef PRINTER
}

proc callTheGetter(param name: string, in x) {
  select name {
    // Can generate calls to these using 'name' and pass a 'void*' to copy,
    // but opt not to do that here in order to simplify the testing surface.
    when 'chpl_global_serialize_table'  do getter_chpl_global_serialize_table(x);
    when 'chpl_globals_registry'        do getter_chpl_globals_registry(x);
    when 'chpl_mem_descs'               do getter_chpl_mem_descs(x);
    when 'chpl_private_broadcast_table' do getter_chpl_private_broadcast_table(x);
    otherwise                           halt('Not handled!');
  }
}

// chpl_global_serialize_table
// chpl_globals_registry
// chpl_mem_descs
// chpl_private_broadcast_table
proc test() {
  extern const chpl_global_serialize_table:     c_ptr(c_ptr(void));
  extern const chpl_globals_registry:           c_ptr(c_ptr(void));
  extern const chpl_mem_descs:                  c_ptr(c_ptr(c_char));
  extern const chpl_private_broadcast_table:    c_ptr(c_ptr(void));

  callTheGetter('chpl_global_serialize_table',
                 chpl_global_serialize_table);
  callTheGetter('chpl_globals_registry',
                 chpl_globals_registry);
  callTheGetter('chpl_mem_descs', chpl_mem_descs);
  callTheGetter('chpl_private_broadcast_table',
                 chpl_private_broadcast_table);

  var x: holder;
  x.global_serialize_table  = chpl_global_serialize_table;
  x.globals_registry        = chpl_globals_registry;
  x.mem_descs               = chpl_mem_descs;
  x.private_broadcast_table = chpl_private_broadcast_table;

  get_from_holder(c_ptrTo(x));
}

proc main() {
  test();
}
