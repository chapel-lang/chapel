/******************************************************************************
* This is a benchmark written to emulate the behavior of a typical 
* computational fluid dynamics benchmark.  The physical space being simulated
* is broken into boxes (cubes).  All boxes have the same number of cells.
*
*   build:
*       gcc -DOPEN_MP -fopenmp cfd-mini-c.c -o cfd-mini-c
*           OR
*       gcc cfd-mini-c.c -o cfd-mini-c
*
*   usage:
*       ./cfd-mini-c -b 2 -c 8 -v 1       // 2 blocks, 8^3 cells each, verify on
*       ./cfd-mini-c -b 32 -c 128         // 32 blocks, 128^3 cells each
*
* There are several versions of this benchmark - the original written with
* the use of CHOMBO. This versions here are written in pure c.
*   cfd_explain:    is meant to be easy to read and understand in 
*                   order to explain the algorithm.
*   cfd_*_pa:       uses a 1D data allocation with pointer arithmetic 
*                   in order to achieve the same performance as the 
*                   baseline CHOMBO version.
*
* STATUS: The openMP pragmas are not finalized and there is currently a memory
* bug associated with using them.
*
* authors: Catherine Olschanowsky
*
* Copyright (c) 2014, Colorado State University
* All rights reserved.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>


//#define OPEN_MP
#ifdef OPEN_MP
#include <omp.h>
#endif

typedef double Real;

const int nGhost=2;
const size_t numComp = 5; // Dim(3) + 2
const Real dx = 0.5;
const Real factor1 = 1./12.;
const Real factor2 = 2.;
const Real compMultiplier = 1.;

// these are for testing only (test=1 prints data verify=1 does an internal
// comparison)
int tests = 0;
int verify=0;
int np;

int cfd_mini(int numCell, int numBox, int kernel);
// kernel 0
Real** cfd_explain(int numCell, int numBox,int nGhost);
// kernel 1
Real** cfd_baseline(int numCell, int numBox);

/******************************************************************************
* 1. process command line arguments
* 2. call mini-app function
* 3. exit
*******************************************************************************/
int main(int argc, char **argv) {

  int index;
  int c;

  int numCell = 128;
  int numBox = 32;
  // by default the explain kernel is used
  int kernel = 0;

#ifdef OPEN_MP
  np = omp_get_num_threads();
#else 
  np = 1;
#endif

  // Step 1: process command line arguments
  opterr = 0;
     
  while ((c = getopt (argc, argv, "tvb:c:k:p:")) != -1){

    switch (c)
      {
      case 't':
        tests = 1;
        break;
      case 'v':
        verify = 1;
        break;
      case 'k':
        kernel = atoi(optarg);
        break;
      case 'c':
        numCell = atoi(optarg);
        break;
      case 'b':
        numBox = atoi(optarg);
        break;
      case 'p':
        np = atoi(optarg);
#ifdef OPEN_MP
        omp_set_num_threads(np);
#endif
        break;
      case '?':
        if (optopt == 'c' || optopt == 'b') 
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      } 
  }
  if(optind < (argc)){
    fprintf(stderr,"Command Line Parsing Terminated Prematurely.\n");
    fprintf(stderr,"Check that extra arguments were not passed in.\n");
    abort();
  }
  // print out the parameters
  //printf("numComp    : %d\nnumCell    : %d\nnumBox     : %d\n",
          //(int)numComp,(int)numCell,(int)numBox); 
  printf("nCell=%d nBox=%d\n",numCell,numBox);
  if(tests)
  printf("Resulting Data\n");

  // Step 2: Call the function to do the work
  cfd_mini(numCell, numBox, kernel); 


  return 1;
}

#undef GET_VAL_PTR
#define  GET_VAL_PTR(b,c,z,y,x) (b)+\
                                (c)*full_numCell3+\
                                ((z)+nGhost)*full_numCell2+\
                                ((y)+nGhost)*full_numCell+\
                                ((x)+nGhost) 
                                
#define GET_FACE_VAL_PTR(d,b,c,z,y,x) (b)+\
        (c)*(numCell+((d)==2))*(numCell+((d)==1))*(numCell+((d)==0)) +\
        (z)*(numCell+((d)==1))*(numCell+((d)==0))+\
        (y)*(numCell+((d)==0))+\
        (x)  
