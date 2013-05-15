#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <map>
using namespace std;

//======================================================================
//========= Declarations for the scan() function =======================
//======================================================================

// Each token has one of the following kinds.

enum Kind {
    ID,                 // Opcode or identifier (use of a label)
    INT,                // Decimal integer
    HEXINT,             // Hexadecimal integer
    REGISTER,           // Register number
    COMMA,              // Comma
    LPAREN,             // (
    RPAREN,             // )
    LABEL,              // Declaration of a label (with a colon)
    DOTWORD,            // .word directive
    WHITESPACE,         // Whitespace
    NUL                 // Bad/invalid token
};

// kindString(k) returns string a representation of kind k
// that is useful for error and debugging messages.
string kindString(Kind k);

// Each token is described by its kind and its lexeme.

struct Token {
    Kind      kind;
    string    lexeme;
    /* toInt() returns an integer representation of the token. For tokens
     * of kind INT (decimal integer constant) and HEXINT (hexadecimal integer
     * constant), returns the integer constant. For tokens of kind
     * REGISTER, returns the register number.
     */
    int       toInt();
};

// scan() separates an input line into a vector of Tokens.
vector<Token> scan(string input);

// =====================================================================
// The implementation of scan() and associated type definitions.
// If you just want to use the scanner, skip to the next ==== separator.

// States for the finite-state automaton that comprises the scanner.

enum State {
    ST_NUL,
    ST_START,
    ST_DOLLAR,
    ST_MINUS,
    ST_REGISTER,
    ST_INT,
    ST_ID,
    ST_LABEL,
    ST_COMMA,
    ST_LPAREN,
    ST_RPAREN,
    ST_ZERO,
    ST_ZEROX,
    ST_HEXINT,
    ST_COMMENT,
    ST_DOT,
    ST_DOTW,
    ST_DOTWO,
    ST_DOTWOR,
    ST_DOTWORD,
    ST_WHITESPACE
};

// The *kind* of token (see previous enum declaration)
// represented by each state; states that don't represent
// a token have stateKinds == NUL.

Kind stateKinds[] = {
    NUL,            // ST_NUL
    NUL,            // ST_START
    NUL,            // ST_DOLLAR
    NUL,            // ST_MINUS
    REGISTER,       // ST_REGISTER
    INT,            // ST_INT
    ID,             // ST_ID
    LABEL,          // ST_LABEL
    COMMA,          // ST_COMMA
    LPAREN,         // ST_LPAREN
    RPAREN,         // ST_RPAREN
    INT,            // ST_ZERO
    NUL,            // ST_ZEROX
    HEXINT,         // ST_HEXINT
    WHITESPACE,     // ST_COMMENT
    NUL,            // ST_DOT
    NUL,            // ST_DOTW
    NUL,            // ST_DOTWO
    NUL,            // ST_DOTWOR
    DOTWORD,        // ST_DOTWORD
    WHITESPACE      // ST_WHITESPACE
};

State delta[ST_WHITESPACE+1][256];

#define whitespace "\t\n\r "
#define letters    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define digits     "0123456789"
#define hexDigits  "0123456789ABCDEFabcdef"
#define oneToNine  "123456789"

void setT(State from, string chars, State to) {
    for(int i = 0; i < chars.length(); i++ ) delta[from][chars[i]] = to;
}

