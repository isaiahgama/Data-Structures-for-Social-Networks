//Isaiah Gama, 5/15/2021
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "DSClass.h"
using namespace std;


void printOptions(){ // Prints all of the options, doesn't actually take any user input.
    cout <<"Welcome to my very trendy commandline social media platform :)" << endl;
    cout << "1) Print All"<<endl;
    cout << "2) Add user" <<endl;
    cout << "3) Add friendship" <<endl;
    cout << "4) Get info of a person's friends" <<endl;
    cout << "5) Range search" <<endl;
    cout << "6) Single user search" <<endl;
    cout << "7) Quit" <<endl;
    cout << "Which operation would you like to perform? (1,2,3,4,5,6,7): "<<endl;
}


int userSelection(){ // does not print anything, only records the user input.
    string n;
    getline(cin,n);
    return stoi(n);
}

int main(int argc, char* argv[]){

    DSClass tree;

    string name;
    string age;
    string occupation;
    string friends;
    ifstream fileName(argv[1]);
    ofstream myFile;
    myFile.open("output.out");
    string outstring;
    int i = 0;
    // cout<<"segfault test"<<endl;
    while(fileName.good()){
        //initialize variables that we will need for insertion and writing to file
        getline(fileName,name,',');
        getline(fileName,age,',');
        getline(fileName,occupation,',');
        getline(fileName,friends,'\n');
        //do all the stuff
        if(name!="" && name!="Name"){ // ignores the first line with headers and the empty new line at end
            //cout<<name<<endl; // person being inserted
            // insert into the graph
            tree.graphInsert(name,i);
            // add all the friends of the person we just added
            stringstream ss(friends);
            while (ss.good()){
                string sub;
                getline(ss,sub,',');
                sub.erase(remove( sub.begin(), sub.end(), '\"' ),sub.end()); // code on how to remove extra " characters adapted from https://stackoverflow.com/questions/5674374/remove-double-quotes-from-a-string-in-c
                sub.erase(remove( sub.begin(), sub.begin() +2, ' ' ),sub.begin()+2);
                if(sub != "")
                    tree.addFriend(name,sub);
            }
            // add into the r/b tree
            tree.insert(name,i);
            // write person to file
                //pad every string with the correct space
                string writeName = fixName(name);
                string writeAge = fixAge(age);
                string writeOccupation = fixOccupation(occupation);
            string completeTuple = writeName+writeAge+writeOccupation;
            myFile << completeTuple;
            //cout<<endl;
            //counter used for keeping track of graph index in r/b tree nodes and the position in file
            i++;
        }
    }
    myFile.close();


    // UI stuff
    int userInput = 0;
    while(userInput != 7){

        if(userInput == 1){ // print all
            tree.printAll();
            cout<<endl;
        }

        else if (userInput == 2){ // Add new User
            string s;
            string a;
            string o;
            cout<<"Enter the new user's name: ";
            getline(cin,s);
            cout<<"Enter the new user's age: ";
            getline(cin,a);
            cout<<"Enter the new user's occupation: ";
            getline(cin,o);

            tree.insertNewUser(s,a,o);

            cout<<s<<" is now a user of CoolBook!"<<endl;
            cout<<endl;
        }

        else if (userInput == 3){ // Add friendship between 2 users

            string u1;
            string u2;
            cout<<"Enter the first user's name: ";
            getline(cin, u1);
            cout<<"Enter the second user's name: ";
            getline(cin, u2);

            tree.addFriendship(u1, u2);
            cout<<endl;

        }

        else if (userInput == 4){ // Get Friend Info (also point query)
            string s;

            cout<<"Enter the user's first name to search friend info: ";
            getline(cin, s);

            tree.friendsInfo(s);
            cout<<endl;

        }

        else if (userInput == 5){ // Range Search
            string u1;
            string u2;

            cout<<"Enter the first user's name: ";
            getline(cin, u1);
            cout<<"Enter the second user's name: ";
            getline(cin, u2);

            tree.rangeQuerySearch(u1,u2);
            cout<<endl;
            
        }

        else if (userInput == 6){ // single search query
            string ss;
            
            cout<<"Enter the user's name to search for: ";
            getline(cin, ss);

            tree.singleQuerySearch(ss);
        }

        else if (userInput == 7){ // exit
            break;
        }

        else{
            cout<<"Please enter a number 1-7."<<endl;
        }

        printOptions();
        userInput = userSelection();

    }

    cout<<"Thanks for using my very trendy social media platform :)!"<<endl;
    // tree.insertNewUser("Mike Jones","32","Rapper");
    // tree.addFriendship("Sarah Perez","Mike Jones");
    // cout<<endl;
    // tree.printAll();
    // cout<<endl;
    // tree.friendsInfo("Sarah Perez");
    // tree.rangeQuerySearch("Eileen Lucas", "Eileen Rose");
    // cout<<endl;
    // tree.rangeQuerySearch("Gregory Mays", "Ngolo Kante");

    

  
    return 0;
}