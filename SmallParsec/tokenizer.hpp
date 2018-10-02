#include <iostream>
#include <string>
#include <list>

using namespace std;

class token {
public:
    string type_name;
    string token_text;

    token(string type, string text) : type_name(type), token_text(text) {}
};

using tokenlist = list<token>;

class ParseTree {
public:
    token content;
    ParseTree(token content, ParseTree *Parent)
        :content(content), Parent(Parent) {}
    list<ParseTree*> child;
    ParseTree *Parent;
};

class Parser {
public:
    virtual bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) = 0;
};

class OrParser : public Parser {
    Parser *p1;
    Parser *p2;
    string term;
public:
    OrParser(string term, Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {}
    OrParser(Parser *p1, Parser *p2) : term(term), p1(p1), p2(p2) {}
    void setTerm(string t) { term = t; }
    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        pair<ParseTree*, tokenlist*> *p1ret = new pair<ParseTree*, tokenlist*>();
        pair<ParseTree*, tokenlist*> *p2ret = new pair<ParseTree*, tokenlist*>();
        if (p1->run(s, p1ret)) {
            ret->first = new ParseTree{ token{term,"null"} , nullptr };
            p1ret->first->Parent = ret->first;
            ret->first->child.push_back(p1ret->first);
            ret->second = p1ret->second;
            return true;
        }
        else if (p2->run(s, p2ret)) {
            ret->first = new ParseTree{ token{term,"null"} , nullptr };
            p2ret->first->Parent = ret->first;
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
};

class AndParser : public Parser {
    Parser *p1;
    Parser *p2;
    string term;
public:
    AndParser(string term,Parser *p1, Parser *p2) : term(term),p1(p1),p2(p2) {}

    AndParser operator*(Parser &p2) {
        return AndParser("And", this, &p2);
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
    }

    bool run(tokenlist& s, pair<ParseTree*, tokenlist*> *ret) {
        pair<ParseTree*, tokenlist*> *p1ret = new pair<ParseTree*, tokenlist*>();
        pair<ParseTree*, tokenlist*> *p2ret = new pair<ParseTree*, tokenlist*>();
        if (p1->run(s, p1ret)) {
            if (p2->run(*(p1ret->second), p2ret)) {
                ret->first = new ParseTree{ token{term,"null"} , nullptr };
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

class UnitParser : public Parser {
    string Unit;
public:
    UnitParser(string unit) : Unit(unit) {}

    AndParser operator*(Parser &p2) {
        return AndParser("And", this, &p2);
    }

    OrParser operator|(Parser &p2) {
        return OrParser(this, &p2);
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