/******************************************************************************
* cfd_mini
* Input
* -----
* numCell - the number of cells in a single dimension of a single box
* numBoxes - the number of independent boxes to process
*******************************************************************************/
int cfd_mini(int numCell, int numBox, int kernel){

  Real ** verify_data_base;
  Real ** verify_me;
  if(verify && !kernel){
    fprintf(stderr,"You must specify a kernel to verify\n");
    return 0;
  }
  if(verify || !kernel){
    verify_data_base = cfd_explain(numCell,numBox,nGhost);
  }

  switch (kernel)
  {
    case 1:
      verify_me = cfd_baseline(numCell, numBox);
      break;
  }
  if(verify){
    int full_numCell = numCell+2*nGhost;
    int full_numCell2 = full_numCell*full_numCell;
    int full_numCell3 = full_numCell*full_numCell+full_numCell;
    int totalCells = full_numCell*full_numCell*full_numCell;
    int flux_totalSize = numCell*numCell*(numCell+1);
    int idx,iz,iy,ix;
    Real e = 0.001;
    for(idx=0;idx < numBox;idx++){
      Real* truth = verify_data_base[idx];
      Real* hope = verify_me[idx];
      for(iz=0;iz<numCell;iz++){
        for(iy=0;iy<numCell;iy++){
          for(ix=0;ix<numCell;ix++){
            if(*GET_VAL_PTR(hope,0,iz,iy,ix)>*GET_VAL_PTR(truth,0,iz,iy,ix)+e||
               *GET_VAL_PTR(hope,0,iz,iy,ix)<*GET_VAL_PTR(truth,0,iz,iy,ix)-e){
                 fprintf(stderr,"VERIFICATION FAILURE!!!\n");
                 return 1;
            }
          }
        }
      }
    }
    fprintf(stderr,"Code is verified\n");
  }
  
  return 0;
}


/******************************************************************************
 * defines for cfd_basic code -- this makes for easier reading and
 * debugging
 ******************************************************************************/
#define p_DATA_old(z,y,x) *(GET_VAL_PTR(old_box,0,z,y,x))
#define e_DATA_old(z,y,x) *(GET_VAL_PTR(old_box,1,z,y,x))
#define u_DATA_old(z,y,x) *(GET_VAL_PTR(old_box,2,z,y,x))
#define v_DATA_old(z,y,x) *(GET_VAL_PTR(old_box,3,z,y,x))
#define w_DATA_old(z,y,x) *(GET_VAL_PTR(old_box,4,z,y,x))

#define p_DATA_new(z,y,x) *(GET_VAL_PTR(new_box,0,z,y,x))
#define e_DATA_new(z,y,x) *(GET_VAL_PTR(new_box,1,z,y,x))
#define u_DATA_new(z,y,x) *(GET_VAL_PTR(new_box,2,z,y,x))
#define v_DATA_new(z,y,x) *(GET_VAL_PTR(new_box,3,z,y,x))
#define w_DATA_new(z,y,x) *(GET_VAL_PTR(new_box,4,z,y,x))

#define p_CACHE_x(z,y,x) *(GET_FACE_VAL_PTR(0,g_cache,0,z,y,x))
#define e_CACHE_x(z,y,x) *(GET_FACE_VAL_PTR(0,g_cache,1,z,y,x))
#define u_CACHE_x(z,y,x) *(GET_FACE_VAL_PTR(0,g_cache,2,z,y,x))
#define v_CACHE_x(z,y,x) *(GET_FACE_VAL_PTR(0,g_cache,3,z,y,x))
#define w_CACHE_x(z,y,x) *(GET_FACE_VAL_PTR(0,g_cache,4,z,y,x))

#define p_CACHE_y(z,y,x) *(GET_FACE_VAL_PTR(1,g_cache,0,z,y,x))
#define e_CACHE_y(z,y,x) *(GET_FACE_VAL_PTR(1,g_cache,1,z,y,x))
#define u_CACHE_y(z,y,x) *(GET_FACE_VAL_PTR(1,g_cache,2,z,y,x))
#define v_CACHE_y(z,y,x) *(GET_FACE_VAL_PTR(1,g_cache,3,z,y,x))
#define w_CACHE_y(z,y,x) *(GET_FACE_VAL_PTR(1,g_cache,4,z,y,x))

#define p_CACHE_z(z,y,x) *(GET_FACE_VAL_PTR(2,g_cache,0,z,y,x))
#define e_CACHE_z(z,y,x) *(GET_FACE_VAL_PTR(2,g_cache,1,z,y,x))
#define u_CACHE_z(z,y,x) *(GET_FACE_VAL_PTR(2,g_cache,2,z,y,x))
#define v_CACHE_z(z,y,x) *(GET_FACE_VAL_PTR(2,g_cache,3,z,y,x))
#define w_CACHE_z(z,y,x) *(GET_FACE_VAL_PTR(2,g_cache,4,z,y,x))

