#include <iostream>
#include "mlist.h"
using namespace std;

//Constructor, construct an empty doubly linked list.
MList::MList() : ntop(NULL), nbottom(NULL) { }

//Destructor, properly clears and deallocates everything of current list.
//simply call the clear function if you have already implemented that.
MList::~MList(){
    // List is empty
    if (ntop == NULL)
        return;


    clear();
}

//Note: RECURSION --- This function should be implemented using recursion.
//this function properly clears and deallocates everything of current list.
//there should be no loop in this function
void MList::clear(){
    // Base case: List is empty
    if (ntop == NULL) {
        return;
    }

    pop_top();
    clear();
}

//returns a boolean true if the list is empty, false if otherwise.
bool MList::isEmpty(){
    return ntop == NULL && nbottom == NULL;
}

/*
	Add or insert a new node with d_item as its data at the top of the list.
	You need to dynamically allocate memory for the new node.
*/
void MList::push_top(Data* d_item){
    Node* newItem = new Node;
    newItem->nodeData = d_item;
    newItem->next = NULL;
    newItem->prev = NULL;


    // List is empty
    if (ntop == NULL && nbottom == NULL) {
        ntop = newItem;
        nbottom = newItem;
        return;
    }

    // Configure newItem
    newItem->next = ntop;
    newItem->prev = NULL;

    // Configure top pointer
    ntop->prev = newItem;
    ntop = newItem;

}

/*
	Add or insert a new node with d_item as its data at the bottom of the list.
	You need to dynamically allocate memory for the new node.
*/
void MList::push_bottom(Data* d_item){
    // Dyanamically allocate new Node
    Node* newItem = new Node;
    newItem->nodeData = d_item;
    newItem->next = NULL;
    newItem->prev = NULL;

    // List is empty
    if (ntop == NULL && nbottom == NULL) {
        ntop = newItem;
        nbottom = newItem;
        return;
    }

    // Configure newItem
    newItem->prev = nbottom;
    newItem->next = NULL;

    // Configure nbottom
    nbottom->next = newItem;
    nbottom = newItem;
}

/*
	Delete or remove the top node of the list.
	Once pop, you need to properly deallocate the memory for the node (not the data).
	If your list is currently empty, you don't have to do anything.
*/
void MList::pop_top(){
    if (ntop == NULL) {
        return;
    }

    Node* temp = ntop;
    ntop = ntop->next;    // move head 
    delete temp;


    if (ntop == NULL) { // list is now empty
        nbottom = NULL; 
    } else {
        ntop->prev = NULL;
    }
}

