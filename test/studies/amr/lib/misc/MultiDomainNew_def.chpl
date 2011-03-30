
use BasicDataStructures;



proc main {
  
      
  var mD = new MultiDomainNew( 2, false );
  mD.add( [1..10,1..10] );
  
  
  mD.subtract( [3..5, 4..6] );
  mD.subtract( [7..8, 2..9] );  
  mD.add( [11..12, 13..14] );  

  mD.prettyPrint();
    
  mD.subtract( [-3..15, -3..15] );
  // 
  // mD.add( [1..10,1..10] );

  writeln("");  
  mD.prettyPrint();
  
  
  for node in mD.nodes() {
    writeln("");
    write("Parent: ", node.Domain);
    if node.left
    {
      writeln("");
      write("  Left child:  ", node.left.Domain);
      if node.left.left == nil then
        write(" filled = ", node.left.filled);
      writeln("");
  
      write("  Right child: ", node.right.Domain);
      if node.right.left == nil then
        write(" filled = ", node.right.filled);
      writeln("");
    }
    else
      writeln("  filled = ", node.filled);
    
  }

  
}





//|\"""""""""""""""""""""""""""|\
//| >    class: MultiDomainNew    | >
//|/___________________________|/

//--------------------------------------------------------------------
// The MultiDomainNew class is used to describe an index set composed
// of a union of domains.  It relies on a binary tree representation,
// adapted from the kd-tree data structure, with nodes that are 
// members of the MDNode class.
//--------------------------------------------------------------------

class MultiDomainNew
{
  
  param rank:      int;
  param stridable: bool;
      
  var root: MDNode(rank,stridable);


  proc initialize ()
  {
    root = new MDNode( rank, stridable );
    // root.Domain initializes to empty automatically
    root.filled = false;
  }


  proc clear ()
  {
    root.clearChildren();
    delete root;
  }


  proc isEmpty ()
  {
    //---- Just checking !root.filled should suffice; being safe for now ----
    
    return (!root.filled && root.left==nil);
  }


  proc subtract ( D: domain(rank,stridable=stridable) )
  {
    if root.Domain(D).numIndices > 0 then root.incorporate( D, false );
  }  
  
  
  proc add ( D: domain(rank,stridable=stridable) )
  {
    
    if root.Domain.numIndices > 0 then {
      root = root.extendToContain( D );
      root.incorporate( D, true ); 
    }
    else {
      root.Domain = D;
      root.filled = true;
    }
  }
  
  
  proc add ( domain_collection )
    where domain_collection.type != domain(rank,stridable=stridable)
  {
    for D in domain_collection do add(D);
  }
  
  
  // proc add ( mD: MultiDomainNew(rank,stridable) )
  // {
  //   for D in mD do add(D);
  // }
  // 
  // proc add ( dList: List( domain(rank,stridable=stridable) ) ) 
  // {
  //   for D in dList do add(D);
  // }
  
  
  
  
  
  proc intersect ( D: domain(rank,stridable=stridable) )
  {
    if root.Domain(D).numIndices > 0 then 
      root.intersect( D );
    else {
      root.clearChildren();
      root.Domain.clear();
      root.filled    = false;
      root.sep_d     = 0;
      root.sep_index = 0;
    }
  }
  
  
  
  
  
  iter these ()
  {
    
    const q = new Queue( MDNode(rank,stridable) );
    var node: MDNode(rank,stridable);

    if root then q.enqueue( root );
    
    while !q.isEmpty()
    {
      node = q.dequeue();
      
      if node.left {
        q.enqueue( node.left );
        q.enqueue( node.right );        
      }
      else if node.filled then
        yield node.Domain;
    }
  }
  
  
  
