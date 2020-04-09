//This code is based on the XMT version of the Graph5000 benchmark submitted by
//John Feo <john.feo@pnl.gov> and Kristi Maschhoff <kristyn@cray.com> in 2010.

module Create_Parent_Tree
{
use Graph500_defs;

proc BFS ( root : vertex_id, ParentTree, G )
{

  type Vertex_List = domain ( vertex_id );
  var visited$ : [vertex_domain] sync int = -1;

  var Active_Level = new Level_Set (Vertex_List);
  var Next_Level = new Level_Set (Vertex_List);

  var Root_vertex : vertex_id = root;

  ParentTree[root] = root;
  Active_Level.Members.add ( Root_vertex );
  Next_Level.Members.clear ();

  visited$ (root).writeFF(1);
  Active_Level.previous = nil;
  Next_Level.previous = Active_Level;

  while Active_Level.Members.size > 0 do
  {

    forall u in Active_Level.Members do {

      forall v in G.Neighbors (u) do {

        if ( visited$ (v).readXX() < 0 ) 
        {
          if (visited$ (v).readFE() < 0 )
          {
              visited$ (v).writeEF (1);
              Next_Level.Members.add (v);
              ParentTree (v) = u;
          }
          else
          {
              visited$ (v).writeEF(1);
          }
        }
      }
    }


    delete Active_Level;
    Active_Level = Next_Level;
    Next_Level = new Level_Set (Vertex_List);

    Next_Level.Members.clear ();
    Next_Level.previous = Active_Level;

  }

  delete Active_Level;
  delete Next_Level;
}


}