/*******************************************************************************
 * The following function is meant to be explainatory code. This implementation
 * is meant purely to explain the control flow and algorithm. Other 
 * implementations include optimizations that sometimes obfuscate the 
 * algorithm.
 * 
 *  Processing the boxes means that we will be reading the data from
 *  old_boxes and writing them to new_boxes 
 *  The following are the equations for this calculation
 * 
 *  There are 5 components: p, e, u, v, w (density, energy, velocity (3D))
 *  Each of these components is represented as a 3D array (initialized
 *  above).
 * p_{t+1}(z,y,x) = h(p_t,z,y,x) + h'(p_t,z,y,x) + h"(p_t,z,y,x)
 * e_{t+1}(z,y,x) = h(e_t,z,y,x) + h'(e_t,z,y,x) + h"(e_t,z,y,x)
 * u_{t+1}(z,y,x) = h(u_t,z,y,x) + h'(u_t,z,y,x) + h"(u_t,z,y,x)
 * v_{t+1}(z,y,x) = h(v_t,z,y,x) + h'(v_t,z,y,x) + h"(v_t,z,y,x)
 * w_{t+1}(z,y,x) = h(w_t,z,y,x) + h'(w_t,z,y,x) + h"(w_t,z,y,x)
 *
 *  Computing face-centered, flux values based on cell-centered values
 *  g(component,z,y,x) is a stencil operation that looks like the following:
 *  g(c,z,y,x) = factor1*
 *        (c[z][y][x-2]+7*(c[z][y][x-1]+c[z][y][x])+c[z][y][x+1])
 *  similarly for g' and g"
 *  g'(c,z,y,x) = factor1*
 *        (c[z][y-2][x]+7*(c[z][y-1][x]+c[z][y][x])+c[z][y+1][x])
 *  g"(c,z,y,x) = factor1*
 *        (c[z-2][y][x]+7*(c[z-1][y][x]+c[z][y][x])+c[z+1][y][x])
 *  
 *  Computing cell-centered values based on face-centered flux values
 *  h(component,z,y,x) is a stencil operation that looks like the following:
 *  h(c,z,y,x)  = factor2*
 *                  (g(c,z,y,x+1)*g(u_t,z,y,x+1)-g(c,z,y,x)*g(u_t,z,y,x))
 *  h'(c,z,y,x) = factor2*
 *                  (g'(c,z,y+1,x)*g'(v_t,z,y+1,x)-g'(c,z,y,x)*g'(v_t,z,y,x))
 *  h"(c,z,y,x) = factor2*
 *                  (g"(c,z+1,y,x)*g"(w_t,z+1,y,x)-g"(c,z,y,x)*g"(w_t,z,y,x))
 * 
 *  in this example code we omit some space and time saving optimizations
 *  in order to make the code easy to learn.  FIXME: are these correct?
 *  Step 1 is to calculate all of the g() values
 *  Step 2 multiplies the values together for the first column in the 
 *         equations above
 *  Step 3 Return to Step 1 for g' and then for g"
 * 
 *  The following is a table describing how the notation above 
 *  maps to the storage in code that we are using below
 *  value      name | variable name
 *  ----------------------------------
 *  p_{t+1}         | new_data[0]
 *  e_{t+1}         | new_data[1]
 *  u_{t+1}         | new_data[2]
 *  v_{t+1}         | new_data[3]
 *  w_{t+1}         | new_data[4]
 *  g(p_t)          | g_cache[0]
 *  g(e_t)          | g_cache[1]
 *  ... same pattern for u,v,w
 *  g'(p_t)         | g_cache[0]
 *  ... continue pattern
 *  g"(p_t)         | g_cache[0]
 * 
 *  g_cache can be reused because we accumulate into new_data between
 *  iterations
 ****************************************************************************/