void initT(){
    int i, j;

    // The default transition is ST_NUL (i.e., no transition
    // defined for this char).
    for ( i=0; i<=ST_WHITESPACE; i++ ) {
        for ( j=0; j<256; j++ ) {
            delta[i][j] = ST_NUL;
        }
    }
    // Non-null transitions of the finite state machine.
    // NB: in the third line below, letters digits are macros
    // that are replaced by string literals, which the compiler
    // will concatenate into a single string literal.
    setT( ST_START,      whitespace,     ST_WHITESPACE );
    setT( ST_WHITESPACE, whitespace,     ST_WHITESPACE );
    setT( ST_START,      letters,        ST_ID         );
    setT( ST_ID,         letters digits, ST_ID         );
    setT( ST_START,      oneToNine,      ST_INT        );
    setT( ST_INT,        digits,         ST_INT        );
    setT( ST_START,      "-",            ST_MINUS      );
    setT( ST_MINUS,      oneToNine,      ST_INT        );
    setT( ST_START,      ",",            ST_COMMA      );
    setT( ST_START,      "(",            ST_LPAREN     );
    setT( ST_START,      ")",            ST_RPAREN     );
    setT( ST_START,      "$",            ST_DOLLAR     );
    setT( ST_DOLLAR,     digits,         ST_REGISTER   );
    setT( ST_REGISTER,   digits,         ST_REGISTER   );
    setT( ST_START,      "0",            ST_ZERO       );
    setT( ST_ZERO,       "x",            ST_ZEROX      );
    setT( ST_ZERO,       oneToNine,      ST_INT        );
    setT( ST_ZEROX,      hexDigits,      ST_HEXINT     );
    setT( ST_HEXINT,     hexDigits,      ST_HEXINT     );
    setT( ST_ID,         ":",            ST_LABEL      );
    setT( ST_START,      ";",            ST_COMMENT    );
    setT( ST_START,      ".",            ST_DOT        );
    setT( ST_DOT,        "w",            ST_DOTW       );
    setT( ST_DOTW,       "o",            ST_DOTWO      );
    setT( ST_DOTWO,      "r",            ST_DOTWOR     );
    setT( ST_DOTWOR,     "d",            ST_DOTWORD    );

    for ( j=0; j<256; j++ ) delta[ST_COMMENT][j] = ST_COMMENT;
}

static int initT_done = 0;

vector<Token> scan(string input){
    // Initialize the transition table when called for the first time.
    if(!initT_done) {
        initT();
        initT_done = 1;
    }

    vector<Token> ret;

    int i = 0;
    int startIndex = 0;
    State state = ST_START;

    if(input.length() > 0) {
        while(true) {
            State nextState = ST_NUL;
            if(i < input.length())
                nextState = delta[state][(unsigned char) input[i]];
            if(nextState == ST_NUL) {
                // no more transitions possible
                if(stateKinds[state] == NUL) {
                    throw("ERROR in lexing after reading " + input.substr(0, i));
                }
                if(stateKinds[state] != WHITESPACE) {
                    Token token;
                    token.kind = stateKinds[state];
                    token.lexeme = input.substr(startIndex, i-startIndex);
                    ret.push_back(token);
                }
                startIndex = i;
                state = ST_START;
                if(i >= input.length()) break;
            } else {
                state = nextState;
                i++;
            }
        }
    }

    return ret;
}

int Token::toInt() {
    if(kind == INT) {
        if(lexeme.length() > 11) throw("ERROR: constant out of range: "+lexeme);
        long long l;
        sscanf( lexeme.c_str(), "%lld", &l );
        if(l < -2147483647-1 || l > 4294967295LL)
            throw("ERROR: constant out of range: "+lexeme);
        return l;
    } else if(kind == HEXINT) {
        if(lexeme.length() > 10) throw("ERROR: constant out of range: "+lexeme);
        long long l;
        sscanf( lexeme.c_str(), "%llx", &l );
        return l;
    } else if(kind == REGISTER) {
        if(lexeme.length() > 3) throw("ERROR: constant out of range: "+lexeme);
        long long l;
        sscanf( lexeme.c_str()+1, "%lld", &l );
        if(l > 31)
            throw("ERROR: constant out of range: "+lexeme);
        return l;
    }
    throw("ERROR: attempt to convert non-integer token "+lexeme+" to Int");
}

// kindString maps each kind to a string for use in error messages.

