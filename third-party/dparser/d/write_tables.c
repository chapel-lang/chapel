/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/
#include "d.h"
#include "dparse_tables.h"

void myfprintf(FILE *f, const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  if(!f)
    d_fail("trying to write code to binary file");

  vfprintf(f, format, ap);
  va_end(ap);
}
#define fprintf myfprintf

typedef struct OffsetEntry {
  char *name;
  int len;
  int offset;
} OffsetEntry;

typedef Vec(OffsetEntry*) OffsetEntrySet;
typedef Vec(char*) CharPtrVec;

typedef struct Buf {
  char *start;
  char *cur;
  int len;
} Buf;

typedef struct File {
  int binary;
  FILE *fp;
  Buf tables;
  Buf strings;
  OffsetEntrySet entries;
  CharPtrVec relocations;
  CharPtrVec str_relocations;
  int first_member;
  int array_length;
  int n_elems;
  int elem_size;
  int d_parser_tables_loc;
} File;

OffsetEntry null_entry = {"NULL", sizeof("NULL")-1, -1};
OffsetEntry spec_code_entry = {"#spec_code", sizeof("#spec_code")-1, -2};
OffsetEntry final_code_entry = {"#final_code", sizeof("#final_code")-1, -3};

uint32 
offset_hash_fn(OffsetEntry *entry, struct hash_fns_t* fn) {
  fn = fn;
  return strhashl(entry->name, entry->len);
}

int 
offset_cmp_fn(OffsetEntry *a, OffsetEntry *b, struct hash_fns_t* fn) {
  fn = fn;
  return strcmp(a->name, b->name);
}

hash_fns_t 
offset_fns = {
  (hash_fn_t)offset_hash_fn,
  (cmp_fn_t)offset_cmp_fn,
  {0, 0}
};

static void 
write_chk(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
  if (fwrite(ptr, size, nmemb, stream) != nmemb) 
    d_fail("error writing binary file\n");
}

static void
save_binary_tables(File *f) {
  FILE *fp = f->fp;
  int i;
  BinaryTablesHead tables;
  
  tables.n_relocs = f->relocations.n;
  tables.n_strings = f->str_relocations.n;
  tables.d_parser_tables_loc = f->d_parser_tables_loc;
  tables.tables_size = f->tables.cur - f->tables.start;
  tables.strings_size = f->strings.cur - f->strings.start;

  write_chk(&tables, sizeof(BinaryTablesHead), 1, fp);
  write_chk(f->tables.start, sizeof(char), tables.tables_size, fp);
  write_chk(f->strings.start, sizeof(char), tables.strings_size, fp);
  for (i=0; i<f->relocations.n; i++) {
    write_chk(&f->relocations.v[i], sizeof(void*), 1, fp);
  }
  for (i=0; i<f->str_relocations.n; i++) {
    write_chk(&f->str_relocations.v[i], sizeof(void*), 1, fp);    
  }
}

static void
free_tables(File *f) {
  int i;
  if (f->tables.start)
    FREE(f->tables.start);
  if (f->strings.start)
    FREE(f->strings.start);
  vec_free(&f->str_relocations);
  vec_free(&f->relocations);
  for (i=0; i<f->entries.n; i++) {
    if (f->entries.v[i]) {
      FREE(f->entries.v[i]->name);
      FREE(f->entries.v[i]);
      f->entries.v[i] = 0;
    }
  }
  vec_free(&f->entries);
}

static void
init_buf(Buf *buf, int initial_size) {
  buf->len = initial_size;
  buf->start = MALLOC(buf->len); 
  memset(buf->start, 0, buf->len);
  buf->cur = buf->start;
}

static void
file_init(File *file, int binary, FILE* fp) {
  memset(file, 0, sizeof(File));
  file->binary = binary;
  file->fp = fp;
  if (binary) {
    init_buf(&file->tables, 1024);
    init_buf(&file->strings, 1024);
  }
  vec_clear(&file->relocations);
  vec_clear(&file->entries);
  vec_clear(&file->str_relocations);
}

static void
make_room_in_buf(Buf *buf, size_t size) {
  while (buf->cur + size > buf->start + buf->len) {
    int cur = buf->cur - buf->start;
    buf->len = buf->len*2 + 1;
    buf->start = REALLOC(buf->start, buf->len);
    buf->cur = buf->start + cur;
    memset(buf->cur, 0, buf->len - (buf->cur - buf->start));
  }
}

static void
new_offset(File *fp, char *name) {
  OffsetEntry *entry = MALLOC(sizeof(OffsetEntry));
  memset(entry, 0, sizeof(OffsetEntry));
  entry->name = name;
  entry->offset = fp->tables.cur - fp->tables.start;
  entry->len = strlen(name);
  set_add_fn(&fp->entries, entry, &offset_fns);
}

static uintptr_t
make_string(File *fp, char *name) {
  intptr_t size = strlen(name)+1;
  Buf *buf = &fp->strings;
  char *dest;
  make_room_in_buf(buf, size);
  dest = buf->cur;
  strcpy(dest, name);
  buf->cur += size;
  return dest - buf->start;
}

static OffsetEntry *
search_for_offset(File *fp, char *name) {
  uint32 tt = strhashl(name, strlen(name));
  OffsetEntrySet *v = &fp->entries;
  int j, n = v->n;
  uint i;
  if (n) {
    uint h = tt % n;
    for (i = h, j = 0; 
	 i < v->n && j < SET_MAX_SEQUENTIAL; 
	 i = ((i + 1) % n), j++) 
    {
      if (!v->v[i]) {
	assert(0);
	return 0;
      } else {
	if (!strcmp(v->v[i]->name, name))
	  return v->v[i];
      }
    }
  }
  assert(0);
  return 0;
}

static OffsetEntry *
get_offset(File *fp, char* name, ...) {
  int n;
  char buf[256];
  va_list ap;
  va_start(ap, name);
  n = vsnprintf(buf, sizeof(buf), name, ap);
  va_end(ap);
  assert(n < 256 && n >= 0);
  return search_for_offset(fp, buf);
}

static char*
make_name(char* fmt, ...) {
  int n;
  char *h_buf, buf[256];
  va_list ap;
  va_start(ap, fmt);
  n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  assert(n < 256 && n >= 0);
  h_buf = MALLOC(n+1);
  strcpy(h_buf, buf);
  return h_buf;
}

static void
print_no_comma(File *fp, char *str) {
  if (!fp->binary) {
    fprintf(fp->fp, "%s", str);
    fp->first_member = 1;
  }
}

static void 
print(File *fp, char *str) {
  if (!fp->binary) {
    fprintf(fp->fp, "%s", str);
  }
}

/* 
struct:
  start_struct
  repeat: add_struct_X_member
  end_struct

array of primitives:
  start_array
  repeat: add_array_member or add_array_ptr_member
  end_array

array of structs:
  start_array
  repeat:
    start_struct_in_array
    repeat: add_struct_X_member
    end_struct_in_array
  end_array
*/

#define start_struct(file, type, name, whitespace) start_struct_fn(file, sizeof(type), #type, name, whitespace)
#define start_array(file, type, name, length_str, length_data, whitespace) \
   start_array_fn(file, sizeof(type), "", #type, name, length_str, length_data, whitespace)
#define add_struct_member(file, struct_type, format, data, member_name) do { \
  if ((file)->binary) { \
    ((struct_type*)((file)->tables.cur))->member_name = data; \
  } else { \
     handle_comma(file); \
     fprintf((file)->fp, #format, data); \
  }} while(0)
#define add_struct_const_member(file, struct_type, string, value, member_name) do { \
  if ((file)->binary) { \
     ((struct_type*)((file)->tables.cur))->member_name = value; \
  } else { \
     handle_comma(file); \
     fprintf((file)->fp, string); \
  }} while(0)
#define add_struct_str_member(file, struct_type, string, member_name) \
  add_struct_str_member_fn(file, (char**)&((struct_type*)(file)->tables.cur)->member_name, string)
#define add_struct_ptr_member(file, struct_type, ampersand, entry, member_name) \
  add_struct_ptr_member_fn(file, (void**)&((struct_type*)(file)->tables.cur)->member_name, entry, ampersand "%s")
#define add_array_member(file, type, format, data, last) do {\
if ((file)->binary) {\
    add_array_member_internal(file);\
    *((type*)((file)->tables.cur)) = data;\
    (file)->tables.cur += (file)->elem_size;\
  } else {\
    fprintf((file)->fp, #format "%s", data, last ? "" : ",");\
  }} while(0)
#define add_array_ptr_member(file, type, ampersand, entry, last) \
  add_array_ptr_member_fn(file, entry, ampersand "%s%s", last)
#define end_struct(file, type, whitespace) end_struct_fn(file, sizeof(type), whitespace)

static void 
add_array_member_internal(File *fp) {
  fp->n_elems++;
  make_room_in_buf(&fp->tables, fp->elem_size);
}

