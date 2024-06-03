#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Node{
    int id;
    int parent_id;
    Node* leftChild;
    Node* rightChild;
    int size = 0;

    Node(int _id,int _parent_id):id(_id),parent_id(_parent_id){}; 
};

void Pre_Print(vector<Node*> Tree, Node* root);
void In_Print(vector<Node*> Tree, Node* root);
void Post_Print(vector<Node*> Tree, Node* root);

int main()
{
    string input,input_line;
    while(getline(cin,input_line) && !input_line.empty()){
        input += input_line + '\n';
    }
    istringstream inputData(input);
    int num,ops;
    inputData>>num>>ops;
    //cout<<num<<" "<<ops<<endl;

    vector<vector<Node*>> tree(num);
    
    for(int i=0;i<ops;i++){
        string command;
        inputData>>command;
        if(command=="Insert"){
            int index,parent_value,value;
            inputData>>index>>parent_value>>value;
            //cout<<command<<" "<<index<<" "<<parent_value<<" "<<value<<endl;
            if(tree[index].empty()){
                Node* newNode = new Node(value,parent_value);
                tree[index].push_back(newNode);
                tree[index].front()->parent_id=-1;
            }
            else{
                Node* newNode = new Node(value,parent_value);
                for(Node* temp:tree[index]){
                    if(temp->id==parent_value){
                        if(temp->size==1){
                            //cout<<newNode->id<<endl;
                            tree[index].push_back(newNode);
                            temp->rightChild = newNode;   
                            temp->size += 1;
                        }
                        if(temp->size==0){
                            //cout<<newNode->id<<endl;
                            tree[index].push_back(newNode);
                            temp->leftChild = newNode;
                            temp->size += 1;
                        }
                        else
                            break;
                    }
                } 
            }
        }
        if(command=="Delete"){
            int index,value;
            inputData>>index>>value;
            //cout<<command<<" "<<index<<" "<<value<<endl;
            // for(Node* temp:tree[index]){
            //     if(temp->id==value){
            //         for(Node* temp_parent:tree[index]){
            //             if(temp_parent->id==temp->parent_id){
            //                 if(temp_parent->leftChild->id==value){
            //                     temp_parent->leftChild = nullptr;
            //                     temp_parent->size--;
            //                 }  
            //                 if(temp_parent->rightChild->id==value){
            //                     temp_parent->rightChild = nullptr;
            //                     temp_parent->size--;
            //                 }
            //             }
            //         }
            //     }
            // }
        }
        if(command=="Print"){
            int index;
            string mode;
            inputData>>index>>mode;
            //cout<<command<<" "<<index<<" "<<mode<<endl;
            Node* root;
            for(Node* temp:tree[index]){
                if(temp->parent_id==-1){
                    root = temp;
                }
            }
            vector<Node*> temp_tree = tree[index];
            if(mode=="pre"){
                //cout<<root->id<<endl;
                Pre_Print(temp_tree,root);
                cout<<endl;
            }
            if(mode=="in"){
                //cout<<root->id<<endl;
                In_Print(temp_tree,root);
                cout<<endl;
            }
            if(mode=="post"){
                //cout<<root->id<<endl;
                Post_Print(temp_tree,root);
                cout<<endl;
            }
            // for(Node* temp:temp_tree){
            //     cout<<temp->id<<" ";
            // }
            // cout<<endl;
        }
        if(command=="Max"){
            int index;
            inputData>>index;
            //cout<<command<<" "<<index<<endl;
            int max_val = 0;
            for(Node* temp:tree[index]){
                if(temp->id>max_val)
                    max_val=temp->id;
            }
            cout<<max_val<<endl;
        }
        if(command=="Merge"){
            int destination,from,value;
            inputData>>destination>>from>>value;
            //cout<<command<<" "<<destination<<" "<<from<<" "<<value<<endl;
            Node* newNode = new Node(value,-1);
            for(Node* temp:tree[destination]){
                if(temp->parent_id==-1){
                    temp->parent_id = value;
                    newNode->leftChild = temp;
                    newNode->size++;
                }
            }
            for(Node* temp:tree[from]){
                tree[destination].push_back(temp);
                tree[from].clear();
            }
            for(Node* temp:tree[destination]){
                if(temp->parent_id == -1){
                    temp->parent_id = value;
                    newNode->rightChild = temp;
                    newNode->size++;
                }
            }
            tree[destination].push_back(newNode);
            tree[from].clear();
            // for(Node* temp:tree[from]){
            //     cout<<temp->id<<" ";
            // }
            // cout<<endl;
        }
        if(command=="Disjoint"){
            int index,value;
            inputData>>index>>value;
            //cout<<command<<" "<<index<<" "<<value<<endl;
        }
    }

    
    for(int i=0;i<num;i++){
        cout<<"Tree "<<i<<endl;
        Node* root;
        for(Node* temp:tree[i]){
            if(temp->parent_id==-1){
                root = temp;
            }
        }
        if(tree[i].empty()){
            cout<<endl;
            continue;
        } 
        In_Print(tree[i],root);
        cout<<endl;
    }

    return 0;
}

void Pre_Print(vector<Node*> tree,Node* root){
    cout<<root->id<<" ";
    if(root->size>=1){
        Pre_Print(tree, root->leftChild);
    }
    if(root->size==2){
        cout<<root->rightChild->id<<" ";
    }
}
void In_Print(vector<Node*> tree,Node* root){  
    if(root->size>0){
        In_Print(tree, root->leftChild);
    }
    //cout<<root->size<<endl;
    cout<<root->id<<" ";
    if(root->size==2){
        cout<<root->rightChild->id<<" ";
    }
}
void Post_Print(vector<Node*> tree,Node* root){
    if(root->size>0){
        Post_Print(tree, root->leftChild);
    }
    if(root->size==2){
        cout<<root->rightChild->id<<" ";
    }
    cout<<root->id<<" ";
}