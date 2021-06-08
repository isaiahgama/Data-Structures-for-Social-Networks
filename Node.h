//Isaiah Gama, 5/15/2021
#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

struct Node {
    string name;
    string color;
    int index;
    Node *left;
    Node *right;
    Node *parent;
  
    Node(string name, int index) {
        this->index = index;
        this->name = name;
        left = right = parent = NULL;
        this->color = "red";
    }
};

struct friendNode { 
    string name;
    int fileIndex;
    friendNode *next;

    friendNode(string name) {
        this->name = name;
        next = NULL; 
        fileIndex = 0;
    }
};

#endif