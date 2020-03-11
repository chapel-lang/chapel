//This code was based on the XMT version of the Graph5000 benchmark submitted by
//John Feo <john.feo@pnl.gov> and Kristi Maschhoff <kristyn@cray.com> in 2010.

// This version incorporates ideas from Kamesh Madduri's implementation
// in terms of using a 1D distribution of the Vertex records, maintaining 
// a distributed queue, and sending work to the locale which owns a particular
// vertex. Chapel accomplishes this using the Chapel on statement
// The beauty here is that we are able to preserve the basic simplicity and
// readability of the original XMT implementation.

module Create_Parent_Tree
{
use Graph500_defs;

proc BFS ( root : vertex_id, ParentTree, G )
{

  type Vertex_List = domain (index(vertex_domain) );
  var visited$ : [vertex_domain] sync int = -1;

  use ReplicatedVar;
  var Active_Level: [rcDomain] unmanaged Level_Set (Vertex_List)?;
  var Next_Level: [rcDomain] unmanaged Level_Set (Vertex_List)?;
  var Active_Remaining: [LocaleSpace] bool = true;

  var Root_vertex : vertex_id = root;

  coforall loc in Locales do on loc {
    rcLocal(Active_Level) = new unmanaged Level_Set (Vertex_List);
    rcLocal(Active_Level)!.previous = nil;
    rcLocal(Next_Level) = new unmanaged Level_Set (Vertex_List);
    rcLocal(Next_Level)!.previous = rcLocal(Active_Level);
  }

  on G.Vertices(root) {
    rcLocal(Active_Level)!.Members.add ( root );
    rcLocal(Next_Level)!.Members.clear ();
    ParentTree[root] = root;
    visited$ (root).writeFF(1);
    rcLocal (Active_Level)!.previous = nil;
    rcLocal (Next_Level)!.previous = rcLocal (Active_Level);
  }


  while || reduce Active_Remaining do {

    // barrier
    var count: sync int = numLocales;
    var barrier: single bool;

    coforall loc in Locales do on loc {
      forall u in rcLocal(Active_Level)!.Members do {

        forall v in G.Neighbors (u) do on v {

          if ( visited$ (v).readXX() < 0 ) 
          {
            if (visited$ (v).readFE() < 0 )
            {
                visited$ (v).writeEF (1);
                rcLocal(Next_Level)!.Members.add (v);
                ParentTree (v) = u;
            }
            else
            {
                visited$ (v).writeEF(1);
            }
          }
        }

      }

// barrier needed to insure all updates to Next_Level are complete

      var myc = count;
      if myc==1 {
        barrier=true;
      } else {
        count = myc-1;
        barrier;
      }


      delete rcLocal(Active_Level);
      rcLocal(Active_Level) = rcLocal(Next_Level);
      rcLocal(Next_Level) = new unmanaged Level_Set (Vertex_List);

      rcLocal(Next_Level)!.previous = rcLocal(Active_Level);

      Active_Remaining[here.id] = rcLocal(Active_Level)!.Members.size:bool;

      }
    }

  coforall loc in Locales do on loc {
    delete rcLocal(Active_Level);
    delete rcLocal(Next_Level);
  }
}


}