/*
	Delete or remove the bottom node of the list.
	Once pop, you need to properly deallocate the memory for the node (not the data).
	If your list is currently empty, you don't have to do anything.
*/
void MList::pop_bottom(){
    // Base case: List is empty
    if (nbottom == NULL) { 
        return; 
    }

    // New nbottom node
    Node* temp = nbottom;
    nbottom = nbottom->prev;

    delete temp;

    // Adjust nbottom if necessary
    if (nbottom == NULL) { 
        // List is now empty
        ntop = NULL;
        nbottom = NULL;
    } else {
        // Adjust new nbottom node
        nbottom->next = NULL;
    }
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Search a node in the list that has data with the same name as the parameter.
	If one is found, return the pointer to the node.
	If one is not found, return NULL.
*/
Node* MList::search(Node* start, string name) {
    // List is empty
    if (start == NULL) {
        return NULL;
    }

    // Node with name found
    if (start->nodeData->name == name) {
        return start;
    }

    // Search rest of the list
    return search(start->next, name);
}


//Swapping node a with node b.
// Precondition: a != NULL && b != NULL
void MList::swapNode(Node* a, Node* b){
    Node* a_prev = a->prev;
    Node* a_next = a->next;
    Node* b_prev = b->prev;
    Node* b_next = b->next;

    // If a or b is the top or bottom
    // adjust the top and bottom pointers
    if (ntop == a) { ntop = b; }
    else if (ntop == b) { ntop = a; }

    if (nbottom == a) { nbottom = b; }
    else if (nbottom == b) { nbottom = a; }

    // Adjust neighboring nodes of a
    // and b. Prevent accessing a NULL pointer
    if (a_prev != NULL) { 
        a_prev->next = b; 
    }
    if (a_next != NULL) { 
        a_next->prev = b; 
    }
    if (b_prev != NULL) { 
        b_prev->next = a; 
    }
    if (b_next != NULL) { 
        b_next->prev = a; 
    }

    // Adjust pointers based on
    // if a and b are next to each other
    if (b_next != a) { 
        a->next = b_next; 
    }
    else { 
        a->next = b; 
    }
    if (b_prev != a) { 
        a->prev = b_prev; 
    }
    else { 
        a->prev = b; 
    }
    if (a_next != b) { 
        b->next = a_next; 
    }
    else { 
        b->next = a;
    }
    if (a_prev != b) { 
        b->prev = a_prev; 
    }
    else { 
        b->prev = a; 
    }
}

/*
	This function PERMANENTLY deletes a node from the list given a pointer to that node.
	You can assume that the node exists in the list - no need to search for the node.
*/
void MList::deleteNode(Node* a){
    // Special base: Node doesn't exist
    if (a == NULL) {
        return;
    }

    removeNode(a);
    delete a;
}

/*
	Unlike the delete function above, this function does NOT permanently delete the node.
	What you need to do at the last step is to set the node's prev and next pointers to NULL.
	Again, you can assume that the node exists in the list - no need to search for the node.
	Note: this function will be useful
        when we want to insert the given node later at another position after you remove it from the list.
*/
void MList::removeNode(Node* a){
    if (a == NULL) {
        return; 
    }

    Node* a_prev = a->prev;
    Node* a_next = a->next;

    // Adjust head and tail
    // if a is one or both of them
    if (ntop == a) {
        ntop = a->next;   
    }
    if (nbottom == a) {
        nbottom = a->prev;
    }

    // Adjust neighboring nodes
    if (a_prev != NULL) {
        a_prev->next = a_next;
    }
    if (a_next != NULL) {
        a_next->prev = a_prev;
    }

    // Removed node is no longer part of list
    a->next = NULL;
    a->prev = NULL;
}

/*
	Insert node a after node b.
	Note: b can be NULL, Q: what does that mean?
    otherwise, you can assume that b exists in the list.
*/
void MList::insertAfter(Node *a, Node* b){
    // Check a is not NULL
    if (a == NULL) {
        return;   
    }

    if (b == NULL) {
        // If b == NULL then a is the new ntop
        // If the list is empty, it is also the new nbottom
        if (ntop != NULL) {
            ntop->prev = a;
        }

        if (nbottom == NULL) {
            nbottom = a;
        }

        a->prev = NULL;
        a->next = ntop;
        ntop = a;
    } else {
        // Insert the node somewhere after ntop.

        Node* b_original_next = b->next;

        // Adjusting inserted node
        a->prev = b;
        a->next = b->next;

        // Adjust previous node
        b->next = a;

        // Adjust next node
        if (b_original_next != NULL) { // Not end of list
            b_original_next->prev = a;   
        } else {
            // Adjust nbottom in case it is inserted at the end.
            nbottom = a;
        }
    }
}


/*
	Note: RECURSION --- This function should be implemented using recursion.
	Implement a SELECTION SORT algorithm using recursion.
	The function takes in a start node.
	Things to keep in mind:
		1). sort should NOT be run if the list is empty.
		2). if mode = true, sort by name in alphabetical order
			(i.e., A-Za-z) based on ASCII number if mode = true
        3). if mode = false, sort by size in ascending order (low->high)
	You can search any online resources available to you (i.e. search 'selection sort')
	to understand the basic idea behind the selection sort algorithm.
    Note: 
        1). there can only be at most one loop in this function
        2). this function is private
            see sortByNameSelection and sortBySizeSelection for the calls to this function
*/
void MList::sortSelection(Node* start, bool mode){
    // List is empty
    if (isEmpty() || start == NULL) {
        return;
    }

    Node* minimum = start;
    Node* temp = start;

    // Find minimum element (based on the mode)
    // for the list
    while (temp != NULL) {
        // Sort by name
        if (mode) {
            if (temp->nodeData->name < minimum->nodeData->name) {
                minimum = temp;   
            }
        }
        // Sort by size
        else {
            if ((temp->nodeData->size) < (minimum->nodeData->size)) {
                minimum = temp;   
            }
        }
        temp = temp->next;
    }

    // Swap minimum item with the first item in the list
    swapNode(minimum, start);

    // Sort the rest of the list
    sortSelection(minimum->next, mode);
}


/*
	Note: RECURSION --- This function should be implemented using recursion.
	Implement an INSERTION SORT algorithm using recursion.
	The function takes in a start node.
	Things to keep in mind:
		1). sort should NOT be run if the list is empty.
		2). we are trying to sort by name in alphabetical order
			(i.e., A-Za-z) based on ASCII number.
	You can search any online resources available to you (i.e. search 'insertion sort')
	to understand the basic idea behind the insertion sort algorithm.
	The gif from the wikipedia page of this algorithm is the easiest to understand in my opinion
	Link: https://en.wikipedia.org/wiki/Insertion_sort
    Note: 
        1). there can only be at most one loop in this function
        2). this function is private, see sortByNameInsertion for the call to this function
*/
void MList::sortInsertion(Node* start){
    // List if empty
    if (isEmpty() || start == NULL) {
        return;
    }

    // Last item of sorted portion of list
    Node* temp = start->prev;

    // Traverse until start will be inserted in correct spot
    while (temp != NULL && temp->nodeData->name > start->nodeData->name) {
        temp = temp->prev;
    }

    // Start of the rest of the list
    Node* restOfList = start->next;

    // Insert first item in unsorted portion of list
    // to correct place in sorted portion of list
    removeNode(start);
    insertAfter(start, temp);

    // Sort the rest of the list
    sortInsertion(restOfList);
}


/*
	Note: RECURSION --- This function should be implemented using recursion.
	Traverse and print our list from node n to the top.
	we will seperate each node printed by the passed-in delimiter.
	If a node you encounter is a folder type, do

		cout<<....name.....<<delim;

	If a node you encounter is a file type, do

		cout<<....name.....<<"("<<..size..<<")"<<delim;

	Note: please do NOT include <<endl; (aka newline)
	Example output (assume that the delimiter is a single space):

		folder1 file1(0) folder2 file2(0)

    There should be no loop in this function
	See printBtT function for the call to this function.
    This function is private
*/
void MList::traverseToTop(Node* n, string delim){
    // Base case: List is empty, nothing else to print
    if(n == NULL) {
        return;
    }

    // Print current node
    if (!n->nodeData->isFolder) {
        cout<< n->nodeData->name << "(" << n->nodeData->size << ")" << delim;
    } else {
        cout<< n->nodeData->name << delim;

    }

    // Print everything before node n
    traverseToTop(n->prev, delim);
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Traverse and print our list from node n to the bottom.
	we will seperate each node printed by the passed-in delimiter.
	If a node you encounter is a folder type, do

		cout<<....name.....<<delim;

	If a node you encounter is a file type, do

		cout<<....name.....<<"("<<..size..<<")"<<delim;

	Note: please do NOT include <<endl; (aka newline)
	Example output (assume that the delimiter is a single space):

		folder1 file1(0) folder2 file2(0)

    There should be no loop in this function
	See printTtB function for the call to this function.
    This function is private
*/
void MList::traverseToBottom(Node* n,string delim){
    // Base case: List is empty, nothing else to print
    if(n == NULL) {
        return;
    }

    // Print current node
    if (!n->nodeData->isFolder) {
        cout<< n->nodeData->name << "(" << n->nodeData->size << ")" << delim;
    } else {
        cout<< n->nodeData->name << delim;

    }

    // Print everything else after current node
    traverseToBottom(n->next, delim);

}

//------------------------------------------------------------------------------
//FUNCTIONS BELOW ARE COMPLETE, PLEASE DON'T CHANGE ANYTHING HERE
//------------------------------------------------------------------------------

//getting the pointer to the top node.
Node* MList::top(){
    return ntop;
}

//getting the pointer to the bottom node.
Node* MList::bottom(){
    return nbottom;
}

//call traverseToBottom to print all item in the list from bottom node to top node
//the list of items is separated by a given delimiter
void MList::printBtT(string delim){
    //create a temp pointer to hold bottom
    Node* tmp = nbottom;
    traverseToTop(tmp,delim);
}

//call traverseToBottom to print all item in the list from top node to bottom node
//the list of items is separated by a given delimiter
void MList::printTtB(string delim){
    Node* tmp = ntop;
    traverseToBottom(tmp,delim);
}

//call sortSelection function, mode = true = sort by name
//public member
void MList::sortByNameSelection(){
    bool mode = true;
    Node *tmp = ntop;
    sortSelection(tmp,mode);
}

//call sortSelection function, mode = false = sort by size
//public member
void MList::sortBySizeSelection(){
    bool mode = false;
    Node *tmp = ntop;
    sortSelection(tmp,mode);
}

//call sortInsertion function
//public member
void MList::sortByNameInsertion(){
    Node *tmp = ntop;
    sortInsertion(tmp);
}