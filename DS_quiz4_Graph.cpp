// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct path_distance{
    int distance;
    vector<int> path;
};

void allPathUtil(vector<vector<int>> &graph,int busStop_num,vector<bool> &visited,vector<int> &path,int src,int des,vector<path_distance> &StoD);
void findAllPath(vector<vector<int>> &graph,int busStop_num,int src,int des,vector<path_distance> &StoD);

int main()
{
    string input,input_line;
    while(getline(cin,input_line)&&!input_line.empty()){
        input+=input_line+'\n';
    }
    stringstream inputData(input);
    int busStop_num,edge_num;
    inputData>>busStop_num;
    inputData>>edge_num;
    // vector<int> A(busStop_num);
    vector<vector<int>> graph(busStop_num,vector<int>(busStop_num,0));
    for(int i=0;i<edge_num;i++){
        int source,destination,distance;
        inputData>>source>>destination>>distance;
        graph[source][destination]=distance;
    }
    // for(int i=0;i<busStop_num;i++){
    //     for(int j=0;j<busStop_num;j++){
    //         cout<<graph[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    int t;
    inputData>>t;
    
    if(t==1){
        vector<path_distance> StoD;
        int A,Center;
        inputData>>A>>Center;
        // cout<<A<<" "<<Center<<endl;
        findAllPath(graph,busStop_num,A,Center,StoD);
        // for(path_distance ele:StoD){
        //     cout<<ele.distance<<"\t";
        //     for(int v:ele.path){
        //         cout<<v<<" ";
        //     }
        //     cout<<endl;
        // }
        if(StoD.empty()){
            cout<<-1<<endl;
        }
        else{
            int minDistance=1000000000;
            for(path_distance ele:StoD){
                if(ele.distance<minDistance){
                    minDistance=ele.distance;                
                }
            }
            cout<<minDistance<<endl;
        }
    }

    if(t==2){
        vector<path_distance> StoDA;
        vector<path_distance> StoDB;
        int A,B,Center;
        inputData>>A>>B>>Center;
        findAllPath(graph,busStop_num,A,Center,StoDA);
        findAllPath(graph,busStop_num,B,Center,StoDB);
        // for(path_distance ele:StoDA){
        //     cout<<ele.distance<<"\t";
        //     for(int v:ele.path){
        //         cout<<v<<" ";
        //     }
        //     cout<<endl;
        // }
        // for(path_distance ele:StoDB){
        //     cout<<ele.distance<<"\t";
        //     for(int v:ele.path){
        //         cout<<v<<" ";
        //     }
        //     cout<<endl;
        // }

        if(StoDA.empty() or StoDB.empty()){
            cout<<-1<<endl;
        }
        else{
            int minDistanceA=1000000000;
            int minDistanceB=1000000000;
            vector<path_distance> combine;
            for(path_distance eleA:StoDA){
                for(int v:eleA.path){
                    if(v==B){
                        path_distance temp{eleA.distance,eleA.path};
                        combine.push_back(temp);
                        break;
                    }
                }
                if(eleA.distance<minDistanceA){
                    minDistanceA=eleA.distance;
                }
            }
            for(path_distance eleB:StoDB){
                for(int v:eleB.path){
                    if(v==A){
                        path_distance temp{eleB.distance,eleB.path};
                        combine.push_back(temp);
                        break;
                    }
                }
                if(eleB.distance<minDistanceB){
                    minDistanceB=eleB.distance;
                }
            }
            // for(path_distance ele:combine){
            //     cout<<ele.distance<<"\t";
            //     for(int v:ele.path){
            //         cout<<v<<" ";
            //     }
            //     cout<<endl;
            // }

            int minDistance=minDistanceA+minDistanceB;
            for(path_distance ele:combine){
                if(ele.distance<minDistance){
                    minDistance=ele.distance;
                }
            }
            cout<<minDistance<<endl;
        }
    }

    return 0;
}
void allPathUtil(vector<vector<int>> &graph,int busStop_num,vector<bool> &visited,vector<int> &path,int src,int des,vector<path_distance> &StoD){
    visited[src]=true;
    path.push_back(src);
     
    if(src==des){
        // for(int v:path){
        //     cout<<v<<" ";
        // }
        // cout<<endl;
        int distance=0;
        for(int v=0;v<path.size()-1;v++){
            int s=path[v];
            int d=path[v+1];
            distance+=graph[s][d];
        }
        path_distance temp{distance,path};
        StoD.push_back(temp);
    }
    else{
        for(int i=0;i<busStop_num;i++){
            if(graph[src][i]>0 && !visited[i]){
                allPathUtil(graph,busStop_num,visited,path,i,des,StoD);
            }
        }
    }

    visited[src]=false;
    path.pop_back();
}
void findAllPath(vector<vector<int>> &graph,int busStop_num,int src,int des,vector<path_distance> &StoD){
    vector<bool> visited(busStop_num,false);
    vector<int> path;

    allPathUtil(graph,busStop_num,visited,path,src,des,StoD);
}