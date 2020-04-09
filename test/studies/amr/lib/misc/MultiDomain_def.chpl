
public use BasicDataStructures;


proc main {
  
      
  const mD = new unmanaged MultiDomain( 2, false );
  
  mD.add( {1..10,1..10} );
  
  // mD.subtract( [59..161 by 2, -1..17 by 2] );
  // mD.subtract( [-1..45 by 2, 71..161 by 2] );
  // mD.subtract( [43..61 by 2, -1..9 by 2] );
  // mD.subtract( [71..161 by 2, 15..61 by 2] );
  // mD.subtract( [63..161 by 2, 59..161 by 2] );
  // mD.subtract( [-1..21 by 2, -1..17 by 2] );
  // mD.subtract( [-1..9 by 2, 59..65 by 2] );
  // mD.subtract( [-1..21 by 2, 63..73 by 2] );
  // mD.subtract( [43..65 by 2, 71..161 by 2] );
  // mD.subtract( [19..45 by 2, -1..9 by 2] );
  // mD.subtract( [-1..9 by 2, 15..53 by 2] );
  // mD.subtract( [-1..9 by 2, 51..61 by 2] );
  // mD.subtract( [27..45 by 2, 31..53 by 2] );
  // mD.subtract( [43..53 by 2, 31..49 by 2] );
  // 
  // 
  // 
  // for D in mD do writeln(D);
  
  mD.subtract( {7..8, 2..9} );  
  mD.add( {11..12, 13..14} );  
  

  const mD2 = mD.copy();
  mD2.prettyPrint();

  
}





//|\"""""""""""""""""""""""""""|\
//| >    class: MultiDomain    | >
//|/___________________________|/

//--------------------------------------------------------------------
// The MultiDomain class is used to describe an index set composed
// of a union of domains.  It relies on a binary tree representation,
// adapted from the kd-tree data structure, with nodes that are 
// members of the MDNode class.
//--------------------------------------------------------------------

class MultiDomain
{
  
  param rank:      int;
  param stridable: bool;
      
  var root: unmanaged MDNode(rank,stridable);


  proc init (param rank=0, param stridable=false)
  {
    this.rank = rank;
    this.stridable = stridable;
    root = new unmanaged MDNode( rank, stridable );
  }


  proc deinit () { root.clearChildren();  delete root; }


  proc copy ()
  {
    const new_mD = new unmanaged MultiDomain(rank,stridable);
    root.copy(new_mD.root);  // the initializer has already allocated 'root', so pass it in for re-use
    return new_mD;
  }



  proc isEmpty ()
  {    
    return (root.bisect_dim==-1 || root.Domain.size==0);
  }


  proc subtract ( D: domain(rank,stridable=stridable) )
  {
    if !isEmpty() && root.Domain(D).size>0 then 
      root.subtract(D);
    if root.bisect_dim==-1 then root.Domain.clear();
  }  
  
  
  proc add ( D: domain(rank,stridable=stridable) )
  {
    if !isEmpty() then
    {
      root = root.extendToContain( D );
      
      root.add( D );
    }
    else
    {
      root.Domain = D;
      root.bisect_dim = 0;
    }
    
  }
  
  
  proc add ( domain_collection )
    where domain_collection.type != domain(rank,stridable=stridable)
  {
    for D in domain_collection do add(D);
  }
  
  
  
  proc intersect ( D: domain(rank,stridable=stridable) )
  {
    
    if !isEmpty()
    {
      root.intersect( D );
      if root.bisect_dim==-1 then root.Domain.clear();
    }
    
  }
  
  
  
  
  
  iter these ()
  {
    
    const q = new unmanaged Queue( unmanaged MDNode(rank,stridable) );

    q.enqueue( root );
    
    while !q.isEmpty()
    {
      const node = q.dequeue();

      if node.bisect_dim==0 then yield node.Domain;
      else
      {
        if node.left then q.enqueue( node.left! );
        if node.right then q.enqueue( node.right! );
      }
    }

    delete q;
  }
  
  
  