static void 
handle_comma(File *file) {
  if (!file->first_member)
    fprintf(file->fp, ", ");
  file->first_member = 0;
}

static void
start_array_internal(File *fp, int length, int size) {
  fp->array_length = length;
  fp->n_elems = 0;
  fp->elem_size = size;
}

static void 
start_struct_fn(File *fp, int size, char *type_str, char *name, char *whitespace) {
  new_offset(fp, name);
  fp->first_member = 1;
  if (fp->binary) {
    make_room_in_buf(&fp->tables, size);
  } else {
    fprintf(fp->fp, "%s %s = {%s", type_str, name, whitespace);
  }
}

static void 
start_struct_in_array(File *fp) {
  fp->first_member = 1;
  if (fp->binary) {
    add_array_member_internal(fp);
  } else {
    fprintf(fp->fp, "{");
  }
}

static void 
start_array_fn(File *fp, int type_size, char *type_prefix, char *type_str, 
		    char *name, char *length_str, int length_data, char *whitespace) {
  new_offset(fp, name);
  if (fp->binary) {
    start_array_internal(fp, length_data, type_size);
  } else {
    if (length_data == 0)
      fprintf(fp->fp, "%s%s %s[] = {%s", type_prefix, type_str, name, whitespace);
    else if(strlen(length_str) == 0)
      fprintf(fp->fp, "%s%s %s[%d] = {%s", type_prefix, type_str, name, length_data, whitespace);
    else 
      fprintf(fp->fp, "%s%s %s[%s] = {%s", type_prefix, type_str, name, length_str, whitespace);
  }
}

static void 
add_struct_str_member_fn(File *fp, char **dest, char *str) {
  if (fp->binary) {
    *dest = (char*)make_string(fp, str);
    vec_add(&fp->str_relocations, (void*)((char*)dest - fp->tables.start));
  } else {
    if (!fp->first_member)
      fprintf(fp->fp, ", ");
    fprintf(fp->fp, "\"%s\"", str);
  }
  fp->first_member = 0;
}

static void
add_struct_ptr_member_fn(File *fp, void **dest, OffsetEntry *oe, char *format) {
  if (fp->binary) {
    *dest = (void*)(uintptr_t)oe->offset;
    vec_add(&fp->relocations, (void*)((char*)dest - fp->tables.start));
  } else {
    if (*format == '&' && strcmp(oe->name, "NULL") == 0)
      format++;
    if (!fp->first_member)
      fprintf(fp->fp, ", ");
    fprintf(fp->fp, format, oe->name);
  }
  fp->first_member = 0;
}

static void
add_array_ptr_member_fn(File *fp, OffsetEntry *oe, char *format, int last) {
  if (fp->binary) {
    add_array_member_internal(fp);
    *(void**)fp->tables.cur = (void*)oe->offset;
    vec_add(&fp->relocations, (void*)(fp->tables.cur - fp->tables.start));
    fp->tables.cur += fp->elem_size;
  } else {
    if (*format == '&' && strcmp(oe->name, "NULL") == 0)
      format++;
    fprintf(fp->fp, format, oe->name, last ? "" : ",");
  }
}

typedef void (*CopyFuncType)(void*,int);
static void
add_array_member_fn(File *file, CopyFuncType copy, char *format, unsigned int data, int last) {
  if (file->binary) {
    add_array_member_internal(file);
    copy((void*)(file->tables.cur), data);
    file->tables.cur += file->elem_size;
  } else {
    fprintf((file)->fp, format, data);
    if(!last)
      fprintf(file->fp, ", ");
  }
}

static void 
end_struct_fn(File *fp, int size, char *whitespace) {
  if (fp->binary) {
    fp->tables.cur += size;
  } else {
    fprintf(fp->fp, "};%s", whitespace);
  }
}

static void
end_struct_in_array(File *fp, char *last) {
  if (fp->binary) {
    fp->tables.cur += fp->elem_size;
  } else {
    fprintf(fp->fp,"}%s", last);
  }
}

static void
end_array(File *fp, char *whitespace) {
  if (fp->binary) {
    if (fp->array_length != 0) {
      int remaining = (fp->array_length - fp->n_elems)*fp->elem_size;
      if (remaining) {
	make_room_in_buf(&fp->tables, remaining);
	memset(fp->tables.cur, 0, remaining);
	fp->tables.cur += remaining;
      }
    }
  } else {
    fprintf(fp->fp, "};%s", whitespace);
  }
}

typedef struct ScannerBlock { 
  int state_index; 
  int scanner_index; 
  int block_index; 
  ScanState **chars; 
  ScanStateTransition **transitions; 
} ScannerBlock;
typedef Vec(ScannerBlock*)	VecScannerBlock;
typedef Vec(State *)		VecState;

static int
scanner_size(State *s) {
  if (s->scanner.states.n < 255 && s->scanner.transitions.n < 255)
    return 1;
  if (s->scanner.states.n < 32384 && s->scanner.transitions.n < 32384)
    return 2;
  return 4;
}

#define copy_func(name, type) static void name(void *dest, int data) { (*(type*)(dest)) = (data); }
copy_func(unsigned_char_copy, unsigned char);
copy_func(unsigned_short_copy, unsigned short);
copy_func(unsigned_int_copy, unsigned int);

static CopyFuncType 
get_copy_func(int i) {
  switch (i) {
  case 1: return unsigned_char_copy;
  case 2: return unsigned_short_copy;
  case 4: return unsigned_int_copy;
  default: d_fail("bad case"); return 0;
  }
}

static char *
make_type(int i) {
  switch (i) {
    case 1: return "unsigned char";
    case 2: return "unsigned short";
    case 4: return "unsigned int";
    default: d_fail("bad case"); return "";
  }
}

static char *
scanner_type(State *s) {
  return make_type(scanner_size(s));
}

static char *
make_u_type(int i) {
  switch (i) {
    case 1: return "uint8";
    case 2: return "uint16";
    case 4: return "uint32";
    default: d_fail("bad case"); return "";
  }
}

static char *
scanner_u_type(State *s) {
  return make_u_type(scanner_size(s));
}

static uint32
scanner_block_hash_fn(ScannerBlock *b, hash_fns_t *fns) {
  uint32 hash = 0;
  intptr_t i, block_size = (intptr_t)fns->data[0];
  ScanState **sb = b->chars;

  for (i = 0; i < block_size; i++) {
    hash *= 17;
    hash += sb[i] ? sb[i]->index + 2 : 1;
  }
  return hash;
}

