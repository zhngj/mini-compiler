#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

//stroe identifier table
map<string,string> symbol;

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
    vector<string> tokens;
    vector<tree*> children;
    ~tree() { for(int i=0; i<children.size(); i++) delete children[i]; }
};

// data stucture for symbol table
struct info {
	info(string a,string b): type(a),id(b) {}
	string type; // either 'use' or 'dcl'
	string id; // int or int*
};


// Call this to display an error message and exit the program.
void bail(const string &msg) {
  // You can also simply throw a string instead of using this function.
  throw string(msg);
}

// Read and return wlppi parse tree.
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

// Compute symbols defined in t.
void genSymbols(tree *t) {
}

// Generate the code for the parse tree t.
void genCode(tree *t) {
}
bool contains(string target){
	int count = symbol.count(target);
	if (count == 0) return false;
	return true;
}
// print the identifier table
tree* errorCheck(tree *t){
	int childrenSize = t->children.size();
	// check if a undcl var is used:
	if (t->rule == "lvalue ID" || t->rule == "factor ID") {
		// get content of ID
		string id = t->children[0]->tokens[1];
		if (!contains(id)) {
		    string err = "ERROR undeclair var " + id;
			bail(err);
		}
		else {
			// get type
			t->exp = symbol[id];
		}
	}
	// this is dcl, need to check double dcl
    else if (t->rule == "dcl type ID") {
	    string id = t->children[1]->tokens[1];
	    tree* sub = t->children[0];
	    int tokenSize = sub->tokens.size();
	    string int_id;
	    if (tokenSize == 3) {
		    int_id = "int*";
		    t->exp = "int*";
	    }
	    else { int_id = "int"; t->exp = "int";}
	    // error checking
	    if (contains(id)) {
           	string err = "ERROR double dcl for " + id;
            bail(err);
	    }
	    else {
		    symbol.insert(pair<string,string>(id,int_id));
	    }
    }
	for (int i = 0 ; i < childrenSize ; ++i) {
		t->children[i] = errorCheck(t->children[i]);
	}
	if (t->rule == "term factor" || t->rule == "expr term") t->exp = t->children[0]->exp;
	if (t->rule == "factor NUM") t->exp = "int";
	if (t->rule == "factor NULL") t->exp = "int*";
	// check if int a = NULL or int* a = NUM
	if (t->tokens.size() == 15){
		if (t->children[11]->exp != "int") bail("ERROR unexpected return type");
		if (t->children[5]->exp != "int") bail("ERROR 2nd arg should be int");
	}
	else if (t->tokens.size() == 6){
		if (t->tokens[0] == "dcls"){
		   if (t->children[1]->exp == "int"){
			  if (t->tokens[4] == "NULL"){
				  bail("ERROR wrong dcl for int");
			  }
		  }
		  else if (t->children[1]->exp == "int*"){
			  if (t->tokens[4] == "NUM") bail("ERROR wrong dcl for ptr");
		  }
	    }
	    else if (t->tokens[0] == "statement"){
			if (t->tokens[1] == "DELETE"){
				if (t->children[3]->exp != "int*") bail("ERROR invalid delete");
			}
			else if (t->tokens[1] == "PRINTLN"){
				if (t->children[2]->exp != "int") bail("ERROR invalid print");
			}
		}
		else if (t->tokens[0] == "factor"){
			if (t->children[3]->exp != "int") {
				bail("ERROR invalid allocation");
			}
			t->exp = "int*";
		}
		
	}
	//check expr
	else if (t->tokens.size() == 5){
		if (t->tokens[0] =="statement"){
			if (t->children[0]->exp != t->children[2]->exp) bail("ERROR unbalanced expr by lhs and rhs");
		}
	}
	else if (t->tokens.size() == 4){
		if (t->tokens[0] == "term"){
			if (t->children[0]->exp != "int" || t->children[2]->exp != "int"){
				 bail("ERROR wrong type for * / %");
			 }
			t->exp = "int";
		}
		else if (t->tokens[0] == "expr"){
			if (t->tokens[2] == "PLUS"){
				if (t->children[0]->exp == "int*" && t->children[2]->exp == "int*") bail("ERROR cant add two ptr");
				else if (t->children[0]->exp == "int" && t->children[2]->exp == "int") t->exp = "int";
				else t->exp = "int*";
			}
			if (t->tokens[2] == "MINUS"){
				if (t->children[0]->exp == "int" && t->children[2]->exp == "int*") bail("ERROR cant sub a ptr from int");
				else if (t->children[0]->exp == "int*" && t->children[2]->exp == "int") t->exp = "int*";
				else t->exp = "int";
			}
		}
		else if (t->tokens[0] == "factor"){
			if (t->rule == "factor LPAREN expr RPAREN") t->exp = t->children[1]->exp;
		}
		else if (t->tokens[0] == "lvalue"){
			if (t->rule == "lvalue LPAREN lvalue RPAREN") t->exp = t->children[1]->exp;
		}
		else if (t->tokens[0] == "test"){
			if (t->children[0]->exp != t->children[2]->exp) bail("ERROR cmp diff pair");
		}
			
	}
	else if (t->tokens.size() == 3){
		if (t->rule == "factor STAR factor"){
			//first check rhs factor indeed a ptr
			if (t->children[1]->exp != "int*") bail("ERROR invalid deref");
			// pass in exp
			t->exp = "int";
		}
		else if (t->rule == "factor AMP lvalue"){
			if (t->children[1]->exp != "int") bail("ERROR invalid ref");
			t->exp = "int*";
		}
		else if (t->rule == "lvalue STAR factor"){
			if (t->children[1]->exp != "int*") bail("ERROR invalid deref");
			t->exp = "int";
		}
	}
	return t;
}
void printSymbol(){
	map<string,string>::iterator it;
	for (it = symbol.begin() ; it != symbol.end(); ++it) {
		cerr << it->first << " " << it->second << endl;
	}
}


int main() {
  // Main program.
  try {
    parseTree = readParse("S");
    parseTree = errorCheck(parseTree);
    
    genSymbols(parseTree);
    genCode(parseTree);
    //cout << parseTree->children[0]->tokens[1] << endl;
    //cout << parseTree->children[1]->children[0]->rule << endl;
  } catch(string msg) {
    cerr << msg << endl;
  }
  //printSymbol();
  if (parseTree) delete parseTree;
  return 0;
}
