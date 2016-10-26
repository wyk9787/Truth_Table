#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>

using namespace std;

void printWelcome() {

}

void print_matrix(vector<vector<bool>> matrix) {
	for (int i = 0; i < matrix.size();i++) {
		for (int j = 0; j < matrix[0].size(); j++) {
			if (matrix[i][j])
				printf("T\t");
			else
				printf("F\t");
		}
		printf("\n");
	}
}

void print_first_line(int count, string str){
    for(int i = 0; i < count; i++)
        printf("%c\t", 80+i);
    printf("\n");
}

bool conditional(bool a, bool b) {
	if (a && !b)
		return false;
	else
		return true;
}

bool conjunct(bool a, bool b) {
	return a && b;
} 

bool disjunct(bool a, bool b) {
	return (a || b);
}

bool negate(bool a) {
	return !a;
}

bool biconditional(bool a, bool b){
	if (a == b)
		return true;
	else
		return false;
}

int number_of_symbols(int *a, string str) {
	for (int i = 0; i < str.length(); i++) {
		switch (str[i]) {
		case 'P':
			a[0]++;
			break;
		case 'Q':
			a[1]++;
			break;
		case 'R':
			a[2]++;
			break;
		case 'S':
			a[3]++;
			break;
		case 'T':
			a[4]++;
			break;
		}
	}
	int count = 0;
	for (int i = 0; i < 5; i++) {
		if (a[i])
			count++;
	}
	return count;
}

bool is_symbol(char a) {
	if (a >= 80 && a <= 84)
		return true;
	else
		return false;
}

int numberOfColumns(string str) {
	int count = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '-' || str[i] == '^' || str[i] == 'v' || str[i] == '~')
			count++;
		if (str[i] == '-' && str[i - 1] != '<')
			count--;
	}
	return count;
}

void implement(vector<vector<bool>> &matrix, int count) {
	int k = matrix.size() / 2;
	bool flag = false;
	for (int j = 0; j < count; j++) {
		for (int i = 0; i < matrix.size(); i++) {
			if (i % k == 0) {
				flag = !flag;
			}
			matrix[i][j] = flag;
		}
		k /= 2;
	}

}

//pos is the column to be filled in the matrix
//k-1 is the first letter in the string
//k+1 is the second letter in the string
//When there are two symbols comparing with each other, flag is true
//When there is not two symbols comparing with each other, flag is false 
void implementDisjunct(vector<vector<bool>> &matrix, string str, int pos, int k, bool flag, int extra_else) {
	if (flag) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][str[k - 1] - 80], matrix[i][str[k + 1] - 80]);
		}
	}
	else {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][str[k - 1] - 80], matrix[i][pos+extra_else]);
		}
	}
}

// pos is the beginning position of the column(the position of last symbol)
// Return value is the position of column in the matrix of the answer of this segment of the string
int analyze(vector<vector<bool>> &matrix, string str, int pos) {
	bool flag = true;
	int extra = 0;
	int extra_else = 0;
	for (int k = 0; k < str.length(); k++) {
		if (str[k] == 'v') {
            extra++;
            if (is_symbol(str[k+1]) && is_symbol(str[k-1])){
                implementDisjunct(matrix, str, pos+extra, k, flag, 0);
            }
            else {
				flag = !flag;
                if(str[k+1] == '('){
					for (int i = k + 1; i < str.length(); i++) {
						if (str[i] == ')') {
							extra_else += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra);
						}
					}
					//print_matrix(matrix);
					if (is_symbol(str[k - 1])) {
						implementDisjunct(matrix, str, pos+extra, k, flag, extra_else);
					}

                }
                
            }
		}
	}
	return extra;
}

int main() {
	string str;
	cin >> str;
	int a[5] = { 0 };
	int count = number_of_symbols(a, str);
	int rows = pow(2.0, count);
	int columns = numberOfColumns(str);
	vector <vector<bool>> matrix(rows);
	for (int i = 0; i < rows; i++) {
		matrix[i].resize(count + columns);
	}
	implement(matrix, count);
	int abc = analyze(matrix, str, count-1);
    print_first_line(count, str);
	print_matrix(matrix);

	return 0;
}