static int
scanner_block_cmp_fn(ScannerBlock *a, ScannerBlock *b, hash_fns_t *fns) {
  intptr_t i, block_size = (intptr_t)fns->data[0];
  ScanState **sa = a->chars;
  ScanState **sb = b->chars;
    
  for (i = 0; i < block_size; i++) {
    if (sa[i] == sb[i])
      continue;
    if (!sa[i] || !sb[i])
      return 1;
    if (sa[i]->index != sb[i]->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
scanner_block_fns = {
  (hash_fn_t)scanner_block_hash_fn,
  (cmp_fn_t)scanner_block_cmp_fn,
  {0, 0}
};

static uint32
trans_scanner_block_hash_fn(ScannerBlock *b, hash_fns_t *fns) {
  uint32 hash = 0;
  intptr_t i, block_size = (intptr_t)fns->data[0];
  ScanStateTransition **sb = b->transitions;

  for (i = 0; i < block_size; i++) {
    hash *= 3;
    hash += sb[i] ? sb[i]->index + 1 : 0;
  }
  return hash;
}

static int
trans_scanner_block_cmp_fn(ScannerBlock *a, ScannerBlock *b, hash_fns_t *fns) {
  intptr_t i, block_size = (intptr_t)fns->data[0];
  ScanStateTransition **sa = a->transitions;
  ScanStateTransition **sb = b->transitions;
    
  for (i = 0; i < block_size; i++) {
    if (sa[i] == sb[i])
      continue;
    if (!sa[i] || !sb[i])
      return 1;
    if (sa[i]->index != sb[i]->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
trans_scanner_block_fns = {
  (hash_fn_t)trans_scanner_block_hash_fn,
  (cmp_fn_t)trans_scanner_block_cmp_fn,
  {0, 0}
};

static uint32
shift_hash_fn(Action *sa, hash_fns_t *fns) {
  return sa->term->index + (sa->kind == ACTION_SHIFT_TRAILING ? 1000000 : 0);
}

static int
shift_cmp_fn(Action *sa, Action *sb, hash_fns_t *fns) {
  return (sa->term->index != sb->term->index) || (sa->kind != sb->kind);
}

hash_fns_t 
shift_fns = {
  (hash_fn_t)shift_hash_fn,
  (cmp_fn_t)shift_cmp_fn,
  {0, 0}
};

static void
write_scanner_data_as_C(File *fp, Grammar *g, char *tag) {
  State *s;
  ScannerBlock *vsblock, *xv, *yv;
  VecScannerBlock scanner_block_hash[4], *pscanner_block_hash;
  VecScannerBlock trans_scanner_block_hash[4], *ptrans_scanner_block_hash;
  VecAction shift_hash;
  int nvsblocks, ivsblock, i, j, k, x, xx;
  VecScanState *ss;
  char speculative_code[256];
  Term *t;

  /* shift_actions */
  for (i = 0; i < g->terminals.n; i++) {
    int action_index = -1;
    t = g->terminals.v[i];
    if (t->regex_production && t->regex_production->rules.v[0]->speculative_code.code) {
      assert(!fp->binary);
      sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
	      t->regex_production->index, t->regex_production->rules.v[0]->index, tag);
    } else {
      strcpy(speculative_code, "NULL");
    }
    if (t->regex_production) {
      action_index = t->regex_production->rules.v[0]->action_index;
    }
    start_struct(fp, D_Shift, make_name("d_shift_%d_%s", i, tag), "");
    add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->index + g->productions.n, symbol);
    add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->scan_kind, shift_kind);
    add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->op_assoc, op_assoc);
    add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->op_priority, op_priority);
    add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->term_priority, term_priority);
    if (fp->binary) {
      add_struct_member(fp, D_Shift, %d, action_index, action_index);
      add_struct_ptr_member(fp, D_Shift, "", &spec_code_entry, speculative_code);
    } else {
      add_struct_member(fp, D_Shift, %d, 0, action_index);
      fprintf(fp->fp, ", %s", speculative_code);
    }
    end_struct(fp, D_Shift, "\n");
    g->write_line++;
    if (g->terminals.v[i]->trailing_context) {
      start_struct(fp, D_Shift, make_name("d_tshift_%d_%s", i, tag), "");
      add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->index + g->productions.n, symbol);
      add_struct_member(fp, D_Shift, %d, D_SCAN_TRAILING, shift_kind);
      add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->op_assoc, op_assoc);
      add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->op_priority, op_priority);
      add_struct_member(fp, D_Shift, %d, g->terminals.v[i]->term_priority, term_priority);
      if (fp->binary) {
	add_struct_member(fp, D_Shift, %d, action_index, action_index);
	add_struct_ptr_member(fp, D_Shift, "", &spec_code_entry, speculative_code);
      } else {
	add_struct_member(fp, D_Shift, %d, 0, action_index);
	fprintf(fp->fp, ", %s", speculative_code);
      }
      end_struct(fp, D_Shift, "\n");
      g->write_line++;
    }
  }
  print(fp, "\n");
  g->write_line++;
  /* scanners */
  nvsblocks = 0;
  for (i = 0; i < g->states.n; i++)
    nvsblocks += g->states.v[i]->scanner.states.n * g->scanner_blocks;
  vsblock = MALLOC((nvsblocks ? nvsblocks : 1) * sizeof(ScannerBlock));
  for (i = 0; i < 4; i++) {
    vec_clear(&scanner_block_hash[i]);
    vec_clear(&trans_scanner_block_hash[i]);
  }
  scanner_block_fns.data[0] = (void*)(uintptr_t)g->scanner_block_size;
  scanner_block_fns.data[1] = (void*)g;
  trans_scanner_block_fns.data[0] = (void*)(uintptr_t)g->scanner_block_size;
  trans_scanner_block_fns.data[1] = (void*)g;
  /* shift */
  vec_clear(&shift_hash);
  ivsblock = 0;
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->same_shifts)
      continue;
    ss = &s->scanner.states;
    /* build accepts differences */
    for (j = 0; j < s->scanner.transitions.n; j++) {
      VecAction *va = &s->scanner.transitions.v[j]->accepts_diff;
      start_array(fp, D_Shift *, make_name("d_accepts_diff_%d_%d_%s", i, j, tag), "", 0, "");
      for (k = 0; k < va->n; k++) {
	if (va->v[k]->kind != ACTION_SHIFT_TRAILING)
	  add_array_ptr_member(fp, D_Shift*, "&", get_offset(fp, "d_shift_%d_%s", va->v[k]->term->index, tag), 0); 
	else
	  add_array_ptr_member(fp, D_Shift*, "&", get_offset(fp, "d_tshift_%d_%s", va->v[k]->term->index, tag), 0);
      }
      add_array_member(fp, D_Shift*, %d, 0, 1);
      end_array(fp, "\n");
      g->write_line += 2;
    }
    if (s->scanner.transitions.n) {
      start_array(fp, D_Shift **, make_name("d_accepts_diff_%d_%s", i, tag), "", 0, "\n");
      for (j = 0; j < s->scanner.transitions.n; j++) {
	add_array_ptr_member(fp, D_Shift **, "", get_offset(fp, "d_accepts_diff_%d_%d_%s", i, j, tag), j == s->scanner.transitions.n - 1);
	print(fp, "\n");
	if (j != s->scanner.transitions.n - 1)
	  g->write_line++;
      }
      end_array(fp, "\n\n");
      g->write_line += 3;
    }
    /* build scanner_block_hash */
    pscanner_block_hash = &scanner_block_hash[scanner_size(s)-1]; 
    ptrans_scanner_block_hash = &trans_scanner_block_hash[scanner_size(s)-1]; 
    for (j = 0; j < ss->n; j++) {
      if (!s->same_shifts) {
	for (k = 0; k < g->scanner_blocks; k++) {
	  vsblock[ivsblock].state_index = s->index;
	  vsblock[ivsblock].scanner_index = j;
	  vsblock[ivsblock].block_index = k;
	  vsblock[ivsblock].chars = 
	    (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	  vsblock[ivsblock].transitions = 
	    (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	  xv = &vsblock[ivsblock];
	  ivsblock++;
	  assert(ivsblock <= nvsblocks);
	  /* output state scanner blocks */
	  yv = set_add_fn(pscanner_block_hash, xv, &scanner_block_fns);
	  if (xv == yv) {
	    int size = scanner_size(s);
	    start_array_fn(fp, size, "", make_type(size), make_name("d_scanner_%d_%d_%d_%s", i, j, k, tag), "SCANNER_BLOCK_SIZE", SCANNER_BLOCK_SIZE, "\n");
	    for (x = 0; x < g->scanner_block_size; x++) {
	      xx = x + k * g->scanner_block_size;
	      add_array_member_fn(fp, get_copy_func(size), "%d", ss->v[j]->chars[xx] ? ss->v[j]->chars[xx]->index + 1 : 0, x == g->scanner_block_size);
	      if (x % 16 == 15) { print(fp, "\n"); /*fprintf(fp, "\n");*/ g->write_line++; }
	    }
	    end_array(fp, "\n\n");
	    g->write_line += 3;
	  }
	  if (s->scan_kind != D_SCAN_LONGEST || s->trailing_context) {
	    /* output accept_diff scanner blocks */
	    yv = set_add_fn(ptrans_scanner_block_hash, xv, 
			    &trans_scanner_block_fns);
	    if (xv == yv) {
	      int size = scanner_size(s);
	      start_array_fn(fp, size, "", make_type(size), make_name("d_accepts_diff_%d_%d_%d_%s", i, j, k, tag), "SCANNER_BLOCK_SIZE", SCANNER_BLOCK_SIZE, "\n");
	      for (x = 0; x < g->scanner_block_size; x++) {
		xx = x + k * g->scanner_block_size;
		add_array_member_fn(fp, get_copy_func(size), "%d", ss->v[j]->transition[xx]->index, x == g->scanner_block_size);
		if (x % 16 == 15) { print(fp, "\n"); g->write_line++; }
	      }
	      end_array(fp, "\n\n");
	      g->write_line += 3;
	    }
	  }
	}
	/* output shifts */
	if (ss->v[j]->accepts.n) {
	  char tmp[256];
	  sprintf(tmp, "d_shift_%d_%d_%s", i, j, tag);
	  for (k = 0; k < ss->v[j]->accepts.n; k++) {
	    Action *a = ss->v[j]->accepts.v[k], *aa;
	    if (ss->v[j]->accepts.n == 1) {
	      a->temp_string = strdup(tmp);
	      aa = set_add_fn(&shift_hash, a, &shift_fns);
	      if (aa != a)
		continue;
	    }
	    /* output shifts */
	    if (!k) 
	      start_array(fp, D_Shift *, make_name(tmp), "", 0, "");
	    if (a->kind != ACTION_SHIFT_TRAILING) {
	      add_array_ptr_member(fp, D_Shift *, "&", get_offset(fp, "d_shift_%d_%s", a->term->index, tag), 0);
	      if (k == ss->v[j]->accepts.n - 1) {
		add_array_ptr_member(fp, D_Shift *, "&", &null_entry, 1);
		end_array(fp, "\n\n");
	      }
	    } else {
	      add_array_ptr_member(fp, D_Shift *, "&", get_offset(fp, "d_tshift_%d_%s", a->term->index, tag), 0); 
	      if (k == ss->v[j]->accepts.n - 1) {
		add_array_ptr_member(fp, D_Shift *, "&", &null_entry, 1);
		end_array(fp, "\n\n");
	      }
	    }
	    if (k == ss->v[j]->accepts.n - 1)
	      g->write_line += 2;
	  }
	}
      }
    }
  }
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    ss = &s->scanner.states;
    ivsblock = 0;
    if (ss->n && !s->same_shifts) {
      /* output scanner state transition tables */
      /* assume SB_uint8, 16, and 32 have same member offsets */
      assert(sizeof(SB_uint8) == sizeof(SB_uint16) && sizeof(SB_uint16) == sizeof(SB_uint32));
      start_array_fn(fp, sizeof(SB_uint8), "SB_", scanner_u_type(s), make_name("d_scanner_%d_%s", i, tag), 
		     "", ss->n, "\n");
      g->write_line += 1;
      pscanner_block_hash = &scanner_block_hash[scanner_size(s)-1]; 
      for (j = 0; j < ss->n; j++) {
	Action *a;
	start_struct_in_array(fp);
	if (ss->v[j]->accepts.n) {
	  a = ss->v[j]->accepts.v[0];
	  if (ss->v[j]->accepts.n == 1) {
	    a = set_add_fn(&shift_hash, a, &shift_fns);
	    add_struct_ptr_member(fp, SB_uint8, "", get_offset(fp, "%s", a->temp_string), shift);
	  } else
	    add_struct_ptr_member(fp, SB_uint8, "", get_offset(fp, "d_shift_%d_%d_%s", i, j, tag), shift);
	} else
	  add_struct_ptr_member(fp, SB_uint8, "", &null_entry, shift);
	print_no_comma(fp, ", {");
	for (k = 0; k < g->scanner_blocks; k++) {
	  ScannerBlock vs;
	  vs.state_index = s->index;
	  vs.scanner_index = j;
	  vs.block_index = k;
	  vs.chars = (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	  vs.transitions = 
	    (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	  xv = &vs;
	  yv = set_add_fn(pscanner_block_hash, xv, &scanner_block_fns);
	  assert(yv != xv);
	  add_struct_ptr_member(fp, SB_uint8, "", get_offset(fp, "d_scanner_%d_%d_%d_%s", yv->state_index, yv->scanner_index, yv->block_index, tag), scanner_block[k]);
	  if (k != g->scanner_blocks-1) {
	    if ((k % 2) == 1) { print(fp, "\n "); g->write_line += 1; }
	  }
	}
	print(fp, "}");
	end_struct_in_array(fp, j != ss->n-1 ? ",\n" : "\n");
	g->write_line += 1;
      }
      end_array(fp, "\n\n");
      g->write_line += 2;
      if (s->scan_kind != D_SCAN_LONGEST || s->trailing_context) {
	/* output scanner accepts diffs tables */
	start_array_fn(fp, sizeof(SB_trans_uint8), "SB_trans_", scanner_u_type(s), 
			make_name("d_transition_%d_%s", i, tag), "", ss->n, "\n");
	g->write_line += 1;
	ptrans_scanner_block_hash = 
	  &trans_scanner_block_hash[scanner_size(s)-1]; 
	for (j = 0; j < ss->n; j++) {
	  start_struct_in_array(fp);
	  print(fp, "{");
	  for (k = 0; k < g->scanner_blocks; k++) {
	    ScannerBlock vs;
	    vs.state_index = s->index;
	    vs.scanner_index = j;
	    vs.block_index = k;
	    vs.chars = (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	    vs.transitions = 
	      (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	    xv = &vs;
	    yv = set_add_fn(ptrans_scanner_block_hash, xv, 
			    &trans_scanner_block_fns);
	    assert(yv != xv);
	    add_struct_ptr_member(fp, SB_trans_uint8, "", 
				    get_offset(fp, "d_accepts_diff_%d_%d_%d_%s", 
					      yv->state_index, yv->scanner_index,
					      yv->block_index, tag), scanner_block[k]);	    
	    if (k != g->scanner_blocks-1) {
	      if ((k % 2) == 1) { print(fp, "\n "); g->write_line += 1; }
	    }
	  }
	  print(fp, "}");
	  end_struct_in_array(fp, j != ss->n-1 ? ",\n" : "\n");
	  g->write_line += 1;
	}
	end_array(fp, "\n\n");
	g->write_line += 2;
      }
    }
  }
  for (i = 0; i < 4; i++)
    vec_free(&scanner_block_hash[i]);
  FREE(vsblock);
}

#define reduction_index(_r) \
((_r)->same_reduction ? (_r)->same_reduction->index : (_r)->index)

static void
write_goto_data_as_C(File *fp, Grammar *g, char *tag) {
  Vec(int) vgoto;
  State *s;
  uint8 *goto_valid = NULL;
  int i, j, x, again, lowest, nvalid_bytes, sym, lowest_sym;

  nvalid_bytes = ((g->productions.n + g->terminals.n) + 7) / 8;
  goto_valid = MALLOC(nvalid_bytes);
  vec_clear(&vgoto);
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->gotos.n) {
      /* check for goto on token */
      for (j = 0; j < s->gotos.n; j++)
	if (s->gotos.v[j]->elem->kind == ELEM_TERM &&
	    s->gotos.v[j]->elem->e.term->kind == TERM_TOKEN)
	  s->goto_on_token = 1;
      /* find lowest goto, set valid bits */
      memset(goto_valid, 0, nvalid_bytes);
      lowest = 0;
      lowest_sym = elem_symbol(g, s->gotos.v[0]->elem);
      SET_BIT(goto_valid, lowest_sym);
      for (j = 1; j < s->gotos.n; j++) {
	sym = elem_symbol(g, s->gotos.v[j]->elem);
	SET_BIT(goto_valid, sym);
	if (sym < lowest_sym) {
	  lowest = j;
	  lowest_sym = sym;
	}
      }
      /* insert into vgoto */
      again = 1;
      while (again) {
	again = 0;
	for (j = 0; j < s->gotos.n; j++) {
	  x = elem_symbol(g, s->gotos.v[j]->elem);
	  x -= lowest_sym;
	  while (vgoto.n <= x)
	    vec_add(&vgoto, 0);
	  if (vgoto.v[x]) {
	    again = 1;
	    /* undo the damage */
	    for (--j;j >= 0;j--) {
	      x = elem_symbol(g, s->gotos.v[j]->elem);
	      x -= lowest_sym;
	      vgoto.v[x] = 0;
	    }
	    lowest_sym--;
	    break;
	  } else
	    vgoto.v[x] = s->gotos.v[j]->state->index + 1;
	}
      }
      s->goto_table_offset = lowest_sym;
      /* valid bits */
      start_array(fp, unsigned char, make_name("d_goto_valid_%d_%s", i, tag), "", 0, "");
      print(fp, "\n");
      g->write_line += 1;
      for (j = 0; j < nvalid_bytes; j++)
	add_array_member(fp, unsigned char, 0x%x, goto_valid[j], j == nvalid_bytes - 1);
      end_array(fp, "\n");
      g->write_line += 1;
    } else
      s->goto_table_offset = -INT_MAX;
    /* reduce_actions */
    if (s->reduce_actions.n) {
      start_array(fp, D_Reduction *, make_name("d_reductions_%d_%s", i, tag), "", 0, "");
      for (j = 0; j < s->reduce_actions.n; j++)
	add_array_ptr_member(fp, D_Reduction *, "&",
				 get_offset(fp, "d_reduction_%d_%s", reduction_index(s->reduce_actions.v[j]->rule), tag), 
				 j == s->reduce_actions.n - 1);
      end_array(fp, "\n");
      g->write_line += 1;
    }
    /* modified_reduce_actions */
    if (s->right_epsilon_hints.n) {
      start_array(fp, D_RightEpsilonHint, make_name("d_right_epsilon_hints_%d_%s", i, tag), "", 0, "");
      for (j = 0; j < s->right_epsilon_hints.n; j++) {
	start_struct_in_array(fp);
	add_struct_member(fp, D_RightEpsilonHint, %d, s->right_epsilon_hints.v[j]->depth, depth);
	add_struct_member(fp, D_RightEpsilonHint, %d, s->right_epsilon_hints.v[j]->state->index, preceeding_state);
	add_struct_ptr_member(fp, D_RightEpsilonHint, "&", get_offset(fp, "d_reduction_%d_%s", reduction_index(s->right_epsilon_hints.v[j]->rule), tag), reduction);
	end_struct_in_array(fp, ((j == s->right_epsilon_hints.n - 1) ? "" : ","));
      }
      end_array(fp, "\n");
      g->write_line += 1;
    }
  }
  /* gotos */
  if (vgoto.n) {
    start_array(fp, unsigned short, make_name("d_gotos_%s", tag), "", vgoto.n, "\n");
    g->write_line += 1;
    for (j = 0; j < vgoto.n; j++) {
      if (vgoto.v[j] < 0 || vgoto.v[j] > 65535)
	d_fail("goto table overflow");
      add_array_member(fp, unsigned short, %d, vgoto.v[j], j == vgoto.n - 1);
      if (j % 16 == 15) {
	print(fp, "\n");
	g->write_line += 1;
      }
    }
    end_array(fp, "\n\n");
    g->write_line += 2;
  } else {
    start_array(fp, unsigned short, make_name("d_gotos_%s", tag), "", 1, "");
    add_array_member(fp, unsigned short, %d, 0, 1);
    end_array(fp, "\n");
    g->write_line += 1;
  }
}

static void
write_scanner_code_as_C(File *file, Grammar *g, char *tag) {
  int i, j, l;
  Action *a;
  State *s;
  FILE *fp = file->fp;

  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    for (j = 0; j < s->shift_actions.n; j++) {
      a = s->shift_actions.v[j];
      if (a->kind == ACTION_SHIFT && a->term->kind == TERM_CODE) {
	if (!s->scanner_code) {
	  s->scanner_code = 1;
	  new_offset(file, make_name("d_scan_code_%d_%s", i, tag));
	  fprintf(fp, "int d_scan_code_%d_%s(char **as, int *col, int *line,"
		  "unsigned short *symbol, int *term_priority,"
		  "unsigned char *op_assoc, int *op_priority) {\n"
		  "  int res;\n",
		  i, tag);
	  g->write_line += 1;
	}
	fprintf(fp, "  if ((res = ");
	l = strlen(a->term->string);
	if (a->term->string[l - 1] == ')') {
	  fwrite(a->term->string, l - 1, 1, fp);
	  fprintf(fp, ", ");
	} else
	  fprintf(fp, "%s(", a->term->string);
	fprintf(fp, "as, col, line, op_assoc, op_priority))) {\n"
		"    *symbol = %d;\n"
		"    *term_priority = %d;\n"
		"    return res;\n"
		"  }\n",
		a->term->index + g->productions.n, 
		a->term->term_priority);
	g->write_line += 1;
      }
    }
    if (s->scanner_code) {
      fprintf(fp, "  return 0;\n}\n\n");
      g->write_line += 3;
    }
  }
}

