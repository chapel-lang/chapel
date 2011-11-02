/*GASNet Collective Tuner*/
/*This small program is intended to probe the target machine and choose among the trees
  that GASNet has and pick the best one and write the result back to a file that is 
  later loaded by every invocation of the GASNet program
*/

#include <stdio.h>
#include <stdlib.h>


#include <gasnet.h>
#include <gasnet_tools.h>
#include <gasnet_coll.h>

/*file for writing out XML information*/
#include <myxml/myxml.h>


typedef struct {
  int my_local_thread;
  int mythread;
  gasnet_coll_handle_t *hndl;
  uint8_t *mysrc, *mydest;
  uint8_t *node_src, *node_dst;
  char _pad[GASNETT_CACHE_LINE_BYTES];

} thread_data_t;

#define DEFAULT_PERFORMANCE_ITERS 10
#define PRINT_TIMERS 1
#define VERBOSE_VERIFICATION_OUTPUT 0

/*max_dsize is a variable set in main*/
#define TOTAL_THREADS threads_per_node*gasnet_nodes()

#if 1
#define ERROR_EXIT() gasnet_exit(1)
#else
#define ERROR_EXIT() do {} while(0)
#endif

gasnet_node_t mynode;
gasnet_node_t nodes;
gasnet_image_t threads_per_node;
gasnet_image_t THREADS;

int performance_iters;
size_t max_data_size;


#define SEG_PER_THREAD (max_data_size*2)*THREADS
#define TEST_SEGSZ_EXPR ((SEG_PER_THREAD*threads_per_node))

uint8_t **my_srcs;
uint8_t **my_dsts;
uint8_t **all_srcs;
uint8_t **all_dsts;

char *outputfile = (char*) "./gasnet_coll_tuning_defaults.bin";
char *profile_file = (char*) "./gasnet_coll_profile.bin";

#include <test.h>

#define COLL_BARRIER() do {\
gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);\
gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);\
} while(0);

myxml_node_t *profile_info;

