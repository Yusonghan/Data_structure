#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Patient{
    string name;
    int age;
    int money; 
    int membership;
    int membership_date;
};

void Output(Patient patient);
int convertSTRINGtoINT(string date);
// int partition_2nd(vector<Patient> &patients,int start,int end);
// void quicksort_2nd(vector<Patient> &have_membership,int start,int end);
void sort_1st(vector<Patient> &paitents,vector<Patient> &have_membership,vector<Patient> &nothave_membership);
void sort_2nd(vector<Patient> &have_membership);
void sort_3rd(vector<Patient> &paitents);
void sort_money(vector<Patient> &paitent_bear);
void Output_income(vector<Patient> &paitent_panda,int X,vector<Patient> &originalpaitents);
void bear_hospital(vector<Patient> &paitents,int K);
void merge_2nd(vector<Patient> &patients,int start,int mid,int end);
void mergesort_2nd(vector<Patient> &patients,int start,int end);
void merge_3rd(vector<Patient> &patients,int start,int mid,int end);
void mergesort_3rd(vector<Patient> &patients,int start,int end);
void merge_money(vector<Patient> &patients,int start,int mid,int end);
void mergesort_money(vector<Patient> &patients,int start,int end);
void selectionsort_2nd(vector<Patient> &paitents,int X);
void selectionsort_3rd(vector<Patient> &paitents,int X);
void selectionsort_money(vector<Patient> &paitents,int K);

int main()
{
    string input,input_line;
    while(getline(cin,input_line) && !input_line.empty()){
        input += input_line + '\n';
    }
    istringstream inputData(input);

    int N,M;
    inputData>>N>>M;
    // cout<<N<<" "<<M<<endl;
    inputData.ignore();

    vector<Patient> paitents;
    vector<Patient> have_membership;
    vector<Patient> nothave_membership;
    for(int i=0;i<N;i++){
        Patient p;
        getline(inputData,p.name);

        inputData>>p.age>>p.money>>p.membership;
        inputData.ignore();
        string date;
        getline(inputData,date);
        if(date=="NaN"){
            p.membership_date=0;
        }
        else{
            p.membership_date=convertSTRINGtoINT(date);
        }

        // if(p.membership==1){
        //     have_membership.push_back(p);
        // }
        // else{
        //     nothave_membership.push_back(p);
        // }
        paitents.push_back(p);
    }
    // for(Patient p:paitents){
    //     Output(p);
    // }

    for(int day=0;day<M;day++){ //next M day
        int P,X,K;
        inputData>>P>>X>>K;
        inputData.ignore();
        // cout<<P<<" "<<X<<" "<<K<<endl;
        for(int j=0;j<P;j++){ //input patients for that day
            Patient p;
            getline(inputData,p.name);
            inputData>>p.age>>p.money>>p.membership;
            inputData.ignore();
            string date;
            getline(inputData,date);
            if(date=="NaN"){
                p.membership_date=0;
            }
            else{
                p.membership_date=convertSTRINGtoINT(date);
            }
            
            // Output(p);
            // if(p.membership==1){
            //     have_membership.push_back(p);
            // }
            // else{
            //     nothave_membership.push_back(p);
            // }
            paitents.push_back(p);
        }

        vector<Patient> originalpaitents = paitents;
        sort_1st(paitents,have_membership,nothave_membership);
        // sort_3rd(have_membership);
        // sort_2nd(have_membership);
        // quicksort_2nd(have_membership,0,have_membership.size()-1);

        // if(X < have_membership.size()/4){        
        //     selectionsort_2nd(have_membership,X);
        //     selectionsort_3rd(have_membership,X);
        // }
        // else{
        mergesort_3rd(have_membership,0,have_membership.size()-1); // Alphabat
        mergesort_2nd(have_membership,0,have_membership.size()-1); // membership_date                
        // }
    

        // sort_3rd(nothave_membership);
        if(X > have_membership.size()){
            mergesort_3rd(nothave_membership,0,nothave_membership.size()-1); //alphabat
        }
        
        paitents = have_membership;
        for(Patient p:nothave_membership){
            paitents.push_back(p);
        }
        
        // paitents.insert(paitents.end(),nothave_membership.begin(),nothave_membership.end());
        have_membership.clear();
        nothave_membership.clear();

        // for(Patient p:paitents){
        //     Output(p);
        // }         

        cout<<"DAY #"<<day+1<<endl;
        Output_income(paitents,X,originalpaitents);

        // for(Patient p:paitents){
        //     Output(p);
        // }   

        // sort_money(paitents);
        // if(K<paitents.size()/4){
        //     selectionsort_money(paitents,K);
        // }
        // else{
        mergesort_money(originalpaitents,0,originalpaitents.size()-1);
        // }
        bear_hospital(originalpaitents,K);
        paitents = originalpaitents;

        // for(Patient p:paitents){
        //     Output(p);
        // } 
    }

    return 0;
}