static int
find_symbol(Grammar *g, char *s, char *e, int kind) {
  while (*s && isspace(*s)) s++;
  if (e > s) {
    if (kind == D_SYMBOL_NTERM) {
      Production *p;
      if ((p = lookup_production(g, s, e-s)))
	return p->index;
    } else if (kind == D_SYMBOL_STRING) {
      int i;
      int found = -1;
      for (i = 0; i < g->terminals.n;i++)
	if (g->terminals.v[i]->kind == TERM_STRING &&
	    ((g->terminals.v[i]->term_name &&
	      strlen(g->terminals.v[i]->term_name) == e-s &&
	      !strncmp(s, g->terminals.v[i]->term_name, e-s)) ||
	     (!g->terminals.v[i]->term_name &&
	      g->terminals.v[i]->string_len == (e-s) &&
	      !strncmp(s, g->terminals.v[i]->string, e-s)))) {
	  if (found > 0) {
	    d_fail("attempt to find symbol for non-unique string '%s'\n",
		   g->terminals.v[i]->string);
	  } else
	    found = i;
	}
      if (found > 0)
	return found + g->productions.n;
    }
  }
  return -1;
}

static void
write_code_as_C(FILE *fp, Grammar *g, Rule *r, char *code,
		char *fname, int line, char *pathname) 
{
  char *c;

  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", line, pathname);
    g->write_line++;
  }
  fprintf(fp, "%s{ ", fname);
  c = code;
  while (*c) {
    if (*c == '\n')
      g->write_line++;
    if (*c == '$') {
      c++;
      if (*c == '#') {
	c++;
	if (isdigit(*c)) {
	  int n = atoi(c);
	  fprintf(fp, "(d_get_number_of_children((D_PN(_children[%d], _offset))))", n);
	  if (n > r->elems.n-1)
	    d_fail("$nXXXX greater than number of children at line %d", line);
	  while (isdigit(*c)) c++;
	} else
	  fprintf(fp, "(_n_children)");
      } else if (*c == 'g') {
	fprintf(fp, "(D_PN(_ps, _offset)->globals)");
	c++;
      } else if (*c == 'n') {
	++c;
	if (isdigit(*c)) {
	  int n = atoi(c);
	  fprintf(fp, "(*(D_PN(_children[%d], _offset)))", n);
	  if (n > r->elems.n-1)
	    d_fail("$nXXXX greater than number of children at line %d", line);
	  while (isdigit(*c)) c++;
	} else 
	  fprintf(fp, "(*(D_PN(_ps, _offset)))");
      } else if (*c == '$') {
	fprintf(fp, "(D_PN(_ps, _offset)->user)");
	c++;
      } else if (isdigit(*c)) {
	int n = atoi(c);
	fprintf(fp, "(D_PN(_children[%d], _offset)->user)", n);
	while (isdigit(*c)) c++;
      } else if (*c == '{') {
	char *e = ++c, *a;
	while (*e && *e != '}' && !isspace(*e)) e++;
	a = e;
	if (isspace(*a)) a++;
	while (*a && *a != '}') a++;
	if (!*a)
	  d_fail("unterminated ${...} at line %d", line);
	if (STREQ(c, e-c, "child")) {
	  char xx[2][4096], *x, *y;
	  int i = 0;
	  *xx[0] = 0; *xx[1] = 0;
	  while (*e != '}') {
	    char *ss = e, *n;
	    x = xx[i];
	    y = xx[!i];
	    while (*e && *e != '}' && *e != ',') e++;
	    if (!*e || ss == e)
	      d_fail("bad ${...} at line %d", line);
	    n = dup_str(ss, e);
	    if (!*y)
	      sprintf(x, "(D_PN(_children[%s], _offset))", n);
	    else
	      sprintf(x, "d_get_child(%s, %s)", y, n);
	    if (*e == ',') e++;
	    if (isspace(*e)) e++;
	    i = !i;
	  }
	  if (!xx[!i])
	    d_fail("empty ${child } at line %d", line);
	  fprintf(fp, "%s", xx[!i]);
	} else if (STREQ(c, e-c, "reject")) {
	  fprintf(fp, " return -1 ");
	} else if (STREQ(c, e-c, "free_below")) {
	  fprintf(fp, " free_D_ParseTreeBelow(_parser, (D_PN(_ps, _offset)))");
	} else if (STREQ(c, e-c, "scope")) {
	  fprintf(fp, "(D_PN(_ps, _offset)->scope)");
	} else if (STREQ(c, e-c, "parser")) {
	  fprintf(fp, "_parser");
	} else if (STREQ(c, e-c, "nterm")) {
	  fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_NTERM));
	} else if (STREQ(c, e-c, "string")) {
	  fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_STRING));
	} else if (STREQ(c, e-c, "pass")) {
	  D_Pass *p = find_pass(g, e, a);
	  if (!p)
	    d_fail("unknown pass '%s' line %d", dup_str(e, a), line);
	  fprintf(fp, "%d", p->index);
	} else
	  d_fail("bad $ escape in code line %u\n", line);
	c = a + 1;
      } else
	d_fail("bad $ escape in code line %u\n", line);
    } else { 
      fputc(*c, fp); 
      c++;
    }
  }
  fprintf(fp, "  return 0;");
  fprintf(fp, "}\n\n");
  g->write_line += 2;
  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", g->write_line, g->write_pathname);
    g->write_line++;
  }
}

