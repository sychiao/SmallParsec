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


void AndParser::Add(Parser *_p) {
    plst->push_back(_p);
}

AndParser AndParser::operator*(Parser &p2) {
    Add(&p2);
    return *this;
}

OrParser AndParser::operator|(Parser &p2) {
    return OrParser(this, &p2);
}