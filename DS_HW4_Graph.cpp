#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

struct available_vertex{
    int vertex_id;
    int distance;
};

struct order_info{
    int id;
    int traffic_space;
    int src_distance;
    int des_distance;
    int soure_vertex;
    int destination_vertex;
    vector<int> src_path;
    vector<int> des_path;
};

struct allPath{
    int distance;
    vector<int> path;
};

void getValue(vector<vector<int>> &graph,int row,int col,int value);
int minDistance(vector<int> &dist,vector<bool> &sptSet,int vertex_num);
void dijkstra(vector<vector<int>> &graph,int src,int vertex_num,vector<int> &dist,vector<int> &prev);
void Print_sol(vector<int>dist,vector<int> &prev,int vertex_num);
void Print_path(vector<int> &prev,int v,vector<int> &path);
void Store_path(vector<vector<int>> &vertex_minDistance_path,vector<int> &prev,vector<available_vertex> &available_driver);
void vertex_sort(vector<int> &dist,vector<available_vertex> &available_driver,int target,vector<vector<int>> &temp_path);
void StoD(vector<vector<int>> &graph,int src,int des,vector<bool> &visited,vector<int> &path,int vertex_num,vector<allPath> &SRCtoDES);
void findallPath(vector<vector<int>> &graph,int src_vertex,int des_vertex,int vertex_num,vector<allPath> &SRCtoDES);
void SortPath(vector<allPath> &SRCtoDES);
void PickPath(vector<allPath> &SRCtoDES,vector<vector<int>> &graph_capacity,order_info &tempOrder);

