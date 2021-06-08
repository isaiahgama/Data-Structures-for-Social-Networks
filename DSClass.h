// Isaiah Gama, 5/15/2021
#include <iostream>
#include <fstream>
#include <vector>
#include "Node.h"
using namespace std;

//functions to pad strings before writing to file
string fixAge(string age){
    string temp = "";
    for(int i = 0; i < (3 - age.length()); i++){
        temp+=" ";
    }
    return age + temp;
}

string fixName(string name){
    string temp = "";
    for(int i = 0; i < (20 - name.length()); i++){
        temp+=" ";
    }
    return name + temp;
}

string fixOccupation(string occupation){
    string temp = "";
    for(int i = 0; i < (30 - occupation.length()); i++){
        temp+=" ";
    }
    return occupation + temp;
}

string depadAge(string age){
    string tmp = "";
    for(int i = 0; i < 3; i++){
        string s = to_string(age[20+i]);
        if(s != " "){
            tmp += age[20+i];
        }
    }

    tmp.erase(remove( tmp.begin(), tmp.begin()+3, ' ' ),tmp.begin()+3);
    // cout<<"depadded age:"<<tmp<<".";
    return tmp;
}

string depadOccupation(string occupation){
    string tmp = occupation.substr(23,52);
    int index = tmp.find("  ");
    if(index == -1){
        if(to_string(tmp[29]) == "32"){//ascii space
            return tmp.substr(0,29);
        }
        return tmp;
    }
    tmp = tmp.substr(0,index);

    return tmp;
}
    
// one class for all data strutcures
class DSClass {
    public:
        //general
        void insertNewUser(string name, string age, string occupation);
        void printAll();
        void friendsInfo(string name);
        void rangeQuerySearch(string lower, string upper);
        void singleQuerySearch(string target);
        //tree functions
        DSClass();
        void insert(string name, int index);
        //graph functions
        void addFriend(string name, string f);
        void addFriendship(string name1, string name2);
        void graphInsert(string name, int index);
        void printGraph();
    private:
        //helpers
        void rangeQuerySearchRecursive(string lower, string upper, Node* root);
        void singleQuerySearchRecursive(string target, Node* root);
        //tree helper functions
        Node* basicInsert(Node* root, Node* newPerson);
        void rotateLeft(Node *& root, Node *& n);
        void rotateRight(Node *& root, Node *& n);
        void makeValidRBTree(Node *& root, Node *& n);
        //variables
        Node *root;
        vector<friendNode*> graph;
        vector<string> rangeNames;

};

// General Functions

void DSClass::rangeQuerySearch(string lower, string upper){

    cout<<"Printing all the information for people between "<<lower<<" and "<<upper<<": "<<endl;

    //call helper function to initialize vector of names from the RB tree
    rangeQuerySearchRecursive(lower,upper,root);

    //iterate through the vector of names we found and print their information from file
    for(string name:rangeNames){ // for each name we found
        // cout<<"name:"<<name<<endl;
        for(friendNode *person : graph){ //find them in the graph
            if(person->name == name){  
                friendNode *tmp = person;
                cout<<tmp->name<<",";

                //read from file
                ifstream myFile ("output.out", ifstream::binary);
                myFile.seekg (person->fileIndex, myFile.beg);
                char * buffer = new char [53];
                myFile.read (buffer,53);
                myFile.close();

                string str(buffer);
                string age = depadAge(buffer);
                string occupation = depadOccupation(buffer);

                cout<<age<<","<<occupation<<". Friends: ";
                //print friends from graph
                while(tmp->next){
                    tmp = tmp->next;
                    cout<<tmp->name;
                    if(tmp->next)
                        cout<<", ";
                }
                cout<<endl;
            }
        }
    }

    //reset the vector for future searches
    for(string s : rangeNames){
        rangeNames.pop_back();
    }
}
//helper function that adds all names in the RB tree between lower and upper to a vector
void DSClass::rangeQuerySearchRecursive(string lower, string upper, Node* root){
    //base case
    if(root==NULL){
        return;
    }
    //recurse left subtree while lower not reached
    if(lower < root->name)
        rangeQuerySearchRecursive(lower,upper,root->left);
    //if current node is between range, add name to vector
    if(root->name >= lower && root->name <= upper){
        rangeNames.push_back(root->name);
    }
    //recurse right subtree while upper not reached
    if(upper > root->name)
        rangeQuerySearchRecursive(lower,upper,root->right);  
}

