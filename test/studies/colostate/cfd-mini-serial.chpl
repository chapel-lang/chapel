/******************************************************************************
* This is a benchmark written to emulate the behavior of a typical 
* computational fluid dynamics benchmark.
* There are several versions of this benchmark - the original written with
* the use of CHOMBO, other versions are written in pure c. This version is 
* written in Chapel in order to explore the use of Chapel iterators and
* performance.
*******************************************************************************/
module CFD_mini { 

 use Time;

 const dx = 0.5;
 const factor1 = 1.0/12.0;
 const factor2 = 2.0;

 config var nCell = 128;
 config var nBox = 32;
 config var nGhost = 2;
 config var nComp = 5;
 config var test = 0;

 var totalWidth = nCell+2*nGhost;
 var cellData: domain(3) = {0..nCell-1,0..nCell-1,0..nCell-1};
 var withGhosts: domain(3) = cellData.expand(nGhost);
 var boxDomain: domain(1) = {0..nBox-1};
 var compDomain: domain(1) = {0..nComp-1};
 var xFluxDomain: domain(3) = {0..nCell-1,0..nCell-1,0..nCell};
 var yFluxDomain: domain(3) = {0..nCell-1,0..nCell,0..nCell-1};
 var zFluxDomain: domain(3) = {0..nCell,0..nCell-1,0..nCell-1};
 var FluxDomain: domain(3) = {0..nCell,0..nCell,0..nCell};
 var loopTime: Timer;


