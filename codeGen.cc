// Starter code for CS241 assignments 9-11
//
// C++ translation by Simon Parent (Winter 2011),
// based on Java code by Ondrej Lhotak,
// which was based on Scheme code by Gord Cormack.
// Modified July 3, 2012 by Gareth Davies
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
using namespace std;
int location = -12; // stack
int printLabel = 0;
int WhileCounter = 0;
int IfCounter = 0;
int newLabel = 0;
int deleteLabel = 0;
int compound = 0;
int back = 0;
int backtotest = 0;
int backtoptr = 0;
string var1;
string var2;
//stroe identifier table

// The set of terminal symbols in the WLPP grammar.
const char *terminals[] = {
  "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID",
  "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM",
  "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI",
  "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW",
  "DELETE", "NULL"
};
int isTerminal(const string &sym) {
  int idx;
  for(idx=0; idx<sizeof(terminals)/sizeof(char*); idx++)
    if(terminals[idx] == sym) return 1;
  return 0;
}

// Data structure for storing the parse tree.
class tree {
  public:
    string rule;
    string exp;
    int executable;
    string id;
    int value;
    int loc;
    vector<string> tokens;
    vector<tree*> children;
    ~tree() { for(int i=0; i<children.size(); i++) delete children[i]; }
};

// data stucture for symbol table
struct info {
	info(int a,int b,int n,int m,string c, string d): count(a),value(b),loc(n),executable(m),reg(c),type(d) {}
	info(){}
	int count;
	int value;
	int loc;
	int executable;
	string reg; // register
	string type; // int or int*
	
};






map<string,info> symbol;
map<int,string> reg_value;
vector<tree*> statements;
// Call this to display an error message and exit the program.


void WriteGlobalHeader(){
	cout << endl;
	cout << ";;code gen V1.0  by  JI ZHANG" << endl;
	cout << ";;2012/07/13" << endl;
	cout << endl;
	cout << ";;global constant" << endl;
	cout << "lis $4" << endl;
	cout << ".word 1" << endl;
	cout << "lis $5" << endl;
	cout << ".word 2" << endl;
	cout << "lis $6" << endl;
	cout << ".word 3" << endl;
	cout << "lis $7" << endl;
	cout << ".word 4" << endl;
	cout << "lis $8" << endl;
	cout << ".word 5" << endl;
	cout << "add $9,$1,$0" << endl;
	cout << "add $10,$2,$0" << endl;
	cout << "sw $1,-4($30)" << endl;
	cout << "sw $2,-8($30)" << endl;
	cout << endl;
}

void bail(const string &msg) {
  // You can also simply throw a string instead of using this function.
  throw string(msg);
}
ostream& operator<< (ostream& out,  info& myinfo){
	out << myinfo.type << "\t   " << myinfo.value << "\t        " << myinfo.count << "\t         " << myinfo.reg;
	out << "      " << myinfo.loc;
	return out;
}
ostream& operator<<(ostream& out, tree* t){
	out << "rule: " << t->rule << " ; executable: " << t->executable << " value: " << t->value << " id: " <<t->id;
	return out;
}
// Read and return wlppi parse tree.


void begin(tree* t);
void statement(tree* t);
void whileCode(tree* expr);
void printCode(tree* t);
void test(tree* t);
void printCodeWhile(tree* t);
void whileFoo(tree* expr);
void whileExpr(tree* expr);
void kernelTest(tree* t,int counter);
void doingIf(tree* t);
void ptr_mani(tree* expr);
void newCode();
void deleteCode();
tree *readParse(const string &lhs) {
  // Read a line from standard input.
  string line;
  getline(cin, line);
  if(cin.fail())
    bail("ERROR: Unexpected end of file.");
  tree *ret = new tree();
  // Tokenize the line.
  stringstream ss;
  ss << line;
  while(!ss.eof()) {
    string token;
    ss >> token;
    if(token == "") continue;
    ret->tokens.push_back(token);
  }
  ret->exp = "";
  ret->executable = 0;
  ret->value = 0;
  ret->id = "";
  ret->loc = 0;
  // Ensure that the rule is separated by single spaces.
  for(int idx=0; idx<ret->tokens.size(); idx++) {
    if(idx>0) ret->rule += " ";
    ret->rule += ret->tokens[idx];
  }
  // Recurse if lhs is a nonterminal.
  if(!isTerminal(lhs)) {
    for(int idx=1/*skip the lhs*/; idx<ret->tokens.size(); idx++) {
      ret->children.push_back(readParse(ret->tokens[idx]));
    }
  }
  return ret;
}

tree *parseTree;