static void
write_global_code_as_C(FILE *fp, Grammar *g, char *tag) {
  int i;
  char *c;
  
  for (i = 0; i < g->ncode; i++) {
    if (g->write_line_directives) {
      fprintf(fp, "#line %d \"%s\"\n", g->code[i].line, g->pathname);
      g->write_line++;
    }
    c = g->code[i].code;
    while (*c) {
      if (*c == '\n')
	g->write_line++;
      if (*c == '$') {
	c++;
	if (*c == '{') {
	  char *e = ++c, *a;
	  while (*e && *e != '}' && !isspace(*e)) ++e;
	  a = e;
	  if (isspace(*a)) ++a;
	  while (*a && *a != '}') a++;
	  if (STREQ(c, e-c, "nterm")) {
	    fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_NTERM));
	  } else if (STREQ(c, e-c, "string")) {
	    fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_STRING));
	  } else if (STREQ(c, e-c, "pass")) {
	    D_Pass *p = find_pass(g, e, a);
	    if (!p)
	      d_fail("unknown pass '%s' line %d", dup_str(e, a), g->code[i].line + i);
	    fprintf(fp, "%d", p->index);
	  } else
	    d_fail("bad $ escape in code line %u\n", g->code[i].line + i);
	  c = a + 1;
	}
	else
	  d_fail("bad $ escape in code line %u\n", g->code[i].line + i);
      } else {
	fputc(*c, fp);
	c++;
      }
    }
    fprintf(fp, "\n");
    g->write_line += 1;
  }
  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", g->write_line, g->write_pathname);
    g->write_line++;
  }
}

