/* -*-Mode: c++;-*-
 Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "cdb.h"
#include "if1.h"

static char *
cdb_filename(FA *fa) {
  (void) fa;
  return "chapel.cdb";
}

static int
read_cdb_function(FA *fa, FILE *fp) {
  char id[1024];
  int prof_id, nes;
  if (fscanf(fp, "%1023s %d %d", id, &prof_id, &nes) != 3)
    return -1;
  id[1023] = 0;
  Fun *f = fa->cdb->funid.get(id);
  if (!f)
    return 0; // function disappeared
  f->prof_id = prof_id;
  f->prof_ess.clear();
  for (int i = 0; i < nes; i++) {
    int esid;
    if (fscanf(fp, "%d", &esid) != 1)
      return -1;
    f->prof_ess.add(esid);
  }
  return 0;
}

static int
read_cdb_entry_set(FA *fa, FILE *fp) {
  char id[1024];
  int esid, ncsids, nedges;
  if (fscanf(fp, "%1023s %d %d %d", id, &esid, &ncsids, &nedges) != 4)
    return -1;
  id[1023] = 0;
  Fun *f = fa->cdb->funid.get(id);
  if (!f)
    return 0; // function disappeared
  CDB_EntrySet *esinfo = new CDB_EntrySet(esid);
  for (int i = 0; i < ncsids; i++) {
    int csid;
    if (fscanf(fp, "%d", &csid) != 1)
      return -1;
    esinfo->cs_ids.add(csid);
  }
  for (int i = 0; i < nedges; i++) {
    char pnode_id[1024];
    if (fscanf(fp, "%1023s %d", pnode_id, &esid) != 2)
      return -1;
    pnode_id[1023] = 0;
    esinfo->edge_pnode_id.add(if1_cannonicalize_string(fa->pdb->if1, pnode_id));
    esinfo->edge_es_id.add(esid);
  }
  f->es_info.add(esinfo);
  return 0;
}

static int
read_cdb_creation_set(FA *fa, FILE *fp) {
  char id[1024];
  int csid, ncp;
  if (fscanf(fp, "%1023s %d %d", id, &csid, &ncp) != 3)
    return -1;
  id[1023] = 0;
  Sym *s = fa->cdb->classid.get(id);
  if (!s)
    return 0; // function disappeared
  CDB_CreationSet *csinfo = new CDB_CreationSet(csid);
  for (int i = 0; i < ncp; i++) {
    char pnode_id[1024];
    int esid;
    if (fscanf(fp, "%1023s %d", pnode_id, &esid) != 2)
      return -1;
    pnode_id[1023] = 0;
    csinfo->edge_pnode_id.add(if1_cannonicalize_string(fa->pdb->if1, pnode_id));
    csinfo->edge_es_id.add(esid);
  }
  Vec<CDB_CreationSet *> *v = fa->cdb->cs_info.get(s);
  if (!v)
    fa->cdb->cs_info.put(s,(v = new Vec<CDB_CreationSet *>));
  v->add(csinfo);
  return 0;
}

int
read_cdb(FA *fa) {
  char *fn = cdb_filename(fa);
  FILE *fp = fopen(fn, "r");
  if (!fp)
    return -1;
  while (1) {
    int c = fgetc(fp);
    if (!c) break;
    fgetc(fp); // space
    switch (c) {
      default: goto Lerror;
      case 'F' : if (read_cdb_function(fa, fp) < 0) goto Lerror; break;
      case 'E' : if (read_cdb_entry_set(fa, fp) < 0) goto Lerror; break;
      case 'C' : if (read_cdb_creation_set(fa, fp) < 0) goto Lerror; break;
    }
  }
  return 0;
 Lerror:
  fprintf(stderr, "error: reading database '%s'\n", fn);
  return -1;
}

int
write_cdb(FA *fa) {
  (void) fa;
  // use a temporary file and link for atomicity
  return -1;
}
