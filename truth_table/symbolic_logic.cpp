#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>

using namespace std;

enum state {both_symbol, left_symbol, right_symbol, neither_symbol, symbol};
//left_symbol means on the left of the command is a combination, which means the symbol is on the right; vice versa for the right_symbol
enum left_right_side {left_side, right_side, first_case};
//to see when "analyze" was called, we are going to the left or the right side of the main connection
enum answer {tautology, contradiction, contigency};

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

void print_first_line(int count){
    for(int i = 0; i < count; i++)
        printf("%c\t", 80+i);
    printf("\n");
}

bool condition(bool a, bool b) {
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

bool bicondition(bool a, bool b){
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
		if (str[i] == '-' || str[i] == '^' || str[i] == 'v' || str[i] == '~' || str[i] == '>')
			count++;
	}
	return count;
}

//implement the basic true and false for each symbol(fills out the left side of the table)
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
void implementDisjunct(vector<vector<bool>> &matrix, string str, int pos, int k, state status, int extra_else_left, int extra_else_right) {
	if (status == both_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][str[k - 1] - 80], matrix[i][str[k + 1] - 80]);
		}
	}
	else if(status == right_symbol){
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][str[k - 1] - 80], matrix[i][ pos+extra_else_right]);
		}
	}
	else if (status == left_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][str[k + 1] - 80], matrix[i][pos - extra_else_left]);
		}
	}
	else if (status == neither_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = disjunct(matrix[i][pos - extra_else_left], matrix[i][pos + extra_else_right]);
		}
	}
}

void implementConjunct(vector<vector<bool>> &matrix, string str, int pos, int k, state status, int extra_else_left, int extra_else_right) {
	if (status == both_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = conjunct(matrix[i][str[k - 1] - 80], matrix[i][str[k + 1] - 80]);
		}
	}
	else if (status == right_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = conjunct(matrix[i][str[k - 1] - 80], matrix[i][pos + extra_else_right]);
		}
	}
	else if (status == left_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = conjunct(matrix[i][str[k + 1] - 80], matrix[i][pos - extra_else_left]);
		}
	}
	else if (status == neither_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = conjunct(matrix[i][pos - extra_else_left], matrix[i][pos + extra_else_right]);
		}
	}
}

void implementCondition(vector<vector<bool>> &matrix, string str, int pos, int k, state status, int extra_else_left, int extra_else_right) {
	if (status == both_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = condition(matrix[i][str[k - 1] - 80], matrix[i][str[k + 1] - 80]);
		}
	}
	else if (status == right_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = condition(matrix[i][str[k - 1] - 80], matrix[i][pos + extra_else_right]);
		}
	}
	else if (status == left_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = condition(matrix[i][pos - extra_else_left], matrix[i][str[k + 1] - 80]);
		}
	}
	else if (status == neither_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = condition(matrix[i][pos - extra_else_left], matrix[i][pos + extra_else_right]);
		}
	}
}

void implementBicondition(vector<vector<bool>> &matrix, string str, int pos, int k, state status, int extra_else_left, int extra_else_right) {
	if (status == both_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = bicondition(matrix[i][str[k - 1] - 80], matrix[i][str[k + 1] - 80]);
		}
	}
	else if (status == right_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = bicondition(matrix[i][str[k - 1] - 80], matrix[i][pos + extra_else_right]);
		}
	}
	else if (status == left_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = bicondition(matrix[i][str[k + 1] - 80], matrix[i][pos - extra_else_left]);
		}
	}
	else if (status == neither_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = bicondition(matrix[i][pos - extra_else_left], matrix[i][pos + extra_else_right]);
		}
	}
}

void implementNegation(vector<vector<bool>> &matrix, string str, int pos, int k, state status, int extra_else_right) {
	if (status == right_symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = negate(matrix[i][pos + extra_else_right]);
		}
	}
	else if (status == symbol) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][pos] = negate(matrix[i][str[k + 1] - 80]);
		}
	}
}

