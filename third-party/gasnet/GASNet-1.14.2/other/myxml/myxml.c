/*   $Source: /var/local/cvs/gasnet/other/myxml/myxml.c,v $
 *     $Date: 2009/10/22 20:24:53 $
 * $Revision: 1.4 $
 * Description: code to manage xml data
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


#define STR_ALLOC_AND_COPY(OUTSTR, INSTR) do {(OUTSTR) = gasneti_malloc(strlen(INSTR)+1); strcpy((OUTSTR), (INSTR));} while(0)

myxml_node_t *myxml_createNode_attr_list(myxml_node_t* parent, const char *tag, const char **attribute_list, const char **attribute_vals, int num_attributes, const char *value) {
  int i,j;
  myxml_node_t *ret=gasneti_calloc(1,sizeof(myxml_node_t));
  ret->parent = parent;
  ret->num_children = 0;
  ret->children = NULL;
  /*make sure that we aren't adding to a leaf or know that this is the root node*/
  if(parent==NULL) {
    ret->nodeclass = MYXML_ROOT_NODE;
  } else if(parent->nodeclass == MYXML_LEAF_NODE) {
    fprintf(stderr, "can't add a child to a leaf node!\n");
    exit(1);
  }

  if(tag==NULL) {
    fprintf(stderr, "tag can't be null!\n");
    exit(1);
  } else {
    STR_ALLOC_AND_COPY(ret->tag, tag); 
  } 
  
  /*this mustbe a leaf node since an explicit value was declared*/
  if(value) {
    STR_ALLOC_AND_COPY(ret->value, value); 
    ret->nodeclass = MYXML_LEAF_NODE;
  } else if(parent!=NULL)  {
    ret->nodeclass = MYXML_INTER_NODE;
  }
  
  ret->attribute_list = gasneti_malloc(sizeof(myxml_attribute_t)*num_attributes);
  
  for(i=0; i<num_attributes; i++) {
    STR_ALLOC_AND_COPY(ret->attribute_list[i].attribute_name, attribute_list[i]);
    STR_ALLOC_AND_COPY(ret->attribute_list[i].attribute_value, attribute_vals[i]);
  }
  
  /*add myself to my parents children list*/
  if(parent) {
    parent->num_children++;
    if(parent->children) {
      parent->children = gasneti_realloc(parent->children,parent->num_children*sizeof(myxml_node_t*));
    } else {
      parent->children = gasneti_malloc(parent->num_children*sizeof(myxml_node_t*));
    }
    parent->children[parent->num_children-1] = ret;
  }
  
  return ret;
}


void myxml_addAttribute(myxml_node_t *node, const char *attribute_name, const char *attribute_value) {

  node->attribute_list = gasneti_realloc(node->attribute_list, sizeof(myxml_attribute_t)*(node->num_attributes+1));
  /*if adding an attribute name and value can't be NULL*/
  if(attribute_name == NULL || attribute_value == NULL) {
    fprintf(stderr, "myxml error: attribute_name and attribute value must be non null when adding new attribute!\n");
    exit(1);
  }
  STR_ALLOC_AND_COPY(node->attribute_list[node->num_attributes].attribute_name, attribute_name);
  STR_ALLOC_AND_COPY(node->attribute_list[node->num_attributes].attribute_value, attribute_value);
  node->num_attributes++;
}

void myxml_addAttributeInt(myxml_node_t *node, const char *attribute_name, int attribute_value) {
  char buffer[50];
   if(attribute_name == NULL ) {
    fprintf(stderr, "myxml error: attribute_name must be non null when adding new attribute!\n");
    exit(1);
  }
  sprintf(buffer, "%d", attribute_value);
  node->attribute_list = gasneti_realloc(node->attribute_list, sizeof(myxml_attribute_t)*(node->num_attributes+1));
  STR_ALLOC_AND_COPY(node->attribute_list[node->num_attributes].attribute_name, attribute_name);
  STR_ALLOC_AND_COPY(node->attribute_list[node->num_attributes].attribute_value, buffer);
  node->num_attributes++;
}

myxml_node_t *myxml_createNode(myxml_node_t* parent, const char *tag, const char *attribute, const char *attribute_val, const char *value) {
  myxml_node_t *ret = myxml_createNode_attr_list(parent, tag, NULL, NULL, 0, value);
  if(attribute!=NULL && attribute_val!=NULL) myxml_addAttribute(ret, attribute, attribute_val);
  return ret;
}

myxml_node_t *myxml_createNodeInt(myxml_node_t* parent, const char *tag, const char *attribute, int attribute_val, const char *value) {
  char buffer[100];
  myxml_node_t *ret = myxml_createNode_attr_list(parent, tag, NULL, NULL, 0, value);
  sprintf(buffer, "%d", attribute_val);
  myxml_addAttribute(ret, attribute, buffer);
  return ret;
}