tree* Scan(tree* t){
	if (t->rule == "procedure INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
		string p_1 = t->children[3]->children[1]->tokens[1];
		string p_2 = t->children[5]->children[1]->tokens[1];
		tree* t_1 = t->children[3]->children[0];
		tree* t_2 = t->children[5]->children[0];
		string type;
		if (t_1->tokens.size() == 3){
			type = "int*";
			info myinfo(1,-1,-4,0,"$9",type);
            var1 = p_1;
			symbol.insert(pair<string,info>(p_1,myinfo));
		}
	    else {
		    type = "int";
		    info myinfo(1,-1,-4,0,"$9",type);
            var1 = p_1;
		    symbol.insert(pair<string,info>(p_1,myinfo));
	    }
	    if (t_2->tokens.size() == 3){
		    type = "int*";
		    info myinfo(1,-2,-8,0,"$10",type);
            var2 = p_2;
		    symbol.insert(pair<string,info>(p_2,myinfo));
	    }
        else {
	        type = "int";
	        info myinfo(1,-2,-8,0,"$10",type);
            var2 = p_2;
	        symbol.insert(pair<string,info>(p_2,myinfo));
        }
	}
	if (t->rule == "dcls dcls dcl BECOMES NUM SEMI" || t->rule == "dcls dcls dcl BECOMES NULL SEMI"){
		string identifier = t->children[1]->children[1]->tokens[1];
		if (t->tokens[4] == "NUM"){
			stringstream ss(t->children[3]->tokens[1]);
			int value;
			ss >> value;
			int loc = location;
			location -=4;
			cout << "lis $3" << endl;
			cout << ".word " << value << endl;
			cout << "sw $3," << loc << "($30)" << endl;
			info myinfo(1,value,loc,1,"no","int");
			symbol.insert(pair<string,info>(identifier,myinfo));
		}
		else {
			int loc = location;
			location -= 4;
			cout << "sw $3," << loc << "($30)" << endl;
			info myinfo(1,-1,loc,1,"no","int*");
			symbol.insert(pair<string,info>(identifier,myinfo));
		}
	}
	if (t->rule == "lvalue ID"){
		string id = t->children[0]->tokens[1];
		t->id = id;
        symbol[id].count++;
	}
	
	
	

		 
	for (int i = 0 ; i < t->children.size(); ++i) {
		t->children[i] = Scan(t->children[i]);
	}
//=============================================================
//====================property inheritance=====================
	if (t->rule == "term factor" || t->rule == "expr term") {
		t->executable = t->children[0]->executable;
		t->value = t->children[0]->value;
		t->id = t->children[0]->id;
	}
//=================================================================
//====================factor=======================================
	if (t->rule == "factor NUM"){
		t->executable = 1;
		stringstream ss;
		int value;
		ss << t->children[0]->tokens[1];
		ss >> value;
		t->value = value;
	}
	
	else if (t->rule == "factor ID"){
		string id = t->children[0]->tokens[1];
		t->id = id;
		++symbol[id].count;
		int value = symbol[id].value;
		if (value == -1 || value == -2) t->executable = 0;
		else if (symbol[id].executable == 1) t->executable = 1;
		else if (symbol[id].executable == 0) t->executable = 0;
		//else cout << symbol[id].executable << endl;
	}
	
	else if (t->rule == "factor NULL"){
		
	}
    else if (t->rule == "factor STAR factor"){
        t->id = t->children[1]->id;
    }
    else if (t->rule == "factor NEW INT LBRACK expr RBRACK") newLabel = 1;
	else if (t->rule == "factor LPAREN expr RPAREN") {
		t->executable = t->children[1]->executable;
		t->value = t->children[1]->value;
		t->id = t->children[1]->id;
	}
//================================================================
//=============================expr===============================
	else if (t->rule == "expr expr PLUS term"){
		if (t->children[0]->id != "" || t->children[2]->id != ""){}
		else if (t->children[0]->executable && t->children[2]->executable){
			t->executable = 1;
			t->value = t->children[0]->value + t->children[2]->value;
		}
		else if (t->children[2]->executable){
			if (t->children[2]->value == 0){
				int size = t->children[0]->children.size();
				if (size == 1){
					t->id = t->children[0]->id;
					t->children.pop_back();
					t->children.pop_back();
					tree* tmp = t->children[0]->children[0];
					t->children.pop_back();
					t->children.push_back(tmp);
					t->rule = "expr term";
				}
				else if (size == 3){
					t->id = t->children[0]->id;
					t->rule = t->children[0]->rule;
					t->children.pop_back();
					t->children.pop_back();
					tree* tmp1 = t->children[0]->children[0];
					tree* tmp2 = t->children[0]->children[1];
					tree* tmp3 = t->children[0]->children[2];
					t->children.pop_back();
					t->children.push_back(tmp1);
					t->children.push_back(tmp2);
					t->children.push_back(tmp3);
				}
			}
		}
		else if (t->children[0]->executable){
			if (t->children[0]->value == 0){
					tree* tmp = t->children[2];
					t->children.pop_back();
					t->children.pop_back();
					t->children.pop_back();
					t->children.push_back(tmp);
					t->rule = "expr term";
					t->id = tmp->id;
			}
		}
	}
	else if (t->rule == "expr expr MINUS term"){
		if (t->children[0]->id != "" || t->children[2]->id != ""){}
		else if (t->children[0]->id != "" && (t->children[0]->id == t->children[2]->id)){
			t->executable = 1;
			t->value = 0;
		}
		else if (t->children[0]->executable && t->children[2]->executable){
			t->executable = 1;
			t->value = t->children[0]->value - t->children[2]->value;
		}
		else if (t->children[2]->executable){
			if (t->children[2]->value == 0){
				int size = t->children[0]->children.size();
				if (size == 1){
					t->id = t->children[0]->id;
					t->children.pop_back();
					t->children.pop_back();
					tree* tmp = t->children[0]->children[0];
					t->children.pop_back();
					t->children.push_back(tmp);
					t->rule = "expr term";
				}
				else if (size == 3){
					t->id = t->children[0]->id;
					t->rule = t->children[0]->rule;
					t->children.pop_back();
					t->children.pop_back();
					tree* tmp1 = t->children[0]->children[0];
					tree* tmp2 = t->children[0]->children[1];
					tree* tmp3 = t->children[0]->children[2];
					t->children.pop_back();
					t->children.push_back(tmp1);
					t->children.push_back(tmp2);
					t->children.push_back(tmp3);
				}
			}
		}
	}
//================================================================================
//==============================term==============================================
	else if (t->rule == "term term STAR factor"){
		if (t->children[0]->id != "" || t->children[2]->id != ""){}
		else if (t->children[0]->executable && t->children[2]->executable){
		    t->executable = 1;
		    t->value = t->children[0]->value * t->children[2]->value;   
		}
		else if (!t->children[0]->executable && t->children[2]->executable){
			if (t->children[2]->value == 0){
				t->executable = 1;
				t->value = 0;
			}
			
			else if (t->children[2]->value == 1){
				t->children.pop_back();
				t->children.pop_back();
					string rule = t->children[0]->rule;
					int size = t->children[0]->children.size();
					if (size == 3){
					    tree* tmp1 = t->children[0]->children[0];
					    tree* tmp2 = t->children[0]->children[1];
					    tree* tmp3 = t->children[0]->children[2];
					    t->children.pop_back();
					    t->children.push_back(tmp1);
					    t->children.push_back(tmp2);
					    t->children.push_back(tmp3);
					    t->rule = rule;
					}
					else if (size == 1){
						t->rule = "term factor";
						tree *tmp4 = t->children[0];
						t->children.pop_back();
						t->children.push_back(tmp4);
					}
			}
			
		}
		else if (t->children[0]->executable && !t->children[2]->executable){
			if (t->children[0]->value == 0){
				t->executable = 1;
				t->value = 0;
			}
			
			else if (t->children[0]->value == 1){
					t->rule = "term factor";
					tree* tmp1 = t->children[2];
					t->children.pop_back();
					t->children.pop_back();
					t->children.pop_back();
					t->children.push_back(tmp1);
				}
			
		}
	}

	else if (t->rule == "term term SLASH factor"){
		if (t->children[0]->id != "" || t->children[2]->id != ""){}
		else if (t->children[0]->executable && t->children[2]->executable){
		    t->executable = 1;
		    t->value = t->children[0]->value / t->children[2]->value;   
		}
		else if (!t->children[0]->executable && t->children[2]->executable){
			if (t->children[2]->value == 1){
				t->children.pop_back();
				t->children.pop_back();
					string rule = t->children[0]->rule;
					int size = t->children[0]->children.size();
					if (size == 3){
					    tree* tmp1 = t->children[0]->children[0];
					    tree* tmp2 = t->children[0]->children[1];
					    tree* tmp3 = t->children[0]->children[2];
					    t->children.pop_back();
					    t->children.push_back(tmp1);
					    t->children.push_back(tmp2);
					    t->children.push_back(tmp3);
					    t->rule = rule;
					}
					else if (size == 1){
						t->rule = "term factor";
						tree *tmp4 = t->children[0];
						t->children.pop_back();
						t->children.push_back(tmp4);
					}
			}
			
		}
		else if (t->children[0]->executable && !t->children[2]->executable){
			if (t->children[0]->value == 0){
				t->executable = 1;
				t->value = 0;
			}
		}
		else if (t->children[0]->id != "" && t->children[0]->id == t->children[2]->id){
			t->executable = 1;
			t->value = 1;
		}
	}
	else if (t->rule == "term term PCT factor"){
		if (t->children[0]->id != "" || t->children[2]->id != ""){}
		else if (t->children[0]->executable && t->children[2]->executable){
		    t->executable = 1;
		    t->value = t->children[0]->value % t->children[2]->value;   
		}
		else if (!t->children[0]->executable && t->children[2]->executable){
			 if (t->children[2]->value == 1){
				 t->executable = 1;
				 t->value = 0;
			 }
		}
		else if (t->children[0]->executable && !t->children[2]->executable){
			if (t->children[0]->value == 0){
					t->rule = "term factor";
					tree* tmp1 = t->children[2];
					t->children.pop_back();
					t->children.pop_back();
					t->children.pop_back();
					t->children.push_back(tmp1);
			}
		}
		else if (t->children[0]->id != "" && t->children[0]->id == t->children[2]->id){
			t->executable = 1;
			t->value = 0;
		}
	}
//============================================================================
//=============================statement======================================
   else if (t->rule == "statement lvalue BECOMES expr SEMI"){
		if (t->children[2]->executable){
	        symbol[t->children[0]->id].value = t->children[2]->value;
	        symbol[t->children[0]->id].executable = 1;
		}
		else symbol[t->children[0]->id].executable = 0;
   }
   
   else if (t->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
	   
   }
   else if (t->rule == "statement DELETE LBRACK RBRACK expr SEMI") deleteLabel = 1;
	    
   else if (t->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
		printLabel = 1;
   }
//============================================================================
//===============================lvalue=======================================
	else if (t->rule == "lvalue LPAREN lvalue RPAREN") {
		t->executable = t->children[1]->executable;
		t->value = t->children[1]->value;
		t->id = t->children[1]->id;
	}
	return t;
}


void printSymbol(){
	map<string,info>::iterator it;
	for (it = symbol.begin() ; it != symbol.end(); ++it) {
		cerr << it->first << "     " << it->second << endl;
	}
}