  iter nodes () : unmanaged MDNode(rank,stridable)
  {
    
    const q = new unmanaged Queue( unmanaged MDNode(rank,stridable) );
    var node: unmanaged MDNode(rank,stridable);
  
    if root then q.enqueue( root );
    
    while !q.isEmpty()
    {
      node = q.dequeue();
      yield node;
      
      if node.left  then q.enqueue( node.left );
      if node.right then q.enqueue( node.right );
    }

    delete q;
  }
    

  
  //---- Print as an array of 0's and 1's ----
  
  proc prettyPrint ()
  {
    var A: [root.Domain] int;
    for D in these() do A(D) = 1;
    writeln(A);
  }

  
}

// /|"""""""""""""""""""""""""""/|
//< |    class: MultiDomain    < |
// \|___________________________\|





//|\""""""""""""""""""""""|\
//| >    class: MDNode    | >
//|/______________________|/

//-------------------------------------------------------------------
// The MDNode class represents a node in the tree structure that 
// constitutes a MultiDomain.  It consists of a domain, a descriptor 
// of whether it is filled, a description of a hyperplane separating 
// its domain, and left/right child nodes.
//
// The separatrix is described by bisect_dim, the dimension to which it
// is orthogonal, and right_low, its index in that dimension, which
// is the lower bound of the right child.
//------------------------------------------------------------------

class MDNode
{
  
  param rank:      int;
  param stridable: bool;
  
  var Domain:    domain(rank,stridable=stridable);

  var bisect_dim: int = -1;  // Indicates temporary, unfilled status
  var right_low:  int = 0;

  var left, right:  unmanaged MDNode(rank,stridable)?;



  proc deinit () {}  // Can't clear children here or node merging breaks
  

  proc clearChildren ()
  {
    if left  then { left!.clearChildren();   delete left;  left = nil; }
    if right then { right!.clearChildren();  delete right;  right = nil; }
  }


  proc copy (in new_node_arg: unmanaged MDNode(rank, stridable)? = nil) : unmanaged MDNode(rank,stridable)
  {
    const new_node = if new_node_arg != nil then new_node_arg!
                     else new unmanaged MDNode(rank, stridable);

    new_node.Domain     = Domain;
    new_node.bisect_dim = bisect_dim;
    new_node.right_low  = right_low;
    
    if left  then new_node.left  = left!.copy();
    if right then new_node.right = right!.copy();
    
    return new_node;
  }


  //|\''''''''''''''''''''''''''''''''|\
  //| >    method: locateBisection    | >
  //|/................................|/

  proc locateBisection ( D: domain(rank,stridable=stridable) )
  {
    
    //===> Choose bisection dimension ===>
    
    var dim_length     = 0;
    var max_dim_length = 0;
    bisect_dim = 0;
    
    for d in 0..rank-1
    {

      //--------------------------------------------------------------------
      // We know that D and Domain are not disjoint, so dimension d may be 
      // split as long as D.dim(d) does not strictly contain Domain.dim(d).
      //--------------------------------------------------------------------

      if D.low(d) > Domain.low(d) || D.high(d) < Domain.high(d)
      {

        dim_length = Domain.dim(d).size;

        if dim_length > max_dim_length {
          max_dim_length = dim_length;
          bisect_dim = d+1;
        }

      }
      
    }
    
    //<==== Choose bisection dimension <===
    
    
    //---- Set bisection index (low index of right child) ----
    //---------------------------------------------------------------------
    // (Comment-only shorthand d = bisect_dim)
    // The larger offset indicates the bisection closer to the midpoint of
    // Domain.dim(d).  This follows from the fact that
    //    low_offset + high_offset + D.width(d) = Domain.width(d)
    // where D.width = D.high - D.low.
    //---------------------------------------------------------------------
    
    if bisect_dim > 0
    {    
      
      var low_offset  = D.low(bisect_dim-1) - Domain.low(bisect_dim-1);
      var high_offset = Domain.high(bisect_dim-1) - D.high(bisect_dim-1);
      
      if low_offset > high_offset then
        right_low = D.low(bisect_dim-1);
      else
        right_low = D.high(bisect_dim-1) + Domain.stride(bisect_dim-1);
    }
    
  }

  // /|''''''''''''''''''''''''''''''''/|
  //< |    method: locateBisection    < |
  // \|................................\|




