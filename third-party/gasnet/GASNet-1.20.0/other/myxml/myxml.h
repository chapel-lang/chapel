/*   $Source: /var/local/cvs/gasnet/other/myxml/myxml.h,v $
 *     $Date: 2009/10/22 20:24:53 $
 * $Revision: 1.3 $
 * Description: code to manage xml data
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef __MYXML_H__
#define __MYXML_H__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  MYXML_ROOT_NODE = 0,
  MYXML_LEAF_NODE,
  MYXML_INTER_NODE,
MYXML_NUM_NODE_CLASSES} myxml_node_class_t;

typedef struct myxml_attribute_t_ {
  char *attribute_name;
  char *attribute_value;
} myxml_attribute_t;

struct myxml_node_t_{
  struct myxml_node_t_ *parent;
  struct myxml_node_t_ **children;
  int num_children;
  myxml_node_class_t nodeclass;
  
  char *tag;
  
  myxml_attribute_t *attribute_list;
  int num_attributes;
  
  char* value;
  int id;
};

typedef struct myxml_bytstream_t_ {
  char* bytes;
  size_t offset;
  size_t size;
} myxml_bytestream_t;

typedef struct myxml_node_t_ myxml_node_t;
typedef struct myxml_node_t_ *myxml_nodeptr_t;

myxml_node_t *myxml_createNode_attr_list(myxml_node_t* parent, const char *tag, const char **attribute_list, const char **attribute_vals, int num_attributes, const char *value);
myxml_node_t *myxml_createNode(myxml_node_t* parent, const char *tag, const char *attribute, const char *attribute_val, const char *value);
myxml_node_t *myxml_createNodeInt(myxml_node_t* parent, const char *tag, const char *attribute, int attribute_val, const char *value);

void myxml_addAttribute(myxml_node_t *node, const char *attribute_name, const char *attribute_value);
void myxml_addAttributeInt(myxml_node_t *node, const char *attribute_name, int attribute_value);

void myxml_printTreeXML(FILE *outstream, myxml_node_t *node, const char *whitespace);
void myxml_printTreeBIN(FILE *outstream, myxml_node_t *node);


myxml_bytestream_t myxml_loadFile_into_bytestream(FILE *instream);
myxml_node_t* myxml_loadTreeBIN(FILE *instream);
myxml_node_t* myxml_loadTreeBYTESTREAM(char *bytes, size_t nbytes);


#define MYXML_PARENT(NODEPTR) ((NODEPTR)->parent)
#define MYXML_CHILDREN(NODEPTR) ((NODEPTR)->children)
#define MYXML_NUM_CHILDREN(NODEPTR) ((NODEPTR)->num_children)
#define MYXML_NODEPTRCLASS(NODEPTR) ((NODEPTR)->nodeclass)
#define MYXML_TAG(NODEPTR) ((NODEPTR)->tag)
#define MYXML_NUM_ATTRIBUTES(NODEPTR) ((NODEPTR)->num_attributes)
#define MYXML_ATTRIBUTES(NODEPTR) ((NODEPTR)->attribute_list)
#define MYXML_VALUE(NODEPTR) ((NODEPTR)->value)

#endif