bool reg_search(int value){
	map<int,string>::iterator it;
	it = reg_value.find(value);
	if (it != reg_value.end()) return true;
	return false;
}
void arithmetic(tree* expr){
	if (expr->executable) return;
	if (expr->id != ""){
		if (symbol[expr->id].loc != 0){
			cout << "lw $21," << symbol[expr->id].loc << "($30)" << endl;
			cout << "add $20,$0,$21" << endl;
			return;
		}
		string reg = symbol[expr->id].reg;
		cout << "add $20,$0," << reg << endl;
		return;
	}
	if (expr->children.size() == 1) {
		if (expr->rule == "expr term") {
			arithmetic(expr->children[0]);
			return;
		}
		arithmetic(expr->children[0]->children[1]);
		return;
	}
	else {
		// get symbol
		if (expr->tokens[2] == "expr") {
			arithmetic(expr->children[1]);
			return;
		}
//==============================================================================
//===================================PLUS=======================================
		if (expr->rule == "expr expr PLUS term") {
			if (expr->children[0]->executable && expr->children[2]->executable){
				expr->executable = 1;
				int value1 = expr->children[0]->value;
				int value2 = expr->children[2]->value;
				if (expr->children[0]->id != "" && expr->children[2]->id == "")value1 = symbol[expr->children[0]->id].value;
				else if (expr->children[0]->id == "" && expr->children[2]->id != "")value2 = symbol[expr->children[2]->id].value;
				else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
					value1 = symbol[expr->children[0]->id].value;
					value2 = symbol[expr->children[2]->id].value;
				}
				expr->value = value1+value2;
				cout << "lis $20" << endl;
				cout << ".word " << expr->value << endl;
				return;
			}

			else if (expr->children[0]->executable && !(expr->children[2]->executable)){
				int value = expr->children[0]->value;
				if (expr->children[0]->id != ""){
					value = symbol[expr->children[0]->id].value;
				}
				if (expr->children[2]->id == ""){
					arithmetic(expr->children[2]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "add $20,$20," << reg << endl;
						return;
					}
				    cout << "lis $21" << endl;
				    cout << ".word " << value << endl;
				    cout << "add $20,$20,$21" << endl;
				    return;
				}
				else if (expr->children[2]->id != ""){
					string reg = symbol[expr->children[2]->id].reg;
					if (reg_search(value)){
						string reg1 = reg_value[value];
						if (symbol[expr->children[2]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "add $20," << reg1 << ",$21" << endl;
							return;
						}
						cout << "add $20," << reg1 << ","<< reg << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "add $20,$22,$21" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "add $20,$22," << reg << endl;
					}
				}
			}
			else if (expr->children[2]->executable && !(expr->children[0]->executable)){
				int value = expr->children[2]->value;
				if (expr->children[2]->id != ""){
					value = symbol[expr->children[2]->id].value;
				}
				if (expr->children[0]->id == ""){
					arithmetic(expr->children[0]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "add $20,$20," << reg << endl;
						return;
					}
				    cout << "lis $22" << endl;
				    cout << ".word " << value << endl;
				    cout << "add $20,$20,$22" << endl;
				    return;
				}
				else if (expr->children[0]->id != ""){
					string reg = symbol[expr->children[0]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "add $20," << reg1 << ",$21" << endl;
							return;
						}
						cout << "add $20," << reg1 << ","<< reg << endl;
						return;
					}
					else if (symbol[expr->children[0]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
					    cout << "add $20,$22,$21" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "add $20,$22," << reg << endl;
					}
				}
			}
			else if (!expr->children[0]->executable && !expr->children[2]->executable){
				    if (expr->children[0]->id != "" && expr->children[2]->id == ""){
						if (symbol[expr->children[0]->id].loc != 0){
							arithmetic(expr->children[2]);
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "add $20,$20,$21" << endl;
							return;
						}
					    arithmetic(expr->children[2]);
					    string reg = symbol[expr->children[0]->id].reg;
					    cout << "add $20,$20," << reg << endl;
					    return;
				    }
				    else if (expr->children[2]->id != "" && expr->children[0]->id == ""){
						if (symbol[expr->children[2]->id].loc != 0){
							arithmetic(expr->children[0]);
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "add $20,$20,$21" << endl;
							return;
						}
					    arithmetic(expr->children[0]);
					    string reg = symbol[expr->children[2]->id].reg;
					    cout << "add $20,$20," << reg << endl;
					    return;
				    }
			        else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
				        string reg1 = symbol[expr->children[0]->id].reg;
				        string reg2 = symbol[expr->children[2]->id].reg;
						if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "add $20,$21,$22" << endl;
							return;
						}
					else if (symbol[expr->children[2]->id].loc == 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "add $20,$21," << reg2 << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc == 0){
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "add $20," << reg1 << ",$22" << endl;
						return;
					}
				        cout << "add $20," << reg1 << "," << reg2 << endl;
				        return;
			        }
				else{
				    arithmetic(expr->children[0]);
				    int loc = location;
				    cout << "sw $20," << loc << "($30)" << endl;
				    arithmetic(expr->children[2]);
				    cout << "lw $23," << loc << "($30)" << endl;
				    cout << "add $20,$20,$23" << endl;
				    location -= 4;
				    return;
				}
			}
		}
//===================================================================================
//==================================MINUS============================================
		else if (expr->rule == "expr expr MINUS term") {
			if (expr->children[0]->executable && expr->children[2]->executable){
				expr->executable = 1;
				int value1 = expr->children[0]->value;
				int value2 = expr->children[2]->value;
				if (expr->children[0]->id != "" && expr->children[2]->id == "")value1 = symbol[expr->children[0]->id].value;
				else if (expr->children[0]->id == "" && expr->children[2]->id != "")value2 = symbol[expr->children[2]->id].value;
				else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
					value1 = symbol[expr->children[0]->id].value;
					value2 = symbol[expr->children[2]->id].value;
				}
				expr->value = value1-value2;
				cout << "lis $20" << endl;
				cout << ".word " << expr->value << endl;
				return;
			}
			else if (expr->children[0]->executable && !(expr->children[2]->executable)){
				int value = expr->children[0]->value;
				if (expr->children[0]->id != ""){
					value = symbol[expr->children[0]->id].value;
				}
				if (expr->children[2]->id == ""){
					arithmetic(expr->children[2]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "sub $20," << reg <<",$20" << endl;
						return;
					}
				    cout << "lis $21" << endl;
				    cout << ".word " << value << endl;
				    cout << "sub $20,$21,$20" << endl;
				    return;
				}
				else if (expr->children[2]->id != ""){
					string reg = symbol[expr->children[2]->id].reg;
					if (reg_search(value)){
					
						string reg1 = reg_value[value];
						if (symbol[expr->children[2]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "sub $20," << reg1 << ",$21" << endl;
							return;
						}
						cout << "sub $20," << reg1 << ","<< reg << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "sub $20,$22,$21" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "sub $20,$22," << reg << endl;
					}
				}
			}
			else if (expr->children[2]->executable && !(expr->children[0]->executable)){
				int value = expr->children[2]->value;
				if (expr->children[2]->id != ""){
					value = symbol[expr->children[2]->id].value;
				}
				if (expr->children[0]->id == ""){
					arithmetic(expr->children[0]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "sub $20,$20," << reg << endl;
						return;
					}
				    cout << "lis $22" << endl;
				    cout << ".word " << value << endl;
				    cout << "sub $20,$20,$22" << endl;
				    return;
				}
				else if (expr->children[0]->id != ""){
					string reg = symbol[expr->children[0]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "sub $20,$21," << reg1 << endl;
							return;
						}
						cout << "sub $20," << reg << ","<< reg1 << endl;
						return;
					}
					else if (symbol[expr->children[0]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "sub $20,$21,$22" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "sub $20," << reg << ",$22" << endl;
					}
				}
			}
			else if (!expr->children[0]->executable && !expr->children[2]->executable){
				if (expr->children[0]->id != "" && expr->children[2]->id == ""){
				    if (symbol[expr->children[0]->id].loc != 0){
						arithmetic(expr->children[2]);
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "sub $20,$21,$20" << endl;
						return;
					}
					arithmetic(expr->children[2]);
					string reg = symbol[expr->children[0]->id].reg;
					cout << "sub $20,"<< reg << ",$20" << endl;
					return;
				}
				else if (expr->children[2]->id != "" && expr->children[0]->id == ""){
				    if (symbol[expr->children[2]->id].loc != 0){
						arithmetic(expr->children[0]);
						cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "sub $20,$20,$21" << endl;
						return;
					}
					arithmetic(expr->children[0]);
					string reg = symbol[expr->children[2]->id].reg;
					cout << "sub $20,$20," << reg << endl;
					return;
				}
			    else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
				    string reg1 = symbol[expr->children[0]->id].reg;
				    string reg2 = symbol[expr->children[2]->id].reg;
					if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "sub $20,$21,$22" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc == 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "sub $20,$21," << reg2 << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc == 0){
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "sub $20," << reg1 << ",$22" << endl;
						return;
					}
				    cout << "sub $20," << reg1 << "," << reg2 << endl;
				    return;
			    }
				else{
				    arithmetic(expr->children[0]);
				    int loc = location;
				    cout << "sw $20," << loc << "($30)" << endl;
				    arithmetic(expr->children[2]);
				    cout << "lw $23," << loc << "($30)" << endl;
				    cout << "sub $20,$23,$20" << endl;
				    location -= 4;
				    return;
				}
			}
		}