  //|\''''''''''''''''''''|\
  //| >    method: add    | >
  //|/....................|/
  
  //-------------------------------------------------------------------
  // This method can be called on a node for which:
  //
  // bisect_dim == -1 : Just created by the parent node, unfilled.
  // bisect_dim ==  0 : Filled, not bisected.  Nothing happens.
  // bisect_dim  >  0 : Already bisected; has at least one child node.
  //-------------------------------------------------------------------
  
  proc add ( D: domain(rank, stridable=stridable) )
  {
  
    //---- Locate bisection of node is unfilled ----
    //-------------------------------------------------------------
    // This should only occur if the node has just been created by
    // its parent.
    //-------------------------------------------------------------
  
    if bisect_dim == -1 then locateBisection(D); 
    
    
    //-------------------------------------------------------------------
    // If bisect_dim == 0 at this point, then we're done.  It means
    // that either the node was filled on entry, or locateBisection left
    // bisect_dim=0 because D contains Domain.
    //-------------------------------------------------------------------
    
    
    //---- If bisected, create and add to children as necessary ----
    
    if bisect_dim > 0
    {
      
      //---- Left child ----
      
      if D.low(bisect_dim-1) <= right_low - Domain.stride(bisect_dim-1)
      {
        if left==nil then createLeft( filled=false );
        left!.add( D );
      }
      
      
      //---- Right child ----
      
      if D.high(bisect_dim-1) >= right_low
      {
        if right==nil then createRight( filled=false );
        right!.add( D );
      }
      
      
      //---- Check whether children can be merged ----
      
      if left!=nil && right!=nil then
        if left!.bisect_dim==0 && right!.bisect_dim==0
        {
          delete left;
          left = nil;

          delete right;
          right = nil;

          bisect_dim = 0;
        }
      
      
    }
    
  }
  // /|''''''''''''''''''''/|
  //< |    method: add    < |
  // \|....................\|




  //|\'''''''''''''''''''''''''|\
  //| >    method: subtract    | >
  //|/.........................|/
  
  proc subtract ( D: domain(rank, stridable=stridable) )
  {
        
    //---- If node is filled, then either bisect or unfill ----
    
    if bisect_dim == 0
    {
      locateBisection(D);

      if bisect_dim == 0 then bisect_dim = -1;
      else
      {
        createLeft( filled=true );
        createRight( filled=true );
      }
    }
    
        
    
    //-----------------------------------------------------------------
    // Subtract from children if necessary.  Note that if this node is
    // bisected but a child doesn't exist, then that child's space is
    // empty, and there is no subtraction to take place.
    //-----------------------------------------------------------------
    
    if bisect_dim > 0
    {
      
      //---- Left child ----
      
      if D.low(bisect_dim-1) <= right_low - Domain.stride(bisect_dim-1)  &&  left!=nil
      {
        left!.subtract( D );
        if left!.bisect_dim == -1 then { delete left;  left=nil; }
      }


      //---- Right child ----

      if D.high(bisect_dim-1) >= right_low  &&  right!=nil
      {
        right!.subtract( D );
        if right!.bisect_dim == -1 then { delete right;  right=nil; }
      }
    }
    
  }
  
  // /|'''''''''''''''''''''''''/|
  //< |    method: subtract    < |
  // \|.........................\|





  //|\''''''''''''''''''''''''''|\
  //| >    method: intersect    | >
  //|/..........................|/
  
