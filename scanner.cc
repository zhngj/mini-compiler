#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

enum Kind {
    ID,                 // string
    NUM,                // Decimal integer
    ZERO,               // INT 0
    LPAREN,             // (
    RPAREN,             // )
    LBRACE,             // {
    RBRACE,             // }
    RETURN,             // "return"
    IF,                 // "if"
    ELSE,               // "else"
    WHILE,              // while
    PRINTLN,            // println
    WAIN,               // wain
    BECOMES,            // =
    INT,                // "int"
    EQ,                 // ==
    NE,                 // !=
    LT,                 // <
    GT,                 // >
    LE,                 // <=
    GE,                 // >=
    PLUS,               // +
    MINUS,              // -
    STAR,               // *
    SLASH,              // /
    PCT,                // %
    COMMA,              // ,
    SEMI,               // ;
    NEW,                // "new"
    DELETE,             // "delete"
    LBRACK,             // [
    RBRACK,             // ]
    AMP,                // &
    NU,                 // "NULL"
    WHITESPACE,         // TAB SPACE NEWLINE COMMENT
    MYNUL               // Bad/invalid token
};

string kindString(Kind k);


struct Token {
       Kind kind;
       string lexeme;
};

vector<Token> scan(string input);

enum State {
    ST_MYNUL,
    ST_START,
    ST_ZERO,
    ST_SURPRISE,
    ST_I,
    ST_IN,
    ST_INT,
    ST_BN,
    ST_BNU,
    ST_BNUL,
    ST_BNULL,
    ST_W,
    ST_WA,
    ST_WAI,
    ST_WAIN,
    ST_P,
    ST_PR,
    ST_PRI,
    ST_PRIN,
    ST_PRINT,
    ST_PRINTL,
    ST_PRINTLN,
    ST_IF,
    ST_E,
    ST_EL,
    ST_ELS,
    ST_ELSE,
    ST_WH,
    ST_WHI,
    ST_WHIL,
    ST_WHILE,
    ST_N,
    ST_NE,
    ST_NEW,
    ST_D,
    ST_DE,
    ST_DEL,
    ST_DELE,
    ST_DELET,
    ST_DELETE,
    ST_ID,
    ST_NUM,
    ST_LPAREN,
    ST_RPAREN,
    ST_LBRACE,
    ST_RBRACE,
    ST_R,
    ST_RE,
    ST_RET,
    ST_RETU,
    ST_RETUR,
    ST_RETURN,
    ST_BECOMES,
    ST_EQ,
    ST_LT,
    ST_GT,
    ST_LE,
    ST_GE,
    ST_PLUS,
    ST_MINUS,
    ST_STAR,
    ST_SLASH,
    ST_PCT,
    ST_COMMA,
    ST_SEMI,
    ST_LBRACK,
    ST_RBRACK,
    ST_AMP,
    ST_WHITESPACE,
    ST_COMMENT
};

Kind stateKinds[] = {
    MYNUL,             // ST_MYNUL
    MYNUL,             // ST_START   
    ZERO,              // ST_ZERO
    MYNUL,             // ST_SURPRISE
    ID,                // ST_I
    ID,                // ST_IN
    INT,               // ST_INT
    ID,                // ST_BN
    ID,                // ST_BNU
    ID,                // ST_BNUL
    NU,                // ST_BNULL
    ID,                // ST_W
    ID,                // ST_WA
    ID,                // ST_WAI
    WAIN,              // ST_WAIN
    ID,                // ST_P
    ID,                // ST_PR
    ID,                // ST_PRI
    ID,                // ST_PRIN
    ID,                // ST_PRINT
    ID,                // ST_PRINTL
    PRINTLN,           // ST_PRINTLN
    IF,                // ST_IF
    ID,                // ST_E
    ID,                // ST_EL
    ID,                // ST_ELS
    ELSE,              // ST_ELSE
    ID,                // ST_WH
    ID,                // ST_WHI
    ID,                // ST_WHIL
    WHILE,             // ST_WHILE
    ID,                // ST_N
    NE,                // ST_NE
    NEW,               // ST_NEW
    ID,                // ST_D
    ID,                // ST_DE
    ID,                // ST_DEL
    ID,                // ST_DELE
    ID,                // ST_DELET
    DELETE,            // ST_DELETE
    ID,                // ST_ID
    NUM,               // ST_NUM
    LPAREN,            // ST_LPAREN
    RPAREN,            // ST_RPAREN
    LBRACE,            // ST_LBRACE
    RBRACE,            // ST_RBRACE
    ID,                // ST_R
    ID,                // ST_RE
    ID,                // ST_RET
    ID,                // ST_RETU
    ID,                // ST_RETUR
    RETURN,            // ST_RETURN
    BECOMES,           // ST_BECOMES
    EQ,                // ST_EQ
    LT,                // ST_LT
    GT,                // ST_GT
    LE,                // ST_LE
    GE,                // ST_GE
    PLUS,              // ST_PLUS
    MINUS,             // ST_MINUS
    STAR,              // ST_STAR
    SLASH,             // ST_SLASH
    PCT,               // ST_PCT
    COMMA,             // ST_COMMA
    SEMI,              // ST_SEMI
    LBRACK,            // ST_LBRACK
    RBRACK,            // ST_RBRACK
    AMP,               // ST_AMP
    WHITESPACE,         // ST_SPACE
    WHITESPACE         // ST_COMMENT
};