string kS[] = {
    "ID",           // ID
    "INT",          // INT
    "HEXINT",       // HEXINT
    "REGISTER",     // REGISTER
    "COMMA",        // COMMA
    "LPAREN",       // LPAREN
    "RPAREN",       // RPAREN
    "LABEL",        // LABEL
    "DOTWORD",      // DOTWORD
    "WHITESPACE",   // WHITESPACE
    "NUL"           // NUL
};

string kindString( Kind k ){
    if ( k < ID || k > NUL ) return "INVALID";
    return kS[k];
}


// ===========operator overload===================
string operator*(int number,string &s){
       string output;
       while (number > 0){
	         output += s;
	         number--;
	   }
	   return output;
}
string operator*(string &s,int number){
       return number * s;
}

//===================================================

void ProcessHex(Token &token, vector<Token> *out){
    int leftbound = 2;
	Token mytoken;
	mytoken.kind = stateKinds[13];
	string input = token.lexeme;
	int inputSize = input.size();
	input = input.substr(leftbound,inputSize);
	
	if (input.size() > 8){ //error checking
	   throw("ERROR: out of range");
	} // end if
	
	
	if (input.size() < 8){
	   inputSize = input.size();
	   int insufficient = 8 - inputSize;
	   string zero = "0";
	   input = zero * insufficient + input;
    }
    
    
	while (input != ""){
		    inputSize = input.size();
		    mytoken.lexeme  = input.substr(0,leftbound);
			input = input.substr(leftbound,inputSize);
			(*out).push_back(mytoken);
			// process
				         
			//decimal = mytoken.toInt();
			//output = static_cast<char>(decimal);
			//(*out).push_back(output);
				         
   }
}
string ProcessDecimal(Token &token){
		long long input = token.toInt(); // error checking(boundary)
        stringstream ss;
        ss << hex << input;
        string result = ss.str();
        int resultSize = result.size();
        if (resultSize > 8){
            result = result.substr(resultSize-8,resultSize);
        }
        result = "0x" + result;
        return result;
}

