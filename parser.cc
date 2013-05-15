#include <map>
#include <set>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <vector>
using namespace std;

string wlpp = "35\nAMP\nBECOMES\nBOF\nCOMMA\nDELETE\nELSE\nEOF\nEQ\nGE\nGT\nID\nIF\nINT\nLBRACE\nLBRACK\nLE\nLPAREN\nLT\nMINUS\nNE\nNEW\nNULL\nNUM\nPCT\nPLUS\nPRINTLN\nRBRACE\nRBRACK\nRETURN\nRPAREN\nSEMI\nSLASH\nSTAR\nWAIN\nWHILE\n12\nS\ndcl\ndcls\nexpr\nfactor\nlvalue\nprocedure\nstatement\nstatements\nterm\ntest\ntype\nS\n38\nS BOF procedure EOF\nprocedure INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE\ntype INT\ntype INT STAR\ndcls\ndcls dcls dcl BECOMES NUM SEMI\ndcls dcls dcl BECOMES NULL SEMI\ndcl type ID\nstatements\nstatements statements statement\nstatement lvalue BECOMES expr SEMI\nstatement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE\nstatement WHILE LPAREN test RPAREN LBRACE statements RBRACE\nstatement PRINTLN LPAREN expr RPAREN SEMI\nstatement DELETE LBRACK RBRACK expr SEMI\ntest expr EQ expr\ntest expr NE expr\ntest expr LT expr\ntest expr LE expr\ntest expr GE expr\ntest expr GT expr\nexpr term\nexpr expr PLUS term\nexpr expr MINUS term\nterm factor\nterm term STAR factor\nterm term SLASH factor\nterm term PCT factor\nfactor ID\nfactor NUM\nfactor NULL\nfactor LPAREN expr RPAREN\nfactor AMP lvalue\nfactor STAR factor\nfactor NEW INT LBRACK expr RBRACK\nlvalue ID\nlvalue STAR factor\nlvalue LPAREN lvalue RPAREN\n107\n710\n6 NE reduce 33\n90 NE reduce 32\n46 NUM shift 1\n54 NUM shift 1\n32 NUM shift 1\n36 NE reduce 31\n80 NE reduce 34\n80 MINUS reduce 34\n30 STAR shift 2\n103 RETURN reduce 8\n59 EQ reduce 36\n12 NE reduce 28\n1 NE reduce 29\n11 NE reduce 30\n53 EQ reduce 35\n47 NEW shift 3\n48 NEW shift 3\n12 MINUS reduce 28\n1 MINUS reduce 29\n11 MINUS reduce 30\n6 MINUS reduce 33\n90 MINUS reduce 32\n45 EQ reduce 37\n36 MINUS reduce 31\n101 RETURN reduce 8\n100 RETURN reduce 8\n34 SEMI shift 4\n51 SEMI shift 5\n4 WHILE reduce 6\n5 WHILE reduce 5\n85 STAR reduce 4\n67 PRINTLN reduce 8\n14 factor shift 6\n6 RBRACK reduce 33\n90 RBRACK reduce 32\n23 GT shift 7\n71 RBRACE shift 8\n72 RBRACE shift 9\n66 RETURN reduce 9\n36 RBRACK reduce 31\n80 RBRACK reduce 34\n73 RBRACE shift 10\n12 RBRACK reduce 28\n1 RBRACK reduce 29\n11 RBRACK reduce 30\n4 INT reduce 6\n5 INT reduce 5\n83 NULL shift 11\n13 NULL shift 11\n7 ID shift 12\n16 ID shift 12\n24 ID shift 12\n21 ID shift 12\n37 ID shift 12\n17 ID shift 12\n81 BECOMES reduce 7\n83 LPAREN shift 13\n46 STAR shift 14\n54 STAR shift 14\n32 STAR shift 14\n83 factor shift 15\n45 GT reduce 37\n13 factor shift 15\n59 GT reduce 36\n13 LPAREN shift 13\n53 GT reduce 35\n23 GE shift 16\n53 GE reduce 35\n59 GE reduce 36\n85 PRINTLN reduce 4\n45 GE reduce 37\n23 EQ shift 17\n68 BECOMES shift 18\n19 RBRACK reduce 21\n78 RBRACK reduce 22\n79 RBRACK reduce 23\n49 NUM shift 1\n18 NUM shift 1\n2 LPAREN shift 13\n53 PCT reduce 35\n27 NUM shift 1\n2 STAR shift 14\n84 term shift 19\n59 PCT reduce 36\n45 PCT reduce 37\n56 RPAREN shift 20\n23 LT shift 21\n95 PLUS shift 22\n94 PLUS shift 22\n93 PLUS shift 22\n92 PLUS shift 22\n91 PLUS shift 22\n96 PLUS shift 22\n83 NUM shift 1\n47 expr shift 23\n48 expr shift 23\n59 BECOMES reduce 36\n53 BECOMES reduce 35\n45 BECOMES reduce 37\n84 ID shift 12\n47 NULL shift 11\n48 NULL shift 11\n14 LPAREN shift 13\n80 SEMI reduce 34\n23 LE shift 24\n89 MINUS shift 25\n36 SEMI reduce 31\n88 MINUS shift 25\n13 STAR shift 14\n84 STAR shift 14\n13 NUM shift 1\n83 STAR shift 14\n27 term shift 19\n49 term shift 19\n18 term shift 19\n25 AMP shift 26\n22 AMP shift 26\n67 RETURN reduce 8\n10 DELETE reduce 11\n105 RBRACE reduce 10\n7 STAR shift 14\n16 STAR shift 14\n24 STAR shift 14\n21 STAR shift 14\n37 STAR shift 14\n17 STAR shift 14\n74 RBRACE reduce 13\n106 RBRACE reduce 14\n66 WHILE reduce 9\n9 DELETE reduce 12\n9 RBRACE reduce 12\n104 RBRACK shift 27\n6 SEMI reduce 33\n6 STAR reduce 33\n90 SEMI reduce 32\n90 STAR reduce 32\n10 RBRACE reduce 11\n12 SEMI reduce 28\n12 STAR reduce 28\n1 SEMI reduce 29\n1 STAR reduce 29\n11 SEMI reduce 30\n11 STAR reduce 30\n53 LE reduce 35\n36 STAR reduce 31\n80 STAR reduce 34\n64 SEMI shift 28\n74 PRINTLN reduce 13\n106 PRINTLN reduce 14\n105 PRINTLN reduce 10\n40 PLUS reduce 25\n41 PLUS reduce 26\n42 PLUS reduce 27\n9 PRINTLN reduce 12\n2 NUM shift 1\n15 PLUS reduce 24\n10 PRINTLN reduce 11\n27 LPAREN shift 13\n88 PLUS shift 22\n7 NULL shift 11\n16 NULL shift 11\n24 NULL shift 11\n21 NULL shift 11\n37 NULL shift 11\n17 NULL shift 11\n89 PLUS shift 22\n7 NUM shift 1\n16 NUM shift 1\n24 NUM shift 1\n21 NUM shift 1\n37 NUM shift 1\n17 NUM shift 1\n49 LPAREN shift 13\n18 LPAREN shift 13\n101 STAR reduce 8\n100 STAR reduce 8\n67 STAR reduce 8\n15 SEMI reduce 24\n103 STAR reduce 8\n39 STAR shift 29\n40 SEMI reduce 25\n41 SEMI reduce 26\n42 SEMI reduce 27\n80 RPAREN reduce 34\n67 WHILE reduce 8\n105 DELETE reduce 10\n74 DELETE reduce 13\n106 DELETE reduce 14\n12 RPAREN reduce 28\n1 RPAREN reduce 29\n11 RPAREN reduce 30\n6 RPAREN reduce 33\n90 RPAREN reduce 32\n47 STAR shift 14\n48 STAR shift 14\n36 RPAREN reduce 31\n19 LT reduce 21\n45 PLUS reduce 37\n97 LPAREN shift 30\n78 LT reduce 22\n79 LT reduce 23\n53 PLUS reduce 35\n59 PLUS reduce 36\n15 NE reduce 24\n64 PLUS shift 22\n40 NE reduce 25\n41 NE reduce 26\n42 NE reduce 27\n28 RBRACE shift 31\n39 ID reduce 2\n14 NEW shift 3\n30 LPAREN shift 30\n80 EQ reduce 34\n12 GT reduce 28\n1 GT reduce 29\n11 GT reduce 30\n36 GT reduce 31\n6 GT reduce 33\n90 GT reduce 32\n78 RPAREN reduce 22\n79 RPAREN reduce 23\n19 RPAREN reduce 21\n36 GE reduce 31\n80 GE reduce 34\n67 ID reduce 8\n10 WHILE reduce 11\n67 IF reduce 8\n66 RBRACE reduce 9\n12 GE reduce 28\n1 GE reduce 29\n11 GE reduce 30\n6 GE reduce 33\n90 GE reduce 32\n78 NE reduce 22\n79 NE reduce 23\n19 NE reduce 21\n4 STAR reduce 6\n5 STAR reduce 5\n9 WHILE reduce 12\n25 NEW shift 3\n22 NEW shift 3\n74 WHILE reduce 13\n106 WHILE reduce 14\n105 WHILE reduce 10\n101 IF reduce 8\n100 IF reduce 8\n103 ID reduce 8\n73 STAR shift 2\n103 IF reduce 8\n79 PCT shift 32\n78 PCT shift 32\n53 RBRACK reduce 35\n59 RBRACK reduce 36\n45 RBRACK reduce 37\n101 ID reduce 8\n100 ID reduce 8\n3 INT shift 33\n71 STAR shift 2\n72 STAR shift 2\n85 LPAREN reduce 4\n46 ID shift 12\n54 ID shift 12\n32 ID shift 12\n66 LPAREN reduce 9\n80 GT reduce 34\n85 IF reduce 4\n43 NULL shift 34\n85 ID reduce 4\n8 ELSE shift 35\n66 DELETE reduce 9\n89 RPAREN shift 36\n23 NE shift 37\n97 WHILE shift 38\n19 LE reduce 21\n14 AMP shift 26\n98 INT shift 39\n70 INT shift 39\n71 LPAREN shift 30\n72 LPAREN shift 30\n46 factor shift 40\n54 factor shift 41\n32 factor shift 42\n81 RPAREN reduce 7\n13 term shift 19\n78 LE reduce 22\n79 LE reduce 23\n83 term shift 19\n73 LPAREN shift 30\n58 BECOMES shift 43\n80 PCT reduce 34\n12 PCT reduce 28\n1 PCT reduce 29\n11 PCT reduce 30\n6 PCT reduce 33\n90 PCT reduce 32\n36 PCT reduce 31\n40 MINUS reduce 25\n41 MINUS reduce 26\n42 MINUS reduce 27\n15 MINUS reduce 24\n99 INT shift 44\n2 AMP shift 26\n15 STAR reduce 24\n45 RPAREN reduce 37\n7 factor shift 15\n16 factor shift 15\n24 factor shift 15\n21 factor shift 15\n37 factor shift 15\n17 factor shift 15\n9 LPAREN reduce 12\n105 LPAREN reduce 10\n74 LPAREN reduce 13\n106 LPAREN reduce 14\n53 RPAREN reduce 35\n69 RPAREN shift 45\n59 RPAREN reduce 36\n84 NUM shift 1\n10 LPAREN reduce 11\n19 STAR shift 46\n79 STAR shift 46\n78 STAR shift 46\n47 LPAREN shift 13\n48 LPAREN shift 13\n52 LPAREN shift 47\n38 LPAREN shift 48\n82 LPAREN shift 49\n19 MINUS reduce 21\n99 procedure shift 50\n78 MINUS reduce 22\n79 MINUS reduce 23\n36 LT reduce 31\n12 LT reduce 28\n1 LT reduce 29\n11 LT reduce 30\n67 INT shift 39\n6 LT reduce 33\n90 LT reduce 32\n80 LT reduce 34\n78 GE reduce 22\n79 GE reduce 23\n43 NUM shift 51\n97 IF shift 52\n19 GE reduce 21\n15 RBRACK reduce 24\n36 LE reduce 31\n6 LE reduce 33\n90 LE reduce 32\n23 PLUS shift 22\n40 RBRACK reduce 25\n41 RBRACK reduce 26\n42 RBRACK reduce 27\n12 LE reduce 28\n1 LE reduce 29\n11 LE reduce 30\n80 LE reduce 34\n97 ID shift 53\n47 ID shift 12\n48 ID shift 12\n78 GT reduce 22\n79 GT reduce 23\n19 SLASH shift 54\n19 GT reduce 21\n79 SLASH shift 54\n78 SLASH shift 54\n84 NULL shift 11\n13 AMP shift 26\n31 EOF reduce 1\n83 AMP shift 26\n40 STAR reduce 25\n41 STAR reduce 26\n42 STAR reduce 27\n67 RBRACE reduce 8\n15 EQ reduce 24\n40 EQ reduce 25\n41 EQ reduce 26\n42 EQ reduce 27\n27 expr shift 55\n49 expr shift 56\n18 expr shift 57\n103 RBRACE reduce 8\n67 dcl shift 58\n13 ID shift 12\n46 AMP shift 26\n54 AMP shift 26\n32 AMP shift 26\n83 ID shift 12\n2 factor shift 59\n4 PRINTLN reduce 6\n5 PRINTLN reduce 5\n98 dcl shift 60\n97 STAR shift 2\n71 DELETE shift 61\n72 DELETE shift 61\n80 SLASH reduce 34\n70 dcl shift 62\n27 ID shift 12\n49 ID shift 12\n18 ID shift 12\n67 type shift 63\n36 PLUS reduce 31\n6 SLASH reduce 33\n90 SLASH reduce 32\n36 SLASH reduce 31\n80 PLUS reduce 34\n67 LPAREN reduce 8\n12 SLASH reduce 28\n1 SLASH reduce 29\n11 SLASH reduce 30\n12 PLUS reduce 28\n1 PLUS reduce 29\n11 PLUS reduce 30\n6 PLUS reduce 33\n90 PLUS reduce 32\n15 GT reduce 24\n40 GT reduce 25\n41 GT reduce 26\n42 GT reduce 27\n26 LPAREN shift 30\n70 type shift 63\n98 type shift 63\n23 MINUS shift 25\n15 GE reduce 24\n84 expr shift 64\n40 GE reduce 25\n41 GE reduce 26\n42 GE reduce 27\n45 STAR reduce 37\n59 STAR reduce 36\n53 STAR reduce 35\n73 DELETE shift 61\n103 WHILE reduce 8\n85 DELETE reduce 4\n60 RPAREN shift 65\n101 RBRACE reduce 8\n100 RBRACE reduce 8\n71 statement shift 66\n72 statement shift 66\n49 factor shift 15\n18 factor shift 15\n7 AMP shift 26\n16 AMP shift 26\n24 AMP shift 26\n21 AMP shift 26\n37 AMP shift 26\n17 AMP shift 26\n27 factor shift 15\n73 statement shift 66\n101 WHILE reduce 8\n100 WHILE reduce 8\n85 dcls shift 67\n97 lvalue shift 68\n85 WHILE reduce 4\n84 NEW shift 3\n85 RETURN reduce 4\n66 ID reduce 9\n66 IF reduce 9\n2 NEW shift 3\n30 lvalue shift 69\n75 LPAREN shift 70\n47 factor shift 15\n48 factor shift 15\n13 NEW shift 3\n101 statements shift 71\n100 statements shift 72\n83 NEW shift 3\n103 statements shift 73\n27 NEW shift 3\n49 NEW shift 3\n18 NEW shift 3\n25 ID shift 12\n22 ID shift 12\n26 STAR shift 2\n2 ID shift 12\n66 STAR reduce 9\n103 PRINTLN reduce 8\n47 AMP shift 26\n48 AMP shift 26\n14 ID shift 12\n101 PRINTLN reduce 8\n100 PRINTLN reduce 8\n19 PCT shift 32\n4 LPAREN reduce 6\n5 LPAREN reduce 5\n71 lvalue shift 68\n72 lvalue shift 68\n73 lvalue shift 68\n36 EQ reduce 31\n20 SEMI shift 74\n12 EQ reduce 28\n1 EQ reduce 29\n11 EQ reduce 30\n49 NULL shift 11\n18 NULL shift 11\n6 EQ reduce 33\n90 EQ reduce 32\n27 NULL shift 11\n40 LT reduce 25\n41 LT reduce 26\n42 LT reduce 27\n15 LT reduce 24\n81 COMMA reduce 7\n10 RETURN reduce 11\n15 LE reduce 24\n97 statement shift 66\n44 WAIN shift 75\n59 MINUS reduce 36\n53 MINUS reduce 35\n15 RPAREN reduce 24\n45 MINUS reduce 37\n40 LE reduce 25\n41 LE reduce 26\n42 LE reduce 27\n105 RETURN reduce 10\n40 RPAREN reduce 25\n41 RPAREN reduce 26\n42 RPAREN reduce 27\n7 term shift 19\n16 term shift 19\n24 term shift 19\n21 term shift 19\n37 term shift 19\n17 term shift 19\n74 RETURN reduce 13\n106 RETURN reduce 14\n7 NEW shift 3\n16 NEW shift 3\n24 NEW shift 3\n21 NEW shift 3\n37 NEW shift 3\n17 NEW shift 3\n9 RETURN reduce 12\n66 PRINTLN reduce 9\n53 LT reduce 35\n59 LT reduce 36\n15 PCT reduce 24\n47 test shift 76\n48 test shift 77\n59 LE reduce 36\n45 LE reduce 37\n9 STAR reduce 12\n22 term shift 78\n25 term shift 79\n40 PCT reduce 25\n41 PCT reduce 26\n42 PCT reduce 27\n10 STAR reduce 11\n101 LPAREN reduce 8\n100 LPAREN reduce 8\n88 RBRACK shift 80\n74 STAR reduce 13\n106 STAR reduce 14\n103 LPAREN reduce 8\n45 LT reduce 37\n105 STAR reduce 10\n97 DELETE shift 61\n103 DELETE reduce 8\n63 ID shift 81\n64 MINUS shift 25\n101 DELETE reduce 8\n100 DELETE reduce 8\n47 term shift 19\n48 term shift 19\n97 PRINTLN shift 82\n71 ID shift 53\n72 ID shift 53\n45 NE reduce 37\n25 NUM shift 1\n22 NUM shift 1\n71 IF shift 52\n72 IF shift 52\n59 NE reduce 36\n53 NE reduce 35\n73 ID shift 53\n73 IF shift 52\n30 ID shift 53\n7 LPAREN shift 13\n16 LPAREN shift 13\n24 LPAREN shift 13\n21 LPAREN shift 13\n37 LPAREN shift 13\n17 LPAREN shift 13\n25 STAR shift 14\n22 STAR shift 14\n57 PLUS shift 22\n56 PLUS shift 22\n55 PLUS shift 22\n40 SLASH reduce 25\n41 SLASH reduce 26\n42 SLASH reduce 27\n15 SLASH reduce 24\n33 LBRACK shift 83\n78 PLUS reduce 22\n79 PLUS reduce 23\n97 RETURN shift 84\n55 MINUS shift 25\n57 MINUS shift 25\n56 MINUS shift 25\n19 PLUS reduce 21\n67 DELETE reduce 8\n46 LPAREN shift 13\n54 LPAREN shift 13\n32 LPAREN shift 13\n45 SEMI reduce 37\n59 SEMI reduce 36\n53 SEMI reduce 35\n49 AMP shift 26\n18 AMP shift 26\n27 AMP shift 26\n95 MINUS shift 25\n94 MINUS shift 25\n93 MINUS shift 25\n92 MINUS shift 25\n91 MINUS shift 25\n96 MINUS shift 25\n65 LBRACE shift 85\n71 WHILE shift 38\n72 WHILE shift 38\n4 DELETE reduce 6\n5 DELETE reduce 5\n73 WHILE shift 38\n22 factor shift 15\n25 factor shift 15\n47 NUM shift 1\n48 NUM shift 1\n76 RPAREN shift 86\n77 RPAREN shift 87\n2 NULL shift 11\n26 ID shift 53\n46 NEW shift 3\n54 NEW shift 3\n32 NEW shift 3\n19 SEMI reduce 21\n83 expr shift 88\n13 expr shift 89\n4 IF reduce 6\n5 IF reduce 5\n78 SEMI reduce 22\n79 SEMI reduce 23\n14 STAR shift 14\n26 lvalue shift 90\n4 ID reduce 6\n5 ID reduce 5\n36 BECOMES reduce 31\n25 LPAREN shift 13\n49 STAR shift 14\n18 STAR shift 14\n22 LPAREN shift 13\n6 BECOMES reduce 33\n90 BECOMES reduce 32\n12 BECOMES reduce 28\n1 BECOMES reduce 29\n11 BECOMES reduce 30\n27 STAR shift 14\n29 ID reduce 3\n10 IF reduce 11\n10 ID reduce 11\n9 IF reduce 12\n105 IF reduce 10\n74 IF reduce 13\n106 IF reduce 14\n9 ID reduce 12\n14 NULL shift 11\n80 BECOMES reduce 34\n95 RPAREN reduce 16\n94 RPAREN reduce 17\n93 RPAREN reduce 18\n92 RPAREN reduce 19\n91 RPAREN reduce 20\n96 RPAREN reduce 15\n46 NULL shift 11\n54 NULL shift 11\n32 NULL shift 11\n14 NUM shift 1\n7 expr shift 91\n16 expr shift 92\n24 expr shift 93\n21 expr shift 94\n37 expr shift 95\n17 expr shift 96\n4 RETURN reduce 6\n5 RETURN reduce 5\n67 statements shift 97\n74 ID reduce 13\n106 ID reduce 14\n105 ID reduce 10\n62 COMMA shift 98\n0 BOF shift 99\n19 EQ reduce 21\n78 EQ reduce 22\n79 EQ reduce 23\n45 SLASH reduce 37\n87 LBRACE shift 100\n86 LBRACE shift 101\n50 EOF shift 102\n59 SLASH reduce 36\n84 LPAREN shift 13\n35 LBRACE shift 103\n53 SLASH reduce 35\n61 LBRACK shift 104\n73 PRINTLN shift 82\n57 SEMI shift 105\n85 INT reduce 4\n55 SEMI shift 106\n84 AMP shift 26\n71 PRINTLN shift 82\n72 PRINTLN shift 82\n25 NULL shift 11\n84 factor shift 15\n22 NULL shift 11\n";
//stack infomation
//string wlpp = "6\nBOF\nEOF\nid\n-\n(\n)\n3\nS\nexpr\nterm\nS\n5\nS BOF expr EOF\nexpr term\nexpr expr - term\nterm id\nterm ( expr )\n11\n28\n8 EOF reduce 2\n9 - reduce 4\n7 - shift 1\n1 id shift 2\n6 ( shift 3\n6 term shift 4\n10 EOF shift 5\n2 - reduce 3\n4 ) reduce 1\n3 id shift 2\n4 EOF reduce 1\n2 ) reduce 3\n0 BOF shift 6\n8 - reduce 2\n2 EOF reduce 3\n3 expr shift 7\n9 ) reduce 4\n9 EOF reduce 4\n4 - reduce 1\n1 term shift 8\n3 term shift 4\n3 ( shift 3\n10 - shift 1\n6 id shift 2\n8 ) reduce 2\n1 ( shift 3\n7 ) shift 9\n6 expr shift 10\n";