  proc intersect ( D: domain(rank,stridable=stridable) )
  {

    if bisect_dim == 0
    {
      Domain = Domain(D);
      if Domain.size==0 then bisect_dim = -1; 
    }


    //---- If the node is bisected... ----

    else
    {

      var intersects_left  = intersectsLeft(D);
      var intersects_right = intersectsRight(D);
      
      
      //------------------------------------------------------------
      // If D intersects both sides of Domain, then both children
      // need to be intersected, if they exist.  A child may become
      // unfilled in the intersection process (as intersectsLeft
      // and intersectsRight only test bounding boxes), so this
      // node may need to delete them.  Then, if both children are
      // removed, this node marks itself unfilled.
      //------------------------------------------------------------
      
      if intersects_left && intersects_right
      {
        
        if left
        {
          left!.intersect(D);
          if left!.bisect_dim==-1  then { delete left;   left = nil; }
        } 

        if right
        {
          right!.intersect(D);
          if right!.bisect_dim==-1 then { delete right;  right = nil; }
        }
        
        if left==nil && right==nil then bisect_dim = -1;
        else Domain = Domain(D);
      }
      
      
      //----------------------------------------------------------------------
      // If D only intersects the left side of Domain, then the right
      // child may be immediately removed.  Furthermore, the intersection
      // will remove this node's bisector, so this node becomes redundant
      // with its left child.  Hence, if the left child survives intersection
      // with D, it is merged with this node.
      //----------------------------------------------------------------------
      
      else if intersects_left
      {
        if right { right!.clearChildren();  delete right;  right = nil; }
        
        if left { left!.intersect(D);  merge(left!); }
        else bisect_dim = -1;
      }
      
      //-----------------------------------------------------
      // Same as the previous case, but for the right child.
      //-----------------------------------------------------
      
      else
      {
        if left { left!.clearChildren();  delete left; left = nil; }
        
        if right { right!.intersect(D);  merge(right!); }
        else bisect_dim = -1;
      }

    }
    
  }
  
  // /|''''''''''''''''''''''''''/|
  //< |    method: intersect    < |
  // \|..........................\|



  proc intersectsLeft( D: domain(rank, stridable=stridable) )
  {
    assert( bisect_dim>0, "Error: Called MDNode.intersectsLeft with bisect_dim<=0");
    return D.low(bisect_dim-1) <= right_low - Domain.stride(bisect_dim-1);
  }
  
  
  proc intersectsRight( D: domain(rank, stridable=stridable) )
  {
    assert( bisect_dim>0, "Error: Called MDNode.intersectsRight with bisect_dim<=0");
    return D.high(bisect_dim-1) >= right_low;
  }


  proc createLeft ( filled: bool=true )
  {
    assert( bisect_dim>0, "Error: Called MDNode.createLeft with bisect_dim<=0.");
    
    var subranges: rank*range(stridable=stridable);
    
    for d in 0..rank-1          do subranges(d) = Domain.dim(d);
    for d in bisect_dim..rank-1 do subranges(d) = Domain.dim(d);    
    
    subranges(bisect_dim-1) = Domain.low(bisect_dim-1) .. right_low-Domain.stride(bisect_dim-1) by Domain.stride(bisect_dim-1);
    
    var child_domain: domain(rank, stridable=stridable) = subranges;
    
    if filled then left = new unmanaged MDNode( rank, stridable, child_domain, 0 );
    else           left = new unmanaged MDNode( rank, stridable, child_domain, -1 );
    
  }


  proc createRight ( filled: bool=true )
  {
    assert( bisect_dim>0, "Error: Called MDNode.createRight with bisect_dim<=0.");
    
    var subranges: rank*range(stridable=stridable);
    
    for d in 0..rank-1          do subranges(d) = Domain.dim(d);
    for d in bisect_dim..rank-1 do subranges(d) = Domain.dim(d);
    
    subranges(bisect_dim-1) = right_low .. Domain.high(bisect_dim-1) by Domain.stride(bisect_dim-1);
    
    var child_domain: domain(rank, stridable=stridable) = subranges;
    
    if filled then right = new unmanaged MDNode( rank, stridable, child_domain, 0 );
    else           right = new unmanaged MDNode( rank, stridable, child_domain, -1 );
  }


}

// /|""""""""""""""""""""""/|
//< |    class: MDNode    < |
// \|______________________\|








//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    method: MDNode.extendToContain    | >
//|/_______________________________________|/

//---------------------------------------------------------------
// This method extends the existing MDNode through a series
// of extensions acroos hyperplanes, creating a structure that
// contains the input domain D.  The return value is a reference
// to the new root of the structure.
//---------------------------------------------------------------