void myxml_destroyTree(myxml_node_t *node) {
  int i,j;
  if(node==NULL) return;
  for(i=0; i<node->num_children; i++) {
    myxml_destroyTree(node->children[i]);
  }
  gasneti_free(node->children);
  for(j=0; j<node->num_attributes; j++) {
    gasneti_free(node->attribute_list[j].attribute_name);
    gasneti_free(node->attribute_list[j].attribute_value);
  }
  gasneti_free(node->attribute_list);
  if(node->tag) gasneti_free(node->tag);
  if(node->value) gasneti_free(node->value);
  gasneti_free(node);
  return;
}

void myxml_printTreeXML_helper(FILE *outstream, myxml_node_t *node, int level, const char *whitespace) {
  int i, l;
  
  for(l=0; l<level; l++) {
    fprintf(outstream, "%s", whitespace);
  } 
  fprintf(outstream, "<%s", node->tag);
  for(i=0; i<node->num_attributes; i++) {
    fprintf(outstream, " %s=\"%s\"", node->attribute_list[i].attribute_name, node->attribute_list[i].attribute_value);
  }
  fprintf(outstream, ">\n");
  if(node->nodeclass == MYXML_LEAF_NODE) {
    for(l=0; l<level+1; l++) {
      fprintf(outstream, "%s", whitespace);
    }
    fprintf(outstream, "%s\n", node->value);
  } else {
    for(i=0; i<node->num_children; i++) {
      myxml_printTreeXML_helper(outstream, node->children[i], level+1, whitespace); 
    }
  }
  
  for(l=0; l<level; l++) {
    fprintf(outstream, "%s", whitespace);
  }
  fprintf(outstream, "</%s>\n", node->tag);
  
}

/*dump the tree in a human readable XML format*/
void myxml_printTreeXML(FILE *outstream, myxml_node_t *node, const char *whitespace) {
  fprintf(outstream, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  if(node != NULL) 
    myxml_printTreeXML_helper(outstream, node, 0, whitespace);
}

#ifndef htonl
#define MYHTONL(IN) IN 
#else
#define MYHTONL(IN) htonl(IN)
#endif

#ifndef ntohl
#define MYNTOHL(IN) IN 
#else
#define MYNTOHL(IN) ntohl(IN)
#endif

#if 0
#define SAFE_READ(PTR, NBYTES, INSTREAM) do {                           \
    size_t __ret;                                                       \
    __ret = fread(PTR, 1, NBYTES, INSTREAM);                            \
    if(__ret != (NBYTES)) {                                             \
      fprintf(stderr, "read error (expected: %d got: %d)", (uint32_t)NBYTES, (uint32_t)__ret); \
      fclose(INSTREAM);                                                 \
      exit(1);                                                          \
    }} while(0)

myxml_node_t* myxml_loadTreeHelper(FILE *instream, myxml_node_t *parent_node) {
  uint32_t temp;
  int i=0;
  myxml_node_t *curr_node = (myxml_node_t*) gasneti_malloc(sizeof(myxml_node_t));
  
  curr_node->parent = parent_node;
  if(parent_node == NULL) {
    curr_node->nodeclass = MYXML_ROOT_NODE;
  } else {
    curr_node->nodeclass = MYXML_INTER_NODE;
  } 
  
  SAFE_READ(&temp, sizeof(uint32_t), instream);
  curr_node->id = MYNTOHL(temp);
    
  SAFE_READ(&temp, sizeof(uint32_t), instream);
  curr_node->num_children = MYNTOHL(temp);
  
  SAFE_READ(&temp, sizeof(uint32_t), instream);
  curr_node->num_attributes = MYNTOHL(temp);

  /*read the tag length and allocate the buffer*/
  SAFE_READ(&temp, sizeof(uint32_t), instream);
  temp = MYNTOHL(temp);
  curr_node->tag = (char*) gasneti_malloc(temp);
  /*read the tag*/
  SAFE_READ(curr_node->tag, temp, instream);
  
  curr_node->attribute_list = (myxml_attribute_t*) gasneti_malloc(sizeof(myxml_attribute_t)*curr_node->num_attributes);
  for(i=0; i<curr_node->num_attributes; i++) {
    /*read the length of the string*/
    SAFE_READ(&temp, sizeof(uint32_t), instream);
    temp = MYNTOHL(temp);
    curr_node->attribute_list[i].attribute_name = gasneti_malloc(temp);
    SAFE_READ(curr_node->attribute_list[i].attribute_name, temp, instream);
    
    
    SAFE_READ(&temp, sizeof(uint32_t), instream);
    temp = MYNTOHL(temp);
    curr_node->attribute_list[i].attribute_value = gasneti_malloc(temp);
    SAFE_READ(curr_node->attribute_list[i].attribute_value, temp, instream);
  }
  /*read the size of the value str*/
  SAFE_READ(&temp, sizeof(uint32_t), instream);
  temp = MYNTOHL(temp);
  
  if(temp > 0) {
    curr_node->value = (char*) gasneti_malloc(temp);
    SAFE_READ(curr_node->value, temp, instream);
    curr_node->nodeclass = MYXML_LEAF_NODE;
  }
  
  
  
  curr_node->children = (myxml_node_t**) gasneti_malloc(sizeof(myxml_node_t*)*curr_node->num_children);
  for(i=0; i<curr_node->num_children; i++) {
    
    curr_node->children[i] = myxml_loadTreeHelper(instream, curr_node);
  }
  
  return curr_node;
  
}