int main()
{
    string input,input_line;
    int empty_line = 0;
    while(getline(cin,input_line)){
        if(input_line.empty()){
            empty_line++;
            if(empty_line>1){
                break;
            }
            continue;
        }
        input += input_line + '\n';
    }
    stringstream inputData(input);

    int vertex_num, edge_num, vertex_driver_num;
    inputData>>vertex_num>>edge_num>>vertex_driver_num;
    //cout<<vertex_num<<" "<<edge_num<<" "<<vertex_driver_num<<endl;

    vector<int> myVertex(vertex_num,0);
    for(int i=0;i<vertex_driver_num;i++){
        string command;
        int id,driver_num;
        inputData>>command>>id>>driver_num;
        //cout<<command<<" "<<id<<" "<<driver_num<<endl;
        myVertex[id-1] += driver_num; //那些點有幾個driver
    }
    // for(int i=0;i<vertex_num;i++)
    //     cout<<myVertex[i]<<" ";
    // cout<<endl;

    vector<vector<int>> graph_distance(vertex_num,vector<int>(vertex_num,0));
    vector<vector<int>> graph_capacity(vertex_num,vector<int>(vertex_num,0));

    for(int i=0;i<edge_num;i++){
        string command;
        int source_vertex,destination_vertex,distance,traffic_capacity;
        inputData>>command>>source_vertex>>destination_vertex>>distance>>traffic_capacity;
        //cout<<command<<" "<<source_vertex<<" "<<destination_vertex<<" "<<distance<<" "<<traffic_capacity<<endl;
        getValue(graph_distance,source_vertex,destination_vertex,distance);
        getValue(graph_capacity,source_vertex,destination_vertex,traffic_capacity);
    }
    // for(int i=0;i<vertex_num;i++){
    //     for(int j=0;j<vertex_num;j++)
    //         cout<<graph_capacity[i][j]<<" ";
    //     cout<<endl;
    // }

    int command_num;
    inputData>>command_num;
    //cout<<endl<<command_num<<endl;
    vector<order_info> myOrder;
    vector<order_info> waitingOrder;

    for(int i=0;i<command_num;i++){
        string command;
        inputData>>command;

        if(command=="Order"){
            int id,source_vertex,traffic_space;
            inputData>>id>>source_vertex>>traffic_space;
            //cout<<command<<" "<<id<<" "<<source_vertex<<" "<<traffic_space<<endl;
            vector<int> dist(vertex_num,10000); // store min distance
            vector<int> prev(vertex_num,-1); //store the path of min distance
            dijkstra(graph_distance,source_vertex,vertex_num,dist,prev);
            // Print_sol(dist,prev,vertex_num);

            //sort
            // int available_driver_num=0;
            vector<available_vertex> available_driver;
            int target=-1;
            vector<vector<int>> temp_path(vertex_num);
            for(int i=0;i<vertex_num;i++){ //print all the path
                vector<int> path;
                Print_path(prev,i,path);
                while(!path.empty()){
                    temp_path[i].push_back(path.front()+1);
                    path.erase(path.begin());
                }
            }
            for(int i=0;i<vertex_num;i++){
                if(myVertex[i]>0){ //有driver的點
                    target=i;
                    vertex_sort(dist,available_driver,target,temp_path);
                    // available_driver_num++; //可以的點
                }
            }
            // for(int i=0;i<available_driver.size();i++){
            //     cout<<available_driver[i].vertex_id<<" "<<available_driver[i].distance<<endl;
            // }
            // cout<<available_driver_num<<endl;
            int available_driver_num=available_driver.size();
            vector<vector<int>> vertex_minDistance_path(available_driver_num);
            Store_path(vertex_minDistance_path,prev,available_driver); //存有外送員路徑

            int vertex_id=-1; //driver location
            int min_distance=0; //distance from driver to resaurtant
            vector<int> src_path; //path of min_distance
            for(int i=0;i<available_driver_num;i++){
                bool ok=true;
                for(int v=0;v<vertex_minDistance_path[i].size()-1;v++){
                    int src=vertex_minDistance_path[i][v];
                    int des=vertex_minDistance_path[i][v+1];
                    if(graph_capacity[src-1][des-1]<traffic_space){
                        // cout<<src<<" "<<des<<" "<<traffic_space<<" "<<graph_capacity[src-1][des-1]<<endl;
                        ok=false; // 不OK
                        break;
                    }
                }
                if(ok==true){ //所有edge都有足夠空間
                    vertex_id=available_driver[i].vertex_id;
                    min_distance=available_driver[i].distance;
                    src_path=vertex_minDistance_path[i]; //actually available_driver should be combined with minDistance
                    for(int v=0;v<vertex_minDistance_path[i].size()-1;v++){
                        int src=vertex_minDistance_path[i][v];
                        int des=vertex_minDistance_path[i][v+1];
                        graph_capacity[src-1][des-1]-=traffic_space; //佔用空間
                        graph_capacity[des-1][src-1]-=traffic_space;
                    }
                    myVertex[vertex_id-1]--; //占用外送員
                    break;
                }
            }
            if(vertex_id==-1){ //no driver
                cout<<"Just walk. T-T"<<endl;
            }
            else{
                cout<<"Order "<<id<<" from: "<<vertex_id<<endl;
                vector<int> des_path; //pre-store empty destination path
                order_info order={id,traffic_space,min_distance,-1,source_vertex,-1,src_path,des_path};
                myOrder.push_back(order);
            }
            // for(int i=0;i<vertex_num;i++){
            //     for(int j=0;j<vertex_num;j++)
            //         cout<<graph_capacity[i][j]<<" ";
            //     cout<<endl;
            // }
        }
        if(command=="Drop"){
            int id,destination_vertex;
            inputData>>id>>destination_vertex;
            //cout<<command<<" "<<id<<" "<<destination_vertex<<endl;
            order_info tempOrder;
            for(int index=0;index<myOrder.size();index++){ //找哪份訂單
                if(myOrder[index].id==id){
                    myOrder[index].destination_vertex=destination_vertex;
                    tempOrder=myOrder[index];
                    break;
                }
            }
            // cout<<tempOrder.id<<" "<<tempOrder.traffic_space<<endl;
            // cout<<tempOrder.soure_vertex<<" "<<tempOrder.destination_vertex<<endl;
            if(!tempOrder.src_path.empty()){
                for(int v=0;v<tempOrder.src_path.size()-1;v++){ //加回空間
                    int src=tempOrder.src_path[v];
                    int des=tempOrder.src_path[v+1];
                    graph_capacity[src-1][des-1]+=tempOrder.traffic_space; //加回佔用空間
                    graph_capacity[des-1][src-1]+=tempOrder.traffic_space;     
                }                
            }
            vector<allPath> SRCtoDES_path; //store all path from source to destinatation and distance
            findallPath(graph_distance,tempOrder.soure_vertex,tempOrder.destination_vertex,vertex_num,SRCtoDES_path); //點對點所有路徑
           
            SortPath(SRCtoDES_path); //根據distance由小排到大
            PickPath(SRCtoDES_path,graph_capacity,tempOrder); //挑路徑加扣capacity

            if(tempOrder.des_distance==-1){
                cout<<"No Way Home"<<endl;
                waitingOrder.push_back(tempOrder); //放到等待清單
            }
            else{
                int totalDistance=tempOrder.src_distance+tempOrder.des_distance;
                cout<<"Order "<<id<<" distance: "<<totalDistance<<endl;
                for(int index=0;index<myOrder.size();index++){ //找哪份訂單
                    if(myOrder[index].id==id){
                        myOrder[index]=tempOrder; //補回des_path
                        break;
                    }
                }
            }
        }
        if(command=="Complete"){
            int id;
            inputData>>id;
            //cout<<command<<" "<<id<<endl;
            order_info tempOrder;
            for(int index=0;index<myOrder.size();index++){ //找哪份訂單
                if(myOrder[index].id==id){
                    tempOrder=myOrder[index];
                }
            }
            // for(int v:tempOrder.des_path)
            //     cout<<v<<" ";
            // cout<<endl;
            for(int v=0;v<tempOrder.des_path.size()-1;v++){ //加回空間
                int src=tempOrder.des_path[v];
                int des=tempOrder.des_path[v+1];
                graph_capacity[src-1][des-1]+=tempOrder.traffic_space; //加回佔用空間
                graph_capacity[des-1][src-1]+=tempOrder.traffic_space;
            }
            myVertex[tempOrder.destination_vertex-1]++; //加回外送員 at client location

            if(waitingOrder.empty()){ //沒有等待清單
                // cout<<"nothing waiting"<<endl;
                continue;
            }
            int waiting_order_size=waitingOrder.size(); //排序waiting Order
            for(int i=1;i<waiting_order_size;i++){
                order_info key=waitingOrder[i];
                int j=i-1;
                while(j>=0 && waitingOrder[j].id>key.id){
                    waitingOrder[j+1]=waitingOrder[j];
                    j--;
                }
                waitingOrder[j+1]=key;
            }
            for(int i=0;i<waiting_order_size;i++){
                vector<allPath> SRCtoDES_path; //store all path from source to destinatation and distance
                // cout<<waitingOrder[i].soure_vertex<<" "<<waitingOrder[i].destination_vertex<<endl;
                // cout<<waitingOrder[i].src_distance<<endl;
                findallPath(graph_distance,waitingOrder[i].soure_vertex,waitingOrder[i].destination_vertex,vertex_num,SRCtoDES_path);
                SortPath(SRCtoDES_path);
                PickPath(SRCtoDES_path,graph_capacity,waitingOrder[i]);
                if(waitingOrder[i].des_distance!=-1){
                    int totalDistance=waitingOrder[i].src_distance+waitingOrder[i].des_distance;
                    cout<<"Order "<<waitingOrder[i].id<<" distance: "<<totalDistance<<endl;
                    for(int index=0;index<myOrder.size();index++){ //找哪份訂單
                        if(myOrder[index].id==waitingOrder[i].id){
                            myOrder[index].des_path=waitingOrder[i].des_path;
                        }
                    }
                    waitingOrder.erase(waitingOrder.begin()+i);
                    waiting_order_size--; //少一個
                    i--; //指標往前移
                    // cout<<"WaitingOrderSize:"<<waitingOrder.size()<<endl;
                }
            }
        }
        // for(int i=0;i<vertex_num;i++){
        //     for(int j=0;j<vertex_num;j++)
        //         cout<<graph_capacity[i][j]<<" ";
        //     cout<<endl;
        // }
        // for(int i=0;i<vertex_num;i++)
        //     cout<<myVertex[i]<<" ";
        // cout<<endl;
    }    

    return 0;
}

