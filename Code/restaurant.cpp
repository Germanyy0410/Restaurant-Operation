#include "main.h"
using namespace std;

class Node {
public:
	long long key;
    string name;
	int dishes;
	Node* left;
	Node* right;
	int height;
	int ID;

	Node() {
		this->left = nullptr;
		this->right = nullptr;
	}

	Node(long long key, string name) {
		this->key = key;
        this->name = name;
		this->left = nullptr;
		this->right = nullptr;
		this->height = 1;
		this->dishes = 1;
		this->ID = key % (MAXSIZE/2);
	}

	Node(long long key, int ID, string name) {
		this->key = key;
        this->name = name;
		this->left = nullptr;
		this->right = nullptr;
		this->height = 1;
		this->dishes = 1;
		this->ID = ID;
	}
};

class AVL {
public:
	Node* root;
	int count;
	AVL() {
		root = nullptr;
		count = 0;
	}
	~AVL() {}

	int max(int a, int b) {
		return (a > b) ? a : b;
	}


	int height(Node *node) {
        if (node == NULL)
            return 0;
        int lh = this->height(node->left);
        int rh = this->height(node->right);
        return (lh > rh ? lh : rh) + 1;
    }

	void reHeight() {
		queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			Node* temp = q.front();
			q.pop();
			temp->height = height(temp);
			if (temp->left != nullptr) {
				q.push(temp->left);
			}
			if (temp->right != nullptr) {
				q.push(temp->right);
			}
		}
	}

	Node* rightRotate(Node* &parent) {
		Node* newParent = parent->left;
		Node* movement = newParent->right;

		newParent->right = parent;
		parent->left = movement;

		parent->height = max(height(parent->left), height(parent->right)) + 1;
		newParent->height = max(height(newParent->left), height(newParent->right)) + 1;

		if (this->root->key == newParent->right->key && this->root->name == newParent->right->name) {
			this->root = newParent;
		}

		reHeight();

		return newParent;
	}

	Node* leftRotate(Node* &parent) {
		Node* newParent  = parent->right;
		Node* movement = newParent->left;

		newParent->left = parent;
		parent->right = movement;

		newParent->height = max(height(newParent->left), height(newParent->right)) + 1;
		parent->height = max(height(parent->left), height(parent->right)) + 1;
		
		if (this->root->key == newParent->left->key && this->root->name == newParent->left->name) {
			this->root = newParent;
		}

		reHeight();

		return newParent;
	}

	int getBalance(Node* x) {
		if (x == nullptr) {
			return 0;
		}
		return height(x->left) - height(x->right);
	}

	Node* insert(Node* &root, long long key, int ID, string name) {
		if (root == nullptr) {
			root = new Node(key, ID, name);
			this->count++;
			return root;
		}

		if (key < root->key) {
			root->left = insert(root->left, key, ID, name);
		} else if (key >= root->key) {
			root->right = insert(root->right, key, ID, name);
		}

		root->height = 1 + max(height(root->left), height(root->right));

		int balance = getBalance(root);

		// LH of LH
		if (balance > 1 && key < root->left->key) {
			return rightRotate(root);
		}

		// RH of RH
		if (balance < -1 && key >= root->right->key) {
			return leftRotate(root);
		}

		// RH of LH
		if (balance > 1 && key >= root->left->key) {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}

		// LH of RH
		if (balance < -1 && key <= root->right->key) {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	Node* traverse(long long result, string name) {
		queue<Node*> q;
		q.push(this->root);
		while (!q.empty()) {
			Node* temp = q.front();
			q.pop();

			if (temp->name == name && temp->key == result) {
				return temp;
			}

			if (temp->left != nullptr) {
				q.push(temp->left);
			}
			if (temp->right != nullptr) {
				q.push(temp->right);
			}
		}

		return nullptr;
	}

	Node* deleteNode(Node* root, long long key, string name) {
		if (root == nullptr) {
			return root;
		}

		if (key <= root->key && name != root->name) {
			root->left = deleteNode(root->left, key, name);

		} 
		if (key >= root->key && name != root->name) {
			root->right = deleteNode(root->right, key, name);

		}

		if (key == root->key && name == root->name) {
			if (root->left == nullptr || root->right == nullptr) {
				Node *child = root->left ? root->left : root->right;

				if (child == nullptr) {
					child = root;
					root = nullptr;
				} else {
					*root = *child;
				}
				count--;
				delete child;

			} else {
				Node* maxNode = root->right;
				if (maxNode != nullptr) {
					while (maxNode->left != nullptr) {
						maxNode = maxNode->left;
					}
				}

				Node* tmp = maxNode;

				root->key = tmp->key;
				root->ID = tmp->ID;
				root->name = tmp->name;
				root->dishes = tmp->dishes;

				root->right = deleteNode(root->right, tmp->key, tmp->name);
			}
			
		}

		if (root == nullptr) {
			return root;
		}

		root->height = 1 + max(height(root->left), height(root->right));

		int balance = getBalance(root);

		// LH of LH
		if (balance > 1 && getBalance(root->left) >= 0) {
			return rightRotate(root);
		}

		// RH of RH
		if (balance < -1 && getBalance(root->right) <= 0) {
			return leftRotate(root);
		}

		// RH of LH
		if (balance > 1 && getBalance(root->left) < 0) {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}

		// LH of RH
		if (balance < -1 && getBalance(root->right) > 0) {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	void deleteAVL() {
		if (this->root == nullptr) {
			return;
		}

		stack<Node*> nodeStack;
		nodeStack.push(this->root);

		while (!nodeStack.empty()) {
			Node* current = nodeStack.top();
			nodeStack.pop();

			if (current->right != nullptr) {
				nodeStack.push(current->right);
			}
			if (current->left != nullptr) {
				nodeStack.push(current->left);
			}
			delete current;
		}

		this->root = nullptr;
	}

	void preOrder(Node* root) {
		if (root != nullptr) {
			cout << root->key << " " << root->name << endl;
			preOrder(root->left);
			preOrder(root->right);
		}
	}

	void inOrder(Node* root, string name) {
		if (root == nullptr) {
			return;
		}

		inOrder(root->left, name);
		if (root->name == name) {
			cout << "\033[1;32m";
		}
		cout <<root->key;
		if (root->key / 10000 > 0) {
			cout << "     ";
		} else if (root->key / 1000 > 0) {
			cout << "      ";
		} else if (root->key / 100 > 0) {
			cout << "       ";
		} else if (root->key / 10 > 0) {
			cout << "        ";
		} else {
			cout << "         ";
		}
		
		if (root->ID < 10) {
			cout << " ";
		}
		cout << root->ID << "      " << root->dishes << "      " << root->name.substr(0, 12) << endl;
		if (root->name == name) {
			cout << "\033[0m";
		}
		inOrder(root->right, name);
	}

	void printNSpace(int n) {
		for (int i = 0; i < n - 1; i++) {
			cout << "  ";
		}
	}

	void printState(Node* root) {
		queue<Node*> q;
		q.push(root);
		int count = 0;
		int maxNode = 1;
		int level = 0;
		int space = pow(2, root->height);
		printNSpace(space / 2);

		while (!q.empty()) {
			// int n = q.size();
			// for (int i = 0; i < n; i++) {
			// 	Node* curr = q.front();
			// 	q.pop();
			// 	cout << "(" << curr->key << "-" << curr->name.substr(0, 4) << ")" << "  ";
			// 	if (curr->left != nullptr) {
			// 		q.push(curr->left);
			// 	}
			// 	if (curr->right != nullptr) {
			// 		q.push(curr->right);
			// 	}
			// }

			Node* temp = q.front();
			q.pop();

			if (temp == nullptr) {
				cout << "  ";
				q.push(nullptr);
				q.push(nullptr);
			} else {
				cout << temp->key;
				q.push(temp->left);
				q.push(temp->right);
			} 
			printNSpace(space);
			count++;
			if (count == maxNode) {
				cout << endl;
				count = 0;
				maxNode *= 2;
				level++;
				space /= 2;
				printNSpace(space / 2);
			}
			if (level == root->height) {
				return;
			}
		}
	}
};

class table {
	public:
		int ID;			
		int initialID;		// result % 32
		int zone;
		int dishes;
		int hashIdx;
		int latestOrder;
		long long result;
		string name;
		table* next;
		table* prev;

		table() {
			this->next = nullptr;
			this->prev = nullptr;
			this->name = "";
		}

		table(int ID, long long result, string name) {
			this->ID = ID;
			this->initialID = result % 32;
			this->result = result;
			this->name = name;
			this->dishes = 1;
			this->prev = nullptr;
			this->next = nullptr;
			this->hashIdx = ID;
		}

		table(int ID, long long result, string name, int hashIdx) {
			this->ID = ID;
			this->initialID = result % 32;
			this->result = result;
			this->name = name;
			this->dishes = 1;
			this->prev = nullptr;
			this->next = nullptr;
			this->hashIdx = hashIdx;
		}

		table(string name, long long result, int zone) {
			this->name = name;
			this->result = result;
			this->zone = zone;
			this->dishes = 1;
			this->next = nullptr;
			this->prev = nullptr;
		}

		void clearTable() {
			this->dishes = 0;
			this->initialID = 0;
			this->name = "";
			this->next = nullptr;
			this->prev = nullptr;
			this->result = 0;
			this->zone = 0;
		}
	};

class DLinkedList {
public: 
	table* head;
	table* tail;
	int count;

	DLinkedList() {
		this->head = nullptr;
		this->tail = nullptr;
		this->count = 0;
	}

	void add(int idx, long long result, string name) {
		table* tmp = new table(idx, result, name);

		if (head == NULL) {
			head = tmp;
			tail = tmp;
		} else {
			tail->next = tmp;
			tmp->prev = tail;
			tail = tail->next;
		}

		count++;
		return;
	}

	void addTotal(int zone, long long result, int ID, string name, int numLine) {
		table* tmp = new table(name, result, zone);
		tmp->latestOrder = numLine;
		tmp->ID = ID;

		if (head == NULL) {
			head = tmp;
			tail = tmp;
		} else {
			tail->next = tmp;
			tmp->prev = tail;
			tail = tail->next;
		}

		count++;
		return;
	}

	void printTotal(string name) {
		cout << endl << "No. Guess in Restaurant = "  << this->count << ":" << endl << endl;
		cout << endl << "Ord   Zone   ID   Result   Dishes   Name" << endl << endl;
		table* tmp = head;
		while (tmp != NULL) {
			if (tmp->name == name) {
				cout << "\033[1;32m";
			}
			if (tmp->latestOrder < 10) {
				cout << " ";
			}
			cout << tmp->latestOrder << "     " << tmp->zone << "     " << tmp->ID;
			if (tmp->ID > 9) {
				cout << "    ";
			} else {
				cout << "     ";
			}
			
			cout << tmp->result;

			if (tmp->result / 10000 > 0) {
				cout << "     ";
			} else if (tmp->result / 1000 > 0) {
				cout << "      ";
			} else if (tmp->result / 100 > 0) {
				cout << "       ";
			} else if (tmp->result / 10 > 0) {
				cout << "        ";
			} else {
				cout << "         ";
			}
			cout << tmp->dishes << "     " << tmp->name.substr(0, 10);
			if (tmp->name == name) {
				cout << "\033[0m";
			}
			tmp = tmp->next;
			cout << endl;
		}
		cout << endl;
	}

	void deleteTail() {
		table* curr = nullptr;

		if (head == nullptr) {
			return;
		} else if (head == tail) {
			curr = head;
		} else {
			curr = tail;
			tail = tail->prev;
			curr->prev = nullptr;
			tail->next = nullptr;
		}

		curr->prev = nullptr;
		curr->next  = nullptr;
		delete curr;
		count--;
	}

	void clear() {
		if (head == nullptr) {
			return;
		}

		table* tmp = head;
		while (tmp != nullptr) {
			table* toDel = tmp;
			tmp = tmp->next;
			delete toDel;
		}
		return;
	}

	table* tempNode() {
		table* tmp = new table();
		return tmp;
	}

	void deleteNode(table *temp) {
        if (temp == NULL || temp->next == NULL) {
            return; // nothing to delete
        }
        if (temp->prev == NULL) {
            temp->next->prev = NULL;
            temp->next = NULL;
            table* remove = temp;
            delete remove;
            return;
        }
        if (temp->next == NULL) {
            temp->prev->next = NULL;
            temp->prev = NULL;
            table* remove = temp;
            delete remove;
            return;
        }
        table* prevNode = temp->prev;
        table* nextNode = temp->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        table* remove = temp;
        delete remove;
	}

	void deleteName(string name) {
		if (head == nullptr) {
			return;
		}

		table* current = head;
		if (head == tail && current == tail) {
			delete current;
			head = nullptr;
			tail = nullptr;
			this->count--;
			return;
		}

        while (current != nullptr && current->name != name) {
            current = current->next;
        }
        if (current == nullptr) {
            return;
        }
        if (current == head) {
            head = current->next;
			head->prev = nullptr;
			current->next = nullptr;
        } else if (current == tail) {
			tail = current->prev;
			tail->next = nullptr;
			current->prev = nullptr;
		} else {
			table* bef = current->prev;
			table* aft = current->next;
			bef->next = aft;
			aft->prev = bef;
			current->next = nullptr;
			current->prev = nullptr;
		}
        delete current;
		this->count--;
	}
	
};

class Compare {
public:
	bool operator()(table* a, table* b) {
		if (a->dishes > b->dishes) {
			return true;
		} 
		else if (a->dishes == b->dishes && a->latestOrder > b->latestOrder) {
			return true;
		}
		return false;
	}
};

void printSequence(DLinkedList* restaurant, vector<int> tableID, string name) {
	cout << endl << "No. Guess in Restaurant = "  << restaurant->count << ":" << endl << endl;

	cout << "ID    Zone   Ord   Dishes    Name" << endl << endl;

	for (unsigned int i = 0; i < tableID.size(); i++) {
		if (tableID[i] < 0) {
			table* tmp = restaurant->head;
			while (tmp != nullptr) {
				if (tmp->ID == int(i) + 1) {
					break;
				}
				tmp = tmp->next;
			}
			if (tmp->name == name) {
				cout << "\033[32m";
			}
			if (tmp->ID < 10) {
				cout << " ";
			}
			cout << tmp->ID << "     " << tmp->zone << "     ";
			if (tmp->latestOrder < 10) {
				cout << " ";
			}
			cout << tmp->latestOrder << "     " << tmp->dishes << "     "<< tmp->name.substr(0, 10);
			cout << endl;
			if (tmp->name == name) {
				cout << "\033[0m";
			}
		}
	}
}

class HashTable {
public:
	unordered_map<int, table*> hash;
	int hashCount;

	HashTable() {
		for (int i = 0; i < MAXSIZE/2; i++) {
			hash[i] = new table(i, 0, "");
		}
		hashCount = 0;
	}

	void deleteHash() {
		for (int i = 0; i < MAXSIZE/2; i++) {
			table* temp = hash[i];
			delete temp;
		}
	}

	void insert(long long result, int ID, string name)  {
		int idx = result % (MAXSIZE / 2);
		while (hash[idx]->name != "") {
			if (idx == MAXSIZE/2 - 1) {
				idx = 0;
			} else {
				idx++;
			}
		}
		delete hash[idx];
		hash[idx] = new table(ID, result, name, idx);
		hashCount++;
	}

	void printHash(string naMe) {
		cout << "Idx   ID    Result   Dishes   Name" << endl << endl;
	
		for (int i = 0; i < MAXSIZE/2; i++) {
			if (hash[i]->name != "") {
				if (hash[i]->name == naMe) {
					cout << "\033[1;32m";
				}
				if (hash[i]->hashIdx < 10) {
					cout << " ";
				}
				cout << hash[i]->hashIdx << "    ";
				if (hash[i]->ID < 10) {
					cout << " ";
				}
				cout << hash[i]->ID << "  ";
				if (hash[i]->result / 10000 > 0) {
					cout << "   ";
				} else if (hash[i]->result / 1000 > 0) {
					cout << "    ";
				} else if (hash[i]->result / 100 > 0) {
					cout << "     ";
				} else if (hash[i]->result / 10 > 0) {
					cout << "      ";
				} else {
					cout << "       ";
				}
				

				cout << hash[i]->result << "    " << hash[i]->dishes << "    " << hash[i]->name.substr(0,12);
				cout << endl;

				if (hash[i]->name == naMe) {
					cout << "\033[0m";
				}
			}
		}
		cout << "\033[0m";
	}

};

class HuffmanTreeNode {
public:
	char data;
	int frequency;
	int state;
	HuffmanTreeNode* left;
	HuffmanTreeNode* right;

	HuffmanTreeNode() {
		this->frequency = 0;
		this->left = nullptr;
		this->right = nullptr;
	}

	HuffmanTreeNode(char data, int frequency) {
		this->data = data;
		this->frequency = frequency;
		this->left = nullptr;
		this->right = nullptr;
	}

	HuffmanTreeNode(int frequency) {
		this->frequency = frequency;
		this->data = '~';
		this->left = nullptr;
		this->right = nullptr;
	}

};

class HuffmanTree : public HuffmanTreeNode {
	public:
		int size;
		int capacity;
		HuffmanTreeNode* rootNode;

		HuffmanTree(int capacity) {
			this->capacity = capacity;
			this->size = 0;
			rootNode = nullptr;
		}

		HuffmanTree() {
			this->capacity = 0;
			this->size = 0;
			this->rootNode = nullptr;
		}

		void inOrder(HuffmanTreeNode* root) {
			if (root == nullptr) {
				return;
			}

			inOrder(root->left);
			cout << root->frequency << root->data << endl;
			inOrder(root->right);
		}

		void preOrder(HuffmanTreeNode* root) {
			if (root == nullptr) {
				return;
			}

			cout << root->frequency << root->data << endl;
			preOrder(root->left);
			preOrder(root->right);
		}

		void printTree(HuffmanTreeNode* root) {
			if (root == nullptr) {
				return;
			}
			queue<HuffmanTreeNode*> q;
			q.push(root);
			while (!q.empty()) {
				int n = q.size();
				for (int i = 0; i < n; i++) {
					HuffmanTreeNode* curr = q.front();
					q.pop();
					cout << "(" << curr->frequency << curr->data << ")" << "  ";
					if (curr->left != nullptr) {
						q.push(curr->left);
					}
					if (curr->right != nullptr) {
						q.push(curr->right);
					}
				}
				cout << endl;
			}
		}

		void Traverse(HuffmanTreeNode* root, unordered_map<char, string> &decoderMap, string binVal) {
			if (root == nullptr) {
				return;
			}

			if (root->left == nullptr && root->right == nullptr) {
				decoderMap[root->data] = binVal;
			}
			
			Traverse(root->left, decoderMap, binVal + "0");
			Traverse(root->right, decoderMap, binVal + "1");
		}

		void deleteTree() {
			if (this->rootNode == nullptr) {
				return;
			}
			stack<HuffmanTreeNode*> nodeStack;
			nodeStack.push(this->rootNode);
			while (!nodeStack.empty()) {
				HuffmanTreeNode* current = nodeStack.top();
				nodeStack.pop();
				if (current->right != nullptr) {
					nodeStack.push(current->right);
				}
				if (current->left != nullptr) {
					nodeStack.push(current->left);
				}
				delete current;
			}
		}
};

void sorting(vector<HuffmanTreeNode*> &arr) {
	for (unsigned int i = 0; i < arr.size(); i++) {
		for (unsigned int j = i + 1; j < arr.size(); j++) {
			if (arr[i]->frequency > arr[j]->frequency) {
				HuffmanTreeNode* temp = arr[i];
				arr[i] = arr[j]; 
				arr[j] = temp;
			} else if (arr[i]->frequency == arr[j]->frequency) {
				if (arr[i]->data > arr[j]->data) {
					HuffmanTreeNode* temp = arr[i];
					arr[i] = arr[j]; 
					arr[j] = temp;
				} else if (arr[i]->data == arr[j]->data && arr[i]->data == '~') {
					if (arr[i]->state > arr[j]->state) {
						HuffmanTreeNode* temp = arr[i];
						arr[i] = arr[j]; 
						arr[j] = temp;
					}
				}
			}
		}
	}
}

void insert(vector<HuffmanTreeNode*> &arr) {
	if (arr.size() <= 1) {
		return;
	}

	sorting(arr);

	HuffmanTreeNode* temp = new HuffmanTreeNode(arr[0]->frequency + arr[1]->frequency);
	temp->state = 1;
	
	int sameFreq = temp->frequency;
	int max = 0;

	for (unsigned int i = 0; i < arr.size(); i++) {
		if (arr[i]->frequency == sameFreq && arr[i]->data == '~') {
			if (arr[i]->state > max) {
				max = arr[i]->state;
			}
		}
	}
	
	if (max != 0) {
		max++;
		temp->state = max;
	}

	temp->left = arr[0];
	temp->right = arr[1];

	arr.erase(arr.begin());
	arr.erase(arr.begin());
	arr.push_back(temp);

	sorting(arr);

	//* Print vector after sorting
	// cout << endl << temp->left->frequency << temp->left->data << " " << temp->right->frequency << temp->right->data << endl;
	// for (int i = 0; i < arr.size(); i++) {
	// 	cout << arr[i]->frequency;
	// 	if (arr[i]->data == '~') {
	// 		HuffmanTreeNode* temp = arr[i];
	// 		while (temp->left != nullptr) {
	// 			temp = temp->left;
	// 		}
	// 		cout << "(" << temp->data;
	// 		temp = arr[i];
	// 		while (temp->right != nullptr) {
	// 			temp = temp->right;
	// 		}
	// 		cout << temp->data << ") ";
	// 	} else {
	// 		cout << arr[i]->data << " ";
	// 	}
	// } 
	// cout << endl;

}

long long stringToInt(const std::string& str) {
    long long result = 0;
    bool isNegative = false;
    unsigned long long i = 0;
    
    if (str[0] == '-') {
        isNegative = true;
        i++;
    }
    
    for (; i < str.size(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            // Invalid input, throw an exception or return an error code
        }
        result = result * 10 + (str[i] - '0');
    }
    
    return isNegative ? -result : result;
}

long long binaryToDecimal(long long n) {
    long long num = n;
    long long dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    long long base = 1;

    long long temp = num;
    while (temp) {
        long long last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

long long HuffmanProcess(string name) {
	vector<HuffmanTreeNode*> vt;

	//* Pair data - freq to vector
	HuffmanTreeNode* tmp = new HuffmanTreeNode(name[0], 1);
	vt.push_back(tmp);
	for (unsigned int i = 1; i < name.size(); i++) {
		bool found = false;

		for (unsigned int j = 0; j < vt.size(); j++) {
			if (name[i] == vt[j]->data) {
				vt[j]->frequency += 1;
				found = true;
				break;
			}
		}

		if (found == false) {
			HuffmanTreeNode* temp = new HuffmanTreeNode(name[i], 1);
			vt.push_back(temp);
		}
	}

	sorting(vt);

	if (vt.size() == 1) {
		int frq = vt[0]->frequency;
		HuffmanTreeNode* toDel = vt[0];
		delete toDel;
		if (frq > 15) {
			frq = 15;
		}
		int res = pow(2, frq) - 1;
		return res;
	}

	// cout << "Initial Frequency:" << endl;
	// for (int i = 0; i < vt.size(); i++) {
	// 	cout << vt[i]->frequency << vt[i]->data << " ";
	// }
	// cout << endl;

	//* Create tree
	while(vt.size() != 1) {
		insert(vt);
	}
	HuffmanTree* encoder = new HuffmanTree();
	encoder->rootNode = vt[0];


	//* Print Encoder Tree
	// cout << endl << "----Encoder Tree----" << endl;
	// encoder->printTree(encoder->rootNode);
	// cout << endl;

	unordered_map<char, string> encoderMap;
	vector<int> binCode;
	string binVal = "";
	string val = "";

	encoder->Traverse(encoder->rootNode, encoderMap, binVal);

	// cout << endl << "Huffman Code:" << endl << endl;
	// for (auto c : encoderMap) {
	// 	cout << c.first << " " << c.second << endl;
	// }

	string res = "";
	for (auto c : name) {
		res += encoderMap[c];
	}
	if (res.size() > 15) {
		res = res.substr(res.size() - 15);
	}

	long long result = binaryToDecimal(stringToInt(res));

	encoder->deleteTree();
	delete encoder;
	// cout << "Binary Code: " << endl << res << endl << endl << "Decimal Code: " << result;
	return result;
}

int checkID(vector<int> &tableID, int ID) {	
	bool found = false;

	if (tableID[ID - 1] == ID) {
		tableID[ID - 1] = -1;
		return -1;
	}
	
	if (found == false) {
		int validID = ID;
		if (validID == MAXSIZE) {
			validID = 1;
		} else {
			validID++;
		}
		for (int i = 1; i <= MAXSIZE; i++) {
			if (tableID[validID - 1] >= 0) {
				tableID[validID - 1] = -1;
				break;
			}
			if (validID == MAXSIZE) {
				validID = 1;
			} else {
				validID++;
			}
		}
		
		return validID;
	}
	return 0;
}

void reheapUp(vector<table*> &minHeap, int pos) {
	// cout << endl << "Before reHeapUp:" << endl;
	// for (int i = 0; i < minHeap.size(); i++) {
	// 	cout << minHeap[i]->ID << "-" << minHeap[i]->dishes << " | ";
	// }

	if (pos <= 0) {
		return;
	}

	if (minHeap.empty() == 1 || minHeap.size() == 1) {
		return;
	}

	int parent = (pos - 1) / 2;
	
	while (pos > 0 && ( (minHeap[parent]->dishes > minHeap[pos]->dishes) ||
	(minHeap[parent]->dishes == minHeap[pos]->dishes && minHeap[parent]->latestOrder > minHeap[pos]->latestOrder) )) {
		swap(minHeap[pos], minHeap[parent]);
		pos = parent;
		parent = (pos - 1) / 2;
	}

	// while ( pos > 0 && minHeap[parent]->dishes > minHeap[pos]->dishes ) {
	// 	swap(minHeap[pos], minHeap[parent]);
	// 	pos = parent;
	// 	parent = (pos - 1) / 2;
	// }
	// cout << endl << endl << "After reHeapUp:" << endl;
	// for (int i = 0; i < minHeap.size(); i++) {
	// 	cout << minHeap[i]->ID << "-" << minHeap[i]->dishes << " | ";
	// }
	// cout << endl << endl;
}

void reheapDown(vector<table*> &minHeap, int pos) {
	// cout << "Before reHeapDown:" << endl;
	// for (int i = 0; i < minHeap.size(); i++) {
	// 	cout << minHeap[i]->ID << "-" << minHeap[i]->dishes << " | ";
	// }

	if (minHeap.empty() == 1 || minHeap.size() == 0) {
		return;
	}
	if (pos == int(minHeap.size() - 1)) {
		return;
	}

	int n =  minHeap.size();
	int j = 2 * pos + 1;

	while (j < n) {

		if (j + 1 < n) {
			if (minHeap[j]->dishes > minHeap[j + 1]->dishes) {
				j++;
			} 
			else if (minHeap[j]->dishes == minHeap[j + 1]->dishes) {
				if (minHeap[j]->latestOrder > minHeap[j + 1]->latestOrder) {
					j++;
				}
			}
		}
		
		if (minHeap[pos]->dishes < minHeap[j]->dishes) {
			break;
		} else if (minHeap[pos]->dishes == minHeap[j]->dishes) {
			if (minHeap[pos]->latestOrder < minHeap[j]->latestOrder) {
				break;
			}
		}
		
		swap(minHeap[pos], minHeap[j]);

		pos = j;
		j = 2 * pos + 1;

	}

	// cout << endl << endl << "After reHeapDown:" << endl;
	// for (int i = 0; i < minHeap.size(); i++) {
	// 	cout << minHeap[i]->ID << "-" << minHeap[i]->dishes << " | ";
	// }
	// cout << endl << endl;
}

void buildHeap(vector<table*> &minHeap) {
	if (minHeap.empty() == 1 || minHeap.size() == 0) {
		return;
	}

	int n = minHeap.size();

	for (int i = (n - 1) / 2; i >= 0; i--) {
		int j = i;
		while (true) {
			int k = j * 2 + 1;

			if (k >= n) {
				break;
			}

			if (k + 1 < n && minHeap[k + 1]->dishes < minHeap[k]->dishes) {
				k++;
			}

			if (minHeap[j]->dishes > minHeap[k]->dishes) {
				swap(minHeap[j], minHeap[k]);
			}
			else {
				break;
			}

			j = k;
        }
	}
}

	void beachZone(HashTable* areaOne, long long result, string name, vector<int> &tableID, DLinkedList* restaurant, int numLine, vector<table*> &minHeap) {
		int flag = checkID(tableID, result % MAXSIZE + 1);

		if (flag == -1) {
			areaOne->insert(result, result % MAXSIZE + 1, name);
			restaurant->addTotal(1, result, result % MAXSIZE + 1, name, numLine);
			tableID[result % MAXSIZE] = -1;

			table* nTable = new table(result % MAXSIZE + 1, result, name);
			nTable->dishes = 1;
			nTable->zone = 1;
			nTable->latestOrder = numLine;
			minHeap.push_back(nTable);
			reheapUp(minHeap, minHeap.size() - 1);
			
		} else {
			areaOne->insert(result, flag, name);
			restaurant->addTotal(1, result, flag, name, numLine);
			tableID[flag - 1] = -1;

			table* nTable = new table(flag, result, name);
			nTable->dishes = 1;
			nTable->zone = 1;
			nTable->latestOrder = numLine;
			minHeap.push_back(nTable);
			reheapUp(minHeap, minHeap.size() - 1);
		}
	}

	void mountainZone(AVL* &areaTwo, long long result, string name, vector<int> &tableID, DLinkedList* &restaurant, int numLine, vector<table*> &minHeap) {
		int flag = checkID(tableID, result % MAXSIZE + 1);

		if (flag == -1) {
			areaTwo->insert(areaTwo->root, result, result % MAXSIZE + 1, name);
			restaurant->addTotal(2, result, result % MAXSIZE + 1, name, numLine);
			tableID[result % MAXSIZE] = -2;

			table* nTable = new table(result % MAXSIZE + 1, result, name);
			nTable->dishes = 1;
			nTable->zone = 2;
			nTable->latestOrder = numLine;
			minHeap.push_back(nTable);		
			reheapUp(minHeap, minHeap.size() - 1);

		} else {
			areaTwo->insert(areaTwo->root, result, flag, name);
			restaurant->addTotal(2,result, flag, name, numLine);
			tableID[flag - 1] = -2;

			table* nTable = new table(flag, result, name);
			nTable->dishes = 1;
			nTable->zone = 2;
			nTable->latestOrder = numLine;
			table* temp = nTable;
			minHeap.push_back(temp);
			reheapUp(minHeap, minHeap.size() - 1);

		}
	}

void FIFO(DLinkedList* restaurant, string name, long long result, HashTable* areaOne, AVL* &areaTwo, vector<int> &tableID, int numLine, vector<table*> &minHeap) {
	int zoneToAdd = restaurant->head->zone;

	//* Delete in Area
	if (zoneToAdd == 1) {
		for (int i = 0; i < MAXSIZE/2; i++) {
			if (areaOne->hash[i]->name == restaurant->head->name) {
				tableID[areaOne->hash[i]->ID - 1] = areaOne->hash[i]->ID;
				areaOne->hash[i]->clearTable();
				break;
			}
		}
		areaOne->hashCount--;

	} else if (zoneToAdd == 2) {
		tableID[restaurant->head->ID - 1] = restaurant->head->ID;
		areaTwo->deleteNode(areaTwo->root, restaurant->head->result, restaurant->head->name);

	}

	//* Delete in minHeap[]
	for (unsigned int i = 0; i < minHeap.size(); i++) {
		if (minHeap[i]->name == restaurant->head->name) {
			swap(minHeap[i], minHeap[minHeap.size() - 1]);
			table* toDel = minHeap[minHeap.size() - 1];
			minHeap.pop_back();
			delete toDel;
			reheapUp(minHeap, i);
			reheapDown(minHeap, i);
			break;
		}
	}

	//* Delete in Restaurant
	restaurant->deleteName(restaurant->head->name);

	if (zoneToAdd == 1) {
		beachZone(areaOne, result, name, tableID, restaurant, numLine, minHeap);

	} else if (zoneToAdd == 2) {
		mountainZone(areaTwo, result, name, tableID, restaurant, numLine, minHeap);

	}
}

void LRCO(DLinkedList* restaurant, string name, long long result, HashTable* areaOne, AVL* &areaTwo, vector<int>& tableID, int numLine, vector<table*> &minHeap) {
	table* curr = restaurant->head;
	table* latest = restaurant->head;

	latest = restaurant->head;
	curr = restaurant->head;
	while (curr != nullptr) {
		if (curr->latestOrder < latest->latestOrder) {
			latest = curr;
		}
		curr = curr->next;
	}

	int zoneToAdd = latest->zone;

	//* Delete in Area
	if (zoneToAdd == 1) {
		for (int i = 0; i < MAXSIZE/2; i++) {
			if (areaOne->hash[i]->name == latest->name) {
				tableID[areaOne->hash[i]->ID - 1] = areaOne->hash[i]->ID;
				areaOne->hash[i]->clearTable();
			}
		}
		areaOne->hashCount--;

	} else if (zoneToAdd == 2) {
		tableID[latest->ID - 1] = latest->ID;
		areaTwo->deleteNode(areaTwo->root, latest->result, latest->name);
	}

	//* Delete in minHeap[]
	for (unsigned int i = 0; i < minHeap.size(); i++) {
		if (minHeap[i]->name == latest->name) {
			swap(minHeap[i], minHeap[minHeap.size() - 1]);
			table* toDel = minHeap[minHeap.size() - 1];
			minHeap.pop_back();
			delete toDel;
			reheapUp(minHeap, i);
			reheapDown(minHeap, i);
			break;
		}
	}

	//* Delete in Restaurant
	restaurant->deleteName(latest->name);

	if (zoneToAdd == 1) {
		beachZone(areaOne, result, name, tableID, restaurant, numLine, minHeap);

	} else if (zoneToAdd == 2) {
		mountainZone(areaTwo, result, name, tableID, restaurant, numLine, minHeap);
	}
}

void LFCO(DLinkedList* restaurant, string name, long long result, HashTable* areaOne, AVL* &areaTwo, vector<int> &tableID, int numLine, vector<table*> &minHeap) {

	table* curr = minHeap[0];
	int idx = 0;

	string nameToDel = curr->name;
	int adjustID = curr->ID;
	int zoneToAdd = curr->zone;
	long long resultToDel = curr->result;

	//* Delete in minHeap[]
	swap(minHeap[idx], minHeap[minHeap.size() - 1]);
	table* toDel = minHeap[minHeap.size() - 1];
	minHeap.pop_back();
	delete toDel;
	// reheapUp(minHeap, idx);
	reheapDown(minHeap, idx);

	//* Delet in Restaurant
	restaurant->deleteName(nameToDel);

	//* Delete in Area
	if (zoneToAdd == 1) {
		for (int i = 0; i < MAXSIZE/2; i++) {
			if (areaOne->hash[i]->name == nameToDel) {
				tableID[areaOne->hash[i]->ID - 1] = areaOne->hash[i]->ID;
				areaOne->hash[i]->clearTable();
			}
		}
		areaOne->hashCount--;

		beachZone(areaOne, result, name, tableID, restaurant, numLine, minHeap);

	} else if (zoneToAdd == 2) {
		tableID[adjustID - 1] = adjustID;
		areaTwo->deleteNode(areaTwo->root, resultToDel, nameToDel);
		
		mountainZone(areaTwo, result, name, tableID, restaurant, numLine, minHeap);
	}

	

}

	int checkExists(DLinkedList* restaurant, string name, int numLine, vector<table*> &minHeap) {
		table* tmp = restaurant->head;
		bool found = false;
		
		while (tmp != nullptr) {
			if (tmp->name == name) {
				found = true;
				break;
			}
			tmp = tmp->next;
		}

		if (found == false) {
			return 0;

		} else {
			
			int updateDishes;
			table* updateTable = nullptr;

			for (unsigned int i = 0; i < minHeap.size(); i++) {
				if (minHeap[i]->name == tmp->name) {
					updateTable = minHeap[i];
					updateDishes = minHeap[i]->dishes;

					swap(minHeap[i], minHeap[minHeap.size() - 1]);
					minHeap.pop_back();
					reheapUp(minHeap, i);
					reheapDown(minHeap, i);
					break;
				}
			}
			
			updateTable->dishes = updateDishes + 1;
			// updateTable->latestOrder = numLine;
			minHeap.push_back(updateTable);
			reheapUp(minHeap, minHeap.size() - 1);
			
			tmp->dishes++;
			tmp->latestOrder = numLine;
			return tmp->zone;
		}
	}

void REG(string name, HashTable* areaOne, AVL* &areaTwo, DLinkedList* &restaurant, int numLine, vector<int>& tableID, vector<table*> &minHeap) {
	long long result = HuffmanProcess(name);
	int checkExist = checkExists(restaurant, name, numLine, minHeap);

	if (checkExist == 1) {
		for (int i = 0; i < MAXSIZE/2; i++) {
			if (areaOne->hash[i]->name == name) {
				areaOne->hash[i]->dishes++;
				break;
			}
		}

		return;

	} else if (checkExist == 2) {
		Node* curr = areaTwo->traverse(result, name);
		curr->dishes++;

		return;
	}

	if (areaOne->hashCount == MAXSIZE/2 && areaTwo->count == MAXSIZE/2) {
		int OPT = result % 3;

		if (OPT == 0) {
			FIFO(restaurant, name, result, areaOne, areaTwo, tableID, numLine, minHeap);

		} else if (OPT == 1) {
			LRCO(restaurant, name, result, areaOne, areaTwo, tableID, numLine, minHeap);

		} else if (OPT == 2) {
			LFCO(restaurant, name, result, areaOne, areaTwo, tableID, numLine, minHeap);

		}

	} else {
		if (result % 2 == 1) {
			if (areaOne->hashCount >= MAXSIZE/2) {
				mountainZone(areaTwo, result, name, tableID, restaurant, numLine, minHeap);
			} else {
				beachZone(areaOne, result, name, tableID, restaurant, numLine, minHeap);
			}

		} else {
			if (areaTwo->count >= MAXSIZE/2) {
				beachZone(areaOne, result, name, tableID, restaurant, numLine, minHeap);

			} else {
				mountainZone(areaTwo, result, name, tableID, restaurant, numLine, minHeap);
			}
		}
	}
}

void CLE(int num, DLinkedList* &restaurant, HashTable* &areaOne, AVL* &areaTwo, vector<int> &tableID, vector<table*> &minHeap) {
	int deletetID = num;

	if (deletetID < 1) {

		//* Delete in minHeap[] & Restaurant
		bool toStop = false;
		while (toStop == false) {
			toStop = true;
			string guess = "";

			table* curr = restaurant->head;
			while (curr != nullptr) {
				if (curr->zone == 1) {
					toStop = false;
					guess = curr->name;
					break;
				}
				curr = curr->next;
			}

			if (toStop == true) {
				break;
			}

			for (unsigned int i = 0; i < minHeap.size(); i++) {
				if (minHeap[i]->name == guess) {
					swap(minHeap[i], minHeap[minHeap.size() - 1]);
					table* toDel = minHeap[minHeap.size() - 1];
					minHeap.pop_back();
					delete toDel;
					reheapUp(minHeap, i);
					reheapDown(minHeap, i);
					break;
				}
			}

			restaurant->deleteName(guess);
			
		}

		//* Delete in HashTable
		for (int i = 0; i < MAXSIZE/2; i++) {
			areaOne->hash[i]->clearTable();
		}
		areaOne->hashCount = 0;

		//* Delete in TableID[]
		for (unsigned int i = 0; i < tableID.size(); i++) {
			if (tableID[i] == -1) {
				tableID[i] = i + 1;
			}
		}		

		return;

	} else if (deletetID > MAXSIZE) {
		table* Temp = restaurant->head;
		vector<string> names;
		
		while (Temp != nullptr) {
			if (Temp->zone == 2) {
				names.push_back(Temp->name);
			}
			Temp = Temp->next;
		}

		//* Delete in minHeap[] & Restaurant
		bool toStop = false;
		while (toStop == false) {
			toStop = true;
			string guess = "";

			table* curr = restaurant->head;
			while (curr != nullptr) {
				if (curr->zone == 2) {
					toStop = false;
					guess = curr->name;
					break;
				}
				curr = curr->next;
			}

			if (toStop == true) {
				break;
			}

			for (unsigned int i = 0; i < minHeap.size(); i++) {
				if (minHeap[i]->name == guess) {
					swap(minHeap[i], minHeap[minHeap.size() - 1]);
					table* toDel = minHeap[minHeap.size() - 1];
					minHeap.pop_back();
					delete toDel;
					reheapUp(minHeap, i);
					reheapDown(minHeap, i);
					break;
				}
			}

			restaurant->deleteName(guess);
			
		}

		//* Delete in AVL
		areaTwo->deleteAVL();

		//* Delete in tableID[]
		for (unsigned int i = 0; i < tableID.size(); i++) {
			if (tableID[i] == -2) {
				tableID[i] = i + 1;
			}
		}

		// ! Leak Chance
		areaTwo->root = nullptr;
		areaTwo->count = 0;
		return;

	} else {

		//* Traverse ID
		bool found = false;
		table* curr = restaurant->head;
		for (int i = 0; i < restaurant->count; i++) {
			if (curr->ID == deletetID) {
				found = true;
				break;
			}
			curr = curr->next;
		}
		
		if (found == false) {
			// cout << "=> No Found" << endl;
			return;
		}

		int deleteZone = curr->zone;

		//* Delete in minHeap[]
		for (unsigned int i = 0; i < minHeap.size(); i++) {
			if (minHeap[i]->name == curr->name) {
				swap(minHeap[i], minHeap[minHeap.size() - 1]);
				table* toDel = minHeap[minHeap.size() - 1];
				minHeap.pop_back();
				delete toDel;
				reheapUp(minHeap, i);
				reheapDown(minHeap, i);
				break;
			}
		}

		//* Delete in HashTable
		if (deleteZone == 1) {
			for (int i = 0 ; i < MAXSIZE/2; i++) {
				if (areaOne->hash[i]->name == curr->name) {
					tableID[areaOne->hash[i]->ID - 1] = areaOne->hash[i]->ID;

					areaOne->hash[i]->clearTable();
					areaOne->hashCount--;
					break;
				}
			}
			restaurant->deleteName(curr->name);

		//* Delete in AVL
		} else if (deleteZone == 2) {
			tableID[curr->ID - 1] = curr->ID; 

			if (areaTwo->count == 1) {
				areaTwo->deleteNode(areaTwo->root, curr->result, curr->name);
				areaTwo->root = nullptr;
				areaTwo->count = 0;
			} else {
				areaTwo->deleteNode(areaTwo->root, curr->result, curr->name);				
			}

			restaurant->deleteName(curr->name);
		}
	}
	return;
}

void printHT(HashTable* areaOne) {
	// cout << endl << "------- PrintHT --------" << endl << endl;
	for (int i = 0; i < MAXSIZE/2; i++) {
		if (areaOne->hash[i]->name != "") {
			cout << areaOne->hash[i]->ID << "-" << areaOne->hash[i]->result << "-" << areaOne->hash[i]->dishes << endl;
		}
	}
	// cout << endl;
}

void printAVL(AVL* areaTwo) {
	if (areaTwo->count == 0) {
		return;
	}
	// cout << endl << "------- PrintAVL --------" << endl << endl;
	queue<Node*> q;
	q.push(areaTwo->root);
	
	while(!q.empty()) {
		int n = q.size();
		for (int i = 0; i < n; i++) {
			Node* curr = q.front();
			q.pop();

			cout << curr->ID << "-" << curr->key << "-" << curr->dishes << endl;

			if (curr->left != nullptr) {
				q.push(curr->left);
			}
			if (curr->right != nullptr) {
				q.push(curr->right);
			}
		}
	}
	// cout << endl;
}

	void printPreorder(vector<table*> &minHeap, int index) {
		if (index >= int(minHeap.size())) {
			return;
		}

		cout << minHeap[index]->ID << "-" << minHeap[index]->dishes << endl;

		printPreorder(minHeap, 2*index + 1);
		printPreorder(minHeap, 2*index + 2);
	}

void printMH(vector<table*> minHeap) {
	// cout << endl << "------- PrintMH --------" << endl << endl;
	
	printPreorder(minHeap, 0);
	// cout << endl;
}


void simulate(string filename) {
	ifstream myfile(filename);
	if (!myfile.is_open()) {
		cout << "ERROR: Could not open" << endl;
		return;
	}

	string command, name;
	int num;

	DLinkedList* restaurant = new DLinkedList();

	// minHeap
	vector<table*> minHeap;
	priority_queue<table*, vector<table*>, Compare> q;

	vector<int> tableID;

	for (int i = 1; i <= MAXSIZE; i++) {
		tableID.push_back(i);
	}

	//REG
	HashTable* areaOne = new HashTable();
	AVL* areaTwo = new AVL();
	
	string line;
	int numLine = 1;
	while (getline(myfile, line)) {
		name = "";
		command = "";
		int position1 = line.find(" ");
		if (position1 != int(string::npos)) {
			command = line.substr(0, position1);
			if (command == "REG") {
				int position2 = line.find(" ", position1 + 1);
				if (position2 == int(string::npos)) {
					name = line.substr(position1 + 1);

					bool flag = true;
					if (name.size() == 0) {
						flag = false;
					}

					for (char c : name) {
						if (!isalpha(c)) {
							flag = false;
							break;	
						}	
					}

					if (flag == true) {
						REG(name, areaOne, areaTwo, restaurant, numLine, tableID, minHeap);
					}
				}

			} else if (command == "CLE") {
				int position2= line.find(" ", position1 + 1);
				if (position2 == int(string::npos)) {

					bool flag = true;
					if (line.substr(position1 + 1).size() == 0) {
						flag = false;
					}
					
					if (line.substr(position1 + 1)[0] == '-') {
						if (line.substr(position1 + 2).size() == 0) {
						flag = false;
						} else {
							for (char c : line.substr(position1 + 2)) {
								if (!isdigit(c)) {
									flag = false;
									break;
								}	
							}
						}
					} else {
						for (char c : line.substr(position1 + 1)) {
							if (!isdigit(c)) {
								flag = false;
								break;
							}	
						}
					}

					if (flag == true) {
						num = stoi(line.substr(position1 + 1));
						CLE(num, restaurant, areaOne, areaTwo, tableID, minHeap);
					}
				}
			}
		} else {
			command = line;
			// cout << command << endl;
			if (command == "PrintHT") {
				printHT(areaOne);

			} else if (command == "PrintAVL") {
				
				printAVL(areaTwo);

			} else if (command == "PrintMH") {
				printMH(minHeap);
			}
		}
		
		// 
		// 	cout << "\n-------------------------\n";

		// 	restaurant->printTotal(name);
		// 	// printSequence(restaurant, tableID, name);
		// 	cout << endl;
		// 	// printStateMH(minHeap);
		// 	for (int i = 0; i < minHeap.size(); i++) {
		// 		cout << minHeap[i]->ID << "-" << minHeap[i]->dishes << " | ";
		// 	}
		// 	Sleep(3000);
		// 	system("cls");
		// }

		numLine++;
	}

	// cout << "\n-------------------------\n\n" << "Beach view = " << areaOne->hashCount << ":\n\n";
	// areaOne->printHash("");	
	// cout << "\n-------------------------\n\n" << "Mountain view = " << areaTwo->count << ":\n\n";
	
	// cout << "Result    ID    Dishes      Name\n\n";
	// areaTwo->inOrder(areaTwo->root, "");
	// // areaTwo->printState(areaTwo->root);
	// cout << "\n-------------------------\n";

	// // restaurant->printTotal();
	// printSequence(restaurant, tableID, "");
	// cout << endl;
	// printStateMH(minHeap);
	
	areaOne->deleteHash();
	delete areaOne;

	areaTwo->deleteAVL();
	delete areaTwo;

	restaurant->clear();
	delete restaurant;

	for (unsigned int i = 0; i < minHeap.size(); i++) {
		delete minHeap[i];
	}
	
	myfile.close();
	return;

}

