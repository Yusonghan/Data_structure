#include<iostream>
#include<string>
#include<sstream>

using namespace std;

struct Command{
	string command;
	char shuffle_clock;
	int shuffle_times;
	int check_num;
	int reverse_num;
};

struct Node{
	int Pokemon_value;
	int damage_value;
	int health_value;
	int Pokemon_level;
	Node* prev;
	Node* next;
	Node(int val1, int val2, int val3, int val4) :Pokemon_value(val1), damage_value(val2), health_value(val3), Pokemon_level(val4), prev(nullptr), next(nullptr){};
};

class CircularDoubleLinkedList{
private:
	Node* head;
	int count;
public:
	CircularDoubleLinkedList() :head(nullptr){};
	void insert(int val1, int val2, int val3, int val4, int position);
	void remove(int position);
	bool isEmpty();
	int size();
	void display();
	void display_one(int position);
	void Shuffle(char c, int times);
	void Check(int check_num);
	void Reverse(int reverse_num,CircularDoubleLinkedList &mylist);
	void Evolve();
	void Attack();
};


int main()
{
	//ios_base::sync_with_stdio(false);
	//cin.tie(0);
	string input;
	string input_line;
	while (getline(cin, input_line) && !input_line.empty()){
		input += input_line + "\n";
	}
	istringstream inputData(input);

	int number_of_commands;
	inputData >> number_of_commands;
	inputData.ignore();

	CircularDoubleLinkedList myList;
	Command Mycommand;
	int target = 0; //head
	for (int i = 0; i < number_of_commands; i++){
		inputData >> Mycommand.command;
		if (Mycommand.command == "Insert"){
			int Pokemon_value;
			int damage_value;
			int health_value;
			int Pokemon_level = 1;
			string Pokemon_stance;

			inputData >> Pokemon_value;
			inputData >> damage_value;
			inputData >> health_value;
			inputData >> Pokemon_stance;
			if (Pokemon_stance == "Neutral"){
				myList.insert(Pokemon_value, damage_value, health_value, Pokemon_level, target);
			}
			if (Pokemon_stance == "Evolve"){
				myList.insert(Pokemon_value, damage_value, health_value, Pokemon_level, target);
				myList.Evolve();
			}
			if (Pokemon_stance == "Attack"){
				myList.insert(Pokemon_value, damage_value, health_value, Pokemon_level, target);
				myList.Attack();
			}
		}
		if (Mycommand.command == "Delete"){
			myList.remove(target); // 不用加target，head會自動往下移
		}
		if (Mycommand.command == "Shuffle"){
			inputData >> Mycommand.shuffle_clock;
			inputData >> Mycommand.shuffle_times;
			myList.Shuffle(Mycommand.shuffle_clock, Mycommand.shuffle_times);
		}
		if (Mycommand.command == "Check"){
			inputData >> Mycommand.check_num;
			myList.Check(Mycommand.check_num);
		}
		if (Mycommand.command == "Reverse"){
			inputData >> Mycommand.reverse_num;
			myList.Reverse(Mycommand.reverse_num, myList);
		}
	}

	myList.display();
	return 0;
}