void Output(Patient patient){
    // cout << "Name: " << patient.name << endl;
    // cout << "Age: " << patient.age << endl;
    // cout << "Money: " << patient.money << endl;
    // cout << "Membership: " << patient.membership << endl;
    // cout << "Membership Date: " << patient.membership_date << endl;
    // cout << "-----------------------------" << endl; 
    cout<<patient.name<<endl;
    cout<<patient.age<<" "<<patient.money<<" "<<patient.membership<<" ";
    cout<<patient.membership_date<<endl;
}
int convertSTRINGtoINT(string date){
    string date_without_dash;
    for(char c:date){
        if(c!='-'){
            date_without_dash+=c;
        }
    }

    return stoi(date_without_dash);
}
// int partition_2nd(vector<Patient> &patients,int start,int end){
//     int pivot = patients[start].membership_date;

//     int count=0;
//     for(int i=start+1;i<=end;i++){
//         if(patients[i].membership_date < pivot){
//             count++;
//         }
//     }

//     int pivotIndex = start+count;
//     swap(patients[pivotIndex],patients[start]);

//     int i=start,j=end;
//     while(i<pivotIndex && j>pivotIndex){
//         while(patients[i].membership_date <= pivot){
//             i++;
//         }
//         while(patients[j].membership_date > pivot){
//             j--;
//         }
//         if(i<pivotIndex && j>pivotIndex){
//             swap(patients[i++],patients[j--]);
//         }
//     }

//     return pivotIndex;
// }
// void quicksort_2nd(vector<Patient> &have_membership,int start,int end){
//     if(start>=end){
//         return;
//     }
//     // int p = partition_2nd(have_membership,start,end);
//     Patient pivot = have_membership[start];
//     int lt = start,gt = end,i = start+1;