  iter nodes () : MDNode(rank,stridable)
  {
    
    const q = new Queue( MDNode(rank,stridable) );
    var node: MDNode(rank,stridable);
  
    if root then q.enqueue( root );
    
    while !q.isEmpty()
    {
      node = q.dequeue();
      yield node;
      
      if node.left {
        q.enqueue( node.left );
        q.enqueue( node.right );        
      }
    }
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
//< |    class: MultiDomainNew    < |
// \|___________________________\|





//|\""""""""""""""""""""""|\
//| >    class: MDNode    | >
//|/______________________|/

//-------------------------------------------------------------------
// The MDNode class represents a node in the tree structure that 
// constitutes a MultiDomainNew.  It consists of a domain, a descriptor 
// of whether it is filled, a description of a hyperplane separating 
// its domain, and left/right child nodes.
//
// 'filled' indicates that some subset of the node's Domain is filled
// in, though it may be farther down the tree. 
//
// The separatrix is described by sep_d, the dimension to which it
// is orthogonal, and sep_index, its index in that dimension, which
// is the lower bound of the right child.
//------------------------------------------------------------------

class MDNode
{
  
  param rank:      int;
  param stridable: bool;
  
  var Domain:    domain(rank,stridable=stridable);
  var filled:    bool;

  var sep_d:     int = 0;
  var sep_index: int = 0;

  var left:  MDNode(rank,stridable);
  var right: MDNode(rank,stridable);

  
  

  // proc MDNode ( D: domain(rank,stridable=stridable), filled: bool )
  // {
  //   Domain      = D;
  //   this.filled = filled;
  // }
  // 
  // 
  // proc MDNode ( subranges: rank*range, filled: bool )
  // {
  //   Domain      = subranges;
  //   this.filled = filled;
  // }


  proc clearChildren () {

    if sep_index > 0 {
      
      left.clearChildren();
      right.clearChildren();
      
      delete left;
      left = nil;
      delete right;
      right = nil;
      
    }
    
  }

}

// /|""""""""""""""""""""""/|
//< |    class: MDNode    < |
// \|______________________\|




//|\"""""""""""""""""""""""""""""""""""|\
//| >    method: MDNode.incorporate    | >
//|/___________________________________|/

//-------------------------------------------------------------------
// This method incorporates the input domain D and its fill status
// into the substructure of the MDNode.  This process implements
// both addition and subtraction, with addition corresponding to
// D_filled=true and subtraction to D_filled=false.
//
// This method is meant to be called either by a MultiDomainNew acting
// on its root, or by a parent MDNode acting on its child.  In
// either case, the caller must ensure that D and this.Domain are
// nondisjoint, as several bound comparisons assume so.
//-------------------------------------------------------------------

proc MDNode.incorporate( D: domain(rank,stridable=stridable), D_filled: bool )
{    
  
  // writeln("Beginning MDNode.incorporate on node with ", Domain, " ", left!=nil, " ", filled);
  
  //---- If children exist, incorporate D into them, and possibly unsplit ----
  
  if sep_d > 0 {
        
    if sep_index >  D.low(sep_d)  then left.incorporate( D, D_filled );
      
    if sep_index <= D.high(sep_d) then right.incorporate( D, D_filled );


    //---- Eliminate children if possible ----
    
    if (left.sep_d==0 && right.sep_d==0) &&
       (left.filled == right.filled)
    {
        this.filled    = left.filled;
        this.sep_d     = 0;
        this.sep_index = 0;
        
        delete left;
        left = nil;
        delete right;
        right = nil;
    }    


  }
    
  
  //---- Otherwise, either split this node or adjust its fill status ----
  
  else {
    
    //===> Choose the dimension to split ===>

    var dim_length     = 0;
    var max_dim_length = 0;
    sep_d = 0;
    
    for d in 1..rank
    {

      //--------------------------------------------------------------------
      // We know that D and Domain are not disjoint, so dimension d may be 
      // split as long as D.dim(d) does not strictly contain Domain.dim(d).
      //--------------------------------------------------------------------

      if D.low(d) > Domain.low(d) || D.high(d) < Domain.high(d)
      {

        dim_length = Domain.dim(d).length;

        if dim_length > max_dim_length {
          max_dim_length = dim_length;
          sep_d = d;
        }

      }
      
    }
    
    //<=== Choose the dimension to split <===
  
    
    
    //---- sep_d=0 indicates that D contains Domain, so the fill status should match D ----
    
    if sep_d == 0 then filled = D_filled;

    //---- Otherwise, split the node, and call 'incorporate' on a child ----

    else
    {
      
      //---- Cut low or high to best balance the children ----
      
      if D.low(sep_d) - Domain.low(sep_d) > Domain.high(sep_d) - D.high(sep_d) then
        sep_index = D.low(sep_d);
      else
        sep_index = D.high(sep_d) + Domain.stride(sep_d);
    
    
    
      //---- Create children ----
      
      var subranges: rank*range(stridable=stridable);
      var D_temp:    domain(rank, stridable=stridable);
    
      for d in 1      ..sep_d-1 do subranges(d) = Domain.dim(d);
      for d in sep_d+1..rank    do subranges(d) = Domain.dim(d);

      subranges(sep_d) = Domain.dim(sep_d) ( ..sep_index-Domain.stride(sep_d) );
      D_temp = subranges;
      left = new MDNode( rank, stridable, D_temp, filled );      
      // left = new MDNode(subranges, filled);
    
      subranges(sep_d) = Domain.dim(sep_d) ( sep_index.. );
      D_temp = subranges;
      right = new MDNode( rank, stridable, D_temp, filled );
      // right = new MDNode(subranges, filled);
      

      //---- Incorporate D into the approprate child ----
      
      if D.low(sep_d) < sep_index then left.incorporate( D, D_filled );
      else                             right.incorporate( D, D_filled );
    
    }
    
  }
  
}

// /|"""""""""""""""""""""""""""""""""""/|
//< |    method: MDNode.incorporate    < |
// \|___________________________________\|




//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    method: MDNode.extendToContain    | >
//|/_______________________________________|/

//---------------------------------------------------------------
// This method extends the existing MDNode through a series
// of extensions acroos hyperplanes, creating a structure that
// contains the input domain D.  The return value is a reference
// to the new root of the structure.
//---------------------------------------------------------------

proc MDNode.extendToContain( D: domain(rank,stridable=stridable) ) : MDNode(rank,stridable)
{
  
  // writeln("Beginning MDNode.extendToContain on node with ", Domain, " ", left!=nil, " ", filled);
  // writeln("  Input domain is ", D);
  
  var new_root: MDNode(rank,stridable);
  
  
  //===> Select the shortest dimension to extend ===>
  
  var dim_length     = 0;
  var min_dim_length = Domain.numIndices+1;
  var ext_d          = 0;
    
  for d in 1..rank {
    
    if D.high(d) > Domain.high(d) || D.low(d) < Domain.low(d) {
      dim_length = Domain.dim(d).length;
      
      if dim_length < min_dim_length {
        min_dim_length = dim_length;
        ext_d = d;
      }
    }
    
  }
  
  //<=== Select the shortest dimension to extend <===
  
  
  //---- If ext_d==0, that means D is contained in this.Domain, and this is the new root ----
  
  if ext_d == 0 then new_root = this;
  
  
  //---- Otherwise, we split along dimension ext_d ----
    
  else {

    //---- Declare and prepare ----
    
    var s = Domain.stride(ext_d);
    var ext_index: int;
    
    var parent:  MDNode(rank,stridable);
    var sibling: MDNode(rank,stridable);
    
    var subranges: rank*range(stridable=stridable);
    var D_temp:    domain(rank,stridable=stridable);
    
    for d in 1..ext_d-1    do subranges(d) = Domain.dim(d);  
    for d in ext_d+1..rank do subranges(d) = Domain.dim(d);
    
    
    //===> Create parent and sibling node ===>   
    
    //---- Extend in the low direction, if that's the smaller extension option ----
    var low_extension  = max( Domain.low(ext_d) - D.low(ext_d), 0 );
    var high_extension = max( D.high(ext_d) - Domain.high(ext_d), 0 );
    
    if ( low_extension < high_extension && low_extension > 0)  ||  ( high_extension == 0 )
    {
      ext_index = Domain.low(ext_d);
      
      subranges(ext_d) = D.low(ext_d) .. ext_index-s by s;
      D_temp           = subranges;
      sibling          = new MDNode( rank, stridable, D_temp, false );
      
      subranges(ext_d) = D.low(ext_d) .. Domain.high(ext_d) by s;
      D_temp           = subranges;

      parent           = new MDNode( rank, stridable, D_temp, this.filled );
      parent.sep_d     = ext_d;
      parent.sep_index = ext_index;
      
      parent.left  = sibling;
      parent.right = this;
       
    }
    
    //---- Otherwise, extend in the high direction ----
    
    else
    {
  
      ext_index = Domain.high(ext_d) + s;

      subranges(ext_d) = ext_index .. D.high(ext_d) by s;
      D_temp           = subranges;
      // sibling          = new MDNode( subranges, false );
      sibling          = new MDNode( rank, stridable, D_temp, false );
      sibling.filled   = false;
      
      subranges(ext_d)   = Domain.low(ext_d) .. D.high(ext_d) by s;
      D_temp             = subranges;
      parent             = new MDNode( rank, stridable, D_temp, this.filled );
      // parent             = new MDNode( subranges, true );
      parent.sep_d       = ext_d;
      parent.sep_index   = ext_index;
      
      parent.left  = this;
      parent.right = sibling;
    }
    
    //<=== Create parent and sibling node <===
    
    
    //---- Now extend the parent; the new root returns down the call chain ----
    
    new_root = parent.extendToContain( D );
    
  }
  
  
  return new_root;
  
}

// /|""""""""""""""""""""""""""""""""""""""|
//< |    method: MDNode.extendToContain    |
// \|______________________________________|





//|\"""""""""""""""""""""""""""""""""|\
//| >    method: MDNode.intersect    | >
//|/_________________________________|/

//----------------------------------------------------
// Intersects a node's subtree with the input domain.
//----------------------------------------------------

proc MDNode.intersect ( D: domain(rank,stridable=stridable) )
{
    
  //---- If the node is split... ----
  
  if sep_d > 0 {

    //---- Determine which children D intersects ----

    var intersects_left, intersects_right: bool = false;

    if D.low(sep_d)  <  sep_index then intersects_left  = true;
    if D.high(sep_d) >= sep_index then intersects_right = true;
  
  
    //---- Modify the children appropriately ----
  
    if intersects_left && intersects_right {
      left.intersect(D);
      right.intersect(D);
      Domain = Domain(D);
    }
  
    else if intersects_left {
      right.clearChildren();
      delete right;
      right = nil;
      
    
      left.intersect(D);
      merge(left);
    }
    
    else {
      left.clearChildren();
      delete left;
      left = nil;
      
      right.intersect(D);
      merge(right);
    }
    
  }
  
  //---- If this is a leaf; shrink its Domain ----
  
  else Domain = Domain(D);  
  
}

// /|""""""""""""""""""""""""""""""""|
//< |    method: MDNode.intersect    |
// \|________________________________|





//|\"""""""""""""""""""""""""""""|\
//| >    method: MDNode.merge    | >
//|/_____________________________|/

//-------------------------------------------------------------
// Merges one MDNode with another.  The caller acquires all
// the data of the argument, and then the argument is deleted.
//-------------------------------------------------------------

proc MDNode.merge( node: MDNode )
{
  
  this.Domain    = node.Domain;
  this.filled    = node.filled;
  this.sep_d     = node.sep_d;
  this.sep_index = node.sep_index;
  this.left      = node.left;
  this.right     = node.right;
  
  delete node;
  
}
// /|""""""""""""""""""""""""""""|
//< |    method: MDNode.merge    |
// \|____________________________|