static char * reduction_args = "(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)";

static void
write_reductions_as_C(File *file, Grammar *g, char *tag) {
  int i, j, k, l, pmax;
  Production *p, *pdefault;
  Rule *r, *rdefault = NULL;
  char final_code[256], speculative_code[256], pass_code[256];
  FILE *fp = file->fp;

  pdefault = lookup_production(g, "_", 1);
  if (pdefault) {
    rdefault = pdefault->rules.v[0];
    fprintf(fp, "int d_speculative_reduction_code_%d_%d_%s%s;\n",
	    rdefault->prod->index, rdefault->index, tag, reduction_args);
    g->write_line += 1;
    fprintf(fp, "int d_final_reduction_code_%d_%d_%s%s;\n",
	    rdefault->prod->index, rdefault->index, tag, reduction_args);
    g->write_line += 1;
    fprintf(fp, "extern D_ReductionCode d_pass_code_%d_%d_%s[];\n",
	    rdefault->prod->index, rdefault->index, tag);
    g->write_line += 1;
    for (i = 0; i < rdefault->pass_code.n; i++) {
      fprintf(fp, "int d_pass_code_%d_%d_%d_%s%s;\n",
	      i, rdefault->prod->index, rdefault->index, tag, reduction_args);
      g->write_line += 1;
    }
  }
  for (i = 0; i < g->productions.n; i++) {
    p = g->productions.v[i];
    for (j = p->rules.n - 1; j >= 0; j--) {
      r = p->rules.v[j];
      for (k = 0; k < j; k++)
	if (r->elems.n == p->rules.v[k]->elems.n &&
	    r->speculative_code.code == p->rules.v[k]->speculative_code.code &&
	    r->final_code.code == p->rules.v[k]->final_code.code &&
	    r->op_priority == p->rules.v[k]->op_priority &&
	    r->op_assoc == p->rules.v[k]->op_assoc &&
	    r->rule_priority == p->rules.v[k]->rule_priority &&
	    r->rule_assoc == p->rules.v[k]->rule_assoc &&
	    r->action_index == p->rules.v[k]->action_index) 
	{
	  if (r->pass_code.n != p->rules.v[k]->pass_code.n)
	    continue;
	  for (l = 0; l < r->pass_code.n; l++) {
	    if (!r->pass_code.v[l] && !p->rules.v[k]->pass_code.v[l])
	      continue;
	    if (!r->pass_code.v[l] || !p->rules.v[k]->pass_code.v[l])
	      goto Lcontinue;
	    if (r->pass_code.v[l]->code != p->rules.v[k]->pass_code.v[l]->code)
	      goto Lcontinue;
	  }
	  r->same_reduction = p->rules.v[k];
	  break;
	Lcontinue:;
	}
    }
    for (j = 0; j < p->rules.n; j++) {
      r = p->rules.v[j];
      if (r->same_reduction)
	continue;
      if (r->speculative_code.code) {
	char fname[256];
	sprintf(fname, "int d_speculative_reduction_code_%d_%d_%s%s ",
		r->prod->index, r->index, tag, reduction_args);
	write_code_as_C(fp, g, r, r->speculative_code.code, fname, r->speculative_code.line, g->pathname);
      }
      if (r->final_code.code) {
	char fname[256];
	sprintf(fname, "int d_final_reduction_code_%d_%d_%s%s ",
		r->prod->index, r->index, tag, reduction_args);
	write_code_as_C(fp, g, r, r->final_code.code, fname, r->final_code.line, g->pathname);
      }
      for (k = 0; k < r->pass_code.n; k++) {
	if (r->pass_code.v[k]) {
	  char fname[256];
	  sprintf(fname, "int d_pass_code_%d_%d_%d_%s%s ",
		  k, r->prod->index, r->index, tag, reduction_args);
	  write_code_as_C(fp, g, r, r->pass_code.v[k]->code, fname, r->pass_code.v[k]->line, g->pathname);
	}
      }
      if (r->speculative_code.code)
	  sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
		  r->prod->index, r->index, tag);
      else if (rdefault && rdefault->speculative_code.code)
	sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
		rdefault->prod->index, rdefault->index, tag);
      else 
	strcpy(speculative_code, "NULL");
      if (r->final_code.code)
	sprintf(final_code, "d_final_reduction_code_%d_%d_%s", r->prod->index, r->index, tag);
      else if (rdefault && rdefault->final_code.code)
	sprintf(final_code, "d_final_reduction_code_%d_%d_%s", 
		rdefault->prod->index, rdefault->index, tag);
      else
	strcpy(final_code, "NULL");
      pmax = r->pass_code.n;
      if (r->pass_code.n || (rdefault && rdefault->pass_code.n)) {
	if (rdefault && rdefault->pass_code.n > pmax)
	  pmax = rdefault->pass_code.n;
	if (!r->pass_code.n)
	  sprintf(pass_code, "d_pass_code_%d_%d_%s", 
		  rdefault->prod->index, rdefault->index, tag);
	else {
	  sprintf(pass_code, "d_pass_code_%d_%d_%s", 
		  r->prod->index, r->index, tag);
	  fprintf(fp, "D_ReductionCode %s[] = {", pass_code);
	  for (k = 0; k < pmax; k++) {
	    if (r->pass_code.n > k && r->pass_code.v[k])
	      fprintf(fp, "d_pass_code_%d_%d_%d_%s%s", k, r->prod->index, r->index, tag,
		      k < pmax-1 ? ", " : "");
	    else
	      if (rdefault && rdefault->pass_code.n > k && rdefault->pass_code.v[k])
		fprintf(fp, "d_pass_code_%d_%d_%d_%s%s", k, rdefault->prod->index, 
			rdefault->index, tag, k < pmax-1 ? ", " : "");
	      else
		fprintf(fp, "NULL%s", k < pmax-1 ? ", " : "");
	  } 
	  fprintf(fp, "};\n\n");
	  g->write_line += 2;
	}
      } else
	strcpy(pass_code, "NULL");
      start_struct(file, D_Reduction, make_name("d_reduction_%d_%s", r->index, tag), "");
      add_struct_member(file, D_Reduction, %d, r->elems.n,       nelements);
      add_struct_member(file, D_Reduction, %d, r->prod->index,   symbol);
      if (file->binary) {
        if (!r->prod->internal && r->action_index >= 0) {
	  add_struct_ptr_member(file, D_Reduction, "", &spec_code_entry,  speculative_code);
	  add_struct_ptr_member(file, D_Reduction, "", &final_code_entry,  final_code);
	} else {
	  add_struct_ptr_member(file, D_Reduction, "", &null_entry,  speculative_code);
	  add_struct_ptr_member(file, D_Reduction, "", &null_entry,  final_code);
	}
      } else {
	fprintf(fp, ", %s", speculative_code);
	fprintf(fp, ", %s", final_code);	
      }
      add_struct_member(file, D_Reduction, %d, r->op_assoc,      op_assoc);
      add_struct_member(file, D_Reduction, %d, r->rule_assoc,    rule_assoc);
      add_struct_member(file, D_Reduction, %d, r->op_priority,   op_priority);
      add_struct_member(file, D_Reduction, %d, r->rule_priority, rule_priority);
      add_struct_member(file, D_Reduction, %d, r->prod->internal ? -1 : r->action_index, action_index);
      add_struct_member(file, D_Reduction, %d, pmax,             npass_code);
      if (file->binary) {
	add_struct_ptr_member(file, D_Reduction, "", &null_entry,  pass_code);	
      } else {
	fprintf(fp, ", %s", pass_code);
      }
      end_struct(file, D_Reduction, "\n");
      g->write_line += 1;
    }
  }
}

static uint32
er_hint_hash_fn(State *a, hash_fns_t *fns) {
  VecHint *sa = &a->error_recovery_hints;
  uint32 hash = 0, i;
  Term *ta;

  for (i = 0; i < sa->n; i++) {
    ta = sa->v[i]->rule->elems.v[sa->v[i]->rule->elems.n - 1]->e.term;
    hash += (sa->v[i]->depth + 1) * 13;
    hash += strhashl(ta->string, ta->string_len);
    if (sa->v[i]->rule)
      hash += sa->v[i]->rule->prod->index * 10007;
  }
  return hash;
}

