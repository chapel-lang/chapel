use List;
use Queue;

class Graph
{
	var V: int;    // No. of vertices
	var adj: list(list(int));    
	public proc Graph(V:int){
		this.V = V;
		var i:int;
		for i in (0..V-1){
			var edge_list:list(int);
			adj.append(edge_list);
		}
	}
	
	public proc addEdge(v:int, w:int){
		adj(v).append(w);
	}
	
	public proc bfs(start_vertex:int){
		var visited: [1..V] bool;
		//Mark all the vertices as not visited		
		forall i in (0..V-1){
			visited(i) = false;
		}
		//Create a queue for BFS
    		var myQueue: queue;
		
		//Mark the current node as visited and enqueue it
		visited(start_vertex) = true;
		queue.enqueue(start_vertex);

		while(!queue.empty())
		{
			//Dequeue a vertex from queue and print it
			start_vertex = queue.front();
			write(start_vertex, " ");
			queue.dequeue();

			//Get all adjacent vertices of the dequeued vertex s
			//If a adjacent has not been visited, then mark it visited
			//and enqueue it
			//var i:list(int);
			for i in adj(start_vertex).these(){
				
	    			if(!visited(i))
	    			{
					visited(i) = true;
					queue.enqueue(i);
	    			}
				
			}	
							
		}
	} 
};
 
var g:Graph;
g = new Graph(4);

g.addEdge(0, 1);
g.addEdge(0, 2);
g.addEdge(1, 2);
g.addEdge(2, 0);
g.addEdge(2, 3);
g.addEdge(3, 3);

writeln("Following is Breadth First Traversal (starting from vertex 2)");

g.bfs(2);