 proc main() { 

   writeln("nCell=",nCell," nBox=",nBox);

   var old_data:[0..nBox-1,0..nComp-1,-2..nCell+1,-2..nCell+1,-2..nCell+1] real;
   var new_data:[0..nBox-1,0..nComp-1,-2..nCell+1,-2..nCell+1,-2..nCell+1] real;

   // initialize the data in the arrays
   // Each cell value in both the old and new data is initialized to be
   // the manhattan distance from the origin
   for b in boxDomain {
     for c in  compDomain {
       for (z,y,x) in withGhosts{
         old_data[b,c,z,y,x] = c + dx*(z+y+x);
         new_data[b,c,z,y,x] = old_data[b,c,z,y,x];
      }
    }
  }
  
/*******************************************************************************
 * Process Boxes:
 *  Processing the boxes means that we will be reading the data from
 *  old_boxes and writing them to new_boxes 
 *  The following are the equations for this calculation
 * 
 *  There are 5 components: p, e, u, v, w (density, energy, velocity (3D))
 *  Each of these components is represented as a 3D array (initialized
 *  above).
 * p_{t+1}=factor2*g(p_t)*g(u_t)+factor2*g'(p_t)*g'(v_t)+factor2*g"(p_t)*g"(w_t)
 * e_{t+1}=factor2*g(e_t)*g(u_t)+factor2*g'(e_t)*g'(v_t)+factor2*g"(e_t)*g"(w_t)
 * u_{t+1}=factor2*g(u_t)*g(u_t)+factor2*g'(u_t)*g'(v_t)+factor2*g"(u_t)*g"(w_t)
 * v_{t+1}=factor2*g(v_t)*g(u_t)+factor2*g'(v_t)*g'(v_t)+factor2*g"(v_t)*g"(w_t)
 * w_{t+1}=factor2*g(w_t)*g(u_t)+factor2*g'(w_t)*g'(v_t)+factor2*g"(w_t)*g"(w_t)
 *
 *  
 *  g(data) is a stencil operation that looks like the following:
 *  g(data[z][y][x]) = factor1*
 *        (data[z][y][x-2]+7*(data[z][y][x-1]+data[z][y][x])+data[z][y][x+1])
 *  similarly for g' and g"
 *  g'(data[z][y][x]) = factor1*
 *        (data[z][y-2][x]+7*(data[z][y-1][x]+data[z][y][x])+data[z][y+1][x])
 *  g"(data[z][y][x]) = factor1*
 *        (data[z-2][y][x]+7*(data[z-1][y][x]+data[z][y][x])+data[z+1][y][x])
 *        
 *  
 *  Step 1 is to calculate all of the g() values
 *  Step 2 multiplies the values together for the first column in the 
 *         equations above
 *  Step 3 Return to Step 1 for g' and then for g"
 ****************************************************************************/
   loopTime.start();
   forall b in boxDomain {
     var FluxCache: [0..nComp-1,0..nCell,0..nCell,0..nCell] real;

     // x-direction
     // compute the flux on the faces in each direction
     for z in 0..nCell-1{
     for y in 0..nCell-1{
     for x in 0..nCell{
       FluxCache[0,z,y,x] = factor1 *  (old_data[b,0,z,y,x-2] + 
                                       7*(old_data[b,0,z,y,x-1] +
                                         old_data[b,0,z,y,x]) + 
                                         old_data[b,0,z,y,x+1]);
       FluxCache[1,z,y,x] = factor1 *  (old_data[b,1,z,y,x-2] + 
                                       7*(old_data[b,1,z,y,x-1] +
                                         old_data[b,1,z,y,x]) + 
                                         old_data[b,1,z,y,x+1]);
       FluxCache[2,z,y,x] = factor1 *  (old_data[b,2,z,y,x-2] + 
                                       7*(old_data[b,2,z,y,x-1] +
                                         old_data[b,2,z,y,x]) + 
                                         old_data[b,2,z,y,x+1]);
       FluxCache[3,z,y,x] = factor1 *  (old_data[b,3,z,y,x-2] + 
                                       7*(old_data[b,3,z,y,x-1] +
                                         old_data[b,3,z,y,x]) + 
                                         old_data[b,3,z,y,x+1]);
       FluxCache[4,z,y,x] = factor1 *  (old_data[b,4,z,y,x-2] + 
                                       7*(old_data[b,4,z,y,x-1] +
                                         old_data[b,4,z,y,x]) + 
                                         old_data[b,4,z,y,x+1]);
       FluxCache[0,z,y,x] *= factor2*FluxCache[2,z,y,x];
       FluxCache[1,z,y,x] *= factor2*FluxCache[2,z,y,x];
       FluxCache[3,z,y,x] *= factor2*FluxCache[2,z,y,x];
       FluxCache[4,z,y,x] *= factor2*FluxCache[2,z,y,x];
       FluxCache[2,z,y,x] *= factor2*FluxCache[2,z,y,x];
     }}}

     // accumulate the difference into the new data cells
    for z in 0..nCell-1{
    for y in 0..nCell-1{
    for x in 0..nCell-1{
      for c in compDomain{
        new_data[b,c,z,y,x] += FluxCache[c,z,y,x+1] - FluxCache[c,z,y,x];
      }
    }}}
  
     // compute the flux on the faces in each direction
     for z in 0..nCell-1{
     for y in 0..nCell{
     for x in 0..nCell-1{
       FluxCache[0,z,y,x] = factor1*  (old_data[b,0,z,y-2,x] + 
                                         7*(old_data[b,0,z,y-1,x] +
                                           old_data[b,0,z,y,x]) + 
                                         old_data[b,0,z,y+1,x]);
       FluxCache[1,z,y,x] = factor1*  (old_data[b,1,z,y-2,x] + 
                                         7*(old_data[b,1,z,y-1,x] +
                                           old_data[b,1,z,y,x]) + 
                                         old_data[b,1,z,y+1,x]);
       FluxCache[2,z,y,x] = factor1*  (old_data[b,2,z,y-2,x] + 
                                         7*(old_data[b,2,z,y-1,x] +
                                           old_data[b,2,z,y,x]) + 
                                         old_data[b,2,z,y+1,x]);
       FluxCache[3,z,y,x] = factor1*  (old_data[b,3,z,y-2,x] + 
                                         7*(old_data[b,3,z,y-1,x] +
                                           old_data[b,3,z,y,x]) + 
                                         old_data[b,3,z,y+1,x]);
       FluxCache[4,z,y,x] = factor1*  (old_data[b,4,z,y-2,x] + 
                                         7*(old_data[b,4,z,y-1,x] +
                                           old_data[b,4,z,y,x]) + 
                                         old_data[b,4,z,y+1,x]);
       FluxCache[0,z,y,x] *= factor2*FluxCache[3,z,y,x];
       FluxCache[1,z,y,x] *= factor2*FluxCache[3,z,y,x];
       FluxCache[2,z,y,x] *= factor2*FluxCache[3,z,y,x];
       FluxCache[4,z,y,x] *= factor2*FluxCache[3,z,y,x];
       FluxCache[3,z,y,x] *= factor2*FluxCache[3,z,y,x];
     }}}

     // accumulate the difference into the new data cells
     for z in 0..nCell-1{
     for y in 0..nCell-1{
     for x in 0..nCell-1{
      for c in compDomain{
        new_data[b,c,z,y,x] += FluxCache[c,z,y+1,x] - FluxCache[c,z,y,x];
      }
     }}}

    // z-direction
     for z in 0..nCell{
     for y in 0..nCell-1{
     for x in 0..nCell-1{
      FluxCache[0,z,y,x] = factor1*  (old_data[b,0,z-2,y,x] + 
                                    7*(old_data[b,0,z-1,y,x] +
                                       old_data[b,0,z,y,x]) + 
                                       old_data[b,0,z+1,y,x]);
      FluxCache[1,z,y,x] = factor1*  (old_data[b,1,z-2,y,x] + 
                                    7*(old_data[b,1,z-1,y,x] +
                                       old_data[b,1,z,y,x]) + 
                                       old_data[b,1,z+1,y,x]);
      FluxCache[2,z,y,x] = factor1*  (old_data[b,2,z-2,y,x] + 
                                    7*(old_data[b,2,z-1,y,x] +
                                       old_data[b,2,z,y,x]) + 
                                       old_data[b,2,z+1,y,x]);
      FluxCache[3,z,y,x] = factor1*  (old_data[b,3,z-2,y,x] + 
                                    7*(old_data[b,3,z-1,y,x] +
                                       old_data[b,3,z,y,x]) + 
                                       old_data[b,3,z+1,y,x]);
      FluxCache[4,z,y,x] = factor1*  (old_data[b,4,z-2,y,x] + 
                                    7*(old_data[b,4,z-1,y,x] +
                                       old_data[b,4,z,y,x]) + 
                                       old_data[b,4,z+1,y,x]);
       FluxCache[0,z,y,x] *= factor2*FluxCache[4,z,y,x];
       FluxCache[1,z,y,x] *= factor2*FluxCache[4,z,y,x];
       FluxCache[2,z,y,x] *= factor2*FluxCache[4,z,y,x];
       FluxCache[3,z,y,x] *= factor2*FluxCache[4,z,y,x];
       FluxCache[4,z,y,x] *= factor2*FluxCache[4,z,y,x];
    }}}
  
     // accumulate the difference into the new data cells
    for z in 0..nCell-1{
    for y in 0..nCell-1{
    for x in 0..nCell-1{
      for c in compDomain{
        new_data[b,c,z,y,x] += FluxCache[c,z+1,y,x] - FluxCache[c,z,y,x];
      }
    }}}
  }
  loopTime.stop();


  if(test){
    writeln("Resulting Data");
    for (z,y,x) in cellData{
      if(x%4==0){
        writeln();
      }
      write(new_data[0,0,z,y,x],",");
    }  
    writeln();
  }else{
    writeln("LoopTime: ",loopTime.elapsed());
  }
    

 } 
} 
