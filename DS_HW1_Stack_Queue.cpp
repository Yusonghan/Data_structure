// not use STL <algorithm>, <stack>, <queue>, <vector>, <list>, etc 
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const int max_cols = 100;
const int max_actions = 100;

struct Action{
	string type;
	int column;
};

class Stack{
private:
	int top; // index of top element 
	int capacity; // allocated memory space of array
	char *stack; // array representing stack
	//ItemType type; // for map
	void DoubleCapacity(); // double the capacity of stack
public:
	Stack() :top(-1), capacity(1){ // constructor
		stack = new char[capacity]; // initial state : top=-1, capacity=1
	}
	~Stack(){
		delete[] stack; //release memory
	}
	void Push(char c);
	void Pop();
	bool IsEmpty();
	int Top();
	int GetSize();
	void Output();
	void Bump_change(int i, int h);
};

class Queue{
private:
	int capacity, front, back;
	char *queue;
	void DoubleCapacity();
public:
	Queue() :capacity(5), front(-1), back(-1){
		queue = new char(capacity);
	};
	void Push(char x);
	void Pop();
	bool IsEmpty();
	bool IsFull();
	int getFront();
	int getBack();
	int getSize();
	void Output();
};

void Action_DIG(int DIG_column, int R, Stack(&Map)[max_cols], Stack &Bag, Queue &Inventory);

int main(){
	string input;
	string input_line;
	while (getline(cin, input_line) && !input_line.empty()){
		input += input_line + "\n";
	}

	istringstream inputData(input);
	int R, L, N;
	inputData >> R >> L >> N;
	inputData.ignore();
	//cout << R << " " << L << " " << N << endl;

	Stack Map_temp[max_cols];
	Stack Map[max_cols];

	for (int i = 0; i < L; i++){
		string map_line;
		getline(inputData, map_line);
		istringstream mapData(map_line);
		char c;
		int j = 0;
		while (mapData >> c){
			Map_temp[j].Push(c);
			++j;
		}
	}
	for (int i = 0; i < L; i++){
		for (int j = 0; j < R; j++){
			Map[j].Push(Map_temp[j].Top());
			Map_temp[j].Pop();
		}
	}
	/*for (int i = 0; i < L; i++){
		for (int j = 0; j < R; j++){
			cout << char(Map[j].Top()) << " ";
			Map[j].Pop();
		}
		cout << endl;
	}*/

	Action action[max_actions];
	for (int i = 0; i < N; ++i){
		inputData >> action[i].type;
		if (action[i].type == "DIG"){
			inputData >> action[i].column;
		}
		//cout << action[i].type << " " << action[i].column << endl;
	}



	// start operating code
	Stack Bag; // store Diamod and Gold
	Queue Inventory; // store Props

	for(int i = 0; i < N; ++i) {
		if (action[i].type == "DIG") {
			for (int j = 0; j < R; j++){
				while (char(Map[j].Top()) == '_')
					Map[j].Pop();
			}
			int DIG_column = action[i].column;
			if (Map[DIG_column].IsEmpty())continue;
			else if (Map[DIG_column].Top() != '_' && !Map[DIG_column].IsEmpty()) {
				Action_DIG(DIG_column, R, Map, Bag, Inventory);
			}
			else if (Map[DIG_column].Top() == '_' && !Map[DIG_column].IsEmpty()) {
				while (Map[DIG_column].Top() == '_') {
					Map[DIG_column].Pop();
				}
				if (!Map[DIG_column].IsEmpty()) {
					Action_DIG(DIG_column, R, Map, Bag, Inventory);
				}
			}
		}
		else if (action[i].type == "USE") {
			if (Inventory.IsEmpty())continue;
			char UseItem = char(Inventory.getFront());
			if (UseItem == 'F'){
				int highest_col = 0;
				for (int j = 0; j < R; j++){
					if (Map[j].GetSize() >= highest_col)
						highest_col = Map[j].GetSize();
				}
				if (highest_col > 0){
					cout << "MINE LEVEL:" << highest_col << endl;
					for (int j = 0; j < R; j++){
						if (Map[j].GetSize() == highest_col)
							cout << char(Map[j].Top()) << " ";
						else
							cout << "_" << " ";	
					}
				}
				else{
					cout << "MINE LEVEL:" << 1 << endl;
					for (int j = 0; j < R; j++)
						cout << "_" << " ";
				}
				cout << endl;
				Inventory.Pop(); 
			}
			else if (UseItem == 'M'){
				for (int j = 0; j < R; j++){
					Action_DIG(j, R, Map, Bag, Inventory);
				}
				Inventory.Pop();
			}
		}
	}

	cout << "FINAL BAG:" << endl;
	Bag.Output();
	cout << endl;
	//cout << "FINAL INVENTORY:" << endl;
	//Inventory.Output();
	//cout << endl;
	cout << "FINAL MAP:" << endl;
	int highest_col = 0;
	for (int j = 0; j < R; j++){	
		if (Map[j].GetSize() >= highest_col)
			highest_col = Map[j].GetSize();
	}
	//cout << "highest_col: " << highest_col << endl;
	for (int j = 0; j < R; j++){
		if (Map[j].GetSize() < highest_col) {
			while (Map[j].GetSize() < highest_col)
				Map[j].Push('_');
		}
	}

	for (int i = 0; i < highest_col; i++){
		for (int j = 0; j < R; j++){
			cout << char(Map[j].Top()) << " ";
			Map[j].Pop();
		}
		cout << endl;
	}

	//system("pause");
	return 0;
}