void DSClass::singleQuerySearch(string target){
    singleQuerySearchRecursive(target, root);

    for(string name:rangeNames){ // for each name we found
        // cout<<"name:"<<name<<endl;
        for(friendNode *person : graph){ //find them in the graph
            if(person->name == name){  
                friendNode *tmp = person;
                cout<<tmp->name<<",";

                //read from file
                ifstream myFile ("output.out", ifstream::binary);
                myFile.seekg (person->fileIndex, myFile.beg);
                char * buffer = new char [53];
                myFile.read (buffer,53);
                myFile.close();

                string str(buffer);
                string age = depadAge(buffer);
                string occupation = depadOccupation(buffer);

                cout<<age<<","<<occupation<<". Friends: ";
                //print friends from graph
                while(tmp->next){
                    tmp = tmp->next;
                    cout<<tmp->name;
                    if(tmp->next)
                        cout<<", ";
                }
                cout<<endl;
            }
        }
    }

    //reset the vector for future searches
    for(string s : rangeNames){
        rangeNames.pop_back();
    }


}

void DSClass::singleQuerySearchRecursive(string target, Node* root){
    //base case
    if(root==NULL){
        return;
    }
    //recurse left subtree while lower not reached
    if(target < root->name)
        singleQuerySearchRecursive(target,root->left);
    //if current node is between range, add name to vector
    if(root->name >= target && root->name <= target){
        rangeNames.push_back(root->name);
    }
    //recurse right subtree while upper not reached
    if(target > root->name)
        singleQuerySearchRecursive(target,root->right);  
}


//print all the friend information for a person
void DSClass::friendsInfo(string name){
    for(friendNode *person : graph){ //iterate
        if(person->name == name){ // until found the person
            cout<<"Printing friend information for "<<person->name<<": "<<endl;
            friendNode *tmp = person->next; // tmp is the first friend

            while(tmp){ // for each friend
                string thisFriend = tmp->name;
                    for(friendNode *p2 : graph){ // find them in the graph
                        if(p2->name == thisFriend){
                            cout<<thisFriend<<",";
                            //read from file
                            ifstream myFile ("output.out", ifstream::binary);
                            myFile.seekg (p2->fileIndex, myFile.beg);
                            char * buffer = new char [53];
                            myFile.read (buffer,53);
                            myFile.close();

                            string str(buffer);
                            string age = depadAge(buffer);
                            string occupation = depadOccupation(buffer);

                            cout<<age<<","<<occupation<<", ";
                        }
                    }
                tmp = tmp->next;
            }
        }
    }
    cout<<endl<<endl;
}

//insert a new user after initialization
void DSClass::insertNewUser(string name, string age, string occupation){
    // cout<<graph.size()<<endl;
    int index = graph.size();

    graphInsert(name, index);
    insert(name,index); // rb tree insert
    ofstream myFile;
    myFile.open("output.out", fstream::app);
    string outstring;
    // write person to file
                //pad every string with the correct space
                string writeName = fixName(name);
                string writeAge = fixAge(age);
                string writeOccupation = fixOccupation(occupation);
            string completeTuple = writeName+writeAge+writeOccupation;
            myFile << completeTuple;
    myFile.close();
}

//print all users and their info
void DSClass::printAll(){
    // for each person in the graph
    cout<<"Print All: "<<endl;
    for(friendNode *person : graph){
            friendNode *tmp = person;
            cout<<tmp->name<<",";

            //read from file
            ifstream myFile ("output.out", ifstream::binary);
            myFile.seekg (person->fileIndex, myFile.beg);
            char * buffer = new char [53];
            myFile.read (buffer,53);
            myFile.close();

            string str(buffer);
            string age = depadAge(buffer);
            string occupation = depadOccupation(buffer);

            cout<<age<<","<<occupation<<". Friends: ";
            //print friends from graph
            while(tmp->next){
                tmp = tmp->next;
                cout<<tmp->name;
                if(tmp->next)
                    cout<<", ";
            }
            cout<<endl;
    }
    cout<<"Print All Over."<<endl<<endl;
}

// Graph Functions

//print the entire graph in order
void DSClass::printGraph(){
    for(friendNode *person : graph){
        friendNode *tmp = person;
        cout<<tmp->name<<": ";
        while(tmp->next){
            tmp = tmp->next;
            cout<<tmp->name<<" ";
        }
        cout<<endl;
    }
}
//insert new person into the graph
void DSClass::graphInsert(string name, int index){
    friendNode *newPerson = new friendNode(name);
    newPerson->fileIndex = index*53;
    graph.push_back(newPerson);
    //cout<<"Inserted "<<newPerson->name<<endl;
}
//make f a friend of name
void DSClass::addFriend(string name, string f){
    int index = 0;

    for(int i = 0; i < graph.size(); i++){
        if(graph[i]->name == name){
            index = i;
            break;
        }
    }

    friendNode *tmp = graph[index];

    while(tmp){
        if(!(tmp->next)){
            // cout<<"no next child";
            break;
        }
        tmp = tmp->next;
    }
    friendNode *newFriend = new friendNode(f);
    tmp->next = newFriend;

    //cout<<name<<" now has friend "<<tmp->next->name<<endl;
}
//add a friendship between n1 and n2
void DSClass::addFriendship(string name1, string name2){
    addFriend(name1,name2);
    addFriend(name2,name1);

    cout<<name1<<" and "<<name2<<" are now friends"<<endl;
}
//Tree Functions

