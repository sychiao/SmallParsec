// SmallParsec.cpp: 定義應用程式的進入點。
//

#include "SmallParsec.h"
#include "tokenizer.hpp"
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


	int i = 0;
	ParseResult ret = AA.run(tokenss);

    for (auto ele : tokens) {
        cout << "!!" << ele.type_name << endl;
    }
	cout << "Hello CMake." << endl;
    system("PAUSE");
	return 0;
}
