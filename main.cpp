// SmallParsec.cpp: 定義應用程式的進入點。
//

//#include "tokenizer.hpp"
#include "SmallParsec.h"
using namespace std;


int main()
{
    token A{ "Hello","ya" };
    token B{ "Bra","ya" };
    token C{ "kuku","ya" };
    token D{ "laal","ya" };

    tokenlist tokens = { A,B,C,D};
	tokenlist tokenss = { C,C,C,D };


    UnitParser Hello = UnitParser{ "Hello" };
    UnitParser Bra = UnitParser{ "Bra" };
    UnitParser kuku = UnitParser{ "kuku" };

	OrParser AA;
	AndParser kuAA = AndParser{ &kuku, &AA };

	AndParser ptr = kuku * AA;

	AA = kuku * AA | kuku;

    token N{ "NUM","1" };
    token P{ "PLUS","+" };
    token T{ "MUL","*" };

    UnitParser Num = UnitParser{ "NUM" };
    UnitParser Plus = UnitParser{ "PLUS" };
    UnitParser Times = UnitParser{ "MUL" };
    
    OrParser Term;
    OrParser Expr;

    tokenlist expr = { N,P,N,T,N,P,N};

    Term = Num * Times * Term | Num;
    Expr = Term * Plus * Expr | Term;

     error
    int i = 0;
    ParseResult ret = AA.run(tokenss);
    ParseResult ret2 = Expr.run(expr);

    for (auto ele : tokens) {
        cout << "!!!" << ele.type_name << endl;
    }
	cout << "Hello CMake." << endl;
    system("PAUSE");
	return 0;
}
