#include "Iris.h"
#include <fstream>

int main()
{
	ifstream file("C:\\Users\\User\\Documents\\MVS\\курсовая\\datasets\\iris.txt");
	int comma_count = 0;
	vector <line> data;
	vector <line> test;
	vector <line> validation;
	int pre = 0, post = 0;
	cout << "Use pre-pruning:" << endl;
	cout << "0 - no, 1 - yes" << endl;
	cin >> pre;
	if (pre > 1) pre = 1;
	cout << "Use post-pruning:" << endl;
	cout << "0 - no, 1 - yes" << endl;
	cin >> post;
	if (post > 1) post = 1;
	if (!file.is_open()) printf("Could not open the file\n");
	else {
		string line;
		bool done = false;
		while (getline(file, line)) {
			if (line != "") {
				if (!done) {
					comma_count = count_att(line);
					done = true;
				}
				read_data(data, line, comma_count);
			}
		}
		if (post == 0) {
			ifstream file1("C:\\Users\\User\\Documents\\MVS\\курсовая\\datasets\\iris_prune.txt");
			while (getline(file1, line)) {
				if (line != "") {
					if (!done) {
						comma_count = count_att(line);
						done = true;
					}
					read_data(data, line, comma_count);
				}
			}
		}
		cout << "DATA COUNT: " << data.size() << endl;
		node* root = new node(data, find_types(data));
		if (pre == 0) root->split_nopre();
		else root->split_pre();
		file.close();
		ifstream file("C:\\Users\\User\\Documents\\MVS\\курсовая\\datasets\\iris_test.txt");
		int correct = 0;
		int incorrect = 0;
		if (!file.is_open()) printf("Could not open the file\n");
		else {
			string line;
			bool done = false;
			double acc;
			while (getline(file, line)) {
				if (line != "") {
					if (!done) {
						comma_count = count_att(line);
						done = true;
					}
					read_data(test, line, comma_count);
				}
			}
			for (int i = 0; i < test.size(); ++i) {
				if (classify(test[i], root) == true) correct++;
				else incorrect++;
			}
			acc = (double)correct / (double)test.size();
			if (post == 0) {
				cout << "Accuracy:" << acc << endl;
				cout << "ROOT:" << endl;
				root->node_print();
			}
			else {
				correct = 0; incorrect = 0;
				ifstream file2("C:\\Users\\User\\Documents\\MVS\\курсовая\\datasets\\iris_prune.txt");
				if (!file2.is_open()) printf("Could not open the file\n");
				else {
					string line;
					bool done = false;
					while (getline(file2, line)) {
						if (line != "") {
							if (!done) {
								comma_count = count_att(line);
								done = true;
							}
							read_data(validation, line, comma_count);
						}
					}
				}
				while (prune(root, root, validation, acc) != 0) prune(root, root, validation, acc);
				for (int i = 0; i < test.size(); ++i) {
					if (classify(test[i], root) == true) correct++;
					else incorrect++;
				}
				cout << "Accuracy: " << acc << endl;
				cout << "ROOT:" << endl;
				root->node_print();
			}
		}
	}
}