void *thread_main(void *arg) {
  int flag_iter;
  thread_data_t *td = (thread_data_t*) arg;
  myxml_node_t *tuning_root, *temp, *temp2;
  char buffer[100];
  int skip_msg_printed = 0;
#if GASNET_PAR
  int i;
  gasnet_image_t *imagearray = test_malloc(nodes * sizeof(gasnet_image_t));
  for (i=0; i<nodes; ++i) { imagearray[i] = threads_per_node; }
  gasnet_coll_init(imagearray, td->mythread, NULL, 0, 0);
  test_free(imagearray);
#else
  gasnet_coll_init(NULL, td->mythread, NULL, 0, 0);
#endif
  return 0;

#if 0
  COLL_BARRIER();

  tuning_root = myxml_createNode(NULL, (char*) "machine", (char*)"CONFIG", (char*) GASNET_CONFIG_STRING, NULL);

  temp = myxml_createNodeInt(tuning_root, (char*)"num_nodes", (char*)"val", nodes, NULL);  
  temp = myxml_createNodeInt(temp, (char*)"threads_per_node", (char*)"val", threads_per_node, NULL);

  for(flag_iter=0; flag_iter<9; flag_iter++) {
    int flags;
    myxml_node_t *sync_node, *test_root;
    char buffer[8];
    COLL_BARRIER();
    

   

    switch(flag_iter) {
    case 0: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_NOSYNC; break;
    case 1: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_MYSYNC; break;
    case 2: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_ALLSYNC; break;
    case 3: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_NOSYNC; break;
    case 4: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_MYSYNC; break;
    case 5: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_ALLSYNC; break;
    case 6: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_NOSYNC; break;
    case 7: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_MYSYNC; break;
    case 8: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC; break;
    default: continue;
    }
    

    sync_node = myxml_createNode(temp, (char*)"sync_mode", (char*)"val", fill_flag_str(flags, buffer), NULL);
    /*do single addr tests*/
 
    if (!TEST_ALIGNED_SEGMENTS()) {
      if(td->mythread == 0 && !skip_msg_printed)
        MSG0("Skipping SINGLE/SINGLE (unaligned segments)");
    } else if(threads_per_node != 1) {
      if(td->mythread == 0 && !skip_msg_printed)
        MSG0("skipping SINGLE/SINGLE (multiple threads per node)");
    } else {
      /*call the single address (coll single) test routines with testroot*/
      run_SINGLE_tree_tests(td, all_dsts, all_srcs, 0, flags | GASNET_COLL_SINGLE, sync_node);
    }
#endif

    if(threads_per_node == 1) {
      /*call the single address (coll local) test routines with testroot*/
      run_SINGLE_tree_tests(td, all_dsts, all_srcs, 0, flags | GASNET_COLL_LOCAL, sync_node);
    } else {
      if(td->mythread == 0 && !skip_msg_printed) MSG0("skipping SINGLE/LOCAL (multiple threads per node) (test unimplemetned for now)");
    }

    skip_msg_printed = 1;
    /*do multi addr tests*/

#if GASNET_ALIGNED_SEGMENTS /* Why this conditional? -PHH */
    if(threads_per_node > 1)
#endif
    {
      /*call the multi address test (coll single) routines with testroot*/
      run_MULTI_tree_tests(td, all_dsts, all_srcs, 0, flags | GASNET_COLL_SINGLE, sync_node);
      
      /*call the multi address test (coll local) routines with testroot*/
     run_MULTI_tree_tests(td, my_dsts, my_srcs, 0, flags | GASNET_COLL_LOCAL, sync_node);
    }
  }


  
  if(td->mythread==0)
    MSG0("starting dump of tuning data");
  if(td->mythread == 0){ 
    FILE *outstream=fopen(outputfile, "w");
    myxml_printTreeBIN(outstream, tuning_root);
    fclose(outstream);
 /*   myxml_printTreeXML(stdout, tuning_root, " "); */
    fflush(stdout);
    fflush(stdout);
  }
  if(td->mythread==0)
    MSG0("tuning data dumped");
      

  COLL_BARRIER();
  return 0;
#endif
}

#define STRINGS_MATCH(STR_A, STR_B) (strcmp(STR_A, STR_B)==0)

myxml_node_t *load_file(char *filename) {
  FILE *instream;
  myxml_bytestream_t file_content;
  myxml_node_t *ret;
  
  if(!filename) {fprintf(stderr, "filename can't be null!\n"); gasnet_exit(0);}
  instream = fopen(filename, "r");
  
  /*load the tuning file into a bytestream*/
  if(!instream) {fprintf(stderr, "failed to open file: %s!\n", filename); gasnet_exit(0);}
  ret = myxml_loadTreeBIN(instream);
  if(!STRINGS_MATCH(GASNET_CONFIG_STRING, MYXML_ATTRIBUTES(ret)[0].attribute_value)) {
    fprintf(stderr, "WARNING: Profile (%s) loaded successfully but GASNet configurations don't match!\n", filename); 
  }
  return ret;
}

void fill_meta_data(myxml_node_t *root_node) {
  int i;
  if(STRINGS_MATCH(MYXML_TAG(root_node), "threads_per_node")) {
    int curr_tpn = atoi(MYXML_ATTRIBUTES(root_node)[0].attribute_value);
    if( curr_tpn > threads_per_node) {
      threads_per_node = curr_tpn;
    }
  } else if(STRINGS_MATCH(MYXML_TAG(root_node), "size")) {
    int curr_sz = atoi(MYXML_ATTRIBUTES(root_node)[0].attribute_value);
    if(curr_sz > max_data_size) {
      max_data_size = curr_sz;
    }
  }
  for(i=0; i<MYXML_NUM_CHILDREN(root_node); i++) {
    fill_meta_data(MYXML_CHILDREN(root_node)[i]);
  }
  return;
}

