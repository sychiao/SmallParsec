#include "SmallParsec.h"

ParseResult& AndParser::run(const TokenList& s) {
    //std::cout << "run And\n";
    list<const ParseTree*> child;
    TokenList token(s);
    int size = (*plst).size();
    for (Parser* p : *plst) {
        ParseResult &pret = p->run(token);
        if (pret.isSuccess()) {
            child.push_back(&pret.getParseTree());
            token = pret.getTokenList();
        }
        else {
            ParseResult *ret = new ParseResult();
            return *ret;
        }
    }
    ParseTree *tree = new ParseTree("And", child);
    ParseResult *ret = new ParseResult(*tree, token);
    return *ret;
}

ParseResult& UnitParser::run(const TokenList& s) {
    //std::cout << "run Unit\n";
    if (s.size() < 1) {
        ParseResult *ret = new ParseResult();
        return *ret;
    }

    if (s.front().type_name == Unit) {
        ParseTree *tree = new ParseTree{ s.front() ,nullptr };
        tokenlist *newlst = new tokenlist{ s };
        newlst->pop_front();
        ParseResult *ret = new ParseResult(*tree, *newlst);
        return *ret;
    }
    else {
        ParseResult *ret = new ParseResult();
        return *ret;
    }
}