State delta[ST_COMMENT+1][256];
#define whitespace "\t\n\r "
#define letters    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define digits     "0123456789"
#define oneToNine  "123456789"
#define zero       "0"
#define noi        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjklmnopqrstuvwxyz"
#define non        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmopqrstuvwxyz"
#define noT        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrsuvwxyz"
#define nof        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdeghijklmnopqrstuvwxyz"
#define noe        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz"
#define nol        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
#define nos        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrtuvwxyz"
#define now        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvxyz"
#define noh        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefgijklmnopqrstuvwxyz"
#define nop        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnoqrstuvwxyz"
#define nor        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqstuvwxyz"
#define noa        "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefghijklmnopqrstuvwxyz"
#define noe        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz"
#define nod        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcefghijklmnopqrstuvwxyz"
#define noN        "ABCDEFGHIJKLMOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define noU        "ABCDEFGHIJKLMNOPQRSTVWXYZabcdefghijklmnopqrstuvwxyz"
#define noL        "ABCDEFGHIJKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define nou        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstvwxyz"
#define noriewpndN "ABCDEFGHIJKLMOPQRSTUVWXYZabcfghjklmoqstuvxyz"
#define noha       "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefgijklmnopqrstuvwxyz"
#define nofn       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdeghijklmopqrstuvwxyz"

void setT(State from, string chars, State to) {
    for(int i = 0; i < chars.length(); i++ ) delta[from][chars[i]] = to;
}