int main(int argc, char **argv) {
  int i,j;
  static uint8_t *A, *B;
  thread_data_t *td_arr;
  GASNET_Safe(gasnet_init(&argc, &argv));
  
  performance_iters = DEFAULT_PERFORMANCE_ITERS;
    
  for(i=1; i<argc; i++) {
    if(strcmp("-i", argv[i])==0 || strcmp("-iters", argv[i])==0) {
      performance_iters = atoi(argv[i+1]);
      i++;
    } else if(strcmp("-f", argv[i])==0 || strcmp("-tune-file", argv[i])==0) {
      outputfile = test_malloc(strlen(argv[i+1])+1);
      strcpy(outputfile, argv[i+1]);
      i++;
    } else if(strcmp("-p", argv[i])==0 || strcmp("-profile", argv[i])==0) {
      profile_file = test_malloc(strlen(argv[i+1])+1);
      strcpy(profile_file, argv[i+1]);
      i++;
    } else if(strcmp("-h", argv[i])==0 || strcmp("-help", argv[i])==0) {
      MSG0("usage: %s (-i iters) (-f output file)\n", argv[0]);
      gasnet_exit(0);
    }
  }                    
  
  max_data_size = 0;
  threads_per_node = 0;
  profile_info = load_file(profile_file);
  fill_meta_data(profile_info);
  printf("data loaded: %d %d\n", (int)max_data_size, threads_per_node);
  if(max_data_size <=0) {
    fprintf(stderr, "max data size(%d) should be >=0\n", (int) max_data_size);
    gasnet_exit(1);
  }
  if(threads_per_node<1) {
    fprintf(stderr, "threads_per_node (%d) should be > 0\n", threads_per_node);
    gasnet_exit(1);
  }
    

  if(performance_iters <=0) {
    gasnet_exit(0);
  }
  
  mynode = gasnet_mynode();
  nodes = gasnet_nodes();
  THREADS = nodes * threads_per_node;

  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  /* ?? test_init("testcolltuner",0,"(-i iters) (-f output_file)"); */
  A = TEST_MYSEG();
  B = A+(SEG_PER_THREAD*threads_per_node);
  my_srcs =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  my_dsts =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  all_srcs = (uint8_t**) test_malloc(sizeof(uint8_t*)*THREADS);
  all_dsts = (uint8_t**) test_malloc(sizeof(uint8_t*)*THREADS);
  td_arr = (thread_data_t*) test_malloc(sizeof(thread_data_t)*threads_per_node);
  
  TEST_SET_WAITMODE(threads_per_node);

  for(i=0; i<threads_per_node; i++) {
    my_srcs[i] = A + i*SEG_PER_THREAD;
    my_dsts[i] = B + i*SEG_PER_THREAD;
    td_arr[i].my_local_thread = i;
    td_arr[i].mythread = mynode*threads_per_node+i;
    td_arr[i].mysrc = my_srcs[i];
    td_arr[i].mydest = my_dsts[i];
  }
  
  for(i=0; i<nodes; i++) {
    /*    assert_always(TEST_SEG(i).size >= SEG_PER_THREAD*threads_per_node); */
    for(j=0; j<threads_per_node; j++) {
      all_srcs[i*threads_per_node+j] = (uint8_t*) TEST_SEG(i) + j*SEG_PER_THREAD;
      all_dsts[i*threads_per_node+j] = (uint8_t*) TEST_SEG(i) + SEG_PER_THREAD*threads_per_node + j*SEG_PER_THREAD;
    }
  }
#if GASNET_PAR
  test_createandjoin_pthreads(threads_per_node, &thread_main, td_arr, sizeof(thread_data_t));
#else
  thread_main(&td_arr[0]);
#endif
  
  test_free(td_arr);
  BARRIER();

  gasnet_exit(0);
  return 0;
}