//========================================================================================
//=======================================STAR=============================================
		else if (expr->rule == "term term STAR factor") {
			if (expr->children[0]->executable && expr->children[2]->executable){
				expr->executable = 1;
				int value1 = expr->children[0]->value;
				int value2 = expr->children[2]->value;
				if (expr->children[0]->id != "" && expr->children[2]->id == "")value1 = symbol[expr->children[0]->id].value;
				else if (expr->children[0]->id == "" && expr->children[2]->id != "")value2 = symbol[expr->children[2]->id].value;
				else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
					value1 = symbol[expr->children[0]->id].value;
					value2 = symbol[expr->children[2]->id].value;
				}
				expr->value = value1*value2;
				cout << "lis $20" << endl;
				cout << ".word " << expr->value << endl;
				return;
			}
			else if (expr->children[0]->executable && !(expr->children[2]->executable)){
				int value = expr->children[0]->value;
				if (expr->children[0]->id != ""){
					value = symbol[expr->children[0]->id].value;
				}
				if (expr->children[2]->id == ""){
					arithmetic(expr->children[2]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "mult $20,"<< reg  << endl;
						cout << "mflo $20" << endl;
						return;
					}
				    cout << "lis $21" << endl;
				    cout << ".word " << value << endl;
				    cout << "mult $21,$20" << endl;
				    cout << "mflo $20" << endl;
				    return;
				}
				else if (expr->children[2]->id != ""){
					string reg = symbol[expr->children[2]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[2]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "mult " << reg1 << ",$21" << endl;
							cout << "mflo $20" << endl;
							return;
						}
						cout << "mult " << reg1 << ","<< reg << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "mult $22,$21" << endl;
					    cout << "mflo $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "mult $22," << reg << endl;
					    cout << "mflo $20" << endl;
					}
				}
			}
			else if (expr->children[2]->executable && !(expr->children[0]->executable)){
				int value = expr->children[2]->value;
				if (expr->children[2]->id != ""){
					value = symbol[expr->children[2]->id].value;
				}
				if (expr->children[0]->id == ""){
					arithmetic(expr->children[0]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "mult $20,"<< reg  << endl;
						cout << "mflo $20" << endl;
						return;
					}
				    cout << "lis $22" << endl;
				    cout << ".word " << value << endl;
				    cout << "mult $20,$22" << endl;
				    cout << "mflo $20" << endl;
				    return;
				}
				else if (expr->children[0]->id != ""){
					string reg = symbol[expr->children[0]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "mult " << reg1 << ",$21" << endl;
							cout << "mflo $20" << endl;
							return;
						}
						cout << "mult " << reg1 << ","<< reg << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[0]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
					    cout << "mult $22,$21" << endl;
					    cout << "mflo $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "mult $22," << reg << endl;
					    cout << "mflo $20" << endl;
					}
				}
			}
			else if (!expr->children[0]->executable && !expr->children[2]->executable){
				if (expr->children[0]->id != "" && expr->children[2]->id == ""){
				    if (symbol[expr->children[0]->id].loc != 0){
						arithmetic(expr->children[2]);
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "mult $20,$21" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					arithmetic(expr->children[2]);
					string reg = symbol[expr->children[0]->id].reg;
					cout << "mult $20," << reg << endl;
					cout << "mflo $20" << endl;
					return;
				}
				else if (expr->children[2]->id != "" && expr->children[0]->id == ""){
				    if (symbol[expr->children[2]->id].loc != 0){
						arithmetic(expr->children[0]);
						cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "mult $20,$21" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					arithmetic(expr->children[0]);
					string reg = symbol[expr->children[2]->id].reg;
					cout << "mult $20," << reg << endl;
					cout << "mflo $20" << endl;
					return;
				}
			    else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
				    string reg1 = symbol[expr->children[0]->id].reg;
				    string reg2 = symbol[expr->children[2]->id].reg;
					if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "mult $21,$22" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc == 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "mult $21," << reg2 << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc == 0){
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "mult " << reg1 << ",$22" << endl;
						cout << "mflo $20" << endl;
						return;
					}
				    cout << "mult " << reg1 << "," << reg2 << endl;
				    cout << "mflo $20" << endl;
				    return;
			    }
				else{
				    arithmetic(expr->children[0]);
				    int loc = location;
				    cout << "sw $20," << loc << "($30)" << endl;
				    arithmetic(expr->children[2]);
				    cout << "lw $23," << loc << "($30)" << endl;
				    cout << "mult $20,$23" << endl;
				    cout << "mflo $20" << endl;
				    location -= 4;
				    return;
				}
			}
		}
//=========================================================================
//===========================SLASH=========================================
		else if (expr->rule == "term term SLASH factor") {
			if (expr->children[0]->executable && expr->children[2]->executable){
				expr->executable = 1;
				int value1 = expr->children[0]->value;
				int value2 = expr->children[2]->value;
				if (expr->children[0]->id != "" && expr->children[2]->id == "")value1 = symbol[expr->children[0]->id].value;
				else if (expr->children[0]->id == "" && expr->children[2]->id != "")value2 = symbol[expr->children[2]->id].value;
				else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
					value1 = symbol[expr->children[0]->id].value;
					value2 = symbol[expr->children[2]->id].value;
				}
				expr->value = value1/value2;
				cout << "lis $20" << endl;
				cout << ".word " << expr->value << endl;
				return;
			}
			else if (expr->children[0]->executable && !(expr->children[2]->executable)){
				int value = expr->children[0]->value;
				if (expr->children[0]->id != ""){
					value = symbol[expr->children[0]->id].value;
				}
				if (expr->children[2]->id == ""){
					arithmetic(expr->children[2]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "div " << reg << ",$20" << endl;
						cout << "mflo $20" << endl;
						return;
					}
				    cout << "lis $21" << endl;
				    cout << ".word " << value << endl;
				    cout << "div $21,$20" << endl;
				    cout << "mflo $20" << endl;
				    return;
				}
				else if (expr->children[2]->id != ""){
					string reg = symbol[expr->children[2]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[2]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "div " << reg1 << ",$21" << endl;
							cout << "mflo $20" << endl;
							return;
						}
						cout << "div " << reg1 << ","<< reg << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "div $22,$21" << endl;
					    cout << "mflo $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "div $22," << reg << endl;
					    cout << "mflo $20" << endl;
					}
				}
			}
			else if (expr->children[2]->executable && !(expr->children[0]->executable)){
				int value = expr->children[2]->value;
				if (expr->children[2]->id != ""){
					value = symbol[expr->children[2]->id].value;
				}
				if (expr->children[0]->id == ""){
					arithmetic(expr->children[0]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "div $20," << reg << endl;
						cout << "mflo $20" << endl;
						return;
					}
				    cout << "lis $22" << endl;
				    cout << ".word " << value << endl;
				    cout << "div $20,$22" << endl;
				    cout << "mflo $20" << endl;
				    return;
				}
				else if (expr->children[0]->id != ""){
					string reg = symbol[expr->children[0]->id].reg;
					if (reg_search(value)){
						string reg1 = reg_value[value];
						if (symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "div $21," << reg1 << endl;
							cout << "mflo $20" << endl;
							return;
						}
						cout << "div " << reg << ","<< reg1 << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[0]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
					    cout << "div $21,$22" << endl;
					    cout << "mflo $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "div "<< reg << ",$22" << endl;
					    cout << "mflo $20" << endl;
					}
				}
			}
			else if (!expr->children[0]->executable && !expr->children[2]->executable){
				if (expr->children[0]->id != "" && expr->children[2]->id == ""){
				    if (symbol[expr->children[0]->id].loc != 0){
						arithmetic(expr->children[2]);
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "div $21,$20" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					arithmetic(expr->children[2]);
					string reg = symbol[expr->children[0]->id].reg;
					cout << "div " << reg << ",$20" << endl;
					cout << "mflo $20" << endl;
					return;
				}
				else if (expr->children[2]->id != "" && expr->children[0]->id == ""){
				    if (symbol[expr->children[2]->id].loc != 0){
						arithmetic(expr->children[0]);
						cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div $20,$21" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					arithmetic(expr->children[0]);
					string reg = symbol[expr->children[2]->id].reg;
					cout << "div $20," << reg << endl;
					cout << "mflo $20" << endl;
					return;
				}
			    else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
				    string reg1 = symbol[expr->children[0]->id].reg;
				    string reg2 = symbol[expr->children[2]->id].reg;
					if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div $21,$22" << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc == 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "div $21," << reg2 << endl;
						cout << "mflo $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc == 0){
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div " << reg1 << ",$22" << endl;
						cout << "mflo $20" << endl;
						return;
					}

				    cout << "div " << reg1 << "," << reg2 << endl;
				    cout << "mflo $20" << endl;
				    return;
			    }
				else{
				    arithmetic(expr->children[0]);
				    int loc = location;
				    cout << "sw $20," << loc << "($30)" << endl;
				    arithmetic(expr->children[2]);
				    cout << "lw $23," << loc << "($30)" << endl;
				    cout << "div $23,$20" << endl;
				    cout << "mflo $20" << endl;
				    location -= 4;
				    return;
				}
			}
		}