myxml_node_t* myxml_loadTreeBIN(FILE *instream) {
   uint32_t *offset_idx;
   uint32_t num_nodes;
   uint32_t temp;
   int i;
   SAFE_READ(&temp, sizeof(uint32_t), instream);
   num_nodes = MYNTOHL(temp);
   
   
   return myxml_loadTreeHelper(instream, NULL);
}
#endif


#define SAFE_READ_BYTES(PTR, NBYTES, BYTESTREAM) do {\
memcpy(PTR, ((BYTESTREAM)->bytes)+(BYTESTREAM)->offset, NBYTES);\
(BYTESTREAM)->offset+=NBYTES; gasneti_assert_always((BYTESTREAM)->offset<=(BYTESTREAM)->size);\
} while(0)

myxml_node_t* myxml_loadTreeHelper_bytestream(myxml_bytestream_t *instream, myxml_node_t *parent_node) {
  uint32_t temp;
  int i=0;
  myxml_node_t *curr_node = (myxml_node_t*) gasneti_malloc(sizeof(myxml_node_t));
  
  curr_node->parent = parent_node;
  if(parent_node == NULL) {
    curr_node->nodeclass = MYXML_ROOT_NODE;
  } else {
    curr_node->nodeclass = MYXML_INTER_NODE;
  } 
  
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
  curr_node->id = MYNTOHL(temp);
  
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
  curr_node->num_children = MYNTOHL(temp);
  
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
  curr_node->num_attributes = MYNTOHL(temp);
  
  /*read the tag length and allocate the buffer*/
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
  temp = MYNTOHL(temp);
  curr_node->tag = (char*) gasneti_malloc(temp);
  /*read the tag*/
  SAFE_READ_BYTES(curr_node->tag, temp, instream);
  
  curr_node->attribute_list = (myxml_attribute_t*) gasneti_malloc(sizeof(myxml_attribute_t)*curr_node->num_attributes);
  for(i=0; i<curr_node->num_attributes; i++) {
    /*read the length of the string*/
    SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
    temp = MYNTOHL(temp);
    curr_node->attribute_list[i].attribute_name = gasneti_malloc(temp);
    SAFE_READ_BYTES(curr_node->attribute_list[i].attribute_name, temp, instream);
    
    
    SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
    temp = MYNTOHL(temp);
    curr_node->attribute_list[i].attribute_value = gasneti_malloc(temp);
    SAFE_READ_BYTES(curr_node->attribute_list[i].attribute_value, temp, instream);
  }
  /*read the size of the value str*/
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), instream);
  temp = MYNTOHL(temp);
  
  if(temp > 0) {
    curr_node->value = (char*) gasneti_malloc(temp);
    SAFE_READ_BYTES(curr_node->value, temp, instream);
    curr_node->nodeclass = MYXML_LEAF_NODE;
  }
  
  
  
  curr_node->children = (myxml_node_t**) gasneti_malloc(sizeof(myxml_node_t*)*curr_node->num_children);
  for(i=0; i<curr_node->num_children; i++) {
    
    curr_node->children[i] = myxml_loadTreeHelper_bytestream(instream, curr_node);
  }
  
  return curr_node;
  
}



myxml_node_t* myxml_loadTreeBYTESTREAM(char *bytes, size_t nbytes) {
  uint32_t *offset_idx;
  uint32_t num_nodes;
  uint32_t temp;
  int i;
  myxml_bytestream_t bytestream;
  bytestream.bytes = bytes;
  bytestream.size = nbytes;
  bytestream.offset = 0;
  
  SAFE_READ_BYTES(&temp, sizeof(uint32_t), &bytestream);
  num_nodes = MYNTOHL(temp);
  
  return myxml_loadTreeHelper_bytestream(&bytestream, NULL);
}

