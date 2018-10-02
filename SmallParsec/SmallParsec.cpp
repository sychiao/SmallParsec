// SmallParsec.cpp: 定義應用程式的進入點。
//

#include "SmallParsec.h"
#include "tokenizer.hpp"
using namespace std;


int main()
{
    token A{ "Hello","ya" };
    token B{ "Bra","ya" };
    token C{ "laal","ya" };
    token D{ "laal","ya" };

    tokenlist tokens = { A,B,C,D};


    UnitParser Hello = UnitParser{ "Hello" };
    UnitParser Bra = UnitParser{ "Bra" };
    UnitParser kuku = UnitParser{ "kuku" };

    OrParser HelloOrHelloBrakuku = Hello * Bra * kuku | Hello;
    HelloOrHelloBrakuku.setTerm("oraora");

    pair<ParseTree*, tokenlist*> *Result = new pair<ParseTree*, tokenlist*>();
    //HelloBrakuku.run(tokens, Result);
    bool ret = HelloOrHelloBrakuku.run(tokens, Result);
    cout << Result->first->content.type_name << "*\n";
    for (auto ele : tokens) {
        cout << "!!" << ele.type_name << endl;
    }
	cout << "Hello CMake." << endl;
    system("PAUSE");
	return 0;
}