// store DFA
struct info {
	info (string a,string b, int c){
		id = a;
		op = b;
		next = c;
	}
	string id;
	string op;
	int next;
};
// tree stack
struct Pnode {
	int children;
	int nextState;
	int ruleNum;
	string id;
	string symbol;
	Pnode* next[20];
};
//init
Pnode *make(int children,int nextState,int ruleNum,string id,string symbol){
	Pnode *p = new Pnode;
	p->children = children;
	p->nextState = nextState;
	p->ruleNum = ruleNum;
	p->id = id;
	p->symbol = symbol;
	for (int i = 0 ; i < 20 ; ++i) {
		p->next[i] = NULL;
	}
	return p;
}
string token;
int ruleNumber;
bool anyError = false;
int loc = 0;
string aa;
int back; // determine whether derivation need read twice.
string derivation = "";
set<string> terminal;
set<string> non_terminal;
vector<string> rule;
multimap<int,info> table;


stack<Pnode*> MyStack;
vector<vector<Pnode*> > pointer;
int num_ptr = 0;

vector<Pnode*> reduce (int n){
	vector<Pnode*> ret;
	while (n > 0) {
		ret.push_back(MyStack.top());
		MyStack.pop();
		--n;
	}
	return ret;
}


bool mapSearch(int current, const string &s, multimap<int,info> &table){
	int n = table.count(current);
	if (n == 0) {
		return false;
	}
	multimap<int,info>::iterator it;
	for (it=table.equal_range(current).first ; it != table.equal_range(current).second ; ++it) {
	    info newinfo = (*it).second;
	    if (s == newinfo.id){
           return true;
        }
    }
	return false;
}
bool is_non_T(string &target){
     int i = non_terminal.count(target);
     if (i > 0)return true;
     return false;
}

