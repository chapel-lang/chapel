module Queue{
	//use List;	
	class queue{
		var underlying_list:list(int);
		var front:int; //front will be either 0 or -1
		var rear:int; //rear can be -1,0,>0
		
		public proc queue(){
			rear = -1;
			front = -1;
		}		
		
		public proc enqueue(data:int){
			if(front == -1){
				//first ele added
				underlying_list.append(data);
				front = 0;
				rear = 0;
			}
			else{
				//any other element
				underlying_list.append(data);
				rear+=1;								
			}
		}
	
		public proc dequeue(){
			if(front == -1){
				//queue is empty
				writeln("Dequeue Failed:: Queue is Empty.");
			}
			else if(front == rear){
				//queue has 1 element
				var data:int = underlying_list(front);
				underlying_list.remove(data);
				rear=-1;
				front=-1;								
			}
			else{
				//queue has more than 1 element
				var data:int = underlying_list(front);
				underlying_list.remove(data);
				rear-=1;
			}
		}
		
		public proc displayQueue(){
			if(front == -1){
				writeln("Queue is empty");
			}
			else{
				var i:int;
				
				for i in this.underlying_list.these(){
					write(i, " ");
				}
				
				writeln();
				
			}
		}
	};
	
	
	var myQueue: queue;
	myQueue = new queue();
		
	myQueue.enqueue(10);
	myQueue.displayQueue();		
	myQueue.enqueue(9);
	myQueue.displayQueue();
	myQueue.enqueue(15);
	myQueue.displayQueue();
	
	myQueue.dequeue();
	myQueue.displayQueue();		
}
