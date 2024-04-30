#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <windows.h>
#include <map>
#include <list>
#include "Table.h"
#include "postfix.h"
using namespace std;

void removSpace(string& s);

bool CheckType(string s);

string ToVar(string s, int& i);


class Text
{
	struct TextNode {
		string text;
		TextNode* pNext;
		TextNode* pDown;
		TextNode();
		TextNode(string s);
		bool not_go = false;
	};
	
	static map<string, list<string>> keyWords;
	static vector<string> reservWords;


	TextNode* text;
	TextNode* pEnd;

	Table<int> ConstI;
	Table<double> ConstD;
	Table<int> VarI;
	Table<double> VarD;

	
public:
	friend ostream& operator<<(ostream& os,Text& t);
	friend istream& operator>>(istream& is, Text& t);

	Text();
	void readFile(string path);
	void writeFile(string path);
	void Clear();

	class iterator {
		TextNode* node;
		stack<TextNode*> Stack;
		stack<pair<TextNode*,int>> path;
		int k;
	public:
		iterator() { node = nullptr; k = 0;}
		iterator(TextNode* text) { node = text; k = 0; path.push({ node,k }); }
		void operator=(TextNode* text) { node = text; path.push({ node,k }); }
		string& operator*() { return node->text; }
		void operator++(int n);
		void Next();
		void Down();
		void Up();
		void endUp();
		void operator--(int n);
		bool operator!=(TextNode* text) { return node != text; }
		bool operator==(TextNode* text) { return node == text; }
		void operator()(TextNode* text);
		friend void Text::Clear();
		friend Text;
		TextNode* teck() { return node; }
		int tab() { return k; }
	};

	bool ifEndWord(string s) {
		return s == "end" || s == "end.";
	}
	bool endUp(Text::iterator& it_ozer) {
		Text::iterator it = this->begin();
		while (it != this->end()) {
			if ((*it) == "begin" && it.node->pNext == it_ozer.node)
				return false;
			it++;
		}
		return true;
	}

	

	TextNode* begin() { return text; }
	TextNode* end() { return pEnd; }
	void add(iterator& it, string s = "");
	void erase(iterator it) {
		
	}
	void setVar();

	void recursRead(istream& is, Text::iterator& it, string prev = "");

	void setAnsverformul(string s);

	void forkIF(string s, Text::iterator& it);



	void Start() {
		setVar();

		Text::iterator it = begin();
		while (it != end()) {
			string s = (*it);
			s = RemoveSpace(s);
			if (s.find(":=") != string::npos) {
				setAnsverformul(s);
				it++;
				continue;
			}
			if (s.find("if") != string::npos) {
				forkIF(s,it);
				continue;
			}
			if (s.find("else") != string::npos) {
				if (it.node->not_go) {
					it.Next();
					it.Next();
					continue;
				}
			}
			if (s.find("Write") != string::npos) {
				int i = 0;
				string ans;
				bool text = false;
				while (s[i] != '(') i++;
				i++;
				
				while (s[i] != ')') {
					if (s[i] == '"') {
						text = !text;
						i++;
						continue;
					}
					if (text) ans += s[i];
					else {
						if (s[i] != ',') {
							string var = ToVar(s, i);
							if (VarD.Find(var) != nullptr) {
								ans += Remove0(to_string(*VarD.Find(var)));
							}
							if (VarI.Find(var) != nullptr) {
								ans += to_string(*VarI.Find(var));
							}
							if (ConstD.Find(var) != nullptr) {
								ans += Remove0(to_string(*ConstD.Find(var)));
							}
							if (ConstI.Find(var) != nullptr) {
								ans += to_string(*ConstI.Find(var));
							}
						}
					}
					i++;
				}
				cout << ans << endl;

			}
			if (s.find("Read") != string::npos) {
				int i = 0;
				while (s[i] != '(')i++;
				i++;
				string var = ToVar(s, i);
				if (VarD.Find(var) != nullptr) {
					double d; cin >> d;
					*VarD.Find(var) = d;
				}
				if (VarI.Find(var) != nullptr) {
					int i; cin >> i;
					*VarI.Find(var) = i;
				}
			}

			it++;
		}
	}

	~Text() { Clear(); }
	void CleatMemory();

	
};