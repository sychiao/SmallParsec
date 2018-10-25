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
	ParseTree(string content, initializer_list<const ParseTree*> childlst)
		:content(token{ content,"null" }), Parent(nullptr), child(childlst) {}
	list<const ParseTree*> child;
	ParseTree *Parent;
};

class ParseResult {
private:
	ParseTree *tree;
	const TokenList *rest;
	bool success;

public:
	ParseResult() : tree(nullptr), rest(nullptr),success(false) {}
	ParseResult(ParseTree &_t, const tokenlist &_r) : tree(&_t), rest(&_r) , success(true) {}
	const ParseTree& getParseTree() { return *tree; }
	const TokenList& getTokenList() { return *rest; }
	bool isSuccess() { return success; }
};

class Parser {
protected:
	set<string> First;
public:
	virtual ParseResult& run(const TokenList& s) = 0;
	set<string> getFirst() { return First; }
};

class OrParser : public Parser {
	Parser *p1;
	Parser *p2;
	string term;
	bool init = true;

public:
	OrParser():init(false){}

	void SetParser1(Parser *p) {
		p1 = p;
	}
	void SetParser2(Parser *p) {
		p2 = p;
	}

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

	virtual ParseResult& run(const TokenList& s) {
		ParseResult &p1ret = p1->run(s);
		if (p1ret.isSuccess()) {
			//ParseResult *ret = new ParseResult(p1ret.getParseTree(), p1ret.getTokenList())
			return p1ret;
		}else{
			ParseResult &p2ret = p2->run(s);
			if (p2ret.isSuccess()) {
				return p2ret;
			}else {
				ParseResult *ret = new ParseResult();
				return *ret;
			}
		}
	}

	void setTerm(string t) { term = t; }
};

class AndParser : public Parser {
	Parser *p1;
	Parser *p2;
	bool init = true;

public:
	AndParser() : init(false) {}

	void SetParser1(Parser *p) {
		p1 = p;
	}
	void SetParser2(Parser *p) {
		p2 = p;
	}

	AndParser(Parser *p1, Parser *p2) : p1(p1), p2(p2) {
		for (auto ele : p1->getFirst()) {
			First.insert(ele);
		}
	}
	ParseResult& run(const TokenList& s) {
		ParseResult &p1ret = p1->run(s);
		if (p1ret.isSuccess()) {
			ParseResult &p2ret = p2->run(p1ret.getTokenList());
			if (p2ret.isSuccess()) {
				ParseTree *tree = new ParseTree("And", { &p1ret.getParseTree(),&p2ret.getParseTree() });
				ParseResult *ret = new ParseResult(*tree, p2ret.getTokenList());
				return *ret;
			}
		}
		ParseResult *ret = new ParseResult();
		return *ret;
	}
};
/*
class ReturnParser : public Parser {
	string Name;
	Parser *p1;

	void AndConcate(ParseTree* tree, list<ParseTree*> &child) {
		for (auto subtree : tree->child) {
			if (subtree->content.type_name == "And") {
				AndConcate(subtree, child);
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
*/

class UnitParser : public Parser {
	string Unit;
public:
	UnitParser(string unit) : Unit(unit) {
		First.insert(unit);
	}

	ParseResult& run(const TokenList& s) {
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
};