//     while(i<=gt){
//         if(have_membership[i].membership_date < pivot.membership_date){
//             swap(have_membership[lt++],have_membership[i++]);
//         }
//         else if(have_membership[i].membership_date > pivot.membership_date){
//             swap(have_membership[i],have_membership[gt--]);
//         }
//         else{
//             i++;
//         }
//     }
//     // sort left part
//     quicksort_2nd(have_membership,start,lt-1);
//     // sort right part
//     quicksort_2nd(have_membership,gt+1,end);
// }
void sort_1st(vector<Patient> &paitents,vector<Patient> &have_membership,vector<Patient> &nothave_membership){
    int size=paitents.size();  //sort have/nothave membership
    for(int i=0;i<size;i++){
        if(paitents[i].membership==0){ // no membership
            nothave_membership.push_back(paitents[i]);
        }
        else{
            have_membership.push_back(paitents[i]);
        }
    }
    paitents.clear();
}
void sort_2nd(vector<Patient> &have_membership){ //sort membership date
    int size = have_membership.size(); 
    for(int i=1;i<size;i++){
        Patient key=have_membership[i];
        int j=i-1;
        while(j>=0 && have_membership[j].membership_date>key.membership_date){
            have_membership[j+1] = have_membership[j];
            j--;
        }
        have_membership[j+1] = key;
    }
}
void sort_3rd(vector<Patient> &paitents){
    int size = paitents.size();
    for(int i=1;i<size;i++){
        Patient key=paitents[i];
        int j=i-1;
        while(j>=0 && paitents[j].name>key.name == 1){
            paitents[j+1] = paitents[j];
            j--;
        }
        paitents[j+1] = key;
    }
}
void sort_money(vector<Patient> &paitent_bear){
    int size = paitent_bear.size();
    for(int i=1;i<size;i++){
        Patient key=paitent_bear[i];
        int j=i-1;
        while(j>=0 && paitent_bear[j].money<key.money){
            paitent_bear[j+1] = paitent_bear[j];
            j--;
        }
        paitent_bear[j+1] = key;
    }
}
void Output_income(vector<Patient> &paitent_panda,int X,vector<Patient> &originalpaitents){
    int income=0;
    int size = paitent_panda.size();
    if(X<size){
        for(int i=0;i<X;i++){
            cout<<paitent_panda[i].name<<" "<<paitent_panda[i].age<<endl;
            income += paitent_panda[i].money;
            // paitent_panda.erase(paitent_panda.begin());
            for(int idx=0;idx<originalpaitents.size();idx++){
                if(originalpaitents[idx].name == paitent_panda[i].name && originalpaitents[idx].age == paitent_panda[i].age && originalpaitents[idx].membership_date == paitent_panda[i].membership_date && originalpaitents[idx].money == paitent_panda[i].money){
                    originalpaitents.erase(originalpaitents.begin()+idx);
                    idx--;
                }
            }
        }        
        // paitent_panda.erase(paitent_panda.begin(),paitent_panda.begin()+X);
        paitent_panda.clear();
    }
    else{
        for(int i=0;i<size;i++){
            cout<<paitent_panda[i].name<<" "<<paitent_panda[i].age<<endl;
            income += paitent_panda[i].money;
        }
        paitent_panda.clear();
        originalpaitents.clear();
    }
    income = income*0.9;
    cout<<"INCOME TODAY: "<<income<<endl;
}
void bear_hospital(vector<Patient> &paitents,int K){
    if(K==0){
        return;
    }
    int size = paitents.size();
    if(K<size){
        // for(int i=0;i<K;K--){
        //     paitents.erase(paitents.begin());
        // }
        paitents.erase(paitents.begin(),paitents.begin()+K);
    }
    else{
        paitents.clear();
    }
}
void merge_2nd(vector<Patient> &patients,int start,int mid,int end){ //membership_date
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<Patient> left(n1), right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = patients[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = patients[mid + 1 + j];

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (left[i].membership_date <= right[j].membership_date) {
            patients[k++] = left[i++];
        } 
        else {
            patients[k++] = right[j++];
        }
    }

    while (i < n1) {
        patients[k++] = left[i++];
    }

    while (j < n2) {
        patients[k++] = right[j++];
    }
}
void mergesort_2nd(vector<Patient> &patients,int start,int end){
    if (start >= end) {
        return;
    }
    int mid = start + (end - start) / 2;
    mergesort_2nd(patients, start, mid);
    mergesort_2nd(patients, mid + 1, end);
    merge_2nd(patients, start, mid, end);
}
void merge_3rd(vector<Patient> &patients,int start,int mid,int end){ //Alphabat
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<Patient> left(n1), right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = patients[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = patients[mid + 1 + j];

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if ((left[i].name <= right[j].name)){
            patients[k++] = left[i++];
        } else {
            patients[k++] = right[j++];
        }
    }

    while (i < n1) {
        patients[k++] = left[i++];
    }

    while (j < n2) {
        patients[k++] = right[j++];
    }
}
void mergesort_3rd(vector<Patient> &patients,int start,int end){
    if (start >= end) {
        return;
    }
    int mid = start + (end - start) / 2;
    mergesort_3rd(patients, start, mid);
    mergesort_3rd(patients, mid + 1, end);
    merge_3rd(patients, start, mid, end);
}
void merge_money(vector<Patient> &patients,int start,int mid,int end){
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<Patient> left(n1), right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = patients[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = patients[mid + 1 + j];

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (left[i].money >= right[j].money){
            patients[k++] = left[i++];
        } else {
            patients[k++] = right[j++];
        }
    }

    while (i < n1) {
        patients[k++] = left[i++];
    }

    while (j < n2) {
        patients[k++] = right[j++];
    }
}
void mergesort_money(vector<Patient> &patients,int start,int end){
    if (start >= end) {
        return;
    }
    int mid = start + (end - start) / 2;
    mergesort_money(patients, start, mid);
    mergesort_money(patients, mid + 1, end);
    merge_money(patients, start, mid, end);
}
void selectionsort_2nd(vector<Patient> &paitents,int X){
    int min_index;
    int n = paitents.size();

    for(int i=0;i<X;i++){
        min_index=i;
        for(int j=i+1;j<n;j++){
            if(paitents[j].membership_date < paitents[min_index].membership_date){
                min_index = j;
            }
        }

        if(min_index != i){
            swap(paitents[min_index],paitents[i]);
        }
    }
}
void selectionsort_3rd(vector<Patient> &paitents,int X){
    int min_index;
    int n = paitents.size();

    for(int i=0;i<X-1;i++){
        min_index=i;
        for(int j=i+1;j<X;j++){
            if(paitents[j].name < paitents[min_index].name){
                min_index = j;
            }
        }

        if(min_index != i){
            swap(paitents[min_index],paitents[i]);
        }
    }
}
void selectionsort_money(vector<Patient> &paitents,int K){
    int min_index;
    int n = paitents.size();

    for(int i=0;i<K;i++){
        min_index=i;
        for(int j=i+1;j<n;j++){
            if(paitents[j].money > paitents[min_index].money){
                min_index = j;
            }
        }

        if(min_index != i){
            swap(paitents[min_index],paitents[i]);
        }
    }
}