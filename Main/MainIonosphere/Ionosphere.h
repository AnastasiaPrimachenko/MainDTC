#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

struct line {

	vector <double> attributes;
	string type;
	line(vector<double>& att, string& t) {
		this->attributes = att;
		this->type = t;
	}
	void print() {
		int i = 0;
		while (i < attributes.size()) {
			cout << attributes[i] << ", ";
			++i;
		}
		cout << type << endl;
	}
};

int count_att(string str) {
	int i = 0;
	int count = 0;
	if (str.size() == 0)
		throw - 1;
	while (i < str.size()) {
		if (str[i] == ',') { ++count; }
		++i;
	}
	return count;
}

void read_data(vector<line>& data, string& str, int& count_comma) {
	string temp_s;
	vector<double> temp_a;
	int i = 0;
	int j = 0;
	while (j < count_comma) {
		string temp;
		while (str[i] != ',') {
			temp.push_back(str[i]);
			++i;
		}
		if (str[i] == ',') {
			temp_a.push_back(stod(temp));
			++i;
		}
		++j;
	}
	temp_s = str.substr(i);
	data.push_back(line(temp_a, temp_s));
}

vector<string> find_types(vector<line> data) {
	if (data.size() == 0) {
		return vector<string>();
	}
	vector<string> unique;
	unique.push_back(data[0].type);
	int i = 0;
	bool type2 = false;
	while (i < data.size()) {
		if (data[0].type != data[i].type) {
			if (!type2) { unique.push_back(data[i].type); type2 = true; }
			if (data[i].type != unique[1]) {
				unique.push_back(data[i].type);
				break;
			}
		}
		++i;
	}
	return unique;
}

struct node {
	vector <line> data;
	vector <string> unique_types;
	string decision;
	double compare;
	int attribute;
	string label;
	node* parent;
	node* left;
	node* right;
	int level;
	node(vector<line> d, vector<string> u_t) {
		this->data = d;
		this->unique_types = u_t;
		this->decision = "Root";
		this->level = 0;
		left = nullptr;
		right = nullptr;
	}
	~node() {
		delete left;
		delete right;
	}

	vector<int> instances_of_each_type(vector<line> d) {
		int type1 = 0, type2 = 0, type3 = 0, i = 0;
		while (i < d.size()) {
			if (d[i].type == this->unique_types[0]) type1++;
			else if (d[i].type == this->unique_types[1]) type2++;
			++i;
		}
		return vector<int>{type1, type2};
	}

	double entropy(vector<line> data) {
		double e = 0, p = 0;
		vector<int> type_count = instances_of_each_type(data);
		for (int i = 0; i < type_count.size(); ++i) {
			if (type_count[i] != 0) {
				p = (double)type_count[i] / (double)data.size();
				e += -p * log2(p);
			}
		}
		return e;
	}

	vector <double> find_split_point(vector <line> data) {
		double gain = 0, cutoff = 0, attribute_cutoff = 0;
		vector <line> smaller;
		vector <line> bigger;
		for (int i = 0; i < data[0].attributes.size(); ++i) {
			for (int j = 0; j < data.size(); ++j) {
				double temp_cutoff = data[j].attributes[i];
				for (int k = 0; k < data.size(); ++k) {
					if (data[k].attributes[i] <= temp_cutoff) smaller.push_back(data[k]);
					else bigger.push_back(data[k]);
				}
				double e1 = entropy(data);
				double e2 = entropy(smaller);
				double e3 = entropy(bigger);

				double temp_gain = e1 - (((double)smaller.size() / (double)data.size()) * e2) - (((double)bigger.size() / (double)data.size()) * e3);
				if (temp_gain > gain) {
					cutoff = temp_cutoff;
					attribute_cutoff = i;
					gain = temp_gain;
				}
				smaller.clear();
				bigger.clear();
			}
		}
		return vector<double>{cutoff, attribute_cutoff};
	}
	void unite_leaves(node* leaf) {
		if (leaf->parent->left->label == leaf->parent->right->label) {
			leaf->parent->label = leaf->parent->left->label;
			leaf->parent->attribute = -1;
			for (int i = 0; i < leaf->parent->data.size(); ++i) {
				leaf->parent->data[i].type = leaf->label;
			}
			leaf->parent->left = nullptr;
			leaf->parent->right = nullptr;
			unite_leaves(leaf->parent);
		}
	}
	void split_pre() {
		if (data.size() > 20) {
			vector<int> type_count = instances_of_each_type(this->data);
			int f = 0;
			for (int i = 0; i < type_count.size(); ++i) if (type_count[i] != 0) f++;
			if (f <= 1) {
				attribute = -1;
				label = data[0].type;
				unite_leaves(this);
				return;
			}
			vector <double> split_point = find_split_point(this->data);
			vector <line> smaller;
			vector <line> bigger;
			for (int i = 0; i < data.size(); ++i) {
				double cutoff = split_point[0];
				if (data[i].attributes[split_point[1]] <= split_point[0]) smaller.push_back(data[i]);
				else  bigger.push_back(data[i]);
			}
			string s_temp = ("Attribute-" + to_string(int(split_point[1] + 1)) + " <= " + to_string(split_point[0]));
			attribute = int(split_point[1]);
			compare = split_point[0];
			this->decision = s_temp;
			left = new node(smaller, this->unique_types);
			left->level = this->level + 1;
			left->parent = this;
			right = new node(bigger, this->unique_types);
			right->parent = this;
			right->level = this->level + 1;
			left->split_pre();
			right->split_pre();
		}
		else {
			vector<int> type_count = instances_of_each_type(this->data);
			if (type_count[0] >= type_count[1]) {
				attribute = -1;
				label = unique_types[0];
				for (int i = 0; i < data.size(); ++i) {
					data[i].type = label;
				}
			}
			else {
				attribute = -1;
				label = unique_types[1];
				for (int i = 0; i < data.size(); ++i) {
					data[i].type = label;
				}
			}
			unite_leaves(this);
		}
	}