static int
er_hint_cmp_fn(State *a, State *b, hash_fns_t *fns) {
  int i;
  VecHint *sa = &a->error_recovery_hints, *sb = &b->error_recovery_hints;
  Term *ta, *tb;
  if (sa->n != sb->n)
    return 1;
  for (i = 0; i < sa->n; i++) {
    ta = sa->v[i]->rule->elems.v[sa->v[i]->rule->elems.n - 1]->e.term;
    tb = sb->v[i]->rule->elems.v[sb->v[i]->rule->elems.n - 1]->e.term;
    if (sa->v[i]->depth != sb->v[i]->depth ||
	strcmp(ta->string, tb->string) ||
	sa->v[i]->rule->prod->index != sb->v[i]->rule->prod->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
er_hint_hash_fns = {
  (hash_fn_t)er_hint_hash_fn,
  (cmp_fn_t)er_hint_cmp_fn,
  {0, 0}
};

static void
write_error_data_as_C(File *fp, Grammar *g, VecState *er_hash, char *tag) {
  int i, j;
  State *s;
  Term *t;
  State *h;
  char *ss;

  if (g->states.n) {
    for (i = 0; i < g->states.n; i++) {
      s = g->states.v[i];
      if (s->error_recovery_hints.n) {
	h = set_add_fn(er_hash, s, &er_hint_hash_fns);
	if (h == s) {
	  start_array(fp, D_ErrorRecoveryHint, make_name("d_error_recovery_hints_%d_%s", i, tag), "", 0, "");
	  print(fp, s->error_recovery_hints.n > 1 ? "\n" : "");
	  for (j = 0; j < s->error_recovery_hints.n; j++) {
	    t = s->error_recovery_hints.v[j]->rule->elems.v[
	      s->error_recovery_hints.v[j]->rule->elems.n - 1]->e.term;
	    ss = escape_string(t->string);
	    start_struct_in_array(fp);
	    add_struct_member(fp, D_ErrorRecoveryHint, %d, s->error_recovery_hints.v[j]->depth, depth);
	    add_struct_member(fp, D_ErrorRecoveryHint, %d, s->error_recovery_hints.v[j]->rule->prod->index, symbol);
	    add_struct_str_member(fp, D_ErrorRecoveryHint, ss, string);
	    end_struct_in_array(fp, j == s->error_recovery_hints.n - 1 ? "" : ",\n");
	    if (j != s->error_recovery_hints.n - 1)
	      g->write_line += 1;
	    FREE(ss);
	  }
	  end_array(fp, "\n");
	  g->write_line += 1;
	}
      }
    }
  }
}
static char *scan_kind_strings[] = {"D_SCAN_ALL", "D_SCAN_LONGEST", "D_SCAN_MIXED",  NULL};

static void
write_state_data_as_C(File *fp, Grammar *g, VecState *er_hash, char *tag) {
  int i;
  State *s, *h, *shifts;
  
  print(fp, "\n");
  start_array(fp, D_State, make_name("d_states_%s", tag), "", 0, "");  
  if (g->states.n) {
    print(fp, "\n");
    for (i = 0; i < g->states.n; i++) {
      s = g->states.v[i];
      shifts = s->same_shifts ? s->same_shifts : s;
      start_struct_in_array(fp); 
      if (s->gotos.n)
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_goto_valid_%d_%s", i, tag), goto_valid);	
      else
	add_struct_ptr_member(fp, D_State, "", &null_entry, goto_valid);	
      add_struct_member(fp, D_State, %d, s->goto_table_offset, goto_table_offset);      
      print_no_comma(fp, ", {");
      if (s->reduce_actions.n) {
	add_struct_member(fp, D_State, %d, s->reduce_actions.n, reductions.n);
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_reductions_%d_%s", i, tag), reductions.v);
      } else {
	add_struct_member(fp, D_State, %d, 0, reductions.n);
	add_struct_ptr_member(fp, D_State, "", &null_entry, reductions.v);
      }
      print(fp, "}, ");
      print_no_comma(fp, "{");
      if (s->right_epsilon_hints.n) {
	add_struct_member(fp, D_State, %d, s->right_epsilon_hints.n, right_epsilon_hints.n);
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_right_epsilon_hints_%d_%s", i, tag), right_epsilon_hints.v);
      } else {
	add_struct_member(fp, D_State, %d, 0, right_epsilon_hints.n);
	add_struct_ptr_member(fp, D_State, "", &null_entry, right_epsilon_hints.v);
      }
      print(fp, "}, ");
      print_no_comma(fp, "{");
      if (s->error_recovery_hints.n) {
	h = set_add_fn(er_hash, s, &er_hint_hash_fns);
	add_struct_member(fp, D_State, %d, s->error_recovery_hints.n, error_recovery_hints.n);
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_error_recovery_hints_%d_%s", h->index, tag), error_recovery_hints.v);
      } else {
	add_struct_member(fp, D_State, %d, 0, error_recovery_hints.n);
	add_struct_ptr_member(fp, D_State, "", &null_entry, error_recovery_hints.v);
      }
      print(fp, "}");
      if (s->shift_actions.n || s->scanner_code || (g->scanner.code && s->goto_on_token))
	add_struct_member(fp, D_State, %d, 1, shifts);
      else
	add_struct_member(fp, D_State, %d, 0, shifts);
      if (g->scanner.code) {
	if (s->goto_on_token) {
	  assert(!fp->binary);
	  fprintf(fp->fp, ", %s", g->scanner.code);
	} else {
	  add_struct_ptr_member(fp, D_State, "", &null_entry, scanner_code);
	}
      } else if (s->scanner_code)
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_scan_code_%d_%s", i, tag), scanner_code);
      else
	add_struct_ptr_member(fp, D_State, "", &null_entry, scanner_code);
      if (s->scanner.states.n) {
	print_no_comma(fp, ", (void*)");
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_scanner_%d_%s", shifts->index, tag), scanner_table);
      } else {
	add_struct_ptr_member(fp, D_State, "", &null_entry, scanner_table);
      }
      if (!fp->binary)
	fprintf(fp->fp, ", sizeof(%s) ", scanner_type(s));
      else
	add_struct_member(fp, D_State, %d, scanner_size(s), scanner_size);
      add_struct_member(fp, D_State, %d, s->accept ? 1 : 0, accept);
      add_struct_const_member(fp, D_State, scan_kind_strings[s->scan_kind], s->scan_kind, scan_kind);
      if ((shifts->scan_kind != D_SCAN_LONGEST || shifts->trailing_context)
	  && shifts->scanner.states.n) {
	print_no_comma(fp, ", (void*)");
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_transition_%d_%s", shifts->index, tag), transition_table);
      } else {
	add_struct_ptr_member(fp, D_State, "", &null_entry, transition_table);
      }
      if ((shifts->scan_kind != D_SCAN_LONGEST || shifts->trailing_context)
	  && shifts->scanner.states.n)
	add_struct_ptr_member(fp, D_State, "", get_offset(fp, "d_accepts_diff_%d_%s", shifts->index, tag), accepts_diff); 
      else
	add_struct_ptr_member(fp, D_State, "", &null_entry, accepts_diff);
      if (s->reduces_to)
	add_struct_member(fp, D_State, %d, s->reduces_to->index, reduces_to);
      else
	add_struct_member(fp, D_State, %d, -1, reduces_to);
      end_struct_in_array(fp, (i == g->states.n - 1 ? "\n" : ",\n"));
    }
    end_array(fp, "\n\n");
  } else {
    if (!fp->binary)  /*BS it doesn't look like this fits a D_State?*/
      fprintf(fp->fp, "{{0, {0, NULL}, 0, NULL}};\n\n");
    else
      d_fail("no states\n");
  }
}

static int
write_header_as_C(Grammar *g, char *base_pathname, char *tag) {
  char pathname[FILENAME_MAX];
  char ver[30];
  int i, tokens = 0, states = 0, col;
  FILE *hfp;

  for (i = 0; i < g->terminals.n; i++)
    if (g->terminals.v[i]->kind == TERM_TOKEN)
      tokens = 1;
  if (g->states_for_all_nterms)
    states = 1;
  else
    for (i = 0; i < g->productions.n; i++)
      if (state_for_declaration(g, i))
	states = 1;
  if (g->write_header > 0 || (g->write_header < 0 && (tokens || states))) {
    strcpy(pathname, base_pathname);
    strcat(pathname, ".d_parser.h");
    hfp = fopen(pathname, "w");
    if (!hfp)
      d_fail("unable to open `%s` for write\n", pathname);
    d_version(ver);
    fprintf(hfp, "/*\n  Generated by Make DParser Version %s\n", ver);  
    fprintf(hfp, "  Available at http://dparser.sf.net\n*/\n\n");  
    fprintf(hfp, "#ifndef _%s_h\n", tag);
    fprintf(hfp, "#define _%s_h\n", tag);
    if (tokens) {
      if (!g->token_type) {
	for (i = 0; i < g->terminals.n; i++)
	  if (g->terminals.v[i]->kind == TERM_TOKEN)
	    fprintf(hfp, "#define %s \t%d\n",
		    g->terminals.v[i]->string,
		    g->terminals.v[i]->index + g->productions.n);
      } else {
	fprintf(hfp, "enum D_Tokens_%s {\n", tag);
	col = 0;
	for (i = 0; i < g->terminals.n; i++) {
	  if (g->terminals.v[i]->kind == TERM_TOKEN) {
	    col += g->terminals.v[i]->string_len + 7;
	    if (col > 70) { printf("\n"); col = 0; }
	    fprintf(hfp, "%s = %d%s",
		    g->terminals.v[i]->string,
		    g->terminals.v[i]->index + g->productions.n,
		    i == g->terminals.n-1 ? "" : ", ");
	  }
	}
	fprintf(hfp, "\n};\n");
      }
    }
    if (states) {
      for (i = 0; i < g->productions.n; i++)
	if (!g->productions.v[i]->internal && g->productions.v[i]->elem)
	  fprintf(hfp, "#define D_START_STATE_%s \t%d\n",
		  g->productions.v[i]->name, g->productions.v[i]->state->index);
    }
    fprintf(hfp, "#endif\n");
    fclose(hfp);
    return 1;
  }
  return 0;
}

#define is_EBNF(_x) (_x == INTERNAL_CONDITIONAL || _x == INTERNAL_STAR || _x == INTERNAL_PLUS)
static char *d_internal[] = {"D_SYMBOL_NTERM", "D_SYMBOL_EBNF", "D_SYMBOL_INTERNAL"};
static int d_internal_values[] = {D_SYMBOL_NTERM, D_SYMBOL_EBNF, D_SYMBOL_INTERNAL};
static char *d_symbol[] = { 
  "D_SYMBOL_STRING", "D_SYMBOL_REGEX", "D_SYMBOL_CODE", "D_SYMBOL_TOKEN" };
static int d_symbol_values[] = { 
  D_SYMBOL_STRING, D_SYMBOL_REGEX, D_SYMBOL_CODE, D_SYMBOL_TOKEN };
static void
write_symbol_data_as_C(File *fp, Grammar *g, char *tag) {
  int i;
  start_array(fp, D_Symbol, make_name("d_symbols_%s", tag), "", 0, "\n");
  g->write_line += 1;
  for (i = 0; i < g->productions.n; i++) {
    int state = -1, internal_index;
    if (!g->productions.v[i]->internal && g->productions.v[i]->elem)
      state = g->productions.v[i]->state->index;
    start_struct_in_array(fp);
    internal_index = g->productions.v[i]->internal ? (is_EBNF(g->productions.v[i]->internal) ? 2 : 1) : 0;
    add_struct_const_member(fp, D_Symbol, d_internal[internal_index], d_internal_values[internal_index], kind);
    add_struct_str_member(fp, D_Symbol, g->productions.v[i]->name, name);
    add_struct_member(fp, D_Symbol, %d, g->productions.v[i]->name_len, name_len);
    add_struct_member(fp, D_Symbol, %d, state, start_symbol);
    end_struct_in_array(fp, ",\n");
    g->write_line += 1;
  }
  for (i = 0; i < g->terminals.n; i++) {
    char *s = escape_string(g->terminals.v[i]->string); /* so it is a string */
    char *ss = escape_string(s);   /* so it is printable */
    char *name = g->terminals.v[i]->term_name ? g->terminals.v[i]->term_name : (fp->binary ? s : ss);
    int symbol_index = g->terminals.v[i]->kind;
    start_struct_in_array(fp);
    add_struct_const_member(fp, D_Symbol, d_symbol[symbol_index], d_symbol_values[symbol_index], kind);
    add_struct_str_member(fp, D_Symbol, name, name);
    add_struct_member(fp, D_Symbol, %d, (int) strlen(name), name_len);  /*BS strlen doesn't always works here, length can change when quoted string is compiled*/
    end_struct_in_array(fp, ",\n");
    g->write_line += 1;
    FREE(s);
    FREE(ss);
  }
  end_array(fp, "\n\n");
  g->write_line += 2;
}

static void
write_passes_as_C(File *fp, Grammar *g, char *tag) {
  int i;
  if (g->passes.n) {
    start_array(fp, D_Pass, make_name("d_passes_%s", tag), "", 0, "");
    g->write_line += 1;
    for (i = 0; i < g->passes.n; i++) {
      D_Pass *p = g->passes.v[i];
      start_struct_in_array(fp);
      add_struct_str_member(fp, D_Pass, p->name, name);
      add_struct_member(fp, D_Pass, %d, p->name_len, name_len);
      add_struct_member(fp, D_Pass, 0x%X, p->kind, kind);
      add_struct_member(fp, D_Pass, %d, p->index, index); 
      end_struct_in_array(fp, i < g->passes.n-1 ? ", " : "");
    }
    end_array(fp, "\n\n");
    g->write_line += 2;
  }
}

void
write_parser_tables_as_C(Grammar *g, char *base_pathname, char *tag, int binary) {
  char pathname[FILENAME_MAX];
  char ver[30];
  FILE *fp;
  File file;
  int whitespace_production = 0, header;
  VecState er_hash;
  Production *p;
  vec_clear(&er_hash);

  strcpy(pathname, base_pathname);
  strcat(pathname, ".d_parser.");
  strcat(pathname, g->write_extension);
  g->write_pathname = pathname;
  if (binary) {
    fp = 0;
  } else {
    fp = fopen(pathname, "w");
    if (!fp)
      d_fail("unable to open `%s` for write\n", pathname);
  }

  file_init(&file, binary, fp);

  if (!binary) {
    header = write_header_as_C(g, base_pathname, tag);
    d_version(ver);
    fprintf(fp, "/*\n  Generated by Make DParser Version %s\n", ver);  
    fprintf(fp, "  Available at http://dparser.sf.net\n*/\n\n");  
    g->write_line = 7;
    write_global_code_as_C(fp, g, tag);
    fprintf(fp, "#include \"dparse.h\"\n");
    g->write_line++;

    if (header) {
      fprintf(fp, "#include \"%s.d_parser.h\"\n", base_pathname);
      g->write_line++;
    }
    fprintf(fp, "\n");
    g->write_line++;
  }
  write_reductions_as_C(&file, g, tag);
  write_scanner_data_as_C(&file, g, tag);
  if (!binary)
    write_scanner_code_as_C(&file, g, tag);
  write_goto_data_as_C(&file, g, tag);
  write_error_data_as_C(&file, g, &er_hash, tag);
  write_state_data_as_C(&file, g, &er_hash, tag);
  write_symbol_data_as_C(&file, g, tag);
  write_passes_as_C(&file, g, tag);

  if ((p = lookup_production(g, "whitespace", sizeof("whitespace")-1)))
    whitespace_production = p->state->index;

  if (binary) {
    file.d_parser_tables_loc = file.tables.cur - file.tables.start;
  }

  start_struct(&file, D_ParserTables, make_name("parser_tables_%s", tag), "\n");
  add_struct_member(&file, D_ParserTables, %d, g->states.n, nstates);
  add_struct_ptr_member(&file, D_ParserTables, "", get_offset(&file, "d_states_%s", tag), state);
  add_struct_ptr_member(&file, D_ParserTables, "", get_offset(&file, "d_gotos_%s", tag), goto_table);
  add_struct_member(&file, D_ParserTables, %d, whitespace_production, whitespace_state);
  add_struct_member(&file, D_ParserTables, %d, g->productions.n + g->terminals.n, nsymbols);
  add_struct_ptr_member(&file, D_ParserTables, "", get_offset(&file, "d_symbols_%s", tag), symbols);
  if (g->default_white_space) {
    assert(!file.binary);
    fprintf(fp, ", %s", g->default_white_space);
  } else 
    add_struct_ptr_member(&file, D_ParserTables, "", &null_entry, default_white_space);
  add_struct_member(&file, D_ParserTables, %d, g->passes.n, npasses);
  if (g->passes.n)
    add_struct_ptr_member(&file, D_ParserTables, "", get_offset(&file, "d_passes_%s", tag), passes);
  else
    add_struct_ptr_member(&file, D_ParserTables, "", &null_entry, passes);
  if (g->save_parse_tree)
    add_struct_member(&file, D_ParserTables, %d, 1, save_parse_tree);
  else
    add_struct_member(&file, D_ParserTables, %d, 0, save_parse_tree);
  end_struct(&file, D_ParserTables, "\n");

  if (binary) {
    fp = fopen(pathname, "wb");
    if (!fp)
      d_fail("unable to open `%s` for write\n", pathname);
    file.fp = fp;
    save_binary_tables(&file);
  }
  free_tables(&file);
  fclose(fp);
}

int
write_ctables(Grammar *g) {
  g->scanner_block_size = 256/g->scanner_blocks;
  write_parser_tables_as_C(g, g->pathname, 
			   *g->grammar_ident ? g->grammar_ident : NULL, 0);
  return 0;
}

int
write_binary_tables(Grammar *g) {
  g->scanner_block_size = 256/g->scanner_blocks;
  write_parser_tables_as_C(g, g->pathname, 
			   *g->grammar_ident ? g->grammar_ident : NULL, 1);
  return 0;
}
