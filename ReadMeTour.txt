My part2A is complete. I use Prim algorithm to generate minimum spanning tree, and use Depth-first search algorithm
to traverse spanning tree in pre-order and print a cycle. 

Even though my part2B can work, it has some bugs. I haven't figured that out. The output of "Legs" is much smaller than
the number of vertices.

Prim algorithm:
1. I define an int array Vnew[nV] and Enew[nE]. Vnew[] is to record if a vertex is marked. For example, if NO.i vertex is
marked, Vnew[i] = 1, otherwise Vnew[i] = -1. In a similar way, Enew[] is to record which edge is in spanning tree. For
example, if Enew[i] = 1, then NO.i edge is in the spanning tree.
2. To do the following tasks until all vertices have been marked:
	a. find an edge in the graph which has the minimum cost (using heap), and this edge's start vertex is marked, and 
	its end vertex is not marked.
	b. put the new edge into Enew[], and marked its end vertex.

Short Cut Algorithm:
Instead of printing the edge one by one when traversing the minimum spanning tree, every time when I need to print a back
edge and return to the vertex that I already visited, I will try to find a new edge that starts from my current position,
and ends at a new unmarked vertex. Once this edge exists, compare the cost of this new edge with that of the old ones. If
this new edge is shorter, print it and make current position to the end of the new edge. If this new edge is longer, print
the old edges and move to the next vertex in the spanning tree. Until all vertices in the graph has been visited.


How to use my code?
To run part2A, please open my "Tour.h", make sure that the NO.283 line code is working. (DFS(Begin, array, Enew);), and
turn off the line NO.305. (shortCutSearch(Begin, array, vertex_order, flag, Begin);)

To generate minimum spanning tree, please open my "Tour.h", make sure that line NO.277 is working (printf("edge index:%d\n", i);)
and make sure that the line NO.283 code is not working. (DFS(Begin, array, Enew);), and turn off the line NO.305 
as well. (shortCutSearch(Begin, array, vertex_order, flag, Begin);)

To run part2B, please open my "Tour.h", make sure that line NO.277 and line NO.283 is not working, and turn on the line 
NO.305. 