#ifndef _cdb_H_
#define _cdb_H_

// The Compilation Database contains information from the last
// time the program was compiled.   This includes:
//
//   - type inference information
//   - profiling information
//

class CDB_EntrySet : public gc {
 public:
  int           es_id;
  Vec<int>      cs_ids;
  Vec<char *>   edge_pnode_id;
  Vec<int>      edge_es_id;
  
  CDB_EntrySet(int aes_id) : es_id(aes_id) {}
};
#define forv_CDB_EntrySet(_p, _v) forv_Vec(CDB_EntrySet, _p, _v)

class CDB_CreationSet : public gc {
 public:
  int           cs_id;
  Vec<char *>   edge_pnode_id;
  Vec<int>      edge_es_id;
  
  CDB_CreationSet(int acs_id) : cs_id(acs_id) {}
};
#define forv_CDB_CreationSet(_p, _v) forv_Vec(CDB_CreationSet, _p, _v)


class CDB : public gc {
 public:
  Map<char *, Fun *> funid;
  Map<char *, Sym *> classid;
  Map<int, EntrySet *> esid;
  Map<Sym *, Vec<CDB_CreationSet *> *> cs_info;
};

int write_cdb(FA *fa);
int read_cdb(FA *fa);

#endif
