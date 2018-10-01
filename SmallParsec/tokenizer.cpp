#include <iostream>
#include <string>
#include <list>
#include <pair>

using namespace std;

class token {
    string type_name;
    string token_text;
public:
    token(string type, string text) : type_name(type), token_text(text) {}
};

using tokenlist = list<token>;

class ParseTree {
    token content;
public:
    list<ParseTree> child;
    ParseTree *Parent;
};

class Parser {
public:
    virtual ParseTree run(tokenlist S) = 0;
    
};

class UnitParser : Parser {
    string Unit;
public:
    UnitParser(string unit) : Unit(unit) {}
    bool run(tokenlist s, pair<ParseTree*, tokenlist*> ret) {
        if (s.front().type_name == Unit) {
            
            ret
        }

    }
};