	void split_nopre() {
		vector<int> type_count = instances_of_each_type(this->data);
		int f = 0;
		for (int i = 0; i < type_count.size(); ++i) if (type_count[i] != 0) f++;
		if (f <= 1) {
			attribute = -1;
			label = data[0].type;
			return;
		}
		vector <double> split_point = find_split_point(this->data);
		vector <line> smaller;
		vector <line> bigger;
		for (int i = 0; i < data.size(); ++i) {
			double cutoff = split_point[0];
			if (data[i].attributes[split_point[1]] <= split_point[0]) smaller.push_back(data[i]);
			else  bigger.push_back(data[i]);
		}
		string s_temp = ("Attribute-" + to_string(int(split_point[1] + 1)) + " <= " + to_string(split_point[0]));
		attribute = int(split_point[1]);
		compare = split_point[0];
		this->decision = s_temp;
		left = new node(smaller, this->unique_types);
		left->level = this->level + 1;
		right = new node(bigger, this->unique_types);
		right->level = this->level + 1;
		left->parent = this;
		right->parent = this;
		left->split_nopre();
		right->split_nopre();
	}

	void node_print() {
		string space = "";
		for (int i = 0; i < this->level; ++i) {
			space += "  ";
		}
		if (left == nullptr && right == nullptr) {
			vector <int> type_count = instances_of_each_type(data);
			if (type_count[0] != 0) {
				cout << space << "Leaf: " << unique_types[0] << "(" << type_count[0] << ")" << endl;
			}
			if (type_count[1] != 0) {
				cout << space << "Leaf: " << unique_types[1] << "(" << type_count[1] << ")" << endl;
			}
			return;
		}
		cout << space << decision << endl;
		left->node_print();
		right->node_print();
	}
};

bool classify(line data, node* att) {
	bool correct = false;
	if (att->attribute != -1) {
		if (data.attributes[att->attribute] <= att->compare) correct = classify(data, att->left);
		else  correct = classify(data, att->right);
	}
	else {
		if (data.type == att->label) return true;
		else return false;
	}
	return correct;
}
int prune(node* node_p, node* root, vector <line> test, double accuracy) {
	int change = 0;
	double new_acc = 0;
	vector<line>data;
	int att;
	string l, a_l;
	double compare, d;
	int max = 0;
	vector<node*>c;
	if (node_p->decision == "Root" && node_p->parent->right->decision == "Root") {
		string leaf;
		att = node_p->parent->attribute;
		l = node_p->parent->label;
		d = node_p->parent->compare;
		vector <int> size;
		c.resize(2);
		for (int i = 0; i < 2; ++i) {
			if (i == 0) c[i] = node_p->parent->left;
			else c[i] = node_p->parent->right;
			if (c[i]->data.size() >= max) {
				max = c[i]->data.size();
				compare = c[i]->compare;
				leaf = c[i]->label;
			}
		}
		data = node_p->data;
		node_p->parent->attribute = -1;
		node_p->parent->label = leaf;
		node_p->parent->compare = compare;
		node_p->parent->left = nullptr;
		node_p->parent->right = nullptr;
		for (int i = 0; i < node_p->parent->data.size(); ++i) node_p->parent->data[i].type = leaf;
		int correct = 0, incorrect = 0;
		for (int i = 0; i < test.size(); ++i) {
			if (classify(test[i], root) == true) correct++;
			else incorrect++;
		}
		new_acc = (double)correct / (double)test.size();
		if (accuracy - new_acc >= 0.1) {
			node_p->parent->left = c[0];
			node_p->parent->right = c[1];
			node_p->parent->attribute = att;
			node_p->parent->label = l;
			node_p->parent->compare = d;
			node_p->parent->data = data;

		}
		else {
			accuracy = new_acc;
			change++;
			return change;
		}
	}
	else {
		if (node_p->left != nullptr) change += prune(node_p->left, root, test, accuracy);
		if (node_p->right != nullptr) change += prune(node_p->right, root, test, accuracy);
		return change;
	}
}