#include <string>
#include <iostream>
#include <queue>
#include <algorithm>	// Library for 'copy' 

using namespace std;

struct Node{					
	int *state;		// The state that current Node is in	
	Node *parentNode;	// Pointer to parent Node of current Node
	
	/* Order of string array 'actions: {"up", "down", "left", "right"}
		String value for action will be "" if action not possible
		i.e. {"", "down", "left", "right"} if can't move up.
	*/
	//string *actions;	// Actions current state can take 
	string action;		// Last action taken to get to curr state

	int path_cost;		// Cost of path to current state of Node	
	int depth;		// Depth of current Node
	int cost2go;		// Estimated cost of path left from current state to goal state
};

Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go);

bool operator<(const Node &a, const Node &b)
{
	return a.path_cost > b.path_cost;
}

void flip_tiles(int *stateTiles, string actionReq){
	/* Find blank tile. In this case, blank tile is
	      gauranteed, so no error checking needed.
	*/	
	int blankLocation;
	for (int i = 0; i<9; i++){
		if (stateTiles[i] == 0){
			blankLocation = i;
		}
	}

	/* Switch tiles. Actions requested are required to 
	      be valid before this function is called, so 
	      no error checking needed.
	*/
	int oldTile;
	if (actionReq == "up") {
		oldTile = stateTiles[blankLocation-3];
		stateTiles[blankLocation-3] = 0;
		stateTiles[blankLocation] = oldTile;
	}
	else if (actionReq == "down") {
		oldTile = stateTiles[blankLocation+3];
		stateTiles[blankLocation+3] = 0;
		stateTiles[blankLocation] = oldTile;
	}
	else if (actionReq == "left") {
		oldTile = stateTiles[blankLocation-1];
		stateTiles[blankLocation-1] = 0;
		stateTiles[blankLocation] = oldTile;
	}
	else if (actionReq == "right") {
		oldTile = stateTiles[blankLocation+1];
		stateTiles[blankLocation+1] = 0;
		stateTiles[blankLocation] = oldTile;
	}
	else {
		// TODO error message?
	}
}

Node *ChildNode(Node *currNode, string actionReq){
	/* Node to be returned as child Node. 
	   Will be empty if action is not allowed */
	Node *childNode;		
	int *stateTiles = new int[9];
	
	/* Creates a copy of the current node's state
	      and saves it into the new child node */
	// TODO is there a better way to do this?
	int *oldStateTiles = currNode->state;
	int *end = &oldStateTiles[9];
	int *start = &oldStateTiles[0];	
	std::copy(start, end, stateTiles);  
	//childNode->state = stateTiles;

	int path_cost = currNode->path_cost;
	int depth = (currNode->depth);
	int cost2go = currNode->cost2go;
	path_cost++;
	depth++;
	cost2go++;
	
	/* Perform action required by flipping tile with adjacent direction
		Then, increment path_cost, depth and cost2go */
	if (actionReq == "up") {
		// Checks if 0 is in the top row
		if ((stateTiles[0] == 0)||(stateTiles[1] == 0)||(stateTiles[2] == 1)) {
			childNode = NULL;
		}
		else {
			// Left child of 0
			flip_tiles(stateTiles, actionReq);
			childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
		}
	}
	else if (actionReq == "down") {
		// Checks if 0 is in the bottom row
		if ((stateTiles[6] == 0)||(stateTiles[7] == 0)||(stateTiles[8] == 1)) {
			childNode = NULL;
		}
		else {
			// Right child of 0
			flip_tiles(stateTiles, actionReq);
			childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
		}
	}
	else if (actionReq == "left") {
		// Checks if 0 is in the left column
		if ((stateTiles[0] == 0)||(stateTiles[3] == 0)||(stateTiles[6] == 1)) {
			childNode = NULL;
		}
		else {
			// Upper child of 0
			flip_tiles(stateTiles, actionReq);
			childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
		}
	}
	else if (actionReq == "right") {
		// Checks if 0 is in the right column
		if ((stateTiles[2] == 0)||(stateTiles[5] == 0)||(stateTiles[8] == 1)) {
			childNode = NULL;
		}
		else {
			// Lower child of 0
			flip_tiles(stateTiles, actionReq);
			childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
		}
	}
	else{
		cout << ("Error");
    	exit (EXIT_FAILURE);

	}


	return childNode;
}

