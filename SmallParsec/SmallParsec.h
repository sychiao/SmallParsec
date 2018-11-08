#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>

using namespace std;

class token {
public:
    string type_name;
    string token_text;

    token(string type, string text) : type_name(type), token_text(text) {}
};

using tokenlist = list<token>;
using TokenList = list<token>;

class ParseTree {
public:
    token content;
    ParseTree(token content, ParseTree *Parent)
        :content(content), Parent(Parent) {}
    ParseTree(string content, initializer_list<const ParseTree*> childlst)
        :content(token{ content,"null" }), Parent(nullptr), child(childlst) {}
    ParseTree(string content, list<const ParseTree*> childlst)
        :content(token{ content,"null" }), Parent(nullptr), child(childlst) {}
    list<const ParseTree*> child;
    ParseTree *Parent;
};

class ParseResult {
private:
    const ParseTree *tree;
    const TokenList rest;
    bool success;

public:
    ParseResult() : tree(nullptr), rest(TokenList{}), success(false) {}
    ParseResult(const ParseTree &_t, const tokenlist _r) : tree(&_t), rest(_r), success(true) {}
    const ParseTree& getParseTree() { return *tree; }
    const TokenList getTokenList() {return rest;}
    bool isSuccess() { return success; }
};

class Parser {
protected:
    set<string> First;
public:
    virtual ParseResult& run(const TokenList& s) = 0;
    set<string> getFirst() { return First; }
};


class AndParser;
class UnitParser;
class OrParser : public Parser {
    Parser *p1;
    Parser *p2;
    string term;
    bool init = true;

public:
    OrParser() :init(false) {}

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    AndParser operator*(Parser &p2);

    void SetParser1(Parser *p) {
        p1 = p;
    }
    void SetParser2(Parser *p) {
        p2 = p;
    }

    OrParser(string term, Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {}

    OrParser(Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {}
    ParseResult& run(const TokenList& s) override;
    void setTerm(string t) { term = t; }
};

class AndParser : public Parser {
    Parser *p1;
    Parser *p2;
    vector<Parser*> *plst;
    bool init = true;

public:
    AndParser() : init(false) {}

    AndParser operator*(Parser &p2) {
        Add(&p2);
        return *this;
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    void Add(Parser *_p) {
        plst->push_back(_p);
    }

    AndParser(Parser *_p1, Parser *_p2) : p1(_p1), p2(_p2) {
        plst = new vector<Parser*>();
        plst->push_back(_p1);
        plst->push_back(_p2);
        for (auto ele : p1->getFirst()) {
            First.insert(ele);
        }
    }

    ParseResult& run(const TokenList& s) override;
};

class UnitParser : public Parser {
    string Unit;
public:
    UnitParser(string unit) : Unit(unit) {}

    AndParser operator*(Parser &p2) {
        return AndParser(this, &p2);
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    ParseResult& run(const TokenList& s) override;
};