void initT(){
    int i, j;

    // The default transition is ST_NUL (i.e., no transition
    // defined for this char).
    for ( i=0; i<=ST_COMMENT; i++ ) {
        for ( j=0; j<256; j++ ) {
            delta[i][j] = ST_MYNUL;
        }
    }
    // Non-null transitions of the finite state machine.
    // NB: in the third line below, letters digits are macros
    // that are replaced by string literals, which the compiler
    // will concatenate into a single string literal.
    setT( ST_START,      whitespace,     ST_WHITESPACE );
    setT( ST_WHITESPACE, whitespace,     ST_WHITESPACE );
    //===================STRING=========================
    setT( ST_START,      noriewpndN,     ST_ID         );
    setT( ST_ID,         letters digits, ST_ID         );
    //=============SPECIAL TOKEN========================
    setT( ST_START,      "r",          ST_R            );
    setT( ST_R,          "e",          ST_RE           );
    setT( ST_R,          noe,          ST_ID           );
    setT( ST_RE,         "t",          ST_RET          );
    setT( ST_RE,         noT,          ST_ID           );
    setT( ST_RET,        nou,          ST_ID           );
    setT( ST_RET,        "u",          ST_RETU         );
    setT( ST_RETU,       nor,          ST_ID           );
    setT( ST_RETU,       "r",          ST_RETUR        );
    setT( ST_RETUR,      non,          ST_ID           );
    setT( ST_RETUR,      "n",          ST_RETURN       ); 
    setT( ST_RETURN,    letters digits,ST_ID           );// done return
    setT( ST_START,      "i",          ST_I            );
    setT( ST_I,          "f",          ST_IF           );
    setT( ST_IF,        letters digits,ST_ID           );
    setT( ST_I,          "n",          ST_IN           );
    setT( ST_I,          nofn,         ST_ID           );// done if
    setT( ST_START,      "e",          ST_E            );
    setT( ST_E,          "l",          ST_EL           );
    setT( ST_E,          nol,          ST_ID           );
    setT( ST_EL,         "s",          ST_ELS          );
    setT( ST_EL,         nos,          ST_ID           );
    setT( ST_ELS,        "e",          ST_ELSE         );
    setT( ST_ELSE,      letters digits,ST_ID           );
    setT( ST_ELS,        noe,          ST_ID           ); // done else
    setT( ST_START,      "w",          ST_W            );
    setT( ST_W,          "h",          ST_WH           );
    setT( ST_W,          "a",          ST_WA           );
    setT( ST_W,          noha,         ST_ID           );
    setT( ST_WH,         "i",          ST_WHI          );
    setT( ST_WH,         noi,          ST_ID           );
    setT( ST_WHI,        "l",          ST_WHIL         );
    setT( ST_WHI,        nol,          ST_ID           );
    setT( ST_WHILE,     letters digits,ST_ID           );
    setT( ST_WHIL,       "e",          ST_WHILE        ); // done while
    setT( ST_START,      "p",          ST_P            );
    setT( ST_P,          "r",          ST_PR           );
    setT( ST_P,          nor,          ST_ID           );
    setT( ST_PR,         "i",          ST_PRI          );
    setT( ST_PR,         noi,          ST_ID           );
    setT( ST_PRI,        "n",          ST_PRIN         );
    setT( ST_PRI,        non,          ST_ID           );
    setT( ST_PRIN,       "t",          ST_PRINT        );
    setT( ST_PRIN,       noT,          ST_ID           );
    setT( ST_PRINT,      "l",          ST_PRINTL       );
    setT( ST_PRINT,      nol,          ST_ID           );
    setT( ST_PRINTL,     "n",          ST_PRINTLN      );
    setT( ST_PRINTLN,   letters digits,ST_ID           );
    setT( ST_PRINTL,     non,          ST_ID           ); // done println
    setT( ST_WA,         "i",          ST_WAI          );
    setT( ST_WA,         noi,          ST_ID           );
    setT( ST_WAI,        "n",          ST_WAIN         );
    setT( ST_WAIN,      letters digits,ST_ID           );
    setT( ST_WAI,        non,          ST_ID           ); // done wain
    setT( ST_IN,         "t",          ST_INT          );
    setT( ST_INT,       letters digits,ST_ID           );
    setT( ST_IN,         noT,          ST_ID           ); // done int
    setT( ST_START,      "n",          ST_N            );
    setT( ST_N,          "e",          ST_NE           );
    setT( ST_N,          noe,          ST_ID           );
    setT( ST_NE,         "w",          ST_NEW          );
    setT( ST_NEW,       letters digits,ST_ID           );
    setT( ST_NE,         now,          ST_ID           ); // done new
    setT( ST_START,      "d",          ST_D            );
    setT( ST_D,          "e",          ST_DE           );
    setT( ST_D,          noe,          ST_ID           );
    setT( ST_DE,         "l",          ST_DEL          );
    setT( ST_DE,         nol,          ST_ID           );
    setT( ST_DEL,        "e",          ST_DELE         );
    setT( ST_DEL,        noe,          ST_ID           );
    setT( ST_DELE,       "t",          ST_DELET        );
    setT( ST_DELE,       noT,          ST_ID           );
    setT( ST_DELET,      "e",          ST_DELETE       );
    setT( ST_DELETE,    letters digits,ST_ID           );
    setT( ST_DELET,      noe,          ST_ID           ); // done delete
    setT( ST_START,      "N",          ST_BN           );
    setT( ST_BN,         "U",          ST_BNU          );
    setT( ST_BN,         noU,          ST_ID           );
    setT( ST_BNU,        "L",          ST_BNUL         );
    setT( ST_BNU,        noL,          ST_ID           );
    setT( ST_BNUL,       "L",          ST_BNULL        );
    setT( ST_BNULL,     letters digits,ST_ID           );
    setT( ST_BNUL,       noL,          ST_ID           ); // done NULL
    //===================NUMBER=========================
    setT( ST_START,      oneToNine,      ST_NUM        );
    setT( ST_START,      zero,           ST_ZERO       );
    setT( ST_NUM,        digits,         ST_NUM        );
    setT( ST_NUM,        letters,        ST_START      );
    setT( ST_ZERO,       letters digits, ST_START      );
    //=================== SYMBOL=========================
    setT( ST_START,      "(",            ST_LPAREN     );
    setT( ST_START,      ")",            ST_RPAREN     );
    setT( ST_START,      "{",            ST_LBRACE     );
    setT( ST_START,      "}",            ST_RBRACE     );
    setT( ST_START,      "=",            ST_BECOMES    );
    setT( ST_BECOMES,    "=",            ST_EQ         );
    setT( ST_START,      "!",            ST_SURPRISE   );
    setT( ST_SURPRISE,   "=",            ST_NE         );
    setT( ST_START,      "<",            ST_LT         );
    setT( ST_START,      ">",            ST_GT         );
    setT( ST_LT,         "=",            ST_LE         );
    setT( ST_GT,         "=",            ST_GE         );
    setT( ST_START,      "+",            ST_PLUS       );
    setT( ST_START,      "-",            ST_MINUS      );
    setT( ST_START,      "*",            ST_STAR       );
    setT( ST_START,      "/",            ST_SLASH      );
    setT( ST_SLASH,      "/",            ST_COMMENT    );
    setT( ST_START,      "%",            ST_PCT        );
    setT( ST_START,      ",",            ST_COMMA      );
    setT( ST_START,      ";",            ST_SEMI       );
    setT( ST_START,      "[",            ST_LBRACK     );
    setT( ST_START,      "]",            ST_RBRACK     );
    setT( ST_START,      "&",            ST_AMP        );


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
            State nextState = ST_MYNUL;
            if(i < input.length())
                nextState = delta[state][(unsigned char) input[i]];
            if(nextState == ST_START){
			   if(stateKinds[state] == ZERO){                    
			      throw("ERROR in lexing after reading " + input.substr(0, i));
               }
			}
            if(nextState == ST_MYNUL) {
                // no more transitions possible
                if(stateKinds[state] == MYNUL) {
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

string kS[] = {
    "ID",                 // string
    "NUM",                // Decimal integer
    "ZERO",               // 0
    "LPAREN",             // (
    "RPAREN",             // )
    "LBRACE",             // {
    "RBRACE",             // }
    "RETURN",             // "return"
    "IF",                 // "if"
    "ELSE",               // "else"
    "WHILE",              // while
    "PRINTLN",            // println
    "WAIN",               // wain
    "BECOMES",            // =
    "INT",                // "int"
    "EQ",                 // ==
    "NE",                 // !=
    "LT",                 // <
    "GT",                 // >
    "LE",                 // <=
    "GE",                 // >=
    "PLUS",               // +
    "MINUS",              // -
    "STAR",               // *
    "SLASH",              // /
    "PCT",                // %
    "COMMA",              // ,
    "SEMI",               // ;
    "NEW",                // "new"
    "DELETE",             // "delete"
    "LBRACK",             // [
    "RBRACK",             // ]
    "AMP",                // &
    "NU",                 // "NULL"
    "WHITESPACE",         // TAB SPACE NEWLINE COMMENT
    "MYNUL"                 // Bad/invalid token
};

string kindString( Kind k ){
    if ( k < ID || k > MYNUL ) return "INVALID";
    return kS[k];
}

int main(){
	vector<string> srcLines;  // store info for each line
    try {
		while (true){
		   string lines;
		   getline(cin,lines);
		   if (cin.fail()) break;
		   srcLines.push_back(lines);
		}
		
		vector<vector<Token> > tokLines;
		
		for (int line = 0 ; line < srcLines.size(); ++line){
		    tokLines.push_back(scan(srcLines[line]));
		}
		
        for(int line=0; line < tokLines.size(); line++ ) {
            for(int j=0; j < tokLines[line].size(); j++ ) {
                Token token = tokLines[line][j];
                string kind = kindString(token.kind);
                if(kind == "ZERO") kind = "NUM";
                if(kind == "NU") kind = "NULL";
                cout << kind << " " << token.lexeme << endl;
            }
        }
	}catch (string msg){
	    cerr << msg << endl;
	}
	
	return 0;
}