Real** cfd_explain(int numCell, int numBox,int nGhost){

  double time_spent;
  struct timeval  tv1, tv2;

  // The size of the 3D data is (numCell+2*nGhost)^3
  int full_numCell = numCell+2*nGhost;
  int full_numCell2 = full_numCell*full_numCell;
  int full_numCell3 = full_numCell*full_numCell+full_numCell;
  int totalCells = full_numCell*full_numCell*full_numCell;
  int flux_totalSize = numCell*numCell*(numCell+1);
  
  // in order to access each box we need an array of pointers
  Real **old_boxes = malloc(sizeof(Real*)*numBox);
  Real **new_boxes = malloc(sizeof(Real*)*numBox);
  
  // Allocate the 1D array of space (which is indexed as a 3D space)
  // numComp is the number of components (think of 5 cubes of data)
  int idx;
  for(idx=0;idx<numBox;idx++){
    old_boxes[idx] = malloc(sizeof(Real)*totalCells*numComp);
    new_boxes[idx] = malloc(sizeof(Real)*totalCells*numComp);
  }

  // initialize the data within each box
  int iz,iy,ix;
  for(idx=0;idx < numBox;idx++){
    Real* old_box = old_boxes[idx];
    Real* new_box = new_boxes[idx];

    // z,y,x traversal of all cells (including ghosts)
    // the initial value is the manhattan distance from the cell coordinates
    // to the origin of the non-ghost data multiplied
    // by dx and added to a different constant per variable
    for(iz=-nGhost;iz<(full_numCell-nGhost);iz++){
      for(iy=-nGhost;iy<(full_numCell-nGhost);iy++){
        for(ix=-nGhost;ix<(full_numCell-nGhost);ix++){
          p_DATA_new(iz,iy,ix) = dx*(iz+iy+ix);
          e_DATA_new(iz,iy,ix) = 1.+dx*(iz+iy+ix);
          u_DATA_new(iz,iy,ix) = 2.+dx*(iz+iy+ix);
          v_DATA_new(iz,iy,ix) = 3.+dx*(iz+iy+ix);
          w_DATA_new(iz,iy,ix) = 4.+dx*(iz+iy+ix);
          
          p_DATA_old(iz,iy,ix) = dx*(iz+iy+ix);
          e_DATA_old(iz,iy,ix) = 1.+dx*(iz+iy+ix);
          u_DATA_old(iz,iy,ix) = 2.+dx*(iz+iy+ix);
          v_DATA_old(iz,iy,ix) = 3.+dx*(iz+iy+ix);
          w_DATA_old(iz,iy,ix) = 4.+dx*(iz+iy+ix);
        }
      }
    }
  }
  // end allocate and init
  
  
  // begin process boxes

  // allocate the g cache -- to save all of the g() calculations
  // There is one value per face in the box. (numCell+1) is the 
  // number of faces in each direction.
  Real* g_cache = malloc(sizeof(Real)*numCell*numCell*(numCell+1)*numComp);
  //ALLOCATE_CACHES(numCell,numComp);
  
  
  // DATA Usage:
  // in this code we are reusing the g_cache to store 2 values per variable 
  // (component).
  // Additionally we are accumulating the result into [p,e,u,v,w]_DATA
  
  // iterate over all of the boxes
  gettimeofday(&tv1, NULL);
  for(idx=0;idx < numBox;idx++){
    Real* old_box = old_boxes[idx];
    Real* new_box = new_boxes[idx];

    //----------------------  x-direction
    
    // Iterate over faces and calculate g()
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell+1;ix++){
          p_CACHE_x(iz,iy,ix) = factor1*
                              (p_DATA_old(iz,iy,ix-2)+
                            7*(p_DATA_old(iz,iy,ix-1)+p_DATA_old(iz,iy,ix)) +
                               p_DATA_old(iz,iy,ix+1));                           
          e_CACHE_x(iz,iy,ix) = factor1*
                              (e_DATA_old(iz,iy,ix-2)+
                            7*(e_DATA_old(iz,iy,ix-1)+e_DATA_old(iz,iy,ix)) +
                               e_DATA_old(iz,iy,ix+1));          
          u_CACHE_x(iz,iy,ix) = factor1*
                              (u_DATA_old(iz,iy,ix-2)+
                            7*(u_DATA_old(iz,iy,ix-1)+u_DATA_old(iz,iy,ix)) +
                               u_DATA_old(iz,iy,ix+1));
          v_CACHE_x(iz,iy,ix) = factor1*
                              (v_DATA_old(iz,iy,ix-2)+
                            7*(v_DATA_old(iz,iy,ix-1)+v_DATA_old(iz,iy,ix)) +
                               v_DATA_old(iz,iy,ix+1));
          w_CACHE_x(iz,iy,ix) = factor1*
                              (w_DATA_old(iz,iy,ix-2)+
                            7*(w_DATA_old(iz,iy,ix-1)+w_DATA_old(iz,iy,ix)) +
                               w_DATA_old(iz,iy,ix+1));                                                                                                                  
        }
      }
    }
    
 
    // compute part of h() and reuse space for g()
    // u_CACHE_x(iz,iy,ix) must be written to last for reuse to work.
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell+1;ix++){
          p_CACHE_x(iz,iy,ix) *= factor2*u_CACHE_x(iz,iy,ix);
          e_CACHE_x(iz,iy,ix) *= factor2*u_CACHE_x(iz,iy,ix);       
          v_CACHE_x(iz,iy,ix) *= factor2*u_CACHE_x(iz,iy,ix);
          w_CACHE_x(iz,iy,ix) *= factor2*u_CACHE_x(iz,iy,ix);
          u_CACHE_x(iz,iy,ix) *= factor2*u_CACHE_x(iz,iy,ix);
        }
      }
    }
  
    // finish h()
    // iterate over cells 
    // and save the difference of the two adjacent faces into the cell data
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell;ix++){
          p_DATA_new(iz,iy,ix)+= p_CACHE_x(iz,iy,ix+1)-p_CACHE_x(iz,iy,ix);
          e_DATA_new(iz,iy,ix)+= e_CACHE_x(iz,iy,ix+1)-e_CACHE_x(iz,iy,ix);
          u_DATA_new(iz,iy,ix)+= u_CACHE_x(iz,iy,ix+1)-u_CACHE_x(iz,iy,ix);
          v_DATA_new(iz,iy,ix)+= v_CACHE_x(iz,iy,ix+1)-v_CACHE_x(iz,iy,ix);
          w_DATA_new(iz,iy,ix)+= w_CACHE_x(iz,iy,ix+1)-w_CACHE_x(iz,iy,ix);
        }
      }
    }
  
    //---------------------- y-direction
    
    // Iterate over faces and calculate g'()
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell+1;iy++){
        for(ix=0;ix<numCell;ix++){
          p_CACHE_y(iz,iy,ix) = factor1*
                              (p_DATA_old(iz,iy-2,ix)+
                            7*(p_DATA_old(iz,iy-1,ix)+p_DATA_old(iz,iy,ix)) +
                               p_DATA_old(iz,iy+1,ix));                           
          e_CACHE_y(iz,iy,ix) = factor1*
                              (e_DATA_old(iz,iy-2,ix)+
                            7*(e_DATA_old(iz,iy-1,ix)+e_DATA_old(iz,iy,ix)) +
                               e_DATA_old(iz,iy+1,ix));          
          u_CACHE_y(iz,iy,ix) = factor1*
                              (u_DATA_old(iz,iy-2,ix)+
                            7*(u_DATA_old(iz,iy-1,ix)+u_DATA_old(iz,iy,ix)) +
                               u_DATA_old(iz,iy+1,ix));
          v_CACHE_y(iz,iy,ix) = factor1*
                              (v_DATA_old(iz,iy-2,ix)+
                            7*(v_DATA_old(iz,iy-1,ix)+v_DATA_old(iz,iy,ix)) +
                               v_DATA_old(iz,iy+1,ix));
          w_CACHE_y(iz,iy,ix) = factor1*
                              (w_DATA_old(iz,iy-2,ix)+
                            7*(w_DATA_old(iz,iy-1,ix)+w_DATA_old(iz,iy,ix)) +
                               w_DATA_old(iz,iy+1,ix));                                                                                                                  
        }
      }
    }
    
    
    // compute part of h'() and reuse space for g'()
    // v_CACHE_x(iz,iy,ix) must be written to last for reuse to work.
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell+1;iy++){
        for(ix=0;ix<numCell;ix++){
          p_CACHE_y(iz,iy,ix) = factor2*p_CACHE_y(iz,iy,ix)*v_CACHE_y(iz,iy,ix);                          
          e_CACHE_y(iz,iy,ix) = factor2*e_CACHE_y(iz,iy,ix)*v_CACHE_y(iz,iy,ix);
          u_CACHE_y(iz,iy,ix) = factor2*u_CACHE_y(iz,iy,ix)*v_CACHE_y(iz,iy,ix);       
          w_CACHE_y(iz,iy,ix) = factor2*w_CACHE_y(iz,iy,ix)*v_CACHE_y(iz,iy,ix);
          v_CACHE_y(iz,iy,ix) = factor2*v_CACHE_y(iz,iy,ix)*v_CACHE_y(iz,iy,ix);                                                                                                                 
        }
      }
    }
    
    
    // finish h'()
    // iterate over cells 
    // and save the difference of the two adjacent faces into the cell data
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell;ix++){
          p_DATA_new(iz,iy,ix)+= p_CACHE_y(iz,iy+1,ix)-p_CACHE_y(iz,iy,ix);
          e_DATA_new(iz,iy,ix)+= e_CACHE_y(iz,iy+1,ix)-e_CACHE_y(iz,iy,ix);
          u_DATA_new(iz,iy,ix)+= u_CACHE_y(iz,iy+1,ix)-u_CACHE_y(iz,iy,ix);
          v_DATA_new(iz,iy,ix)+= v_CACHE_y(iz,iy+1,ix)-v_CACHE_y(iz,iy,ix);
          w_DATA_new(iz,iy,ix)+= w_CACHE_y(iz,iy+1,ix)-w_CACHE_y(iz,iy,ix);
        }
      }
    }
    

    
    //----------------------  z-direction
    
    // Iterate over faces and calculate g"()
    for(iz=0;iz<numCell+1;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell;ix++){
          p_CACHE_z(iz,iy,ix) = factor1*
                              (p_DATA_old(iz-2,iy,ix)+
                            7*(p_DATA_old(iz-1,iy,ix)+p_DATA_old(iz,iy,ix)) +
                               p_DATA_old(iz+1,iy,ix));                           
          e_CACHE_z(iz,iy,ix) = factor1*
                              (e_DATA_old(iz-2,iy,ix)+
                            7*(e_DATA_old(iz-1,iy,ix)+e_DATA_old(iz,iy,ix)) +
                               e_DATA_old(iz+1,iy,ix));          
          u_CACHE_z(iz,iy,ix) = factor1*
                              (u_DATA_old(iz-2,iy,ix)+
                            7*(u_DATA_old(iz-1,iy,ix)+u_DATA_old(iz,iy,ix)) +
                               u_DATA_old(iz+1,iy,ix));
          v_CACHE_z(iz,iy,ix) = factor1*
                              (v_DATA_old(iz-2,iy,ix)+
                            7*(v_DATA_old(iz-1,iy,ix)+v_DATA_old(iz,iy,ix)) +
                               v_DATA_old(iz+1,iy,ix));
          w_CACHE_z(iz,iy,ix) = factor1*
                              (w_DATA_old(iz-2,iy,ix)+
                            7*(w_DATA_old(iz-1,iy,ix)+w_DATA_old(iz,iy,ix)) +
                               w_DATA_old(iz+1,iy,ix));                                                                                                                  
        }
      }
    }

    // compute part of h"() and reuse space for g"()
    // w_CACHE_x(iz,iy,ix) must be written to last for reuse to work.
    for(iz=0;iz<numCell+1;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell;ix++){
          p_CACHE_z(iz,iy,ix) = factor2*p_CACHE_z(iz,iy,ix)*w_CACHE_z(iz,iy,ix);                          
          e_CACHE_z(iz,iy,ix) = factor2*e_CACHE_z(iz,iy,ix)*w_CACHE_z(iz,iy,ix);
          u_CACHE_z(iz,iy,ix) = factor2*u_CACHE_z(iz,iy,ix)*w_CACHE_z(iz,iy,ix);       
          v_CACHE_z(iz,iy,ix) = factor2*v_CACHE_z(iz,iy,ix)*w_CACHE_z(iz,iy,ix);
          w_CACHE_z(iz,iy,ix) = factor2*w_CACHE_z(iz,iy,ix)*w_CACHE_z(iz,iy,ix);                                                                                                                 
        }
      }
    }
    
    // finish h"()
    // iterate over cells 
    // and save the difference of the two adjacent faces into the cell data
    for(iz=0;iz<numCell;iz++){
      for(iy=0;iy<numCell;iy++){
        for(ix=0;ix<numCell;ix++){
          p_DATA_new(iz,iy,ix)+= p_CACHE_z(iz+1,iy,ix)-p_CACHE_z(iz,iy,ix);
          e_DATA_new(iz,iy,ix)+= e_CACHE_z(iz+1,iy,ix)-e_CACHE_z(iz,iy,ix);
          u_DATA_new(iz,iy,ix)+= u_CACHE_z(iz+1,iy,ix)-u_CACHE_z(iz,iy,ix);
          v_DATA_new(iz,iy,ix)+= v_CACHE_z(iz+1,iy,ix)-v_CACHE_z(iz,iy,ix);
          w_DATA_new(iz,iy,ix)+= w_CACHE_z(iz+1,iy,ix)-w_CACHE_z(iz,iy,ix);
        }
      }
    }
    
    
    
    
    if(tests){
        //printf("\nResult\n");
      int iz,iy,ix;

      //printf("\nSTART BOX\n");
      for(iz=0;iz<numCell;iz++){
        printf("\n");
        for(iy=0;iy<numCell;iy++){
          printf("\n");  
          for(ix=0;ix<numCell;ix++){
            printf("%.3lf,",p_DATA_new(iz,iy,ix));            
          }
        }
      }
      //printf("\n");  
    }
  } 
  gettimeofday(&tv2, NULL);

  if(!tests)
  printf ("LoopTime: %f\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec - tv1.tv_sec));
  else
	  printf("\n");
  return new_boxes;
}