DSClass::DSClass(){
    root = NULL;
}

// helper function to insert a new node into the correct location based on name
Node* DSClass::basicInsert(Node* root, Node *newPerson){
    //tree is empty, return new person as new root
    if (root == NULL){
       return newPerson;
    }
  
    //tree isn't empty, we have to recursively traverse it
    if (newPerson->name < root->name){ // less than root, go down the left branch
        root->left  = basicInsert(root->left, newPerson);
        root->left->parent = root;
    }
    else if (newPerson->name > root->name){ // greater than root, go down the right branch
        root->right = basicInsert(root->right, newPerson);
        root->right->parent = root;
    }
  
    //keep the same root
    return root;
}

// insert new node into rbTree
void DSClass::insert(string name, int index){
    Node *newPerson = new Node(name, index);
  
    // Do a normal BST insert
    root = basicInsert(root, newPerson);
  
    // fix Red Black Tree violations
    makeValidRBTree(root, newPerson);
}

// Makes the RB tree correct by fixing any node violations
void DSClass::makeValidRBTree(Node *&root, Node *&node){
    Node *nParent = NULL;
    Node *nGrandParent = NULL;

    while ((node != root) && (node->color != "black") && (node->parent->color == "red")){

        nParent = node->parent;
        nGrandParent = node->parent->parent;
        // if node is a left grandchild (parent is left child of gp), then uncle is right
        if (nParent == nGrandParent->left){
  
            Node *nUncle = nGrandParent->right;
            // if uncle of is red, just recolor
            if (nUncle != NULL && nUncle->color == "red"){
                nGrandParent->color = "red";
                nParent->color = "black";
                nUncle->color = "black";
                node = nGrandParent;
            }else{
                // otherwise if node is right child of parent: rotate left
                if (node == nParent->right){
                    rotateLeft(root, nParent);
                    node = nParent;
                    nParent = node->parent;
                }
                // else node is left child of parent: rotate right
                rotateRight(root, nGrandParent);
                std::swap(nParent->color, nGrandParent->color);
                node = nParent;
            }
        }
        // else node is a right grandchild (parent is right child of gp), then uncle is left
        else{ // basically first conditional but mirrored
            Node *nUncle = nGrandParent->left;
            // if uncle of is red, just recolor
            if ((nUncle != NULL) && (nUncle->color == "red")){
                nGrandParent->color = "red";
                nParent->color = "black";
                nUncle->color = "black";
                node = nGrandParent;
            }
            else{
                // otherwise if node is left child of parent: rotate right
                if (node == nParent->left){
                    rotateRight(root, nParent);
                    node = nParent;
                    nParent = node->parent;
                }
                // else node is right child of parent: rotate left
                rotateLeft(root, nGrandParent);
                std::swap(nParent->color, nGrandParent->color);
                node = nParent;
            }
        }
    }
    
    //make sure root is always black
    root->color = "black";
}

//insert helper function to rotate left
void DSClass::rotateLeft(Node *&root, Node *&node){
    Node *nRight = node->right; 
    node->right = nRight->left;
    if (node->right != NULL){ // n has right child
        node->right->parent = node;
    }
  
    nRight->parent = node->parent;
  
    if (node->parent == NULL){ //n has no parent, set it to root
        root = nRight;
    }
    else if (node == node->parent->left){
        node->parent->left = nRight;
    }else{
        node->parent->right = nRight;
    }
    nRight->left = node;
    node->parent = nRight;
}

//insert helper function to rotate right
void DSClass::rotateRight(Node *&root, Node *&node){
    Node *nLeft = node->left;
    node->left = nLeft->right;
    if (node->left != NULL){ // node has left child
        node->left->parent = node;
    }
    nLeft->parent = node->parent;

    if (node->parent == NULL){ // node has no parent, set it to root
        root = nLeft;
    }
    else if (node == node->parent->left){
        node->parent->left = nLeft;
    }else{
        node->parent->right = nLeft;
    }
    nLeft->right = node;
    node->parent = nLeft;
}