void ProcessID(Token &token){
    int leftbound = 2;
	Token mytoken;
	mytoken.kind = stateKinds[13];
	string input = token.lexeme;
	int inputSize = input.size();
	input = input.substr(leftbound,inputSize);
	
	
	
	if (input.size() < 8){
	   inputSize = input.size();
	   int insufficient = 8 - inputSize;
	   string zero = "0";
	   input = zero * insufficient + input;
    } 
    while (input != ""){
		    inputSize = input.size();
		    mytoken.lexeme  = input.substr(0,leftbound);
			input = input.substr(leftbound,inputSize);		         
			int decimal = mytoken.toInt();
			char output = static_cast<char>(decimal);
			cout << output;
				         
   }
}
void ProcessInstruction(const int word, vector<Token> *out){
	vector<int> bits;
	bits.push_back(((word >> 24) & 0xff));
	bits.push_back(((word >> 16) & 0xff));
	bits.push_back(((word >> 8) & 0xff));
	bits.push_back((word & 0xff));
	// merge each part
	string result = "";
	for (int i = 0 ; i < bits.size(); ++i){
	    stringstream ss;
	    ss << hex << bits[i];
	    string in = ss.str();
	    // if insufficient bits, fill up by 0;
	    if (in.size() < 2){
		   in = "0" + in;
		}
	    result += in;
	}
    result = "0x" + result;
    Token instruction;
    instruction.lexeme = result;
    ProcessHex(instruction,out);
}
void ProcessJR(const int reg , vector<Token> *out){
	 int word = (reg << 21) | (1 << 3);
     ProcessInstruction(word,out);
}
void ProcessJalr(const int reg, vector<Token> *out){
	 int word = (reg << 21) | (1 << 3) | (1 << 0);
     ProcessInstruction(word,out);
}
void ProcessMflo(const int reg, vector<Token> *out){
     int word = (reg << 11) | (1 << 4) | (1 << 1);
     ProcessInstruction(word,out);
}
void ProcessMfhi(const int reg, vector<Token> *out){
     int word = (reg << 11) | (1 << 4);
     ProcessInstruction(word,out);
}
void ProcessLis(const int reg, vector<Token> *out){
     int word = (reg << 11) | (1 << 4) | (1 << 2);
     ProcessInstruction(word,out);
}
void ProcessMult(const int reg1,const int reg2, vector<Token> *out){
     int word = (reg1 << 21) | (reg2 << 16) | (3 << 3);
     ProcessInstruction(word,out);
}
void ProcessMultu(const int reg1,const int reg2, vector<Token> *out){
     int word = (reg1 << 21) | (reg2 << 16) | (3 << 3) | (1 << 0);
     ProcessInstruction(word,out);
}
void ProcessDiv(const int reg1,const int reg2, vector<Token> *out){
     int word = (reg1 << 21) | (reg2 << 16) | (1 << 4) | (1 << 3) | (1 << 1);
     ProcessInstruction(word,out);
}
void ProcessDivu(const int reg1,const int reg2, vector<Token> *out){
     int word = (reg1 << 21) | (reg2 << 16) | (1 << 4) | (1 << 3) | (1 << 1) | (1 << 0);
     ProcessInstruction(word,out);
}
void ProcessBeq(const int reg1,const int reg2,const int offset,vector<Token> *out){
     int word = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (offset & 0xffff);
     ProcessInstruction(word,out);
}
void ProcessBne(const int reg1, const int reg2 ,const int offset,vector<Token> *out){
     int word = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (offset & 0xffff);
     ProcessInstruction(word,out);
}
void ProcessBneLabel(const int reg1, const int reg2, const int offset){
    int word = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (offset & 0xffff);
    vector<int> bits;
	bits.push_back(((word >> 24) & 0xff));
	bits.push_back(((word >> 16) & 0xff));
	bits.push_back(((word >> 8) & 0xff));
	bits.push_back((word & 0xff));
	// merge each part
	string result = "";
	for (int i = 0 ; i < bits.size(); ++i){
	    stringstream ss;
	    ss << hex << bits[i];
	    string in = ss.str();
	    // if insufficient bits, fill up by 0;
	    if (in.size() < 2){
		   in = "0" + in;
		}
	    result += in;
	}
    result = "0x" + result;
    Token instruction;
    instruction.lexeme = result;
    ProcessID(instruction);
}
void ProcessBeqLabel(const int reg1, const int reg2, const int offset){
    int word = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (offset & 0xffff);
    vector<int> bits;
	bits.push_back(((word >> 24) & 0xff));
	bits.push_back(((word >> 16) & 0xff));
	bits.push_back(((word >> 8) & 0xff));
	bits.push_back((word & 0xff));
	// merge each part
	string result = "";
	for (int i = 0 ; i < bits.size(); ++i){
	    stringstream ss;
	    ss << hex << bits[i];
	    string in = ss.str();
	    // if insufficient bits, fill up by 0;
	    if (in.size() < 2){
		   in = "0" + in;
		}
	    result += in;
	}
    result = "0x" + result;
    Token instruction;
    instruction.lexeme = result;
    ProcessID(instruction);
}
void ProcessAdd (const int d,const int s, const int t,vector<Token> *out){
     int word = (d << 11) | (s << 21) | (t << 16) | (2 << 4);
     ProcessInstruction(word,out);
}
void ProcessSub (const int d,const int s, const int t,vector<Token> *out){
     int word = (d << 11) | (s << 21) | (t << 16) | (2 << 4) | (1 << 1);
     ProcessInstruction(word,out);
}
void ProcessSlt (const int d,const int s, const int t,vector<Token> *out){
     int word = (d << 11) | (s << 21) | (t << 16) | (2 << 4) | (2 << 2) | (1 << 1);
     ProcessInstruction(word,out);
}
void ProcessSltu (const int d,const int s, const int t,vector<Token> *out){
     int word = (d << 11) | (s << 21) | (t << 16) | (2 << 4) | (2 << 2) | (3 << 0);
     ProcessInstruction(word,out);
}
void ProcessLw(const int t, const int offset, const int s,vector<Token> *out){
     int word = (8 << 28) | (3 << 26) | (s << 21) | (t << 16) | (offset &0xffff);
     ProcessInstruction(word,out);
}
void ProcessSw(const int t, const int offset, const int s,vector<Token> *out){
     int word = (10 << 28) | (3 << 26) | (s << 21) | (t << 16) | (offset &0xffff);
     ProcessInstruction(word,out);
}