void Stack::DoubleCapacity(){
	capacity *= 2; // double capacity
	char *newStack = new char[capacity]; // create new stack
	
	for (int i = 0; i < capacity / 2; i++){
		newStack[i] = stack[i];
	}

	delete[] stack; // release memory
	stack = newStack;
}
void Stack::Push(char c){
	if (top == capacity - 1){  // if full,double capacity
 		DoubleCapacity();
	}
	stack[++top] = c; // put x to the stack top
}
void Stack::Pop(){
	if (IsEmpty()){
		//cout << "Stack is empty." << endl;
		return;
	}
	top--;
}
bool Stack::IsEmpty(){
	return (top == -1); // 位於頂端回傳true, 反之false
}
int Stack::Top(){
	if (IsEmpty()){
		//cout << "Stack is empty." << endl;
		return -1;
	}
	return stack[top];
}
int Stack::GetSize(){
	return (top + 1);
}
void Stack::Output(){
	for (int i = 0; i <= top; i++){
		cout << stack[i] << " ";
	}
}
void Stack::Bump_change(int i, int h) {
	if (i >= h - 1)
		return;
	else{
		stack[i] = '_';
	}
}

void Queue::DoubleCapacity(){
	capacity *= 2;
	char *newQueue = new char[capacity];
	
	int j = -1;
	for (int i = 0; i < front + 1; i++){
		j++;
		newQueue[j] = queue[i];
	}
	front = -1;
	back = j;
	delete[]queue;
	queue = newQueue;
}
void Queue::Push(char x){
	if (IsFull()){
		DoubleCapacity();
	}
	queue[++back] = x;
}
void Queue::Pop(){
	if (IsEmpty()){
		//cout << "Queue is empty." << endl;
		return;
	}
	front++;
}
bool Queue::IsEmpty(){
	return(front == back);
}
bool Queue::IsFull(){
	return(back + 1 == capacity);
}
int Queue::getFront(){
	if (IsEmpty()){
		//cout << "Queue is empty." << endl;
		return - 1;
	}
	return queue[front + 1];
}
int Queue::getBack(){
	if (IsEmpty()){
		//cout << "Queue is empty." << endl;
		return -1;
	}
	return queue[back];
}
int Queue::getSize(){
	return (back - front);
}
void Queue::Output(){
	for (int i = 0; i + front < back; i++){
		cout << queue[i + front + 1] << " ";
	}
}