int processInfo(const int current, const string &s, multimap<int,info> &table){
	multimap<int,info>::iterator it;
	for (it=table.equal_range(current).first ; it != table.equal_range(current).second ; ++it) {
		info newinfo = (*it).second;
		if (s == newinfo.id){
			//Pair newPair(newinfo.id,newinfo.next);
			if (newinfo.op == "shift") {
				Pnode *p = make(0,newinfo.next,0,s,token);
				if(is_non_T(p->id)) p->symbol = rule[ruleNumber];
				MyStack.push(p);
				back = 0;
				//cout <<"push"<<endl;
				return newinfo.next;
			}
			else if (newinfo.op == "reduce") {
				// first get id
				string id = newinfo.id;
				// check which reduce rule gonna be used
				ruleNumber = newinfo.next;
				string rules = rule[ruleNumber];
				//out.push_back(rules); // push to "ostream"
				// process this rule
				istringstream myiss(rules);
				string lhs,rhs;
				myiss >> lhs;   // here lhs mean "left hand side"
				aa = lhs;
				// get # of element of RHS
				int count = 0;
				while (myiss >> rhs) {
					++count;
				}
				// pop from stack
                num_ptr = pointer.size();
				pointer.push_back(reduce(count));
				//cout <<"pop" << endl;
				// push new item to stack
				Pnode *topStack = MyStack.top();
				back = 1;
				return topStack->nextState;
			}
		}
	}
	return 0;
}
int processDFA(const int current, const string &piece ,multimap<int,info> &table){
	// first do search operation
	if (mapSearch(current,piece,table)) {
		return processInfo(current,piece,table);
	}else {
		return -1; // can reach the end, or does not find any transition
	}
}

