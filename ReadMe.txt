For part 1, I finished "GRAPH ADJACENCY LIST DATA STRUCTURE", "HEAP DATA STRUCTURE", and "Dijkstra Algorithm". My part 1 
works well, and its outputs are all same with the given sample output. 

In "GRAPH ADJACENCY LIST DATA STRUCTURE", I first constructed two data types: one is called "edge", the othe one is called 
"vertexArray". Each "edge" represents a node including an "int" value "index" as edge index, and a pointer called "next" to 
point to the next "edge". The "edge" type is going to be used to create a linked list for each vertex. "vertexArray" 
represents an array containing "edge ** VertexArray". For example, "array.VertexArray[i]" represents the NO.i vertex's 
edges that are all started from NO.i vertex. So, every vertex has connected to its edges.

HEAP DATA STRUCTURE: to build the heap structure, I first constructed a data type called "node". Each node contains "int"
value "index", and "distance". Then, I constructed "heap" data structure. "heap" contains "node *node_array" which points
to a node array, and also has an "int" value "node_num" which records the number of nodes the heap maintains. Next, I wrote 
"insertHeap()" function for insertion, and "deleteHeap()" function for deletion. 

Dijkstra Algorithm: This part implements Dijkstra algorithm by using "graph adjacency list" and "heap" structure. 