/******************************************************************************
* allocate_and_init
* Input
* -----
* old_b pointer to old data
* new_b pointer to new data
* numCell the number of cells in a single direction
* numBox the number of boxes
* nGhost the number of ghost cells
*******************************************************************************/
Real** cfd_baseline(const int numCell,const int numBox){

  // allocate and initialize the data
  Real** old_boxes = malloc(sizeof(Real*)*numBox);
  Real** new_boxes = malloc(sizeof(Real*)*numBox);

  // allocate the space for each box
  // this should be a contiguous area of memory numCell cubed in size
  // they need to hold real values
  int full_numCell = numCell+2*nGhost;
  int full_numCell2 = full_numCell*full_numCell;
  int full_numCell3 = full_numCell*full_numCell+full_numCell;
  int totalCells = (numCell+2*nGhost)*(numCell+2*nGhost)*(numCell+2*nGhost);
  int flux_totalSize = numCell*numCell*(numCell+1);
  
  // the number of components
  int idx;
  for(idx=0;idx<numBox;idx++){
    old_boxes[idx] = malloc(sizeof(Real)*totalCells*numComp);
    new_boxes[idx] = malloc(sizeof(Real)*totalCells*numComp);
  }

  // initialize the data within each box

#ifdef OPEN_MP
#pragma omp parallel for default(shared)  private(idx) 
#endif
  for(idx=0;idx < numBox;idx++){
    Real* old_box = old_boxes[idx];
    Real* new_box = new_boxes[idx];
    int ic,iz,iy,ix;

    for(ic=0;ic<numComp;ic++){
      const Real phiOrigin = ic*compMultiplier;

    // z,y,x traversal of all cells (including ghosts)
    // the initial value is the manhattan distance from the cell coordinates
    // to the origin of the non-ghost data multiplied
    // by dx and added to a different constant per variable
      for(iz=-nGhost;iz<(full_numCell-nGhost);iz++){
        for(iy=-nGhost;iy<(full_numCell-nGhost);iy++){
          // pointer arithmetic
          Real* oldp = GET_VAL_PTR(old_box,ic,iz,iy,-nGhost);
          Real* newp = GET_VAL_PTR(new_box,ic,iz,iy,-nGhost);
          // inner loop with pointer increment
          for(ix=-nGhost;ix<(full_numCell-nGhost);ix++){
            *oldp = phiOrigin + dx*((iz)+(iy)+(ix));
            *newp = phiOrigin + dx*((iz)+(iy)+(ix));
            oldp++;
            newp++;
            }
          }
      }
    }
  }

  // process each of the boxes
  double time_spent;
  struct timeval  tv1, tv2;
  
  // loop bounds
  int phi_comp_mult = ((numCell+2*nGhost)*(numCell+2*nGhost)
                                   *(numCell+2*nGhost));
  const int phi_pencil_size = (numCell+2*nGhost);
  const int flux_comp_mult = ((numCell)*(numCell)*(numCell+1));

  // process each of the boxes one at a time
  //
#ifdef OPEN_MP
printf("Num Threads: %d\n",omp_get_max_threads());
#endif
  gettimeofday(&tv1, NULL);
#ifdef OPEN_MP
#pragma omp parallel for default(shared)  private(idx) 
#endif
  for(idx=0;idx < numBox;idx++){
    Real* old_box = old_boxes[idx];
    Real* new_box = new_boxes[idx];

  int f_xu,f_yu,f_zu;
  int flux_pencil_x;
  int flux_pencil_y;
    int iDir,ic,iz,iy,ix;
    int phiOffset1,phiOffset2,fluxOffset1;
    // the flux cache
    Real* fluxCache = malloc(sizeof(Real)*numCell*numCell*(numCell+1)*numComp);
    // Allocate the space for the velocity cache
    // This is only a single component
    Real* velCache = malloc(sizeof(Real)*numCell*numCell*(numCell+1));

    // compute the fluxes on the faces in each direction
    for(iDir=0;iDir<3;iDir++){

      // x-direction
      if(iDir == 0){
        f_zu = numCell;
    f_yu = numCell;
    f_xu = numCell+1;
    flux_pencil_x = numCell+1;
    flux_pencil_y = numCell;
        phiOffset1 = 1;
        phiOffset2 = 2;
        fluxOffset1 = 1;
      }else if(iDir == 1){
        f_zu = numCell;
    f_yu = numCell+1;
    f_xu = numCell;
    flux_pencil_x = numCell;
    flux_pencil_y = numCell+1;
        phiOffset1 = phi_pencil_size;
        phiOffset2 = phi_pencil_size*2;
    fluxOffset1 = numCell;
      }else if(iDir == 2){
        f_zu = numCell+1;
    f_yu = numCell;
    f_xu = numCell;
    flux_pencil_x = numCell;
    flux_pencil_y = numCell;
        phiOffset1 = phi_pencil_size*phi_pencil_size;
        phiOffset2 = phi_pencil_size*phi_pencil_size*2;
    fluxOffset1 = numCell*numCell;
      }


      // the upper bounds are determined by direction info above
      for(ic=0;ic<numComp;ic++){
        // Iterate over faces and calculate g()
        for(iz=0;iz<f_zu;iz++){
          for(iy=0;iy<f_yu;iy++){
            Real* phip = GET_VAL_PTR(old_box,ic,iz,iy,0); 
            Real* fluxp = GET_FACE_VAL_PTR(iDir,fluxCache,ic,iz,iy,0);
            for(ix=0;ix<f_xu;ix++){
              *fluxp = factor1*
                       ((*(phip - phiOffset2)) +
                     7*((*(phip - phiOffset1)) + (*(phip))) +
                        (*(phip + phiOffset1)));
              ++phip;
              ++fluxp;
            }
          }
        }
      }

      // cache the velocity component for the next half of the calculation
      memcpy(velCache,
             (fluxCache+(iDir+2)*((numCell+1)*numCell*numCell)),
             sizeof(Real)*numCell*numCell*(numCell+1));
  
      for(ic=0;ic<numComp;ic++){
        for(iz=0;iz<f_zu;iz++){
          for(iy=0;iy<f_yu;iy++){
            //pointer arithmetic
            Real* velp = velCache + iz*flux_pencil_y*flux_pencil_x+
                                    iy*flux_pencil_x;
            Real* fluxp = GET_FACE_VAL_PTR(iDir,fluxCache,ic,iz,iy,0);
            // inner loop
            for(ix=0;ix<f_xu;ix++){
              *fluxp *= factor2*(*velp);
              ++fluxp;
              ++velp;
            }
          }
        }
      }    


      // compute the second half of the flux calculation
      // accumulate the differences into the new data box
      for(ic=0;ic<numComp;ic++){
        for(iz=0;iz<numCell;iz++){
          for(iy=0;iy<numCell;iy++){
            // pointer arithmetic
            Real* phip = GET_VAL_PTR(new_box,ic,iz,iy,0);
                  Real* fluxp = GET_FACE_VAL_PTR(iDir,fluxCache,ic,iz,iy,0);

            for(ix=0;ix<numCell;ix++){
              *phip += (*(fluxp + fluxOffset1)) - (*fluxp);
              ++phip;
              ++fluxp;
            }
          }
        }
      }
    } // direction loop
    
    free(fluxCache);
    free(velCache);
  } // box loop
  gettimeofday(&tv2, NULL);

  printf ("LoopTime: %f\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec - tv1.tv_sec));

  if(tests){
    printf("\nEnd Result\n");
    int ic=0,iz,iy,ix;
    printf("\nSTART BOX\n");
    for(iz=0;iz<numCell;iz++){
      printf("\n");
      for(iy=0;iy<numCell;iy++){
        printf("\n");
        Real* phip = GET_VAL_PTR(new_boxes[0],ic,iz,iy,0);
        for(ix=0;ix<numCell;ix++){
          printf("%lf,",*phip);
          ++phip;
        }
      }
    }
    printf("\nEND BOX");
  }

  // cleanup the memory used -- unless we are doing verification
  if(verify){ return new_boxes; }
  for(idx=0;idx<numBox;idx++){
    free(old_boxes[idx]);
    free(new_boxes[idx]);
  }
  free(old_boxes);
  free(new_boxes);
  return NULL;
}
  
