/*NAME AND DATE GOES HERE.*/
/*Brandeis Map*/

/*Standard system stuff - these are the ONLY ones that may be used.*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*Path to the map folder; may need to change this.*/
#include "MapPATH.h"

/*MAY NOT be modified, but should be viewed.*/
#include "Map.h" /*Map data parameters, structures, and functions.*/

/*MAY NOT be modified, and there is no need view them:*/
#include "MapData.h"   /*Functions to input the map data.*/
#include "MapInput.h"  /*Functions to get user input.*/
#include "MapOutput.h" /*Functions to produce output.*/

/*Use this to get the time to travel along an edge.*/
#define EdgeCost(X) ( (TimeFlag) ? Time(X) : Elength[X] )

#define MAX_INT (~(1<<31)) /*define a maximum integer*/

/*************************************************************************************/
/*PRINT ONE LEG OF A ROUTE                                                           */
/*(Defined in Output.h; this prototype shows how to use it.)                         */
/*************************************************************************************/
void PrintLeg(int edge);

/***************************************************************************************/
/*GRAPH ADJACENCY LIST DATA STRUCTURE                                                  */
/***************************************************************************************/
struct edge{
	int index;
	struct edge *next;
};

struct vertexArray{
	struct edge ** VertexArray;
};

/*array: store all vertices, each vertex linked to its edge list*/
struct vertexArray array;

void buildAdjacencyList(){
	/*initial array size*/
	array.VertexArray = (struct edge **)malloc(sizeof(struct edge *) * MaxVertex);//after struct, need to malloc
	
	/*initial array, each is index*/
	int i = 0;
	for(i = 0; i < nV; i ++){
		array.VertexArray[i] = NULL;
	}
	
	/*build the array*/
	for(i = 20; i < nE; i ++){
		struct edge *new_edge;
		new_edge = (struct edge *)malloc(sizeof(struct edge));//after struct, need to malloc

			new_edge->index = i;
			new_edge->next = NULL;
			if(array.VertexArray[Estart[i]] == NULL){
				array.VertexArray[Estart[i]] = new_edge;
			}else{
				new_edge->next = array.VertexArray[Estart[i]];
				array.VertexArray[Estart[i]] = new_edge;
			}
		
	}
	
}


/***************************************************************************************/
/*HEAP DATA STRUCTURE                                                                  */
/***************************************************************************************/
typedef struct node{
	int index;
	int distance;
}node;

typedef struct heap{
	node *node_array;
	int node_num;
}heap;

/*instantiate*/
heap *myHeap;

/*maintain the heap structure after a insert*/
void maintainHeapAfterInsert(heap *myHeap){
	int i;
	for(i = myHeap->node_num - 1; i > 0; i --){
		/*find parent's index*/
		int current_index = i;
		int parent_index;
		if(current_index%2 == 1){
			parent_index = (current_index-1)/2;
		}else{
			parent_index = (current_index-2)/2;
		}
		/*check if parent.dis > current.dis*/
		if(parent_index >= 0 && (myHeap->node_array[parent_index].distance > myHeap->node_array[current_index].distance)){
			node *transfer;
			transfer = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
			*transfer = myHeap->node_array[parent_index];
			myHeap->node_array[parent_index] = myHeap->node_array[current_index];
			myHeap->node_array[current_index] = *transfer;
		}
	} 
}

/*maintain the heap after a delete*/
void maintainHeapAfterDelete(heap *myHeap){//up to down
	int i = 0;
	for(i = 0; i < myHeap->node_num; i ++){
		/*fina children's index*/
		int current_index = i;
		int left_index = current_index * 2 + 1;
		int right_index = current_index * 2 + 2;
		/*compare with children*/
		if(left_index <= myHeap->node_num - 1 && right_index <= myHeap->node_num - 1){//check if both children is valid.
			if(myHeap->node_array[current_index].distance > myHeap->node_array[left_index].distance){
				node *transfer;
				transfer = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
				*transfer = myHeap->node_array[current_index];
				myHeap->node_array[current_index] = myHeap->node_array[left_index];
				myHeap->node_array[left_index] = *transfer;
			}
			if(myHeap->node_array[current_index].distance > myHeap->node_array[right_index].distance){
				node *transfer2;
				transfer2 = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
				*transfer2 = myHeap->node_array[current_index];
				myHeap->node_array[current_index] = myHeap->node_array[right_index];
				myHeap->node_array[right_index] = *transfer2;
			}
		}else if(left_index <= myHeap->node_num - 1 && right_index > myHeap->node_num - 1){//check if left is valid, right is not
			if(myHeap->node_array[current_index].distance > myHeap->node_array[left_index].distance){
				node *transfer;
				transfer = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
				*transfer = myHeap->node_array[current_index];
				myHeap->node_array[current_index] = myHeap->node_array[left_index];
				myHeap->node_array[left_index] = *transfer;
			}
		}else if(left_index > myHeap->node_num - 1){//check if both children are not valud.
			break;
		}		
	}
	
	
}

void insertHeap(heap *myHeap, node *myNode){
	myHeap->node_array[myHeap->node_num] = *myNode;
	myHeap->node_num ++;
	maintainHeapAfterInsert(myHeap);
}

node deleteHeap(heap *myHeap){
	/*fina which node to return*/
	node *minNode;
	minNode = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
	*minNode = myHeap->node_array[0];
	
	/*delete the vertex*/
	myHeap->node_array[0] = myHeap->node_array[myHeap->node_num - 1];
	myHeap->node_num --;	
	maintainHeapAfterDelete(myHeap);
	return *minNode;
	
}


