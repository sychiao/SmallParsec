#include "SmallParsec.h"

AndParser OrParser::operator*(Parser &p2) {
    return AndParser(this, &p2);
}

ParseResult& OrParser::run(const TokenList& s) {
    //std::cout << "run Or\n";
    ParseResult &p1ret = p1->run(s);
    if (p1ret.isSuccess()) {
        ParseResult *ret = new ParseResult(p1ret.getParseTree(), p1ret.getTokenList());
        return *ret;
    }
    else {
        ParseResult &p2ret = p2->run(s);
        if (p2ret.isSuccess()) {
            ParseResult *ret = new ParseResult(p2ret.getParseTree(), p2ret.getTokenList());
            return *ret;
        }
        else {
            ParseResult *ret = new ParseResult();
            return *ret;
        }
    }
}