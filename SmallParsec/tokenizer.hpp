#include <iostream>
#include <string>
#include <list>
#include <set>

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
    ParseTree(string content,initializer_list<const ParseTree*> childlst)
        :content(token{content,"null"}), Parent(nullptr), child(childlst) {}
    list<const ParseTree*> child;
    ParseTree *Parent;
};

class ParseResult {
private:
	ParseTree *tree;
	const TokenList *rest;
    bool success;

public:
    ParseResult() : success(false) {}
    ParseResult(ParseTree &_t, const tokenlist &_r) : tree(&_t), rest(&_r) {}
	const ParseTree& getParseTree() { return *tree; }
	const TokenList& getTokenList() { return *rest; }
    bool isSuccess() { return success; }
};

class Parser {
protected:
    set<string> First;
public:
    virtual bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) = 0;
    virtual ParseResult& run(const TokenList& s) = 0;
    set<string> getFirst() { return First; }
};

class OrParser : public Parser {
    Parser *p1;
    Parser *p2;
    string term;

public:
    OrParser(string term, Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {
        for (auto ele : p1->getFirst()) {
            First.insert(ele);
        }
        for (auto ele : p2->getFirst()) {
            First.insert(ele);
        }
    }

    OrParser(Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {
        for (auto ele : p1->getFirst()) {
            First.insert(ele);
        }
        for (auto ele : p2->getFirst()) {
            First.insert(ele);
        }
    }

    bool run(TokenList& s, ParseResult& ret) {
        return true;
    }

    void setTerm(string t) { term = t; }
    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        auto f1 = p1->getFirst();
        auto f2 = p2->getFirst();
        if ( f1.find(s.front().type_name) != f1.end() ) {
            pair<ParseTree*, tokenlist*> *p1ret = new pair<ParseTree*, tokenlist*>();
            if (p1->run(s, p1ret)) {
                ret->first = new ParseTree{ token{term,"null"} , nullptr };
                p1ret->first->Parent = ret->first;
                ret->first->child.push_back(p1ret->first);
                ret->second = p1ret->second;
                return true;
            }
            else {
                free(p1ret);
                return false;
            }
        }
        else if (f2.find(s.front().type_name) != f2.end()){
            pair<ParseTree*, tokenlist*> *p2ret = new pair<ParseTree*, tokenlist*>();
            if (p2->run(s, p2ret)) {
                ret->first = new ParseTree{ token{term,"null"} , nullptr };
                p2ret->first->Parent = ret->first;
                ret->first->child.push_back(p2ret->first);
                ret->second = p2ret->second;
                return true;
            }
            else {
                free(p2ret);
                return false;
            }
        }
        else {
            return false;
        }
    }
};

class AndParser : public Parser {
    Parser *p1;
    Parser *p2;

public:
    AndParser(Parser *p1, Parser *p2) : p1(p1),p2(p2){
        for (auto ele : p1->getFirst()) {
            First.insert(ele);
        }
    }

    AndParser operator*(Parser &p2) {
        return AndParser(this, &p2);
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    ParseResult& run(const TokenList& s) {
        ParseResult &p1ret = p1->run(s);
        if (p1ret.isSuccess()) {
            ParseResult &p2ret = p2->run(p1ret.getTokenList());
            if (p2ret.isSuccess()) {
                ParseTree *tree = new ParseTree("And", {&p1ret.getParseTree(),&p2ret.getParseTree()});
                ParseResult *ret = new ParseResult(*tree, p2ret.getTokenList());
                return *ret;
            }
        }
        
    }

    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        pair<ParseTree*, tokenlist*> *p1ret = new pair<ParseTree*, tokenlist*>();
        pair<ParseTree*, tokenlist*> *p2ret = new pair<ParseTree*, tokenlist*>();

        if (p1->run(s, p1ret)) {
            if (p2->run(*(p1ret->second), p2ret)) {
                ret->first = new ParseTree{ token{"And","null"} , nullptr };
                p1ret->first->Parent = ret->first;
                p2ret->first->Parent = ret->first;
                ret->first->child.push_back(p1ret->first);
                ret->first->child.push_back(p2ret->first);
                ret->second = p2ret->second;
                return true;
            }
            else {
                free(p1ret);
                free(p2ret);
                return false;
            }
        }
        else {
            free(p1ret);
            free(p2ret);
            return false;
        }
    }
};

class ReturnParser : public Parser {
    string Name;
    Parser *p1;

    void AndConcate(ParseTree* tree, list<ParseTree*> &child) {
        for (auto subtree : tree->child) {
            if (subtree->content.type_name == "And") {
                AndConcate(subtree,child);
            }
            else {
                cout << subtree->content.type_name << endl;
                child.push_back(subtree);
            }
        }
    }

public:
    ReturnParser(string Name, Parser &p1) :Name(Name), p1(&p1) {
        for (auto ele : p1.getFirst())
            First.insert(ele);
    }

    bool run(TokenList& s, ParseResult& ret) {
        return true;
    }

    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        pair<ParseTree*, tokenlist*> *p1ret = new pair<ParseTree*, tokenlist*>();
        p1->run(s, p1ret);
        
        list<ParseTree*> child;
        AndConcate(p1ret->first, child);

        ret->first = new ParseTree{ token{Name,"null"} , nullptr };
        ret->first->child = child;
        ret->second = p1ret->second;
        return true;
    }
};

class UnitParser : public Parser {
    string Unit;
public:
    UnitParser(string unit) : Unit(unit) {
        First.insert(unit);
    }

    AndParser operator*(Parser &p2) {
        return AndParser(this, &p2);
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    bool run(TokenList& s, ParseResult& ret) {
        return true;
    }

    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        if (s.front().type_name == Unit) {
            ParseTree *Result = new ParseTree{ s.front() ,nullptr };
            tokenlist *newlst = new tokenlist{ s };
            newlst->pop_front();
            ret->first = Result;
            ret->second = newlst;
            return true;
        }
        else {
            return false;
        }

    }
};