//===============================================================================
//============================PCT================================================
		else if (expr->rule == "term term PCT factor") {
			if (expr->children[0]->executable && expr->children[2]->executable){
				expr->executable = 1;
				int value1 = expr->children[0]->value;
				int value2 = expr->children[2]->value;
				if (expr->children[0]->id != "" && expr->children[2]->id == "")value1 = symbol[expr->children[0]->id].value;
				else if (expr->children[0]->id == "" && expr->children[2]->id != "")value2 = symbol[expr->children[2]->id].value;
				else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
					value1 = symbol[expr->children[0]->id].value;
					value2 = symbol[expr->children[2]->id].value;
				}
				expr->value = value1%value2;
				cout << "lis $20" << endl;
				cout << ".word " << expr->value << endl;
				return;
			}
			else if (expr->children[0]->executable && !(expr->children[2]->executable)){
				int value = expr->children[0]->value;
				if (expr->children[0]->id != ""){
					value = symbol[expr->children[0]->id].value;
				}
				if (expr->children[2]->id == ""){
					arithmetic(expr->children[2]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "div " << reg << ",$20" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
				    cout << "lis $21" << endl;
				    cout << ".word " << value << endl;
				    cout << "div $21,$20" << endl;
				    cout << "mfhi $20" << endl;
				    return;
				}
				else if (expr->children[2]->id != ""){
					string reg = symbol[expr->children[2]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[2]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
							cout << "div " << reg1 << ",$21" << endl;
							cout << "mfhi $20" << endl;
							return;
						}
						cout << "div " << reg << ","<< reg1 << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
					    cout << "div $22,$21" << endl;
					    cout << "mfhi $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "div $22," << reg << endl;
					    cout << "mfhi $20" << endl;
					}
				}
			}
			else if (expr->children[2]->executable && !(expr->children[0]->executable)){
				int value = expr->children[2]->value;
				if (expr->children[2]->id != ""){
					value = symbol[expr->children[2]->id].value;
				}
				if (expr->children[0]->id == ""){
					arithmetic(expr->children[0]);
					if (reg_search(value)){
						string reg = reg_value[value];
						cout << "div " << reg << ",$20" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
				    cout << "lis $22" << endl;
				    cout << ".word " << value << endl;
				    cout << "div $20,$22" << endl;
				    cout << "mfhi $20" << endl;
				    return;
				}
				else if (expr->children[0]->id != ""){
					string reg = symbol[expr->children[0]->id].reg;
					if (reg_search(value)){
						
						string reg1 = reg_value[value];
						if (symbol[expr->children[0]->id].loc != 0){
							cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
							cout << "div $21," << reg1 << endl;
							cout << "mfhi $20" << endl;
							return;
						}
						cout << "div " << reg << ","<< reg1 << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					else if (symbol[expr->children[0]->id].loc != 0){
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
					    cout << "div $21,$22" << endl;
					    cout << "mfhi $20" << endl;
					    return;
					}
					else {
					    cout << "lis $22" << endl;
					    cout << ".word " << value << endl;
					    cout << "div " << reg << ",$22" << endl;
					    cout << "mfhi $20" << endl;
					}
				}
			}
			else if (!expr->children[0]->executable && !expr->children[2]->executable){
				if (expr->children[0]->id != "" && expr->children[2]->id == ""){
				    if (symbol[expr->children[0]->id].loc != 0){
						arithmetic(expr->children[2]);
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "div $21,$20" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					arithmetic(expr->children[2]);
					string reg = symbol[expr->children[0]->id].reg;
					cout << "div "<< reg << ",$20" << endl;
					cout << "mfhi $20" << endl;
					return;
				}
				else if (expr->children[2]->id != "" && expr->children[0]->id == ""){
				    if (symbol[expr->children[2]->id].loc != 0){
						arithmetic(expr->children[0]);
						cout << "lw $21," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div $20,$21" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					arithmetic(expr->children[0]);
					string reg = symbol[expr->children[2]->id].reg;
					cout << "div $20," << reg << endl;
					cout << "mfhi $20" << endl;
					return;
				}
			    else if (expr->children[0]->id != "" && expr->children[2]->id != ""){
				    string reg1 = symbol[expr->children[0]->id].reg;
				    string reg2 = symbol[expr->children[2]->id].reg;
					if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div $21,$22" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc == 0 && symbol[expr->children[0]->id].loc != 0){
						cout << "lw $21," << symbol[expr->children[0]->id].loc << "($30)" << endl;
						cout << "div $21," << reg2 << endl;
						cout << "mfhi $20" << endl;
						return;
					}
					else if (symbol[expr->children[2]->id].loc != 0 && symbol[expr->children[0]->id].loc == 0){
						cout << "lw $22," << symbol[expr->children[2]->id].loc << "($30)" << endl;
						cout << "div " << reg1 << ",$22" << endl;
						cout << "mfhi $20" << endl;
						return;
					}
				    cout << "div " << reg1 << "," << reg2 << endl;
				    cout << "mfhi $20" << endl;
				    return;
			    }
				else{
				    arithmetic(expr->children[0]);
				    int loc = location;
				    cout << "sw $20," << loc << "($30)" << endl;
				    arithmetic(expr->children[2]);
				    cout << "lw $23," << loc << "($30)" << endl;
				    cout << "div $23,$20" << endl;
				    cout << "mfhi $20" << endl;
				    location -= 4;
				    return;
				}
			}
		}
	}
}
void printCodeWhile(tree* expr){
	int loc = location;
	cout << "add $1,$20,$0" << endl;
	cout << "sw $31," << loc << "($30)" << endl;
    location -= 4;
	cout << "lis $29" << endl;
	cout << ".word " << location << endl;
	cout << "add $30,$30,$29" << endl;
	cout << "lis $3" << endl;
	cout << ".word print" << endl;
	cout << "jalr $3" << endl;
	cout << "sub $30,$30,$29" << endl;
	cout << "lw $31," << loc << "($30)" << endl;
}
void kernelTestIf(tree* t,int counter){
		 if (t->children[2]->tokens[2] == "EQ"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "bne $26,$0,else" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "NE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "beq $26,$0,else" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "LT"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$26,$0" << endl;
			 cout << "beq $27,$0,else" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "LE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$0,$26" << endl;
			 cout << "beq $27,$4,else" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "GE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$26,$0" << endl;
			 cout << "beq $27,$4,else" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "GT"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$0,$26" << endl;
			 cout << "beq $27,$0,else" << counter << endl;
		 }
}
void doingIf(tree* t){
    string id0 = t->children[2]->children[0]->id;
    string id2 = t->children[2]->children[2]->id;
	int counter = IfCounter;
	int loc1 = location;
	location -= 4;
	cout << "if" << counter <<":"<< endl;
    if (symbol[id0].type == "int" || symbol[id2].type == "int"){
	test(t->children[2]->children[0]);
    cout << "sw $20," << loc1 << "($30)" << endl;
	test(t->children[2]->children[2]);
    }
    else{
        ptr_mani(t->children[2]->children[0]);
        cout << "sw $20," << loc1 << "($30)" << endl;
        ptr_mani(t->children[2]->children[2]);
    }
	//cout << t->children[2]->children[2]<<endl;
	cout << "lw $24," << loc1 << "($30)" << endl;
	cout << "add $25,$0,$20" << endl;
	kernelTestIf(t,counter);
	IfCounter += 1;
	whileExpr(t->children[5]);
	cout << "beq $0,$0,endif" << counter << endl;
	cout << "else" << counter << ":" << endl;
	whileExpr(t->children[9]);
	cout << "endif" << counter << ":" << endl;
}
void kernelTest(tree* t,int counter){
		 if (t->children[2]->tokens[2] == "EQ"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "bne $26,$0,endloop" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "NE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "beq $26,$0,endloop" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "LT"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$26,$0" << endl;
			 cout << "beq $27,$0,endloop" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "LE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$0,$26" << endl;
			 cout << "beq $27,$4,endloop" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "GE"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$26,$0" << endl;
			 cout << "beq $27,$4,endloop" << counter << endl;
		 }
		 else if (t->children[2]->tokens[2] == "GT"){
			 cout << "sub $26,$24,$25" << endl;
			 cout << "slt $27,$0,$26" << endl;
			 cout << "beq $27,$0,endloop" << counter << endl;
		 }
}
void whileFoo(tree* t){
         string id0 = t->children[2]->children[0]->id;
         string id2 = t->children[2]->children[2]->id;
		 int counter = WhileCounter;
		 int loc1 = location;
		 location -= 4;
		 cout << "loop" << counter << ":" << endl;
    if (symbol[id0].type == "int" || symbol[id2].type == "int"){
		 test(t->children[2]->children[0]);
		 cout << "sw $20," << loc1 << "($30)" << endl;
		 test(t->children[2]->children[2]);
    }else {
        ptr_mani(t->children[2]->children[0]);
        cout << "sw $20," << loc1 << "($30)" << endl;
        ptr_mani(t->children[2]->children[2]);
    }
		 //cout << t->children[2]->children[2]<<endl;
		 cout << "lw $24," << loc1 << "($30)" << endl;
		 cout << "add $25,$0,$20" << endl;
		 // $24 lvalue; $25 rvalue
		 kernelTest(t,counter);
		 WhileCounter += 1;
		 whileExpr(t->children[5]);
		 cout << "beq $0,$0,loop" << counter << endl;
		 cout << "endloop" << counter << ":" << endl;
}
void whileExpr(tree* expr){
	if (expr->rule == "statements statements statement"){
		whileExpr(expr->children[0]);
		whileCode(expr->children[1]);
	}
}
void whileCode(tree* expr){
	if (expr->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
		test(expr->children[2]);
		printCodeWhile(expr);
	}
	else if (expr->rule == "statement lvalue BECOMES expr SEMI"){
		if (expr->children[0]->rule == "lvalue ID" || expr->children[0]->rule == "lvalue LPAREN lvalue RPAREN"){
            string id = expr->children[0]->id;
            int loc = symbol[id].loc;
            if (symbol[id].type == "int*"){
                ptr_mani(expr->children[2]);
                //symbol[id].loc = 
                cout << "sw $20," << loc << "($30)" << endl;
                return;
            }
		    test(expr->children[2]);
		    
		    cout << "sw $20," << loc << "($30)" << endl;
		}
		else if (expr->children[0]->rule == "lvalue STAR factor"){
			test(expr->children[2]);
			int loc = location;
			location -= 4;
			cout << "sw $20," << loc << "($30)" << endl;
			ptr_mani(expr->children[0]->children[1]);
			cout << "lw $21," << loc << "($30)" << endl;
			cout << "sw $21,0($20)" << endl;
		}
			
	}
	else if(expr->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
		whileFoo(expr);
	}
	else if (expr->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
		doingIf(expr);
	}
    else if (expr->rule == "statement DELETE LBRACK RBRACK expr SEMI"){
        ptr_mani(expr->children[3]);
        deleteCode();
    }
}
void newCode(){
    int loc = location;
    location -= 4;
    cout << "sw $31," << loc << "($30)" << endl;
    cout << "lis $29" << endl;
    cout << ".word " << location << endl;
    cout << "add $30,$30,$29" << endl;    
    
    cout << "add $1,$0,$20" << endl; // # elements
    cout << "lis $2" << endl;
    cout << ".word new" << endl;
    cout << "jalr $2" << endl;
    cout << "sub $30,$30,$29" << endl;
    cout << "lw $31," << loc << "($30)" << endl;
    cout << "add $20,$0,$3" << endl;
}
void deleteCode(){
    int loc = location;
    location -= 4;
    cout << "sw $31," << loc << "($30)" << endl;
    cout << "lis $29" << endl;
    cout << ".word " << location << endl;
    cout << "add $30,$30,$29" << endl;    
    cout << "add $2,$0,$4" << endl;
    cout << "add $1,$0,$20" << endl;
    cout << "lis $3" << endl;
    cout << ".word delete" << endl;
    cout << "jalr $3" << endl;
    cout << "sub $30,$30,$29" << endl;
    cout << "lw $31," << loc << "($30)" << endl;
}
void ptr_mani(tree* expr){
	if (expr->rule == "expr term") ptr_mani(expr->children[0]);
	else if (expr->rule == "term factor") ptr_mani(expr->children[0]);
	else if (expr->rule == "factor ID"){
		 string id = expr->children[0]->tokens[1];
		 int loc = symbol[id].loc;
		 cout << "lw $20," << loc << "($30)" << endl;
		 if (symbol[id].type != "int*"){
		     cout << "mult $20,$7" << endl;
		     cout << "mflo $20" << endl;
		 }
	}
	else if (expr->rule == "factor NUM"){
		 cout << "lis $20" << endl;
		 cout << ".word " << expr->value << endl;
        cout << "mult $20,$7" << endl;
        cout <<"mflo $20" << endl;
	}
     else if (expr->rule == "factor NEW INT LBRACK expr RBRACK"){
         test(expr->children[3]);
         newCode();
     }
	else if (expr->rule == "factor AMP lvalue"){
		if (expr->children[1]->rule == "lvalue ID"){
			string id = expr->children[1]->children[0]->tokens[1];
			int loc = symbol[id].loc;
            cout << "lis $20" << endl;
            cout << ".word " << loc << endl;
			cout << "add $20,$30,$20" << endl;
		}
		else if (expr->children[1]->rule == "lvalue STAR factor"){
			test(expr->children[1]->children[1]);
		}
	}
	else if (expr->rule == "factor LPAREN expr RPAREN") ptr_mani(expr->children[1]);
	else if (expr->rule == "factor STAR factor"){
		ptr_mani(expr->children[1]);
		cout << "lw $20,0($20)" << endl;
	}
     else if (expr->rule == "expr expr PLUS term"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 string type1 = symbol[id0].type;
			 string type2 = symbol[id2].type;
             if (type1 == "int" && type2 == "int"){
                 back = 1;
                 return;
             }
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
             if (type1 == "int*" && type2 == "int*"){
                 
             }
             else if (type1 == "int*"){
				 cout << "mult $22,$7" << endl;
				 cout << "mflo $22" << endl;
			 }
			 else if (type2 == "int*"){
				 cout << "mult $21,$7" << endl;
				 cout << "mflo $21" << endl;
			 }
			 cout << "add $20,$21,$22" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
             if (expr->children[0]->executable && symbol[id2].type == "int"){
                 back = 1;
                 return;
             }
			 if (expr->children[0]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "mult $21,$7" << endl;
				 cout << "mflo $21" << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "add $20,$21,$22" << endl;
			 }else{
				 if (symbol[id2].type == "int*"){
                     test(expr->children[0]);
                     if (backtoptr){
                         ptr_mani(expr->children[0]);
                         backtoptr = 0;
                         if (expr->children[2]->children[0]->rule == "factor STAR factor"){
                             int locc = location;
                             location -= 4;
                             cout << "sw $20," << locc << "($30)" << endl;
                             ptr_mani(expr->children[2]->children[0]->children[1]);
                             cout << "lw $20,0($20)" << endl;
                             cout << "mult $20,$7" << endl;
                             cout << "mflo $20" << endl;
                             cout << "lw $22," << locc << "($30)" << endl;
                             cout << "add $20,$20,$22" << endl;
                             return;
                         }
                         cout << "lw $22," << loc2 << "($30)" << endl;
                         cout << "add $20,$20,$22" << endl;
                         return;
                     }
                     cout << "mult $20,$7" << endl;
                     cout << "mflo $20" << endl;
				    cout << "lw $22," << loc2 << "($30)" << endl;
				}else if (symbol[id2].type == "int"){
                    ptr_mani(expr->children[0]);
                    if (backtotest){
                        test(expr->children[0]);
                        backtotest = 0;
                    }
					cout << "lw $22," << loc2 << "($30)" << endl;
					cout << "mult $22,$7" << endl;
					cout << "mflo $22" << endl;
				}
				 cout << "add $20,$20,$22" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
             if (expr->children[2]->executable && symbol[id0].type == "int"){
                 back = 1;
                 return;
             }
			 if (expr->children[2]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "mult $21,$7" << endl;
				 cout << "mflo $21" << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "add $20,$21,$22" << endl;
			 }else{
				 if (symbol[id0].type == "int*"){
                     test(expr->children[2]);
                     if (backtoptr){
                         ptr_mani(expr->children[2]);
                         backtoptr = 0;
                         if (expr->children[0]->children[0]->rule == "factor STAR factor"){
                             int locc = location;
                             location -= 4;
                             cout << "sw $20," << locc << "($30)" << endl;
                             ptr_mani(expr->children[0]->children[0]->children[1]);
                             cout << "lw $20,0($20)" << endl;
                             cout << "mult $20,$7" << endl;
                             cout << "mflo $20" << endl;
                             cout << "lw $22," << locc << "($30)" << endl;
                             cout << "add $20,$20,$22" << endl;
                             return;
                         }
                         cout << "lw $22," << loc0 << "($30)" << endl;
                         cout << "add $20,$20,$22" << endl;
                         return;
                     }
                     cout <<"mult $20,$7" << endl;
                     cout << "mflo $20" << endl;
				    cout << "lw $22," << loc0 << "($30)" << endl;
				}else if (symbol[id0].type == "int"){
                    ptr_mani(expr->children[2]);
                    if (backtotest){
                        test(expr->children[2]);
                        backtotest = 0;
                    }
					cout << "lw $22," << loc0 << "($30)" << endl;
					cout << "mult $22,$7" << endl;
					cout << "mflo $22" << endl;
				}
				 cout << "add $20,$20,$22" << endl;
			 }
		 }
		 else {
			 ptr_mani(expr->children[0]);
             if (back) {
                 test(expr->children[0]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 back = 0;
             }
             if (backtotest){
                 test(expr->children[0]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 backtotest = 0;
             }
             /*
             if (compound){
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 compound = 0;
             }
              */
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 ptr_mani(expr->children[2]);
             if (back){
                 test(expr->children[2]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 back = 0;
             }
             if (backtotest){
                 test(expr->children[2]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 backtotest = 0;
             }
             /*
             if (compound){
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 compound = 0;
             }
              */
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "add $20,$20,$21" << endl;
		 }
		 
	 }
	 //===============minus=========================
	 else if (expr->rule == "expr expr MINUS term"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 string type1 = symbol[id0].type;
			 string type2 = symbol[id2].type;
             if (symbol[id0].type == "int" && symbol[id2].type == "int"){
                 back = 1;
                 return;
             }
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
			 if (type1 == "int*" && type2 == "int*"){
				 cout << "sub $20,$21,$22" << endl;
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
			 }
			 else if (type1 == "int*" && type2 == "int"){
				 cout << "mult $22,$7" << endl;
				 cout << "mflo $22" << endl;
				 cout << "sub $20,$21,$22" << endl;
			 }
		 }
         else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
             if (expr->children[2]->executable && symbol[id0].type == "int"){
                 back = 1;
                 return;
             }
			 if (expr->children[2]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "mult $21,$7" << endl;
				 cout << "mflo $21" << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "sub $20,$22,$21" << endl;
			 }else{
                 if (symbol[id0].type == "int*"){
				 test(expr->children[2]);
                     if (backtoptr){
                         ptr_mani(expr->children[2]);
                         backtoptr = 0;
                         if (expr->children[0]->children[0]->rule == "factor STAR factor"){
                             int locc = location;
                             location -= 4;
                             cout << "sw $20," << locc << "($30)" << endl;
                             ptr_mani(expr->children[0]->children[0]->children[1]);
                             cout << "lw $20,0($20)" << endl;
                             cout << "mult $20,$7" << endl;
                             cout << "mflo $20" << endl;
                             cout << "lw $22," << locc << "($30)" << endl;
                             cout << "sub $20,$20,$22" << endl;
                             return;
                         }
                         cout << "lw $22," << loc0 << "($30)" << endl;
                         cout << "add $20,$20,$22" << endl;
                         return;
                     }
				 cout << "lw $22," << loc0 << "($30)" << endl;
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 cout << "sub $20,$22,$20" << endl;
                 }
                 else if (symbol[id0].type == "int"){
                     test(expr->children[2]);
                     if (backtoptr){
                         ptr_mani(expr->children[2]);
                         backtoptr = 0;
                         if (expr->children[0]->children[0]->rule == "factor STAR factor"){
                             int locc = location;
                             location -= 4;
                             cout << "sw $20," << locc << "($30)" << endl;
                             ptr_mani(expr->children[0]->children[0]->children[1]);
                             cout << "lw $20,0($20)" << endl;
                             cout << "mult $20,$7" << endl;
                             cout << "mflo $20" << endl;
                             cout << "lw $22," << locc << "($30)" << endl;
                             cout << "add $20,$20,$22" << endl;
                             return;
                         }
                         cout << "lw $22," << loc0 << "($30)" << endl;
                         cout << "add $20,$20,$22" << endl;
                         return;
                     }
                     cout << "lw $22," << loc0 << "($30)" << endl;
                     cout << "sub $20,$22,$20" << endl;
                     cout << "mult $20,$7" << endl;
                     cout << "mflo $20" << endl;
                 }
			 }
         }
         else if (id0 == "" && id2 != ""){
             int loc2 = symbol[id2].loc;
             string id2 = expr->children[2]->id;
             if (expr->children[0]->executable && symbol[id2].type == "int"){
                 back = 1;
                 return;
             }
				 ptr_mani(expr->children[0]);
             if (backtotest){
                 test(expr->children[0]);
                 backtotest = 0;
             }
             if (symbol[id2].type == "int"){
				 cout << "lw $22," << loc2 << "($30)" << endl;
                 cout << "mult $22,$7" << endl;
                 cout << "mflo $20" << endl;
             }else if (symbol[id2].type == "int*"){
                 if (expr->children[2]->children[0]->rule == "factor STAR factor"){
                     int locc = location;
                     location -= 4;
                     cout << "sw $20," << locc << "($30)" << endl;
                     ptr_mani(expr->children[2]->children[0]->children[1]);
                     cout << "lw $20,0($20)" << endl;
                     cout << "mult $20,$7" << endl;
                     cout << "mflo $20" << endl;
                     cout << "lw $22," << locc << "($30)" << endl;
                     cout << "sub $20,$22,$20" << endl;
                     return;
                 }
                 cout << "lw $22," << loc2 << "($30)" << endl;
             }
				 cout << "sub $20,$20,$22" << endl;
         }
		 else {
			 ptr_mani(expr->children[0]);
             if (back){
                 test(expr->children[0]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 back = 0;
             }
             if (backtotest){
                 test(expr->children[0]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 backtotest = 0;
             }
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 ptr_mani(expr->children[2]);
             if (back){
                 test(expr->children[2]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 back = 0;             
             }
             if (backtotest){
                 test(expr->children[2]);
                 cout << "mult $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 backtotest = 0;
             }
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "sub $20,$21,$20" << endl;
		 }
	 }
     else if (expr->rule == "term term STAR factor"){
         backtotest = 1;
     }
     else if (expr->rule == "term term SLASH factor"){
         backtotest = 1;
     }
     else if (expr->rule == "term term PCT factor"){
         backtotest = 1;
     }
} 
	
void test(tree* expr){
	 if (expr->rule == "expr term") test(expr->children[0]);
	 else if (expr->rule == "term factor") test(expr->children[0]);
	 else if (expr->rule == "factor ID"){
		 string id = expr->children[0]->tokens[1];
		 int loc = symbol[id].loc;
		 cout << "lw $20," << loc << "($30)" << endl;
	 }
	 else if (expr->rule == "factor NUM"){
		 cout << "lis $20" << endl;
		 cout << ".word " << expr->value << endl;
	 }
     else if (expr->rule == "factor NEW INT LBRACK expr RBRACK"){
         test(expr->children[3]);
         newCode();
     }
	 else if (expr->rule == "factor LPAREN expr RPAREN") test(expr->children[1]);
	 else if (expr->rule == "factor STAR factor"){
		 ptr_mani(expr->children[1]);
		 cout << "lw $20,0($20)" << endl;
	 }
	else if (expr->rule == "factor AMP lvalue"){
		if (expr->children[1]->rule == "lvalue ID"){
			string id = expr->children[1]->children[0]->tokens[1];
			int loc = symbol[id].loc;
			cout << "lis $20" << endl;
			cout << ".word " << loc << endl;
			cout << "add $20,$30,$20" << endl;
		}
	}
     else if (expr->rule == "expr expr PLUS term"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
			 cout << "add $20,$21,$22" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
			 if (expr->children[0]->executable){
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "add $20,$21,$22" << endl;
			 }else{
                 if (symbol[id2].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 test(expr->children[0]);
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "add $20,$20,$22" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
			 if (expr->children[2]->executable){
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "add $20,$21,$22" << endl;
			 }else{
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 test(expr->children[2]);
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "add $20,$20,$22" << endl;
			 }
		 }
		 else {
			 test(expr->children[0]);
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 test(expr->children[2]);
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "add $20,$20,$21" << endl;
		 }
		 
	 }
	 //===============minus=========================
	 else if (expr->rule == "expr expr MINUS term"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
             if (symbol[id0].type == "int*" && symbol[id2].type == "int*"){
                 cout << "sub $20,$21,$22" << endl;
                 cout << "div $20,$7" << endl;
                 cout << "mflo $20" << endl;
                 cout << "sw $20,0($20)" << endl;
                 return;
             }
			 cout << "sub $20,$21,$22" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
			 if (expr->children[0]->executable){
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "sub $20,$21,$22" << endl;
			 }else{
                 if (symbol[id2].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 test(expr->children[0]);
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "sub $20,$20,$22" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
			 if (expr->children[2]->executable){
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "sub $20,$22,$21" << endl;
			 }else{
                 if (symbol[id0].type == "int*"){
                     backtoptr = 1;
                     return;
                 }
				 test(expr->children[2]);
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "sub $20,$22,$20" << endl;
			 }
		 }
		 else {
			 test(expr->children[0]);
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 test(expr->children[2]);
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "sub $20,$21,$20" << endl;
		 }
	 }
	 //===================star======================
	 else if (expr->rule == "term term STAR factor"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
			 cout << "mult $21,$22" << endl;
			 cout << "mflo $20" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
			 if (expr->children[0]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "mult $21,$22" << endl;
				 cout << "mflo $20" << endl;
			 }else{
				 test(expr->children[0]);
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "mult $20,$22" << endl;
				 cout << "mflo $20" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
			 if (expr->children[2]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "mult $21,$22" << endl;
				 cout << "mflo $20" << endl;
			 }else{
				 test(expr->children[2]);
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "mult $20,$22" << endl;
				 cout << "mflo $20" << endl;
			 }
		 }
		 else {
			 test(expr->children[0]);
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 test(expr->children[2]);
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "mult $20,$21" << endl;
			 cout << "mflo $20" << endl;
		 }
	 }
	 //=============================slash==================
	 else if (expr->rule == "term term SLASH factor"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
			 cout << "div $21,$22" << endl;
			 cout << "mflo $20" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
			 if (expr->children[0]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "div $21,$22" << endl;
				 cout << "mflo $20" << endl;
			 }else{
				 test(expr->children[0]);
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "div $20,$22" << endl;
				 cout << "mflo $20" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
			 if (expr->children[2]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "div $22,$21" << endl;
				 cout << "mflo $20" << endl;
			 }else{
				 test(expr->children[2]);
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "div $22,$20" << endl;
				 cout << "mflo $20" << endl;
			 }
		 }
		 else {
			 test(expr->children[0]);
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 test(expr->children[2]);
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "div $21,$20" << endl;
			 cout << "mflo $20" << endl;
		 }
	 }
	 //============================pct===================
	 else if (expr->rule == "term term PCT factor"){
		 string id0 = expr->children[0]->id;
	     string id2 = expr->children[2]->id;
		 if (id0 != "" && id2 != ""){
			 int loc0 = symbol[id0].loc;
			 int loc2 = symbol[id2].loc;
			 cout << "lw $21," <<loc0 << "($30)" << endl;
			 cout << "lw $22," << loc2 << "($30)" << endl;
			 cout << "div $21,$22" << endl;
			 cout << "mfhi $20" << endl;
		 }
		 else if (id0 == "" && id2 != ""){
			 int loc2 = symbol[id2].loc;
			 if (expr->children[0]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[0]->value << endl;
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "div $21,$22" << endl;
				 cout << "mfhi $20" << endl;
			 }else{
				 test(expr->children[0]);
				 cout << "lw $22," << loc2 << "($30)" << endl;
				 cout << "div $20,$22" << endl;
				 cout << "mfhi $20" << endl;
			 }
		 }
		 else if (id0 != "" && id2 == ""){
			 int loc0 = symbol[id0].loc;
			 if (expr->children[2]->executable){
				 cout << "lis $21" << endl;
				 cout << ".word " << expr->children[2]->value << endl;
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "div $22,$21" << endl;
				 cout << "mfhi $20" << endl;
			 }else{
				 test(expr->children[2]);
				 cout << "lw $22," << loc0 << "($30)" << endl;
				 cout << "div $22,$20" << endl;
				 cout << "mfhi $20" << endl;
			 }
		 }
		 else {
			 test(expr->children[0]);
			 int loc = location;
			 location -= 4;
			 cout << "sw $20," << loc <<"($30)" << endl;
			 test(expr->children[2]);
			 cout << "lw $21," << loc << "($30)" << endl;
			 cout << "div $21,$20" << endl;
			 cout << "mfhi $20" << endl;
		 }
	 }
}
void statement(tree* t){
	if (t->rule == "statements statements statement"){
		statement(t->children[0]);
		begin(t->children[1]);
	}
}
void printCode(tree* t){
		 int loc = location,loc2;
		 if (t->children[2]->executable){
			 if (t->children[2]->id == ""){
			 if(reg_search(t->children[2]->value)){
				 string reg = reg_value[t->children[2]->value];
				 loc2 = location;
				 location -= 4;
				 cout << "sw $31," << loc2 << "($30)" << endl;
				 cout << "lis $29" << endl;
				 cout << ".word " << location+4 << endl;
				 cout << "add $30,$30,$29" << endl;
				 cout << "add $1,$0," << reg << endl;
				 cout << "lis $3" << endl;
				 cout << ".word print" << endl;
				 cout << "jalr $3" << endl;
				 cout << "sub $30,$30,$29" << endl;
				 cout << "lw $31," << loc2 << "($30)" << endl;
			 }
			 else {
				 loc2 = location;
				 location -= 4;
				 cout << "sw $31," << loc2 << "($30)" << endl;
				 cout << "lis $29" << endl;
				 cout << ".word " << location+4 << endl;
				 cout << "add $30,$30,$29" << endl;
				 cout << "lis $3" << endl;
				 cout << ".word " << t->children[2]->value << endl;
				 cout << "add $1,$0,$3" << endl;
				 cout << "lis $3" << endl;
				 cout << ".word print" << endl;
				 cout << "jalr $3" << endl;
				 cout << "sub $30,$30,$29" << endl;
				 cout << "lw $31," << loc2 << "($30)" << endl;
			 }
		     }
		     else{
				 loc2 = location;
				 location -= 4;
				 cout << "sw $31," << loc2 << "($30)" << endl;
				 cout << "lis $29" << endl;
				 cout << ".word " << location+4 << endl;
				 cout << "add $30,$30,$29" << endl;
				 cout << "lis $3" << endl;
				 cout << ".word " << symbol[t->children[2]->id].value << endl;
				 cout << "add $1,$0,$3" << endl;
				 cout << "lis $3" << endl;
				 cout << ".word print" << endl;
				 cout << "jalr $3" << endl;
				 cout << "sub $30,$30,$29" << endl;
				 cout << "lw $31," << loc2 << "($30)" << endl;
			 }
				 
		 }
		 else if (t->children[2]->id != "" && symbol[t->children[2]->id].reg != "no"){
			 if (symbol[t->children[2]->id].reg != "no"){
				 string reg = symbol[t->children[2]->id].reg;
				 loc2 = location;
				 location -= 4;
				 cout << "sw $31," << loc2 << "($30)" << endl;
				 cout << "lis $29" << endl;
				 cout << ".word " << location+4 << endl;
				 cout << "add $30,$30,$29" << endl;
				 cout << "add $1,$0," << reg << endl;
				 cout << "lis $3" << endl;
				 cout << ".word print" << endl;
				 cout << "jalr $3" << endl;
				 cout << "sub $30,$30,$29" << endl;
				 cout << "lw $31," << loc2 << "($30)" << endl;
			 }
		 }
		 else {
		     arithmetic(t->children[2]);
		     loc2 = location;
		     location -= 4;
		     cout << "sw $31," << loc2 << "($30)" << endl;
				 cout << "lis $29" << endl;
				 cout << ".word " << location+4 << endl;
				 cout << "add $30,$30,$29" << endl;
		     cout << "add $1,$0,$20" << endl;
		     cout << "lis $3" << endl;
		     cout << ".word print" << endl;
		     cout << "jalr $3" << endl;
		     cout << "sub $30,$30,$29" << endl;
		     cout << "lw $31," << loc2 << "($30)" << endl;
	     }
	 }
void begin(tree* t){
	 int loc3;
     if (t->rule == "statement lvalue BECOMES expr SEMI"){
		 arithmetic(t->children[2]);
		 if (t->children[2]->executable){
			 symbol[t->children[0]->id].value = t->children[2]->value;
			 t->children[0]->value = t->children[2]->value;
		     loc3 = location;
		     cout << "sw $20," << loc3 << "($30)" << endl;
		     location -= 4;
		     symbol[t->children[0]->id].loc = loc3;
		 }
		 else{
		     loc3 = location;
		     cout << "sw $20," << loc3 << "($30)" << endl;
		     location -= 4;
		     symbol[t->children[0]->id].loc = loc3;
		     symbol[t->children[0]->id].value = -1;
		     symbol[t->children[0]->id].executable = 0;
		 }
	 }
	 else if (t->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
		 test(t->children[2]);
		 printCodeWhile(t);
	 }
	 else if (t->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
         doingIf(t);
	 } 
	 else if (t->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
		 whileFoo(t);
	 }
	 else if (t->rule == "statement DELETE LBRACK RBRACK expr SEMI"){
	 }
}

void returnValue(tree* t){
	if (t->rule == "expr term" || t->rule == "term factor"){
        returnValue(t->children[0]);
        /*
		string id = t->id;
		if (t->id == "") returnValue(t->children[0]);
		
		else if (symbol[id].value >= 0){
			cout << "lis $3" << endl;
			cout << ".word " << symbol[id].value << endl;
		}
		else if (symbol[t->id].reg != "no"){
			cout << "add $3,$0," << symbol[t->id].reg << endl;
		}
		
		else{
		    int loc = symbol[t->id].loc;
		    cout << "lw $3," << loc << "($30)" << endl;
		}
         */
	}
	else if (t->rule == "factor STAR factor"){
		ptr_mani(t->children[1]);
		cout << "lw $3,0($20)" << endl;
	}
	else if (t->rule == "factor NUM"){
		cout << "lis $3" << endl;
		cout << ".word " << t->value << endl;
	}
	else if (t->rule == "factor ID"){
		int loc = symbol[t->id].loc;
		cout << "lw $3," << loc << "($30)" << endl;
	}
	else if (t->rule == "factor LPAREN expr RPAREN"){
		    test(t);
			cout << endl;

			    cout << "add $3,$0,$20" << endl;

	}
	else if (t->rule == "expr expr PLUS term" || t->rule == "expr expr MINUS term"){
		    test(t);
			cout << endl;

			    cout << "add $3,$0,$20" << endl;

	}
	else if (t->rule == "term term STAR factor"){
		test(t);
	    cout << endl;
			cout << "add $3,$0,$20" << endl;
	}
	else if (t->rule == "term term SLASH factor"){
		test(t);
	    cout << endl;

			cout << "add $3,$0,$20" << endl;

	}
	else if (t->rule == "term term PCT factor"){
		test(t);
	    cout << endl;
			cout << "add $3,$0,$20" << endl;
	}
		
}

void PrintTree(tree* t){
	cout << t << endl;
	for (int i = 0 ; i < t->children.size() ; ++i){
	    PrintTree(t->children[i]);
	}
	
}
void init(){
    int loc = location;
    location -= 4;
    cout << "sw $31," << loc << "($30)" << endl;
    cout << "lis $29" << endl;
    cout << ".word " << location << endl;
    cout << "lw $1,-4($30)" << endl;
    if (symbol[var1].type == "int"){
        cout << "add $2,$0,$0" << endl;
    }
    cout << "add $30,$30,$29" << endl;    
    
    cout << "lis $28" << endl;
    cout << ".word init" << endl;
    cout << "jalr $28" << endl;
    cout << "sub $30,$30,$29" << endl;
    cout << "lw $31," << loc << "($30)" << endl;
}
int main() {
  // Main program.
  try {
	reg_value[0] = "$0";  
    reg_value[1] = "$4";
    reg_value[2] = "$5";
    reg_value[3] = "$6";
    reg_value[4] = "$7";
    reg_value[5] = "$8";
    parseTree = readParse("S");
    tree* returnExpr = parseTree->children[1]->children[11];
    tree* statementExpr = parseTree->children[1]->children[9];
    Scan(parseTree);
      WriteGlobalHeader();    //clear
    if (printLabel) cout << ".import print" << endl;
      if (newLabel) {cout << ".import init" << endl; init(); cout << ".import new" << endl;}
    if (deleteLabel) cout << ".import delete" << endl;
    
    //PrintTree(parseTree);
    whileExpr(statementExpr);
    returnValue(returnExpr);
    //printSymbol();
    cout << "jr $31" << endl;
  } catch(string msg) {
    cerr << msg << endl;
  }
  if (parseTree) delete parseTree;
  return 0;
}
