#include "SmallParsec.h"

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

AndParser UnitParser::operator*(Parser &p2) {
    return AndParser(this, &p2);
}

OrParser UnitParser::operator|(Parser &p2) {
    return OrParser(this, &p2);
}