int main() {
    string errorMsg("");
    stringstream ss;
    vector<Token> *out = new vector<Token>; // output stream
    try {
        vector<string> srcLines;
        // used to debug
        vector<string> errorLabel;
        vector<int> errorLine;
        // used to store label and their value
        map<string,int> table;
        map<string,int>::iterator it;
        int PCvalue = -4;
        // store reg
        vector<int> register1;
        vector<int> registerOffset;
        vector<int> CurrentPC;
        int reg1 = 0,reg2 = 0,reg3 = 0,offset = 0;
        
        // Read the entire input file, storing each line as a
        // single string in the array srcLines.
        while(true) {
            string line;
            getline(cin, line);
            if(cin.fail()) break;
            srcLines.push_back(line);
        }

        // Tokenize each line, storing the results in tokLines.
        vector<vector<Token> > tokLines;

        for(int line = 0; line < srcLines.size(); line++) {
            tokLines.push_back(scan(srcLines[line]));
        }
        
            string label; // instruction 
            bool ImDotWord = false;
            bool ImJr = false;
            bool ImJalr = false;
            bool ImAdd = false;
            bool ImSub = false;
            bool ImSlt = false;
            bool ImSltu = false;
            bool ImBeq = false;
            bool ImBne = false;
            bool ImLis = false;
            bool ImMflo = false;
            bool ImMfhi = false;
            bool ImMult = false;
            bool ImMultu = false;
            bool ImDiv = false;
            bool ImDivu = false;
            bool ImSw = false;
            bool ImLw = false;
        // Now we process the tokens.
        // Sample usage: print the tokens of each line.
        for(int line=0; line < tokLines.size(); line++ ) {
			int size = tokLines[line].size(); // number of elements scanned by scaner per line
			// increment PC by 4
			if (size != 0){ // check empty line
			   PCvalue += 4;
			}
            for(int i = 0,j=0; j < size; j++,i++ ) {
                Token token = tokLines[line][i];
                label = kindString(token.kind);
	
                	
                if (j == 0){
					//=============================================
					//======Let me eat these fucking labels========
					//=============================================
					if (label == "LABEL"){
				       string processlabel = token.lexeme;
				       processlabel = processlabel.substr(0,processlabel.size()-1);
				       it = table.find(processlabel);
				       
				       if (it != table.end()){ // check whether label has multiple definition.
				           errorMsg = "ERROR: multiple definition for label: " + processlabel;
					       throw(errorMsg);
				      }else{ // else do insert operation
				          table[processlabel] = PCvalue;
				      }
				      
				      size--; // size: "wow im slim now"
				      j = -1;
				      if (size == 0){ // why you put fucking labels in a whole line?
				         PCvalue -= 4;
				      }
				      continue;
					}
					//=====================================================
					//==I gonna tell those booleans who the hell they are==
					//=====================================================
					if (label == "ID"){
				       if (token.lexeme == "jr") ImJr = true;
				       else if (token.lexeme == "jalr") ImJalr = true;
				       else if (token.lexeme == "add") ImAdd = true;
				       else if (token.lexeme == "sub") ImSub = true;
				       else if (token.lexeme == "slt") ImSlt = true;
				       else if (token.lexeme == "sltu") ImSltu = true;
				       else if (token.lexeme == "beq") ImBeq = true;
				       else if (token.lexeme == "bne") ImBne = true;
				       else if (token.lexeme == "lis") ImLis = true;
				       else if (token.lexeme == "mflo") ImMflo = true;
				       else if (token.lexeme == "mfhi") ImMfhi = true;
				       else if (token.lexeme == "mult") ImMult = true;
				       else if (token.lexeme == "multu") ImMultu = true;
				       else if (token.lexeme == "div") ImDiv = true;
				       else if (token.lexeme == "divu") ImDivu = true;
				       else if (token.lexeme == "sw") ImSw = true;
				       else if (token.lexeme == "lw") ImLw = true;
				       else{
						   errorMsg = "ERROR: unrecognizable instruction in line ";
						   ss << errorMsg << (line+1);
						   throw(ss.str());
					   }
				   }else if (label == "DOTWORD"){
				       ImDotWord = true;
				   }else{
					   errorMsg = "ERROR: undefined instruction in line ";
					   ss << errorMsg << (line+1);
					   throw(ss.str());    
				   }
				}
				//===============================================================
			    //================I just can check length T_T====================
			    //===============================================================
                if (size != 2 && (ImJr || ImJalr || ImDotWord || ImLis || ImMflo || ImMfhi)){
					errorMsg = "ERROR: too many/little arguments in line ";
					ss << errorMsg << (line+1);
					throw(ss.str());
				}
				if (size != 6 && (ImAdd || ImSub || ImSlt || ImSltu || ImBne || ImBeq)){
					errorMsg = "ERROR: too many/little arguments in line ";
					ss << errorMsg << (line+1);
					throw(ss.str());
				} 
				if (size != 4 && (ImMult || ImMultu || ImDiv || ImDivu)){
					errorMsg = "ERROR: too many/little arguments in line ";
					ss << errorMsg << (line+1);
					throw(ss.str());
				}
				if (size != 7 && (ImSw || ImLw)){
					errorMsg = "ERROR: too many/little arguments in line ";
					ss << errorMsg << (line+1);
					throw(ss.str());
				}
				
				//===============================================================
				//==========I can handle instructions with length 2 ^.^==========
				//===============================================================
				if (j == 1 && size == 2){
					if (ImDotWord){
					    if (label == "INT"){//.word 123
				           string result = ProcessDecimal(token);
				           token.lexeme = result;
				           ProcessHex(token,out);
				           ImDotWord = false;
				        }else if (label == "HEXINT"){ // .word 0x
					       ProcessHex(token,out);
					       ImDotWord = false;
					    }else if (label == "ID"){// .word "some label"
                           errorLabel.push_back(token.lexeme);
                           errorLine.push_back(line);
                           Token id;
                           id.kind = stateKinds[6];
                           id.lexeme = token.lexeme ;
                           (*out).push_back(id);
                           ImDotWord = false;
                        }else{
						   // must be error
						   errorMsg = "ERROR: invalid content after .word in line ";
					       ss << errorMsg << (line+1);
					       throw(ss.str());
						}
					}else if (label == "REGISTER"){ 
				       if (ImJr){ // hi im fucking jr
				          ProcessJR(token.toInt(),out);
				          ImJr = false;
				       }
				       else if(ImJalr){ // hi im fucking jalr
					      ProcessJalr(token.toInt(),out);
					      ImJalr = false;
				       }
				       else if (ImLis){ // hi im fucking lis
					      ProcessLis(token.toInt(),out);
					      ImLis = false;
					   }
					   else if (ImMflo){ // hi im fucking mflo
					      ProcessMflo(token.toInt(),out);
					      ImMflo = false;
					   }
					   else if (ImMfhi){ // hi im fcking mfhi
					      ProcessMfhi(token.toInt(),out);
					      ImMfhi = false;
					   }
					   else{ // hi im alien.
					      throw("fuck");
					   }
					}else{
					   // must be error
					   errorMsg = "ERROR: expect register in line ";
					   ss << errorMsg << (line+1) << " but given something else";
					   throw(ss.str());
			       }
			   }
			   
			   //===================================================================
			   //========You want me a Logic Specialist?!!=================
			   //=================================================================== 
			       // so your length only can be 4,6,7
			     if (size > 2){
                   if (j == 1){
					  if (label == "REGISTER"){
					     int reg1 = token.toInt();
					     registerOffset.push_back(reg1);
					     if(ImBne || ImBeq) register1.push_back(reg1);
				      }else{
						  errorMsg = "ERROR: expect register, given else in line ";
						  ss << errorMsg << (line+1);
					      throw(ss.str());
				      }
				   }
				   
				   
				   else if (j == 2){
					   if (label != "COMMA"){
						   errorMsg = "ERROR: expect comma, given else in line ";
					       ss << errorMsg << (line+1);
					       throw(ss.str());
				       }
				   }
				   
				   
				   //ATTEN: lw ,sw are INT, others are REGISTER
				   else if (j == 3){
					     reg2 = token.toInt();
				         registerOffset.push_back(reg2);
					     if (label == "REGISTER"){
				            if (ImBne || ImBeq) register1.push_back(reg2);
				            if (size == 4){
					           reg1 = registerOffset[registerOffset.size()-2];
					           if (ImMult){
						          ProcessMult(reg1,reg2,out);
						          ImMult = false;
						       }
						       else if (ImMultu){
						         ProcessMultu(reg1,reg2,out);
						         ImMultu = false;
						       }
						       else if (ImDiv){
						         ProcessDiv(reg1,reg2,out);
						         ImDiv = false;
						       }
						       else if (ImDivu){
						         ProcessDivu(reg1,reg2,out);
						         ImDivu = false;
						       }
						       else{
								   errorMsg = "ERROR : syntax error in line ";
					               ss << errorMsg << (line+1);
					               throw(ss.str());
						       }
					       }else{
							   continue;
							   //errorMsg = "ERROR : syntax error in line ";
					           //ss << errorMsg << (line+1);
					           //throw(ss.str());
						   }
					     }
				       
				       else if((label == "INT" || label == "HEXINT") && (ImLw || ImSw)){
                              continue;
				      }else{
						  errorMsg = "ERROR: expect register, given else in line ";
					      ss << errorMsg << (line+1);
					      throw(ss.str());
				      }
				   }
				   
				   
				   
				   //ATTEN: lw,sw are LPAREN, others are COMMA
				   else if (j == 4){
				      if (label != "COMMA" && label != "LPAREN"){
						  errorMsg = "ERROR: expect LPAREN/COMMA, given else in line "; 
					      ss << errorMsg << (line+1);
					      throw(ss.str());
					  }
			       }
				   else if (j == 5){
					   // ATTEN : beq and bne are either hex or int or label; others are REGISTER
					  reg1 = registerOffset[registerOffset.size()-2];
				      reg2 = registerOffset[registerOffset.size()-1];
				      if ((label == "HEXINT" || label == "INT" || label == "ID") && (ImBne || ImBeq)){
					      if (label == "HEXINT" || label == "INT"){
							  if (ImBeq) {ProcessBeq(reg1,reg2,token.toInt(),out); ImBeq = false;}
							  else if (ImBne) {ProcessBne(reg1,reg2,token.toInt(),out); ImBne = false;}
						  }
						  else{ // label == "ID"
						    // tell guard guy a label is gonna parse
						    // again we dont know whether this label has been defined
						    if (ImBne){
								CurrentPC.push_back(PCvalue+4);
								errorLabel.push_back(token.lexeme);
                                errorLine.push_back(line);
							    Token id;
							    id.lexeme = token.lexeme;
							    id.kind = stateKinds[9];
							    (*out).push_back(id);
							    ImBne = false;
							}
						    else if (ImBeq){
								CurrentPC.push_back(PCvalue+4);
							    errorLabel.push_back(token.lexeme);
                                errorLine.push_back(line);
						        Token id;
							    id.lexeme = token.lexeme;
							    id.kind = stateKinds[10];
							    (*out).push_back(id);
							    ImBeq = false;
						    }
						  }
					  }
					  else if (label == "REGISTER" && (ImLw || ImSw || ImAdd || ImSub || ImSlt || ImSltu)){
						  reg3 = token.toInt();
						  registerOffset.push_back(reg3);
					      if (size == 6){
						     if (ImAdd) {ProcessAdd(reg1,reg2,reg3,out);ImAdd = false;}
						     else if (ImSub) {ProcessSub(reg1,reg2,reg3,out); ImSub = false;}
						     else if (ImSlt) {ProcessSlt(reg1,reg2,reg3,out); ImSlt = false;}
						     else if (ImSltu) {ProcessSltu(reg1,reg2,reg3,out); ImSltu = false;}
						     else {
							    errorMsg = "ERROR : syntax error in line ";
					            ss << errorMsg << (line+1);
					            throw(ss.str());
							 }
						  }
					  }
					  else {
						 errorMsg = "ERROR : syntax error in line ";
					     ss << errorMsg << (line+1);
					     throw(ss.str());
					  }
			   }
			       else if (j == 6){
					  
					    if (label == "RPAREN" && (ImLw || ImSw)){
							reg1 = registerOffset[registerOffset.size()-3];
							reg3 = registerOffset[registerOffset.size()-1];
							offset = registerOffset[registerOffset.size()-2];
							if(ImLw) {ProcessLw(reg1,offset,reg3,out);ImLw = false;}
							else {ProcessSw(reg1,offset,reg3,out); ImSw = false;}
						}else{
							errorMsg = "ERROR: mexpect right paren in line ";
					        ss << errorMsg << (line+1);
					        throw(ss.str());
						}
				   }
		      }
			   
             }
		 }
        // ======================================================
        // ======Why let me check undefined label? T_T===========
        //=======================================================
	    for (int j = 0; j < errorLine.size(); ++j){
			it = table.find(errorLabel[j]);
			if (it == table.end()){
			    ss << "ERROR: in line " << (errorLine[j]+1) << " label " << errorLabel[j] << " is not defined";
			    throw(ss.str());
		    }
	    }
	    //=========================================================
	    //==========Time to print out something @.@====================
	    //=========================================================
        // print binary data
        if (errorLine.size() == 0){
           for (int i = 0; i < (*out).size(); ++i){
			   int decimal = (*out)[i].toInt();
			   char output = static_cast<char>(decimal);
	           cout << output;
		   }
	    }else{
		   int j = 0,m = 0;
           for (int i = 0 ; i < (*out).size(); ++i){
			   string outputLabel = kindString((*out)[i].kind);
		       if (outputLabel == "ID"){
			       // process operand
			       it = table.find((*out)[i].lexeme);
			       int pc = it->second;
			       stringstream ss;
			       ss << pc;
			       Token id;
			       id.lexeme = ss.str();
			       id.kind = stateKinds[5];
			       string result = ProcessDecimal(id);
			       id.lexeme = result;
			       ProcessID(id);
			   }else if (outputLabel == "LPAREN"){ // bne id
		           it = table.find((*out)[i].lexeme);
		           int offset = it->second;
		           offset = (offset - CurrentPC[m])/4;
		           reg1 = register1[j];
		           reg2 = register1[j+1];
		           ProcessBneLabel(reg1,reg2,offset);
		           j += 2;
		           m++;
		       }else if (outputLabel == "RPAREN"){ // beq id
		           it = table.find((*out)[i].lexeme);
		           int offset = it->second;
		           offset = (offset - CurrentPC[m])/4;
		           reg1 = register1[j];
		           reg2 = register1[j+1];
		           ProcessBeqLabel(reg1,reg2,offset);
		           j += 2;
		           m++;
			   }else{
			      int decimal = (*out)[i].toInt();
			      char output = static_cast<char>(decimal);
	              cout << output;			      
			   }
		}
      }
		
	    
    } catch(string msg) {
        cerr << msg << endl;
    }
    delete out;
    return 0;
}


