proc MDNode.extendToContain( D: domain(rank,stridable=stridable) ) : unmanaged MDNode(rank,stridable)
{
  
  // writeln("Beginning MDNode.extendToContain on node with ", Domain, " ", left!=nil, " ", filled);
  // writeln("  Input domain is ", D);
  
  var new_root: unmanaged MDNode(rank,stridable)?;
  
  
  //===> Select the shortest dimension to extend ===>
  
  var dim_length     = 0;
  var min_dim_length = Domain.size+1;
  var ext_d          = 0;
    
  for d in 0..rank-1 {
    
    if D.high(d) > Domain.high(d) || D.low(d) < Domain.low(d) {
      dim_length = Domain.dim(d).size;
      
      if dim_length < min_dim_length {
        min_dim_length = dim_length;
        ext_d = d;
      }
    }
    
  }
  
  //<=== Select the shortest dimension to extend <===
  
  
  //---- If ext_d==0, that means D is contained in this.Domain, and this is the new root ----
  
  if ext_d == 0 then new_root = _to_unmanaged(this);
  
  
  //---- Otherwise, we split along dimension ext_d ----
    
  else {

    //---- Declare and prepare ----
    
    var s = Domain.stride(ext_d);
    var ext_index: int;
    
    var parent:  unmanaged MDNode(rank,stridable)?;
    // var sibling: MDNode(rank,stridable);
    
    var subranges: rank*range(stridable=stridable);
    var D_temp:    domain(rank,stridable=stridable);
    
    for d in 0..ext_d-1    do subranges(d) = Domain.dim(d);  
    for d in ext_d+1..rank-1 do subranges(d) = Domain.dim(d);
    
    
    //===> Create parent and sibling node ===>   
    
    var low_extension  = max( Domain.low(ext_d) - D.low(ext_d), 0 );
    var high_extension = max( D.high(ext_d) - Domain.high(ext_d), 0 );


    //---- Extend in the low direction, if that's the smaller extension option ----
    
    if ( low_extension < high_extension && low_extension > 0)  ||  ( high_extension == 0 )
    {
      ext_index = Domain.low(ext_d);
      
      // subranges(ext_d) = D.low(ext_d) .. ext_index-s by s;
      // D_temp           = subranges;
      // sibling          = new MDNode( rank, stridable, D_temp, false );
      
      subranges(ext_d) = D.low(ext_d) .. Domain.high(ext_d) by s;
      D_temp           = subranges;
      parent = new unmanaged MDNode( rank, stridable, D_temp,
                                     ext_d, ext_index,
                                     // parent.left = sibling
                                     nil, _to_unmanaged(this) );
    }
    
    //---- Otherwise, extend in the high direction ----
    
    else
    {
  
      ext_index = Domain.high(ext_d) + s;

      // subranges(ext_d) = ext_index .. D.high(ext_d) by s;
      // D_temp           = subranges;
      // sibling          = new MDNode( rank, stridable, D_temp, false );
      // sibling.filled   = false;
      
      subranges(ext_d)  = Domain.low(ext_d) .. D.high(ext_d) by s;
      D_temp            = subranges;
      parent = new unmanaged MDNode( rank, stridable, D_temp,
                                     ext_d, ext_index,
                                     // parent.right = sibling
                                     _to_unmanaged(this), nil );
    }
    
    //<=== Create parent and sibling node <===
    
    
    //---- Now extend the parent; the new root returns down the call chain ----
    
    new_root = parent!.extendToContain( D );
    
  }
  
  
  return new_root!;
  
}

// /|""""""""""""""""""""""""""""""""""""""|
//< |    method: MDNode.extendToContain    |
// \|______________________________________|











//|\"""""""""""""""""""""""""""""|\
//| >    method: MDNode.merge    | >
//|/_____________________________|/

//-------------------------------------------------------------
// Merges one MDNode with another.  The caller acquires all
// the data of the argument, and then the argument is deleted.
//-------------------------------------------------------------

proc MDNode.merge( node: unmanaged MDNode )
{
  
  this.Domain     = node.Domain;
  this.bisect_dim = node.bisect_dim;
  this.right_low  = node.right_low;
  this.left       = node.left;
  this.right      = node.right;
  
  delete node;
  
}
// /|""""""""""""""""""""""""""""|
//< |    method: MDNode.merge    |
// \|____________________________|