myxml_bytestream_t myxml_loadFile_into_bytestream(FILE *instream) {
  char *buffer;
  
  myxml_bytestream_t ret;
  
  ret.offset = 0;
  fseek(instream, 0L, SEEK_END);
  ret.size = ftell(instream);
  rewind(instream);
  
  printf("loading %d bytes\n", (int)ret.size);
  ret.bytes = gasneti_calloc(sizeof(char),ret.size);
  
  if(fread(ret.bytes, 1, ret.size, instream)!=ret.size) {
    fprintf(stderr, "error reading input file!\n");
    exit(1);
  }
  return ret;
  
}

myxml_node_t* myxml_loadTreeBIN(FILE *instream) {
  myxml_bytestream_t bytestream = myxml_loadFile_into_bytestream(instream);
  return myxml_loadTreeBYTESTREAM(bytestream.bytes, bytestream.size);
}

uint32_t myxml_countAndLabelNodes(myxml_node_t *node, uint32_t label) {
  int i;
  uint32_t count = 0;
  uint32_t num_children=0;
  node->id = label;
  /*add one for myself*/
  label++;
  
  for(i=0; i<node->num_children; i++) {
    num_children = myxml_countAndLabelNodes(node->children[i], label);
    label+=num_children;
    count += num_children;
  } 
  /*add one for myself*/
  return count+1;
}


#define SAFE_WRITE(PTR, NBYTES, OUTSTREAM) do { \
  size_t __ret;                                    \
  __ret = fwrite(PTR, 1, NBYTES, OUTSTREAM);  \
 if(__ret != (NBYTES)) {                              \
   fprintf(stderr, "write error (expected: %d got: %d)\n", (int)NBYTES, (int)__ret); \
   fclose(OUTSTREAM);                                             \
   exit(1); }} while(0)


#if 0
void myxml_fillSizes(myxml_node_t *node, size_t *sz_arr) {
  int i,j;
  size_t mysz;
  /*first figure out the size of mynode*/
  
  
  mysz = sizeof(uint32_t)*3; /*id, num_children, num_attributes*/
  mysz += sizeof(uint32_t)*node->num_children;
  mysz += sizeof(uint32_t) + strlen(node->tag)+1; /*tag is always non null*/
  
  for(i=0; i<node->num_attributes; i++) {
    mysz += sizeof(uint32_t) + strlen(node->attribute_list[i].attribute_name)+1;
    mysz +=sizeof(uint32_t) +  strlen(node->attribute_list[i].attribute_value)+1;
  }
  mysz+=sizeof(uint32_t) + (node->value!=NULL ? strlen(node->value)+1 : 0); 
  sz_arr[node->id] = mysz;
  
  /*and then recurse through my children*/
  for(i=0; i<node->num_children; i++) {
    myxml_fillSizes(node->children[i], sz_arr);
  }
  return;
  
}
#endif

static void dump_TreeBIN(FILE *outstream, myxml_node_t *node) {
  uint32_t temp;
  int i;

  temp = MYHTONL(node->id);
  SAFE_WRITE(&temp, sizeof(uint32_t), outstream);




  temp = MYHTONL(node->num_children);
  SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
  
  temp = MYHTONL(node->num_attributes);
  SAFE_WRITE(&temp, sizeof(uint32_t), outstream);

  temp = MYHTONL(strlen(node->tag)+1);
  SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
  SAFE_WRITE(node->tag, strlen(node->tag)+1, outstream);
  for(i=0; i<node->num_attributes; i++) {
    temp = MYHTONL(strlen(node->attribute_list[i].attribute_name)+1);
    SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
    SAFE_WRITE(node->attribute_list[i].attribute_name, strlen(node->attribute_list[i].attribute_name)+1, outstream);
    temp = MYHTONL(strlen(node->attribute_list[i].attribute_value)+1);
    SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
    SAFE_WRITE(node->attribute_list[i].attribute_value, strlen(node->attribute_list[i].attribute_value)+1, outstream);
  }

  if(node->value) {
    temp = MYHTONL(strlen(node->value)+1);
    SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
    SAFE_WRITE(node->value, strlen(node->value)+1, outstream);
  } else {
    temp = MYHTONL(0);
    SAFE_WRITE(&temp, sizeof(uint32_t), outstream);
  }
  
  for(i=0; i<node->num_children; i++) {
    dump_TreeBIN(outstream, node->children[i]);
  }
}



/*dump the tree as a Binary file that the other parts of GASNet can later load*/
void myxml_printTreeBIN(FILE *outstream, myxml_node_t *node) {
  /*first go through and count the number of nodes and the get the size of each one*/
  int num_nodes = myxml_countAndLabelNodes(node, 0);
  int i;
  uint32_t temp;
  

  temp = MYHTONL(num_nodes);
  SAFE_WRITE(&temp, sizeof(uint32_t), outstream);

  dump_TreeBIN(outstream, node);

  fprintf(stdout, "tree size: %d nodes\n", num_nodes);
  
}

#undef SAFE_READ
#undef SAFE_WRITE
#undef MYNTOHL
#undef MYHTONL
