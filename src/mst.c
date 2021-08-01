#include "stdio.h"
#include "limits.h"
#include "stdbool.h"

#include "const.h"

// Prim's minimum spanning tree algorithm
// graph: adjacency matrix representation of the nodes/rooms
int* primMST(int graph[ROOM_COUNT][ROOM_COUNT]) {

	// create the array of parents to be returned
	static int parentRooms[ROOM_COUNT];
	
	int key[ROOM_COUNT];
	bool mstSet[ROOM_COUNT];

	// all keys should start as infinite
	for (int i = 0; i < ROOM_COUNT; i++) {
		key[i] = INT_MAX;
		mstSet[i] = false;
	}

	// always start with the first vertext int MST
	// making the key 0, so that its always picked first
	key[0] = 0;
	parentRooms[0] = -1; // the first node has no parent

	for (int c = 0; c < ROOM_COUNT - 1; c++) {

		// pick the minimum key from the set of nodes that are not yet included
		int min = INT_MAX;
		int min_index;

		for (int v = 0; v < ROOM_COUNT; v++) {
			if (mstSet[v] == false && key[v] < min) {
				min = key[v];
				min_index = v;
			}
		}
		printf("min index is %d\n", min_index);	
		// add the picked node to the set
		mstSet[min_index] = true;

		// Update key values and parent index

		for (int v = 0; v < ROOM_COUNT; v++) {
			if (graph[min_index][v] && mstSet[v] == false && graph[min_index][v] < key[v]) {
				parentRooms[v] = min_index;
				key[v] = graph[min_index][v];
			}
		}
	}
	
	return parentRooms;

}