// function
void getValue(vector<vector<int>> &graph,int row,int col,int value){
    graph[row-1][col-1] = value;
    graph[col-1][row-1] = value;
}
int minDistance(vector<int> &dist,vector<bool> &sptSet,int vertex_num){
    int min = 10000;
    int min_index=-1;
    for(int v=0;v<vertex_num;v++){
        if(sptSet[v]==false && dist[v]<min){
            min=dist[v];
            min_index=v;
        }
    }

    return min_index;
}
void Print_sol(vector<int> dist,vector<int> &prev,int vertex_num){
    cout<<"Vertex\tDistance\tpath"<<endl;
    for(int i=0;i<vertex_num;i++){
        cout<<i+1<<"\t"<<dist[i]<<"\t\t";
        vector<int> path;
        Print_path(prev,i,path);
        while(!path.empty()){
            cout<<path.front()+1<<" ";
            path.erase(path.begin());
        }
        cout<<endl;
    }
}
void Print_path(vector<int> &prev,int v,vector<int> &path){
    if(prev[v]==-1){
        path.push_back(v);
        return;
    }
    Print_path(prev,prev[v],path);
    path.push_back(v);
} 
void Store_path(vector<vector<int>> &vertex_minDistance_path,vector<int> &prev,vector<available_vertex> &available_driver){
    // int index=0;
    // for(available_vertex i:available_driver){
    //     vector<int> path;
    //     Print_path(prev,i.vertex_id-1,path);
    //     while(!path.empty()){
    //         vertex_minDistance_path[index].push_back(path.front()+1);
    //         path.erase(path.begin());
    //     }
    //     index++;
    // }
    for(int i=0;i<available_driver.size();i++){
        vector<int> path;
        Print_path(prev,available_driver[i].vertex_id-1,path);
        while(!path.empty()){
            vertex_minDistance_path[i].push_back(path.front()+1);
            path.erase(path.begin());
        }
    }
    // for(int i=0;i<available_driver.size();i++){
    //     cout<<available_driver[i].vertex_id<<"\t";
    //     for(int v:vertex_minDistance_path[i]){
    //         cout<<v<<" ";
    //     }
    //     cout<<endl;
    // }
}
void dijkstra(vector<vector<int>> &graph,int src,int vertex_num,vector<int> &dist,vector<int> &prev){
    // vector<int> dist(vertex_num,INT8_MAX);
    vector<bool> sptSet(vertex_num,false);
    // vector<int> prev(vertex_num,-1);

    dist[src-1]=0;
    for(int count=0;count<vertex_num-1;count++){
        int u=minDistance(dist,sptSet,vertex_num);
        sptSet[u]=true;
        for(int v=0;v<vertex_num;v++){
            if(!sptSet[v] && graph[u][v] != 0 && dist[u]!=10000 && dist[u]+graph[u][v]<dist[v]){
                dist[v]=dist[u]+graph[u][v];
                prev[v]=u;
            }
        }
    }
    // Print_sol(dist,prev,vertex_num);
}
void vertex_sort(vector<int> &dist,vector<available_vertex> &available_driver,int target,vector<vector<int>> &temp_path){
    if(available_driver.empty()){
        available_vertex temp={target+1,dist[target]}; // id / distance
        available_driver.push_back(temp);
    }
    else{
        int size=available_driver.size();
        bool inserted=false;
        for(int i=0;i<size;i++){
            if(dist[target]<available_driver[i].distance){
                available_vertex temp={target+1,dist[target]};
                available_driver.insert(available_driver.begin()+i,temp);
                inserted = true;
                break;
            }
            if(dist[target]==available_driver[i].distance && temp_path[target].size()<temp_path[available_driver[i].vertex_id-1].size()){
                available_vertex temp={target+1,dist[target]};
                available_driver.insert(available_driver.begin()+i,temp);
                inserted = true;
                break;
            }
        }
        if(inserted==false){
            available_vertex temp={target+1,dist[target]};
            available_driver.push_back(temp);
            inserted=true;
        }
    }
}
void StoD(vector<vector<int>> &graph,int src,int des,vector<bool> &visited,vector<int> &path,int vertex_num,vector<allPath> &SRCtoDES){
    visited[src]=true;
    path.push_back(src+1);

    if(src==des){
        allPath temp={0,path};
        SRCtoDES.push_back(temp);
        // for(int i = 0; i < path.size(); i++) {
        //     cout << path[i] << " ";
        // }
        // cout << endl;
    }
    else{
        for(int i=0;i<vertex_num;i++){
            if(graph[src][i]!=0 && !visited[i]){
                StoD(graph,i,des,visited,path,vertex_num,SRCtoDES);
            }
        }
    }
    visited[src]=false;
    path.pop_back();
}
void findallPath(vector<vector<int>> &graph,int src_vertex,int des_vertex,int vertex_num,vector<allPath> &SRCtoDES){
    vector<bool> visited(vertex_num,false);
    vector<int> path;

    StoD(graph,src_vertex-1,des_vertex-1,visited,path,vertex_num,SRCtoDES);
    for(int i=0;i<SRCtoDES.size();i++){
        if(SRCtoDES[i].path.size()==1){ //停留在同一點
            SRCtoDES[i].distance=0;
        }
        for(int v=0;v<SRCtoDES[i].path.size()-1;v++){
            int s=SRCtoDES[i].path[v];
            int d=SRCtoDES[i].path[v+1];
            SRCtoDES[i].distance+=graph[s-1][d-1];
        } 
    }
}
void SortPath(vector<allPath> &SRCtoDES){
    int allPath_size=SRCtoDES.size();
    for(int i=1;i<allPath_size;i++){
        allPath key=SRCtoDES[i];
        int j=i-1;
        while(j>=0&&SRCtoDES[j].distance>key.distance){
            SRCtoDES[j+1]=SRCtoDES[j];
            j--;
        }
        SRCtoDES[j+1]=key;
    }
    // for(int i=0;i<SRCtoDES.size();i++){
    //     cout<<SRCtoDES[i].distance<<"\t";
    //     for(int v:SRCtoDES[i].path){
    //         cout<<v<<" ";
    //     }cout<<endl;
    // }       
}
void PickPath(vector<allPath> &SRCtoDES,vector<vector<int>> &graph_capacity,order_info &tempOrder){
    for(int i=0;i<SRCtoDES.size();i++){ //n條路徑
        bool ok=true;
        for(int v=0;v<SRCtoDES[i].path.size()-1;v++){ //每條路徑節點
            int src=SRCtoDES[i].path[v];
            int des=SRCtoDES[i].path[v+1];
            if(graph_capacity[src-1][des-1]<tempOrder.traffic_space){
                // cout<<src<<" "<<des<<" "<<graph_capacity[src-1][des-1]-tempOrder.traffic_space<<endl;
                ok=false;
                break;
            }
        }
        if(ok==true){
            tempOrder.des_distance=SRCtoDES[i].distance;
            tempOrder.des_path=SRCtoDES[i].path;
            for(int v=0;v<SRCtoDES[i].path.size()-1;v++){
                int src=SRCtoDES[i].path[v];
                int des=SRCtoDES[i].path[v+1];
                graph_capacity[src-1][des-1]-=tempOrder.traffic_space; //佔用空間
                graph_capacity[des-1][src-1]-=tempOrder.traffic_space;             
            }
            break;
        }
    }
}