void Action_DIG(int DIG_column,int R,Stack (&Map)[max_cols],Stack &Bag,Queue &Inventory){
	if (DIG_column >= 0 && DIG_column < R) {
		char DigItem = Map[DIG_column].Top();
		if (DigItem == 'D' || DigItem == 'G') {
			Bag.Push(DigItem);
			Map[DIG_column].Pop();
		}
		else if (DigItem == 'F' || DigItem == 'M'){
			Inventory.Push(DigItem);
			Map[DIG_column].Pop();
		}
		else if (DigItem == 'P'){
			while (char(Bag.Top()) == 'G')
				Bag.Pop();
			Map[DIG_column].Pop();
		}
		else if (DigItem == 'C'){	
			Map[DIG_column].Pop();
			if (Bag.IsEmpty())return;
			int lower_bound, upper_bound;
			if (DIG_column - 2 < 0)
				lower_bound = 0;
			else
				lower_bound = DIG_column - 2;
			if (DIG_column + 2 > R - 1)
				upper_bound = R - 1;
			else
				upper_bound = DIG_column + 2;

			int highest_col = 0;
			for (int j = 0; j < R; j++){
				if (Map[j].GetSize() > highest_col)
					highest_col = Map[j].GetSize(); //find highest
			}
			if (char(Bag.Top()) == 'G'){
				for (int j = lower_bound; j <= upper_bound; j++){
					while (Map[j].GetSize() < highest_col)
						Map[j].Push('_');
					for (int i = 0; i < 3; i++)
						Map[j].Push('G');
				}
			}
			else if (char(Bag.Top()) == 'D'){
				for (int j = lower_bound; j <= upper_bound; j++){
					while (Map[j].GetSize() < highest_col)
						Map[j].Push('_');
					for (int i = 0; i < 3; i++)
						Map[j].Push('D');
				}
			}
		}
		else if (DigItem == 'B'){
			int h = Map[DIG_column].GetSize();
			Map[DIG_column].Pop(); // pop bomb
			if (!Map[DIG_column].IsEmpty())
				Map[DIG_column].Pop(); // pop under bomb
			while (char(Map[DIG_column].Top()) == '_')
				Map[DIG_column].Pop();
			if (DIG_column + 1 < R){
				while (char(Map[DIG_column + 1].Top()) == '_')
					Map[DIG_column + 1].Pop();
				int h_back = Map[DIG_column + 1].GetSize();
				if (h_back > h + 1 && h_back > 3) {
					Map[DIG_column + 1].Bump_change(h, h_back);
					Map[DIG_column + 1].Bump_change(h - 1, h_back);
					Map[DIG_column + 1].Bump_change(h - 2, h_back);
				}
				else if (h_back > h + 1 && h_back == 3) {
					Map[DIG_column + 1].Bump_change(h, h_back);
					Map[DIG_column + 1].Bump_change(h - 1, h_back);
				}
				else if (h_back == h + 1 && h_back > 2) {
					Map[DIG_column + 1].Pop();
					Map[DIG_column + 1].Pop();
					Map[DIG_column + 1].Pop();
				}
				else if (h_back == h && h_back > 1){
					Map[DIG_column + 1].Pop();
					Map[DIG_column + 1].Pop();
				}
				else if (h_back == h - 1 && h_back > 0){
					Map[DIG_column + 1].Pop();
				}
				else if (h_back == h + 1 && h_back == 2) {
					Map[DIG_column + 1].Pop();
					Map[DIG_column + 1].Pop();
				}
				else if (h_back == h && h_back == 1) {
					Map[DIG_column + 1].Pop();
				}
				else{
					return;
				}
			}
			if (DIG_column - 1 >= 0){
				while (char(Map[DIG_column - 1].Top()) == '_')
					Map[DIG_column - 1].Pop();
				int h_front = Map[DIG_column - 1].GetSize();
				if (h_front > h + 1 && h_front > 3) {
					Map[DIG_column - 1].Bump_change(h, h_front);
					Map[DIG_column - 1].Bump_change(h - 1, h_front);
					Map[DIG_column - 1].Bump_change(h - 2, h_front);
				}
				else if (h_front > h + 1 && h_front == 3) {
					Map[DIG_column - 1].Bump_change(h, h_front);
					Map[DIG_column - 1].Bump_change(h - 1, h_front);
				}
				else if (h_front == h + 1 && h_front > 2) {
					Map[DIG_column - 1].Pop();
					Map[DIG_column - 1].Pop();
					Map[DIG_column - 1].Pop();
				}
				else if (h_front == h && h_front > 1) {
					Map[DIG_column - 1].Pop();
					Map[DIG_column - 1].Pop();
				}
				else if (h_front == h - 1 && h_front > 0) {
					Map[DIG_column - 1].Pop();
				}
				else if (h_front == h + 1 && h_front == 2) {
					Map[DIG_column - 1].Pop();
					Map[DIG_column - 1].Pop();
				}
				else if (h_front == h && h_front == 1) {
					Map[DIG_column - 1].Pop();
				}
				else{
					return;
				}
			}

		}
	}
}