int Cost2Go(Node *current) {
	int sum = 0;
	// Cost array for each number in state
	int costs[9][9] = {
		{ 0,1,2,1,2,3,2,3,4 }, //zero costs
		{ 1,0,1,2,1,2,3,2,3 }, //one costs
		{ 2,1,0,3,2,1,4,3,2 }, //two costs
		{ 1,2,3,0,1,2,1,2,3 }, //three costs
		{ 2,1,2,1,0,1,2,1,2 }, //four costs
		{ 3,2,1,2,1,0,3,2,1 }, //five costs
		{ 2,3,4,1,2,3,0,1,2 }, //six costs
		{ 3,2,3,2,1,2,1,0,1 }, //seven costs
		{ 4,3,2,3,2,1,2,1,0 }  //eight costs
	};

	for (int i = 0; i < 9; i++) {
		int x = current->state[i];
		sum = sum + costs[i][x];
	}

	return sum;
}

bool GoalCheck(Node *current){
	int goal[] ={0,1,2,3,4,5,6,7,8};
	int x = 0;
	while(x < 9){
		if(current->state[x] != goal[x]){
			return false;
		}
		x++;		
	}
	return true;
}

void print_to_screen(Node *state){
	cout << "Step " << state->depth << ": " << state->action << endl;				
	for(int i = 0; i<9; ){
		for(int j = 0; j<3; j++){		
			cout << state->state[i] << " ";
			i++;
	   	}
		cout << endl;
	}
}

/**
Will print out the solution
*/
Node *Solution(Node *n){	
	if(n->parentNode != NULL){
		Solution(n->parentNode);
	}
	print_to_screen(n);
	return n;

}

bool Contains(priority_queue<Node> *queue, Node* element){
	bool result = false;
	int size = queue->size();
	while(!result && size > 0 ){
		Node *temp = new Node(queue->top());
		if(temp == element){
			result = true;
		}
		queue->push(*temp);
		size--;
	}
	return result;
}
int getValue(priority_queue<Node> *queue, Node* element){
	while(true){
		Node *temp = new Node(queue->top());
		if(temp == element){
			return temp->path_cost;
		}
		queue->push(*temp);
	}
}

void ReplaceValue(priority_queue<Node> *frontier, Node* child, int pc){
	bool replaced = false;
	while(!replaced){
		Node *temp = new Node(frontier->top());
		if(temp == child){
			frontier->pop();
			replaced = true;
		}
		else {frontier->push(*temp);}
	}
}
Node *UCS(Node *startNode){
	bool solution = false;
	priority_queue<Node> *explored; 
	priority_queue<Node> *frontier; 
	explored = new priority_queue<Node>;
	frontier = new priority_queue<Node>;

	cout << "hi" << endl;
	
	
	frontier->push(*startNode);
	Node *child;
	Node *OfficialNode;
	while(!frontier->empty() || !solution){

		*child = frontier->top();
		frontier->pop();

		if(GoalCheck(child)){ 
			return child;
		}
		explored->push(*child);
		//for all actions, determine is lowNode can make them, 
		string actions[] = {"up", "down", "left", "right"};
		for(int i = 0; i < 4; i++){
			OfficialNode = ChildNode(child, actions[i]);
			if(OfficialNode != NULL){
				if(!Contains(frontier, child) && !Contains(explored, child)){
					frontier->push(*child);
				}
				else if(Contains(frontier, child)){
					if(getValue(frontier, child) > child->path_cost){
						ReplaceValue(frontier, child, child->path_cost);
						frontier->push(*OfficialNode); //replace node
					}
				}
			}

	}
			
}
if(frontier->empty()){ return NULL;}
return NULL; //should never reach this point

}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "None", 0, 0, 0);

	// Initialize goal state
	int goalState[] = {0,1,2,3,4,5,6,7,8};
	
	// TODO remove test
	//print_to_screen(startNode);

	// Define explored and frontier priority queues
	

	// Generate child nodes
	//Node *child = ChildNode(startNode, "left");
	
	//cout<<"Start: "<<endl;
	//print_to_screen(startNode);
	//cout<<"Child: "<<endl;
	//print_to_screen(child);
	UCS(startNode);
	/**while(!GoalCheck(child)){
		//do algorithm
	}
	*/
	Solution(startNode);		//this will print the solution

	// Deallocation of memory
	//TODO:; some sort of while loop to delete;
	delete startNode;
	//delete child;

 	return 0;
}

Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go){

	// Allocate memory for new node
	Node *newNode = new Node;	

	newNode->state = state;
	newNode->parentNode = parentState;
	newNode->action = action;
	newNode->path_cost = path_cost;
	newNode->depth = depth;
	newNode->cost2go = cost2go;

	return newNode;
}