void printTree(Pnode* &head){
     if (is_non_T(head->id)) cout << head->symbol << endl;
     else cout << head->id << " " << head->symbol << endl;
     for (int i = 0 ; i < head->children ;++i){
	      printTree(head->next[i]);
	 }
}
int main(){
	Pnode* current = NULL;
	try {
	istringstream newiss(wlpp);	
	int terminal_num,non_terminal_num,rule_num,states_num,trans_num;
	string terminal_str,non_terminal_str,rule_str,states_str,trans_str;
	string start,line;
    newiss >> terminal_num;
    getline(newiss,line);
    for (int i = 0 ; i < terminal_num ; ++i) {
	    getline(newiss,terminal_str);
	    terminal.insert(terminal_str);
	}
	
	
	newiss >> non_terminal_num;
	getline(newiss,line);
    for (int i = 0 ; i < non_terminal_num ; ++i) {
	    getline(newiss,non_terminal_str);
	    non_terminal.insert(non_terminal_str);
	}
	getline(newiss,start);
	newiss >> rule_num;
	getline(newiss,line);
    for (int i = 0 ; i < rule_num ; ++i) {
	    getline(newiss,rule_str);
	    rule.push_back(rule_str);
	}
	
	newiss >> states_num;
    newiss >> trans_num;



    // store DFA transition in a multimap
    int header;
    string trans;
    getline(newiss,trans);
    while (trans_num > 0) {
	    newiss >> header;
	    getline(newiss,trans);
	    istringstream iss(trans);
	    string piece1,piece2;
	    int state;
	    iss >> piece1;
	    iss >> piece2;
	    iss >> state;
	    info myinfo(piece1,piece2,state);
	    table.insert(pair<int,info>(header,myinfo));
	    --trans_num;
    }

	// get derivation
	string input,rhs;
	while (cin >> input) {
		cin >> rhs;
		derivation = derivation + " " + input + " " + rhs + " ";
	}
	derivation = "BOF BOF " + derivation;
	derivation += " EOF EOF";
	//cout << derivation << endl;
	istringstream iss(derivation);
	string piece;
	
	int currentState = 0,next;
	while (iss >> piece) {
		iss >> token;
		++loc;
		next = processDFA(currentState,piece,table); // process
		currentState = next; 
		
		while (back) { // we encounter "reduce"
			next = processDFA(currentState,aa,table);
			currentState = next;
			current = MyStack.top();
			for(int i = pointer[num_ptr].size()-1, j = 0 ; i >= 0 ; --i,++j){
			   current->next[j] = pointer[num_ptr][i];
			   ++current->children;
			}
			next = processDFA(currentState,piece,table);
			currentState = next;
		}
		
		if (currentState == -1){
			anyError = true;
			throw string("ERROR at ");
		}
	}
	// no error till now
	Pnode* head = make(3,0,0,"S","S BOF procedure EOF");
	head->next[2] = MyStack.top();
	MyStack.pop();
    head->next[1] = MyStack.top();
	MyStack.pop();
	head->next[0] = MyStack.top();
	MyStack.pop();
	//Print
	//cout << rule[0] << endl;
	printTree(head);
	
	
	delete head->next[0];
	delete head->next[1];
	delete head->next[2];
	delete head;
	}catch(string msg){
		cerr << msg << loc-1 << endl;
	}

	// ====gabarge collector============================
	for (int i = 0 ; i < pointer.size(); ++i){
	    for (int j = 0 ; j < pointer[i].size() ; ++j){
		    delete pointer[i][j];
		}
	}
	if(anyError){
	   int stackSize = MyStack.size();
 	   while (stackSize > 0){
	         current = MyStack.top();
	         delete current;
	         MyStack.pop();
	         stackSize--;
	   }
	}
	//==========================
	return 0;
}













