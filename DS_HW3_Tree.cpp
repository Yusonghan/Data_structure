#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<queue> // BFS
using namespace std;

struct TreeNode{
	int parent_id;
	int id;
	int distance;
	vector<TreeNode*> children;
	vector<int> nearNode;
	TreeNode(int _parent_id, int _id, int _distance) :parent_id(_parent_id), id(_id), distance(_distance){};
};
// function
void Output(TreeNode* node, int depth);
void addNode(vector<TreeNode*>& nodes, int parent, int node, int distane);
void Delete(vector<TreeNode*>& nodes, int node);
void findMaximumPathSum(int& result, int currentNode, int previousNode, vector<TreeNode*> nodes);

int main()
{
	string input;
	string input_line;
	while (getline(cin, input_line) && !input_line.empty()){
		input += input_line + '\n';
	}
	int node_num;
	int operation_num;
	istringstream inputData(input);
	inputData >> node_num >> operation_num;

	int root, root_value;
	inputData >> root >> root_value;

	int MAX_NODES = 10000;
	vector<TreeNode*> nodes(node_num + operation_num + 1);
	nodes[root] = new TreeNode(-1,root, root_value);

	for (int i = 0; i < node_num; i++){
		int node_parent, node, node_value;
		inputData >> node_parent >> node >> node_value;
		addNode(nodes, node_parent, node, node_value);
	}

	for (int j = 0; j < operation_num; j++){
		string command;
		inputData >> command;
		if (command == "Check"){
			int result = 0;
			findMaximumPathSum(result, 0, -1, nodes);
			cout << "Maximum Path: " << result << endl;
			//cout << "Root of the Path: " << result.first << endl;
		}
		if (command == "Add"){
			int node_parent, node, node_value;
			inputData >> node_parent >> node >> node_value;
			addNode(nodes, node_parent, node, node_value);
		}
		if (command == "Delete"){
			int node;
			inputData >> node;
			Delete(nodes, node);
		}
	}

	int depth = 0;
	//cout << "Tree structure" << endl;
	Output(nodes[root], depth);

	//int final_result = findTreeMaxDistance(nodes);
	//cout << "Final Root: " << final_result << endl;

	for (TreeNode* node : nodes){
		delete node;
	}
	return 0;
}
void Output(TreeNode* root, int depth){
	if (!root){
		cout << "Tree is empty!" << endl;
		return;
	}
	for (int i = 0; i < depth; i++){
		cout << " ";
	}
	cout << root->id << "(" << root->distance << "),(" << root->parent_id << ")" << ",(";
	for (int i : root->nearNode){
		cout << i << " ";
	}
	cout << ")" << endl;

	for (TreeNode* child : root->children){
		Output(child, depth + 1);
	}
}
void addNode(vector<TreeNode*>& nodes, int node_parent, int node, int node_value){
	if (!nodes[node_parent]){
		nodes[node_parent] = new TreeNode(-1, node_parent, 0);
	}
	if (!nodes[node]){
		nodes[node] = new TreeNode(node_parent, node, node_value);
	}
	else{
		nodes[node]->distance = node_value;
		nodes[node]->parent_id = node_parent;
	}
	nodes[node_parent]->children.push_back(nodes[node]);
	// 相連點
	nodes[node_parent]->nearNode.push_back(node);
	nodes[node]->nearNode.push_back(node_parent);
}
void Delete(vector<TreeNode*>& nodes, int node){
	if (!nodes[node])
		return;
	else{
		int node_parent = nodes[node]->parent_id;
		int length = nodes[node_parent]->children.size();
		//cout << length << endl;
		for (int i = 0; i < length; length--) {
			TreeNode* temp_node = nodes[node_parent]->children[i];
			//cout << temp_node->id << endl;
			if (temp_node->id == node){
				nodes[node_parent]->children.erase(nodes[node_parent]->children.begin());
				nodes[node_parent]->nearNode.erase(nodes[node_parent]->nearNode.begin());
			}
			else{
				//cout << "ya" << endl;
				nodes[node_parent]->children.push_back(temp_node);
				nodes[node_parent]->children.erase(nodes[node_parent]->children.begin());
				nodes[node_parent]->nearNode.push_back(temp_node->id);
				nodes[node_parent]->nearNode.erase(nodes[node_parent]->nearNode.begin());
			}
		}
		for (TreeNode* temp_node : nodes[node]->children){
			nodes[node_parent]->children.push_back(temp_node); // 把子節點移給節點的母節點
			nodes[node_parent]->nearNode.push_back(temp_node->id);
			nodes[temp_node->id]->parent_id = node_parent;
			int length = nodes[temp_node->id]->nearNode.size();
			for (int i = 0; i < length; length--) {
				if (i == node){
					nodes[temp_node->id]->nearNode.erase(nodes[temp_node->id]->nearNode.begin());
				}
				else{
					nodes[temp_node->id]->nearNode.push_back(i);
					nodes[temp_node->id]->nearNode.erase(nodes[temp_node->id]->nearNode.begin());
				}
			}
		}
		delete nodes[node];
	}
}
void findMaximumPathSum(int& result, int currentNode, int previousNode, vector<TreeNode*> nodes) {
	// node to which currentNode is connected to
	vector<int> v = nodes[currentNode]->nearNode;
	int maximumSum1 = 0;
	int maximumSum2 = 0;
	//cout << v.size() << endl;
	for (int i = 0; v.size(); i++){
		//checking whether the branch is visited already
		if (v[i] == previousNode){
			continue;
		}
		findMaximumPathSum(result, v[i], currentNode, nodes);
		//storing the maximum of value of branch path sums
		// maximumSum1 store maximum value
		// maximumSum2 store 2nd maximum value
		if (nodes[v[i]]->distance > maximumSum1){
			maximumSum2 = maximumSum1;
			maximumSum1 = nodes[v[i]]->distance;
		}
		else{
			maximumSum2 = max(maximumSum2, nodes[v[i]]->distance);
		}
	}
	result = max(result, nodes[currentNode]->distance + maximumSum1 + maximumSum2);
	// updating the value of current valuewith maximum path sum including currentNode
	nodes[currentNode]->distance += maximumSum1;
}

