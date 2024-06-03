#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int sort(vector<int> &arr);
int fast_sort(vector<int> &arr);
void Output(vector<int> &arr);
void merge(vector<int> &arr,int start,int mid,int end,int &times);
void mergesort(vector<int> &arr,int start,int end,int &times);
void quicksort(vector<int> &arr,int start,int end,int &times,int &A);

int main()
{
    string input,input_line;
    while(getline(cin,input_line) && !input_line.empty()){
        input += input_line + '\n';
    }
    istringstream inputData(input);

    int num;
    inputData>>num;

    vector<int> arr(num);
    for(int i=0;i<num;i++){
        inputData>>arr[i];
    }

    // int times = sort(arr);
    // int times = fast_sort(arr);
    int times = 0;
    int A = 0;
    quicksort(arr,0,arr.size()-1,times,A);
    // Output(arr);
    cout<<times<<endl;

    return 0;
}

int sort(vector<int> &arr){
    int num = arr.size();
    int times = 0;
    for(int i=0;i<num-1;i++){
        for(int j=i+1;j<num;j++){
            if(arr[i]==i+1){
                break;
            }
            if(arr[i] - arr[j] == 1){
                swap(arr[i],arr[j]);
                times++;
                j=i;
                continue;
            }            
        }
    }

    return times;
}
int fast_sort(vector<int> &arr){
    int num = arr.size();
    int times = 0;

    for(int i=0;i<num;i++){
        int small = 0;
        for(int j=i+1;j<num;j++){
            if(arr[i]>arr[j]){
                small++;
            }
        }
        
        times += small;
    }

    return times;
}
void Output(vector<int> &arr){
    for(int n:arr){
        cout<<n<<" ";
    }
    cout<<endl;
}
void merge(vector<int> &arr,int start,int mid,int end,int &times){
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<int> left(n1),right(n2);


    int i=0,j=0,k=start;
    while(i < n1 && j < n2){
        if(left[i] < right[j]){
            arr[k++] = left[i++];
        }
        else{
            arr[k++] = right[j++];
        }
    }

    while(i < n1){
        arr[k++] = left[i++];
    }
    while(j < n2){
        arr[k++] = right[j++];
    }
    times++;
}
void mergesort(vector<int> &arr,int start,int end,int &times){
    if(start>=end){
        return;
    }
    // int times = 0;

    int mid = start + (end - start) / 2;
    mergesort(arr,start,mid,times);
    mergesort(arr,mid+1,end,times);
    merge(arr,start,mid,end,times);
}
void quicksort(vector<int> &arr,int start,int end,int &times,int &A){
    if(start>=end){
        return;
    }
    int pivot = end - start + 1 - A;//arr[start];
    cout<<pivot<<endl;
    int lt = start,gt = end,i = start+1;

    while(i<=gt){
        if(arr[i] < pivot){
            swap(arr[lt++],arr[i++]);
            times++;
        }
        else if(arr[i] > pivot){
            swap(arr[i],arr[gt--]);
        }
        Output(arr);
    }
    // sort left part
    A++;
    quicksort(arr,start,lt-1,times,A);
    // // sort right part
    quicksort(arr,gt+1,end,times,A);
}

