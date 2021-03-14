/******************************************************************************
* This is a benchmark written to emulate the behavior of a typical 
* computational fluid dynamics benchmark.
* There are several versions of this benchmark - the original written with
* the use of CHOMBO, other versions are written in pure c. This version is 
* written in Chapel in order to explore the use of Chapel iterators and
* performance.
*******************************************************************************/
pragma "error mode fatal"
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
 var boxRange = 0..nBox-1;
 var compRange = 0..nComp-1;
 var xFluxDomain: domain(3) = {0..nCell-1,0..nCell-1,0..nCell};
 var yFluxDomain: domain(3) = {0..nCell-1,0..nCell,0..nCell-1};
 var zFluxDomain: domain(3) = {0..nCell,0..nCell-1,0..nCell-1};
 var FluxDomain: domain(3) = {0..nCell,0..nCell,0..nCell};
 var loopTime: Timer;


 proc main() { 

   writeln("nCell=",nCell," nBox=",nBox);

   var old_data, new_data:[boxRange][compRange][withGhosts] real;

   // initialize the data in the arrays
   // Each cell value in both the old and new data is initialized to be
   // the manhattan distance from the origin
   for b in boxRange {
     for c in  compRange {
       for (z,y,x) in withGhosts {
         const initval = c + dx*(z+y+x);
         //
         // TODO: Factor [b][c] indexing intou outer loops
         //
         old_data[b][c][z,y,x] = initval;
         new_data[b][c][z,y,x] = initval;
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
   forall b in boxRange {
     var FluxCache: [compRange][FluxDomain] real;

     computeDirection(2, FluxCache, old_data[b], new_data[b], xFluxDomain, 2);
     computeDirection(1, FluxCache, old_data[b], new_data[b], yFluxDomain, 3);
     computeDirection(0, FluxCache, old_data[b], new_data[b], zFluxDomain, 4);

     proc computeDirection(dim, FluxCache, old_data, new_data, FluxDomain, dim2) {
       computeFlux(FluxCache, old_data, FluxDomain, dim, dim2);
       accumulate(dim, FluxCache, new_data);
     }
  }
  loopTime.stop();


  if(test){
    writeln("Resulting Data");
    for (z,y,x) in cellData{
      if(x%4==0){
        writeln();
      }
      write(new_data[0][0][z,y,x],",");
    }  
    writeln();
  }else{
    writeln("LoopTime: ",loopTime.elapsed());
  }
    

 }

 proc computeFlux(FluxCache, old_data_b, FluxDomain, dim, dim2) {
   var offn2, offn1, offp1 = (0,0,0);
   offn2(dim) = -2;
   offn1(dim) = -1;
   offp1(dim) =  1;

   inline proc stencil(old_data, zyx) {
     return factor1 *  (old_data[zyx+offn2] + 
                        7*(old_data[zyx+offn1] + old_data[zyx]) + 
                        old_data[zyx+offp1]);
   }
     
   ref FluxCache_dim2 = FluxCache[dim2];
   ref old_data_b_dim2 = old_data_b[dim2];
   for zyx in FluxDomain {
     FluxCache_dim2[zyx] = stencil(old_data_b_dim2, zyx);
   }
   for c in compRange {
     if (c != dim2) {
       ref FluxCache_c = FluxCache[c];
       ref old_data_b_c = old_data_b[c];
       for zyx in FluxDomain {
         FluxCache_c[zyx] = stencil(old_data_b_c, zyx) * factor2 * FluxCache_dim2[zyx];
       }
     }
   }
   FluxCache_dim2 *= factor2*FluxCache_dim2;
 }

 proc accumulate(dir, FluxCache, new_data_b) {
   var off = (0,0,0);
   off(dir) = 1;
   
   for c in compRange{
     ref FluxCache_c = FluxCache[c];
     ref new_data_b_c = new_data_b[c];
     for (zyx) in cellData {
       new_data_b_c[zyx] += FluxCache_c[zyx+off] - FluxCache_c[zyx];
     }
   }
 }
} 
