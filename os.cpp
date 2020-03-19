#include <iostream>
#include "os.h"
using namespace std;

/*
Constructor
	Dynamically allocate root data.
	set isFolder = true.
	dynamically allocate Mlist for our root data as well (since it's a directory type).
    push our newly created data object pointer to wd and dataStack from the top.
*/
OS::OS(){
    // Create root data for root directory
    root_data = new Data;
    root_data->isFolder = true;
    root_data->name = "root";
    root_data->size = 0;

    // Directory root has
    MList* root = new MList;
    root_data->childList = root;

    // Keep track of allocated mem
    dataStack.push_top(root_data);
    
    // push to work directory
    wd.push_top(root_data);
}

/*
Destructor to clean-up memory, i will leave this for you to figure it out.
*/
OS::~OS(){
    Node* top = dataStack.top();
    
    // Traverse through dataStack list
    while (top != NULL) {
        // Delete dynamically allocated MLists for directories
        if (top->nodeData->isFolder) {
            delete top->nodeData->childList;
        }
        
        // Deallocate Node data
        delete top->nodeData;
        top = top->next;
    }
}

/*
Search a node in the current directory
	If one is found, return a pointer to that node
	If one is not found, return NULL
*/
Node* OS::search_item(string fname){
    MList* currentDir = wd.bottom()->nodeData->childList;
    return currentDir->search(currentDir->top(), fname);

}

/*
Delete a node in the current directory
	Note: this function only deletes (permanently) the node, not the Data obj the node points to
	If the item you want to delete doesn't exist in the current directly, do

		cout<<"Error: cannot find file or directory '"<<fname<<"'"<<endl;
*/
void OS::del(string fname){
    Node* n = search_item(fname);

    // File/Directory cannot be found
    if (n == NULL) {
        cout << "Error: cannot find file or directory '" << fname << "'" << endl;
    } 
    // File/Directory is found
    else {
        wd.bottom()->nodeData->childList->deleteNode(n);   
    }
}

/*
Create a file or a folder, use boolean isFolder to tell (true if folder, false if file)
Things to keep in mind:
	1). Don't forget to search for a node in the current directory with the same name first.
		if the name already exists, do:

				cout<<"Directory or file name '"<<fname<<"' already exists"<<endl;

	2). If you are creating a folder, make sure to allocate a memory for its MList object
		and set the boolean isFolder appropriately.
    3). At this point you should initialize the size of file and folder to 0
	4). Once the data object is created, add the pointer to that to dataStack from the "top" 
		and add the node to the current directory list from the "bottom".
	5). Once added, sort the current directory list by name.
*/
void OS::create_item(string fname, bool isFolder){
    // Search if file already exists in wd
    Node* n = search_item(fname);

    if (n != NULL)  {
        cout<<"Directory or file name '"<<fname<<"' already exists"<<endl;
    } else {
        // Create new Data object
        Data* newData = new Data;
        newData->name = fname;
        newData->isFolder = isFolder;
        newData->size = 0;
        newData->childList = NULL;

        // New MList for Data if folder
        if (isFolder) {
            MList* dir = new MList;
            newData->childList = dir;
        }

        // Increment current wd size
        wd.bottom()->nodeData->size++;

        // Add new Data to current directory MList
        wd.bottom()->nodeData->childList->push_bottom(newData);

        // Sort Data
        wd.bottom()->nodeData->childList->sortByNameInsertion();

        // Add to dataStack        
        dataStack.push_top(newData);
    }
}

/*
Read or write a file according to the boolean isRead (true = read, false = write)
Things to keep in mind:
	1). make sure that a file "fname" exists in our current directly, if not

			cout<<"Error: cannot find file '"<<fname<<"'"<<endl;

	2). if it exists, make sure that it is a file type, not a folder type. If it is a folder type,

			cout<<"Error: '"<<fname<<"' is not a file"<<endl;

	3). read mode is simply just cout<<...text...<<endl;
	4). for write mode you need to allow user input, use these 3 lines below:

                cout<<"$> ";
				string input;
				getline(cin,input);

		then simply just set text to the input string.
	5). the size of the file should be based on the length of the input string.
*/
void OS::file(string fname, bool isRead){
    Node* item = search_item(fname);
    
    // Node with name fname not found
    if (item == NULL) {
        cout<<"Error: cannot find file '"<<fname<<"'"<<endl;
        return;
    }

    // Node is a folder (cannot be read or written)
    if (item->nodeData->isFolder) {
        cout<<"Error: '"<<fname<<"' is not a file"<<endl;
        return;
    }

    // Node is a file (can be read or written)
    // either read or write
    if (isRead) {
        cout << item->nodeData->text << endl;
    } else {
        cout<<"$> ";
        string input;
        getline(cin,input);
        item->nodeData->text = input;
        item->nodeData->size = input.size();
    }
}


//Change directory
void OS::cd_dir(string fname){
    if(fname == ".."){
        //this option takes you up one directory (move the directory back one directory)
        //you should not be able to go back anymore if you are currently at root.
        
        // Base case: Only one item in working directory,
        // that is, the root
        if (wd.top() == wd.bottom()) {
            return;
        }
        
        wd.pop_bottom();


    }else if(fname == "~"){
        //this option takes you directly to the home directory (i.e., root).
        while (wd.top() != wd.bottom()) {
            wd.pop_bottom();   
        }

    }else{
        /*
			This option means that you are trying to access (go into) a directory
			1). check whether there exists a node with that name, if you cannot find it:

					cout<<"Error: cannot find directory '"<<fname<<"'"<<endl;

			2). if it does exist, check whether it is a folder type. If it is a file type:

					cout<<"Error: '"<<fname<<"' is not a directory"<<endl;

			3). after checking both things, you can safely move to another directory
		*/
        Node* item = search_item(fname);
        // Directory not found
        if (item == NULL) {
            cout<<"Error: cannot find directory '"<<fname<<"'"<<endl;
            return;
        }
        // Not a directory
        if (!item->nodeData->isFolder) {
            cout<<"Error: '"<<fname<<"' is not a directory"<<endl;
            return;
        }

        wd.push_bottom(item->nodeData);
    }
}

//Print list of item in the current directory, the way you print it will be according to the passed-in option
void OS::ls(string option){
    Node* currentDir = wd.bottom();
    if(option == "-d"){
        //Default option - print the list of items in the current directory from top to bottom.
        //use a single space as delimiter.
        currentDir->nodeData->childList->printTtB(" ");
    }
    else if(option == "-sort=name"){
        //Use Insertion Sort to sort items in the current directory and print from top to bottom (alphabetically A-Za-z).
        //use a single space as delimiter.
        currentDir->nodeData->childList->sortByNameInsertion();
        currentDir->nodeData->childList->printTtB(" ");

    }else if(option == "-r"){
        //Reverse print the list of items in the current directory (i.e., print form bottom to top).
        //use single space as delimiter.
        currentDir->nodeData->childList->printBtT(" ");

    }else if(option == "-sort=size"){
        //Sort list by size and print the list of items in the current directory from top to bottom.
        //use single space as delimiter.
        currentDir->nodeData->childList->sortBySizeSelection();
        currentDir->nodeData->childList->printTtB(" ");
    }
    else{
        cout<<"usage: ls [optional: -r for reverse list, -sort=size to sort list by file size, -sort=name to soft list by name]";
    }
}

//Priting path from root to your current directory.
//use slash "/" as our delimiter.
//Example output: root/cs103/usc/viterbi/
void OS::present_working_dir(){
    wd.printTtB("/");
}