/***************************************************************************************/
/*Do Dijkstra algorithm, reverse back edges, print path with PrintLeg function.        */
/***************************************************************************************/
int isEmpty(node u[]){
	int i = 0;
	for(i = 0; i < nV; i ++){
		if(u[i].index == i){
			return 0;
		}
	}
	return 1;
}


void Dijkstra() {
	/*create a file to store output*/
//	FILE *Output;
//	Output = fopen("E:\\Brandeis_Study_file\\cs180\\final project\\Output.txt", "w");
	
	buildAdjacencyList(); 
	int vertex_num = nV;
	node s[vertex_num];//array to record which node are done.
	node u[vertex_num];//array to record which node are not done.
	int prev[vertex_num];//store each vertex's previous vertex
	int e[MaxEdge];//store the edge indexs to be printed.
	int j = 0;
	int i = 0;
	
	/*source's previous is itself*/
	prev[Begin] = Begin;
	
	/*instantiate*/
	myHeap = (struct heap*)malloc(sizeof(struct heap));
	myHeap->node_array = (struct node *)malloc(sizeof(struct node) * MaxVertex * 10);//after struct, need to malloc
	myHeap->node_num = 0;
	
	
	/*store the source node into "s" array*/
	node *source;
	source = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
	source->index = Begin;
	source->distance = 0;
	s[Begin] = *source;//"s" array stores the source.
	
	/*initial the "u" array, and add all other nodes into heap*/
	for(i = 0; i < nV; i ++){
		if(i != Begin){
			node *node;
			node = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
			node->distance = MAX_INT;
			node->index = i;
			u[i] = *node;
			insertHeap(myHeap, node);//add into heap
		}else{
			node *node;
			node = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
			node->distance = 0;
			node->index = nV+nV;
			u[i] = *node;
		}
	}//now, heap and "u" stores every node but source.
	
	/*find the vertices which are closed to the source, and update "u" array*/
	struct edge *edge_of_vertex;
	edge_of_vertex = (struct edge *)malloc(sizeof(struct edge));//after struct, need to malloc
	edge_of_vertex = array.VertexArray[Begin];
	while(edge_of_vertex != NULL){
		int edge_index;
		edge_index = edge_of_vertex->index;
		int end_vertex;
		end_vertex = Eend[edge_index];
		int distance;
		distance = EdgeCost(edge_index);//EdgeCost(X)
		
		node *myNode;
		myNode = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
		myNode->distance = distance;
		myNode->index = end_vertex;
		u[end_vertex] = *myNode;
		insertHeap(myHeap, myNode);//add into heap
		
		/*set prev[] for source*/
		prev[end_vertex] = Begin;
		
		/*get next edge*/
		edge_of_vertex = edge_of_vertex->next;
	}
	
	/*find every node's shortest distance to the source.*/
	while(!isEmpty(u)){
		/*the node from the heap*/
		node *returnNode;
		returnNode = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
		*returnNode = deleteHeap(myHeap);//first returned node is not source
		
		/*update "u" and "s" array*/
		s[returnNode->index] = *returnNode;
		u[returnNode->index].index = nV+nV;
		
		/*continue to update "u" array*/
		struct edge *edge_of_vertex;
		edge_of_vertex = (struct edge *)malloc(sizeof(struct edge));
		edge_of_vertex = array.VertexArray[returnNode->index];
		while(edge_of_vertex != NULL){
			/*find its adjacent vertex*/
			int edge_index;
			edge_index = edge_of_vertex->index;
			int end_vertex;
			end_vertex = Eend[edge_index];
			/*check if this vertex has been done*/
			if(u[end_vertex].index == end_vertex){
				int length;
				length = EdgeCost(edge_index);//EdgeCost(X)
				if((length + returnNode->distance) < u[end_vertex].distance){//if (s, k) + (k, v) < (s, v)
					prev[end_vertex] = returnNode->index;
					u[end_vertex].distance = length + returnNode->distance;
					node *myNode;
					myNode = (struct node *)malloc(sizeof(struct node));//after struct, need to malloc
					myNode->distance = length + returnNode->distance;
					myNode->index = end_vertex;
					insertHeap(myHeap, myNode);
				}
				/*check next edge*/
				edge_of_vertex = edge_of_vertex->next;
			}else{
				/*check next edge*/
				edge_of_vertex = edge_of_vertex->next;
			}			
		}
		
	}
	
	/*find the edges between "end vertex" and "start vertex"*/
	/*end -> ... -> start*/
	int current_vertex = Finish;
	int previous_vertex = prev[current_vertex];
	int edge_number = 0;
	while(current_vertex != Begin){
		for(i = 0; i < nE; i ++){
			if(Estart[i] == previous_vertex && Eend[i] == current_vertex){
				e[edge_number] = i;
				edge_number ++;
			}
		}
		current_vertex = previous_vertex;
		previous_vertex = prev[current_vertex];
 	}
 	/*print all edges from "Begin" to "Finish"*/
 	for(i = edge_number - 1; i >= 0; i --){
 		PrintLeg(e[i]);
 	}
	
}


/***************************************************************************************/
/*CAMPUS TOUR                                                                          */
/***************************************************************************************/
#include "Tour.h"


/***************************************************************************************/
/*MAIN PROGRAM (don't modify)                                                          */
/***************************************************************************************/
int main() {
	GetVertices();
	GetEdges();	
	while (GetRequest()) {
		RouteOpen(); 
		TourFlag ? Tour() : Dijkstra(0); 
		RouteClose();
	}
	return(0);
}