void CircularDoubleLinkedList::insert(int val1, int val2, int val3, int val4, int position) {
	Node* newNode = new Node(val1, val2, val3, val4);
	if (head == nullptr){
		head = newNode;
		head->next = head;
		head->prev = head;
		count++;
	}
	else{
		Node* current = head;
		//for (int i = 0; i < position; ++i){
		//	current = current->next;
		//}
		newNode->next = current;
		newNode->prev = current->prev;
		current->prev->next = newNode;
		current->prev = newNode;
		count++;
	}
}
void CircularDoubleLinkedList::remove(int position){
	if (count == 0)
		return;
	if (count == 1){
		head = nullptr;
	}
	Node* current = head;
	if (count>1) {	
		head = head->next; //target 移到下一個
		current->prev->next = current->next;
		current->next->prev = current->prev;
		delete current;
	}
	count--;
}
bool CircularDoubleLinkedList::isEmpty(){
	return count == 0;
}
int CircularDoubleLinkedList::size(){
	return count;
}
void CircularDoubleLinkedList::display(){
	if (count == 0) {
		cout << "no Poke Poke ;-;" << endl;
		//cout << count << endl;
		return;
	}
	Node* current = head;
	/*while (true) {
		cout << "ID: " << current->Pokemon_value << " ";
		cout << "HP: " << current->health_value << " ";
		cout << "LVL: " << current->Pokemon_level << endl;

		current = current->next;

		// 如果下一個節點是頭部節點，表示已經遍歷完整個循環
		if (current == head)
			break;
	}*/
	for (int i = 0; i < count; i++){
		cout << "ID: " << current->Pokemon_value << " ";
		cout << "HP: " << current->health_value << " ";
		cout << "LVL: " << current->Pokemon_level << endl;

		current = current->next;
	}
}
void CircularDoubleLinkedList::display_one(int position){
	if (head == nullptr)
		return;

	Node* current = head;
	for (int i = 0; i < position; ++i){
		current = current->next;
	}
	cout << "ID: " << current->Pokemon_value << " ";
	cout << "HP: " << current->health_value << " ";
	cout << "LVL: " << current->Pokemon_level << endl;
}
void CircularDoubleLinkedList::Shuffle(char c, int times){
	if (count < 2)
		return;
	Node* current = head;
	int turn = times % count;
	if (c == 'c'){
		for (int i = 0; i < turn; i++){
			current = current->next;
		}	
		int temp_Pokemon_value = current->Pokemon_value;
		int temp_demage_value = current->damage_value;
		int temp_health_value = current->health_value;
		int temp_Pokemon_level = current->Pokemon_level;
		current->Pokemon_value = head->Pokemon_value;
		current->damage_value = head->damage_value;
		current->health_value = head->health_value;
		current->Pokemon_level = head->Pokemon_level;
		head->Pokemon_value = temp_Pokemon_value;
		head->damage_value = temp_demage_value;
		head->health_value = temp_health_value;
		head->Pokemon_level = temp_Pokemon_level;
		head = current;
	}
	if (c == 'a'){
		for (int i = 0; i < turn; i++){
			current = current->prev;
		}
		int temp_Pokemon_value = current->Pokemon_value;
		int temp_demage_value = current->damage_value;
		int temp_health_value = current->health_value;
		int temp_Pokemon_level = current->Pokemon_level;
		current->Pokemon_value = head->Pokemon_value;
		current->damage_value = head->damage_value;
		current->health_value = head->health_value;
		current->Pokemon_level = head->Pokemon_level;
		head->Pokemon_value = temp_Pokemon_value;
		head->damage_value = temp_demage_value;
		head->health_value = temp_health_value;
		head->Pokemon_level = temp_Pokemon_level;
		head = current;
	}
}
void CircularDoubleLinkedList::Check(int check_num) {
	if (count < 2)
		return;
	Node* current = head;
	int seen[101] = { 0 }; //存放重複的數字
	for (int i = 0; i < check_num; i++){
		int value = current->Pokemon_value;
		seen[value]++;
		current = current->next;
	}
	current = head;
	for (int i = 0; i < check_num; i++){
		int value = current->Pokemon_value;
		if (seen[value]>1) {
			if (count == 1){
				head = nullptr;
				count--;
			}
			if (count > 1){
				if (head == current)
					head = head->next;
				current->prev->next = current->next;
				current->next->prev = current->prev;
				count--;
			}
		}
		current = current->next;
	}
}
void CircularDoubleLinkedList::Reverse(int reverse_num, CircularDoubleLinkedList &mylist) {
	if (count < 2 || reverse_num < 2)
		return;
	int index = 0;
	int original_length = count;
	int times = 0;
	while (index < original_length) {
		Node* current = head;
		int rest = original_length - reverse_num*times;
		if (rest < reverse_num){
			for (int len = 0; len < rest - 1; len++){
				current = current->next;
			}
			for (int len = 0; len < rest; len++){
				int value = current->Pokemon_value;
				int demage = current->damage_value;
				int health = current->health_value;
				int level = current->Pokemon_level;
				mylist.insert(value, demage, health, level, 0);
				current = current->prev;
			}
			current = current->next;
			for (int len = 0; len < rest; len++){
				mylist.remove(0);
				index++;
			}
		}
		if (rest >= reverse_num){
			for (int len = 0; len < reverse_num - 1; len++){
				current = current->next;
			}
			for (int len = 0; len < reverse_num; len++){
				int value = current->Pokemon_value;
				int demage = current->damage_value;
				int health = current->health_value;
				int level = current->Pokemon_level;
				mylist.insert(value, demage, health, level, 0);
				current = current->prev;
			}
			current = current->next;
			for (int len = 0; len < reverse_num; len++){
				mylist.remove(0);
				index++;
			}
		}	
		times++;
	}
}
void CircularDoubleLinkedList::Evolve(){
	if (count <= 2)
		return;
	if (head->Pokemon_level == 3)
		return;
	if (head->prev->prev->Pokemon_level == 3)
		return;
	if (head->prev->Pokemon_value == head->Pokemon_value && head->prev->Pokemon_value == head->prev->prev->Pokemon_value){
		int highest_damage = 0;
		int highest_health = 0;
		int highest_level = 1;
		Node* current = head;
		for (int i = 0; i < 3; i++){
			if (current->damage_value>highest_damage)
				highest_damage = current->damage_value;
			if (current->health_value > highest_health)
				highest_health = current->health_value;
			if (current->Pokemon_level > highest_level)
				highest_level = current->Pokemon_level;
			current = current->prev;
		}
		current->next = head;
		head->prev = current;
		head->damage_value = highest_damage;
		head->health_value = highest_health;
		head->Pokemon_level = highest_level + 1;
		count = count - 2;
	}
}
void CircularDoubleLinkedList::Attack(){
	if (count == 1)
		return;
	else if (count == 2){
		head->health_value = head->health_value - head->prev->damage_value;
		if (head->health_value <= 0){
			Node* current = head;
			head = head->next; //target 移到下一個
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			count--;
		}
	}
	else if (count >= 3){
		head->health_value = head->health_value - head->prev->damage_value;
		head->prev->prev->health_value = head->prev->prev->health_value - head->prev->damage_value;
		if (head->prev->prev->health_value <= 0) {
			Node* current = head->prev->prev;
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			count--;
		}
		if (head->health_value <= 0){
			Node* current = head;
			head = head->prev; //target 移到insert value
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			count--;
		}
	}
}