//pos is the beginning position of the column(the position of last symbol)
//return value is how much need to be shifted on a outside function, either left or right
//main_pos is the location of the main connection in this string
int analyze(vector<vector<bool>> &matrix, string str, int pos, left_right_side side) {
	state status;
	left_right_side left_right;
	int extra = 0;
	int extra_else_left = 0;
	int extra_else_right = 0;
	int paren = 0;
	int main_pos = 0;
	for (int k = 0; k < str.length(); k++) {
		if (str[k] == '(') {//jump over every possible inner '( )'
			paren++;
			while (paren) {
				k++;
				if (str[k] == ')')
					paren--;
				else if (str[k] == '(')
					paren++;
				else if (str[k] == 'v' || str[k] == '^' || str[k] == '>' || str[k] == '-' || str[k] == '~')
					extra++;
			}
		}
		if (str[k] == 'v') {//if there is a command
			main_pos = k;
            extra++;
            if (is_symbol(str[k+1]) && is_symbol(str[k-1])){//is this command between two symbols
				status = both_symbol;
                implementDisjunct(matrix, str, pos+extra, k, status, 0, 0);
            }
            else {//if it's not between two symbols
				if (str[k + 1] == '(') {// is the one after this command a '('
					for (int i = k + 1; i < str.length(); i++) {
						paren = 0;
						if (str[i] == '(') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i++;
								if (str[i] == ')')
									paren--;
								else if (str[i] == '(')
									paren++;
							}
						}
						if (str[i] == ')') {
							extra_else_right += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra, right_side);
							break;
						}
					}
				}
				if (str[k - 1] == ')') {//or is it a ')'
					for (int i = k - 1; i >= 0; i--) {
						paren = 0;
						if (str[i] == ')') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i--;
								if (str[i] == '(')
									paren--;
								else if (str[i] == ')')
									paren++;
							}
						}
						if (str[i] == '(') {
							extra_else_left += analyze(matrix, str.substr(i+1, k - 2 - i), pos, left_side) ;
							break;
						}
					}
				}
				if (is_symbol(str[k - 1])) {//is the one before this command a symbol
					status = right_symbol;
					implementDisjunct(matrix, str, pos+extra, k, status, 0, extra_else_right);
				}
				else if (is_symbol(str[k + 1])) {//is the one after this command a symbo
					status = left_symbol;
					implementDisjunct(matrix, str, pos + extra, k, status, extra_else_left, 0);
				}
				else {
					status = neither_symbol;
					implementDisjunct(matrix, str, pos + extra, k, status, extra_else_left, extra_else_right);
				}
            }
		}
		else if (str[k] == '^') {//if there is a command
			main_pos = k;
			extra++;
			if (is_symbol(str[k + 1]) && is_symbol(str[k - 1])) {//is this command between two symbols
				status = both_symbol;
				implementConjunct(matrix, str, pos + extra, k, status, 0, 0);
			}
			else {//if it's not between two symbols
				if (str[k + 1] == '(') {// is the one after this command a '('
					for (int i = k + 1; i < str.length(); i++) {
						paren = 0;
						if (str[i] == '(') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i++;
								if (str[i] == ')')
									paren--;
								else if (str[i] == '(')
									paren++;
							}
						}
						if (str[i] == ')') {
							extra_else_right += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra, right_side);
							break;
						}
					}
				}
				if (str[k - 1] == ')') {//or is it a ')'
					for (int i = k - 1; i >= 0; i--) {
						paren = 0;
						if (str[i] == ')') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i--;
								if (str[i] == '(')
									paren--;
								else if (str[i] == ')')
									paren++;
							}
						}
						if (str[i] == '(') {
							extra_else_left += analyze(matrix, str.substr(i + 1, k - 2 - i), pos, left_side);
							break;
						}
					}
				}
				if (is_symbol(str[k - 1])) {//is the one before this command a symbol
					status = right_symbol;
					implementConjunct(matrix, str, pos + extra, k, status, 0, extra_else_right);
				}
				else if (is_symbol(str[k + 1])) {//is the one after this command a symbo
					status = left_symbol;
					implementConjunct(matrix, str, pos + extra, k, status, extra_else_left, 0);
				}
				else {
					status = neither_symbol;
					implementConjunct(matrix, str, pos + extra, k, status, extra_else_left, extra_else_right);
				}
			}
		}
		else if (str[k] == '>') {//if there is a command
			main_pos = k;
			extra++;
			if (is_symbol(str[k + 1]) && is_symbol(str[k - 1])) {//is this command between two symbols
				status = both_symbol;
				implementCondition(matrix, str, pos + extra, k, status, 0, 0);
			}
			else {//if it's not between two symbols
				if (str[k + 1] == '(') {// is the one after this command a '('
					for (int i = k + 1; i < str.length(); i++) {
						paren = 0;
						if (str[i] == '(') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i++;
								if (str[i] == ')')
									paren--;
								else if (str[i] == '(')
									paren++;
							}
						}
						if (str[i] == ')') {
							extra_else_right += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra, right_side);
							break;
						}
					}
				}
				if (str[k - 1] == ')') {//or is it a ')'
					for (int i = k - 1; i >= 0; i--) {
						paren = 0;
						if (str[i] == ')') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i--;
								if (str[i] == '(')
									paren--;
								else if (str[i] == ')')
									paren++;
							}
						}
						if (str[i] == '(') {
							extra_else_left += analyze(matrix, str.substr(i + 1, k - 2 - i), pos, left_side);
							break;
						}
					}
				}
				if (is_symbol(str[k - 1])) {//is the one before this command a symbol
					status = right_symbol;
					implementCondition(matrix, str, pos + extra, k, status, 0, extra_else_right);
				}
				else if (is_symbol(str[k + 1])) {//is the one after this command a symbo
					status = left_symbol;
					implementCondition(matrix, str, pos + extra, k, status, extra_else_left, 0);
				}
				else {
					status = neither_symbol;
					implementCondition(matrix, str, pos + extra, k, status, extra_else_left, extra_else_right);
				}
			}
		}
		else if (str[k] == '-') {//if there is a command
			main_pos = k;
			extra++;
			if (is_symbol(str[k + 1]) && is_symbol(str[k - 1])) {//is this command between two symbols
				status = both_symbol;
				implementBicondition(matrix, str, pos + extra, k, status, 0, 0);
			}
			else {//if it's not between two symbols
				if (str[k + 1] == '(') {// is the one after this command a '('
					for (int i = k + 1; i < str.length(); i++) {
						paren = 0;
						if (str[i] == '(') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i++;
								if (str[i] == ')')
									paren--;
								else if (str[i] == '(')
									paren++;
							}
						}
						if (str[i] == ')') {
							extra_else_right += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra, right_side);
							break;
						}
					}
				}
				if (str[k - 1] == ')') {//or is it a ')'
					for (int i = k - 1; i >= 0; i--) {
						paren = 0;
						if (str[i] == ')') {//jump over every possible inner '( )'
							paren++;
							while (paren) {
								i--;
								if (str[i] == '(')
									paren--;
								else if (str[i] == ')')
									paren++;
							}
						}
						if (str[i] == '(') {
							extra_else_left += analyze(matrix, str.substr(i + 1, k - 2 - i), pos, left_side);
							break;
						}
					}
				}
				if (is_symbol(str[k - 1])) {//is the one before this command a symbol
					status = right_symbol;
					implementBicondition(matrix, str, pos + extra, k, status, 0, extra_else_right);
				}
				else if (is_symbol(str[k + 1])) {//is the one after this command a symbo
					status = left_symbol;
					implementBicondition(matrix, str, pos + extra, k, status, extra_else_left, 0);
				}
				else {
					status = neither_symbol;
					implementBicondition(matrix, str, pos + extra, k, status, extra_else_left, extra_else_right);
				}
			}
		}
		else if (str[k] == '~') {//if there is a command
			main_pos = k;
			extra++;
			if (str[k + 1] == '(') {// is the one after this command a '('
				for (int i = k + 1; i < str.length(); i++) {
					paren = 0;
					if (str[i] == '(') {//jump over every possible inner '( )'
						paren++;
						while (paren) {
							i++;
							if (str[i] == ')')
								paren--;
							else if (str[i] == '(')
								paren++;
						}
					}
					if (str[i] == ')') {
						extra_else_right += analyze(matrix, str.substr(k + 2, i - k - 2), pos + extra, right_side);
						break;
					}
				}
			}

			if (is_symbol(str[k + 1])) {//is the one after this command a symbo
				status = symbol;
				implementNegation(matrix, str, pos + extra, k, status, 0);
			}
			else {
				status = right_symbol;
				implementNegation(matrix, str, pos + extra, k, status, extra_else_right);
			}
		}	
	}
	if (side == left_side)
		return extra - numberOfColumns(str.substr(0, main_pos));
	else if (side == right_side)
		return extra - numberOfColumns(str.substr(main_pos + 1, str.length() - main_pos));
	else if (side == first_case)
		return pos + numberOfColumns(str.substr(0, main_pos)) + 1;
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
	int n = analyze(matrix, str, count-1, first_case);
    print_first_line(count);
	print_matrix(matrix);
	answer ans = tautology;
	int count_ans = 0;
	for (int i = 0; i < matrix.size(); i++) {
		if (matrix[i][n] == false) {
			count_ans++;
			ans = contigency;
		}
	}
	cout << "The answer is the " << n+1 << "th column of the matrix." << endl;
	if (ans == tautology)
		cout << "It's a tautology.\n" << endl;
	else if (count_ans == matrix.size())
		cout << "It's a contradiction.\n" << endl;
	else
		cout << "It's a contigency.\n" << endl;
	return 0;
}
//~(((~P)^(~Q))-(~(PvQ))))