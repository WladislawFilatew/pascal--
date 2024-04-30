#include "Text.h"

void removSpace(string& s) {
	auto it = s.begin();
	while (it != s.end()) {
		if ((*it) == ' ' || (*it) == '\t') {
			s.erase(it);
			continue;
		}
		break;
	}
}

bool CheckType(string s) {
	return s == "double" || s == "integer" || s == "bool" || s == "char";
}

string ToVar(string s, int& i)
{
	string temp;
	char c = s[i];
	while (c != '\0' && !(c == ',' || c == ':' || c == '=' || c == ';' || c == '(' || c == ')')) {
		temp += s[i];
		i++;
		c = s[i];
	}
	i--;
	return temp;
}

Text::TextNode::TextNode()
{
	text = "";
	pNext = nullptr;
	pDown = nullptr;
}

Text::TextNode::TextNode(string s)
{
	text = s;
	pNext = nullptr;
	pDown = nullptr;
}


ostream& operator<<(ostream& os,Text& t)
{
	Text::iterator it = t.begin();
	while (it != t.end()) {
		for (int i = 0; i < it.tab(); i++) os << "   ";
		os << *(it) << endl;
		it++;
	}
	return os;
}

istream& operator>>(istream& is, Text& t)
{
	string s;
	Text::iterator it = t.begin();
	t.recursRead(is, it);

	t.pEnd = it.teck();
	t.CleatMemory();

	return is;

}

void Text::recursRead(istream& is, Text::iterator& it, string prev)
{
	Text temp = *this;
	string s;
	while (getline(is, s)) {
		removSpace(s);
		bool up = false;
		bool down = false;
		for (auto a : keyWords[prev]) {
			if (a == s) up = true;
		}
		for (auto a : keyWords) {
			if (a.first == s) down = true;
		}
		if (up && down) {
			it.Up();
			it.node = it.node->pNext = new TextNode(s);
			this->add(it);
			recursRead(is, it, s);
		}
		if (up && !down) {
			it.Up();
			it.node = it.node->pNext = new TextNode(s);
			this->add(it);
			return;
		}
		if (down && !up) {
			it.node->text = s;
			it.node->pDown = new TextNode;
			it.Down();
			recursRead(is, it, s);
		}
		if (!up && !down) {
			it.node->text = s;
			it.node = it.node->pNext = new TextNode();
		}
	}
}


Text::Text()
{
	text = new TextNode();
	pEnd = text;
}

void Text::readFile(string path)
{
	ifstream f;
	f.open(path);
	if (!f.is_open()) {
		cout << "Error open file";
		return;
	}
	f >> *this;
	f.close();
}

void Text::writeFile(string path)
{
	ofstream f;
	f.open(path);
	if (!f.is_open()) {
		cout << "Error open file";
		return;
	}
	f << *this;
	f.close();
}

void Text::Clear()
{
	stack<TextNode*> Stack;
	Text::iterator it = this->begin();
	while (it != this->end()) {
		Stack.push(it.node);
		it++;
	}

	while (!Stack.empty()) {
		delete Stack.top();
		Stack.pop();
	}
	text = new TextNode();
	pEnd = text;
}

void Text::add(iterator& it, string s)
{
	TextNode* node = it.node;
	bool down = false;
	for (auto a : keyWords) {
		if (node->text == a.first) {
			down = true;
			break;
		}
	}
	if (!down) {
		if (node->pNext == nullptr) {
			it.node = node->pNext = new TextNode(s);
			it.path.push({ it.node,it.k });
		}
		else {
			TextNode* temp = new TextNode(s);
			temp->pNext = node->pNext;
			it.node = node->pNext = temp;
			it.path.push({ it.node,it.k });
		}
	}
	else {
		if (node->pDown == nullptr) {
			it.Stack.push(it.node);
			it.node = node->pDown = new TextNode(s);
			it.k++;
			it.path.push({ it.node,it.k });
		}
		else {
			it.Stack.push(it.node);
			TextNode* temp = new TextNode(s);
			temp->pNext = node->pDown;
			it.node = node->pDown = temp;
			it.k++;
			it.path.push({ it.node,it.k });
		}
	}
}


void Text::iterator::operator--(int n)
{
	if (path.empty()) return;
	path.pop();
	node = path.top().first;
	this->k = path.top().second;
}

void Text::iterator::operator++(int n)
{
	if (node->pDown != nullptr) {
		Stack.push(node);
		node = node->pDown;
		k++;
	}
	else if (node->pNext != nullptr) {
		node = node->pNext;
	}
	else {
		node = Stack.top();
		Stack.pop();
		node = node->pNext;
		k--;
	}
	path.push({ node,k });
}

void Text::iterator::Next()
{
	node = node->pNext;
	path.push({ node,k });
}

void Text::iterator::Down()
{
	Stack.push(node);
	node = node->pDown;
	k++;
	path.push({ node,k });
}

void Text::iterator::Up()
{
	k--;
	node = Stack.top();
	Stack.pop();
	path.push({ node, k });
}

void Text::iterator::endUp()
{

	k--;
	TextNode* temp = path.top().first;
	path.pop();
	path.top().first->pNext = nullptr;
	path.push({ temp, k });

	temp->pNext = Stack.top()->pNext;
	node = Stack.top()->pNext = temp;
	Stack.pop();
}

void Text::iterator::operator()(TextNode* text)
{
	node = text;
	while (!Stack.empty()) Stack.pop();
	while (!path.empty()) path.pop();
	path.push({ node,k });
	k = 0;

}

void Text::setVar()
{
	VarI.Clear();
	VarD.Clear();
	ConstD.Clear();
	ConstI.Clear();



	bool start = false;
	bool fConst = false;
	TextNode* node = nullptr;
	Text::iterator it = begin();



	while (it != end()) {
		if (it.teck() == node) {
			start = false;
			fConst = false;
		}
		if (start) {

			string type;
			vector<string> tempVar;
			double valueD = 0.0;
			double valueI = 0;


			bool flagValue = false;

			string s = (*it);
			s = RemoveSpace(s);

			int i = 0;
			while (i < s.size()) {
				char c = s[i];
				switch (c)
				{
				case ':':
				case ';':

					break;
				case '=':
					flagValue = true;
					break;
				default:
					if (!flagValue && CheckChar(c)) {
						string temp = ToVar(s, i);
						if (CheckType(temp))
							type = temp;
						else
							tempVar.push_back(temp);

					}
					if (flagValue) {
						if (type == "double") {
							valueD = ToDouble(s, i);
						}
						if (type == "integer") {
							valueI = (int)ToDouble(s, i);
						}
						if (type == "char") {
							valueI = (int)c;
						}
						if (type == "bool" && CheckChar(c)) {
							string temp = ToStringVar(s, i);
							valueI = (temp == "true" || temp == "True") ? 1 : 0;
						}
						if (type == "bool" && !CheckChar(c)) {
							int temp = (int)ToDouble(s, i);
							valueI = (temp != 0) ? 1 : 0;
						}
					}

					break;
				}
				i++;
			}

			for (auto a : tempVar) {
				if (type == "double") {
					fConst ? ConstD.Insert(a, type, valueD) : VarD.Insert(a, type, valueD);
				}
				else {
					fConst ? ConstI.Insert(a, type, valueI) : VarI.Insert(a, type, valueI);
				}
			}
		}
		if (!start && ((*it) == "var" || (*it) == "const")) {
			if ((*it) == "const") fConst = true;
			start = true;
			node = it.teck()->pNext;
		}
		it++;
	}
}

void Text::setAnsverformul(string s)
{
	int i = 0;
	string var = ToVar(s, i);
	s.erase(0, i + 3);
	s.pop_back();

	calculator c(s);
	map<string, double> tempVar = c.GetVar();


	for (auto& a : tempVar) {
		if (VarD.Find(a.first) != nullptr) {
			a.second = *(VarD.Find(a.first));
		}
		if (VarI.Find(a.first) != nullptr) {
			a.second = *(VarI.Find(a.first));
		}
		if (ConstD.Find(a.first) != nullptr) {
			a.second = *(ConstD.Find(a.first));
		}
		if (ConstI.Find(a.first) != nullptr) {
			a.second = *(ConstI.Find(a.first));
		}
	}
	c.SetVar(tempVar);

	if (VarD.Find(var) != nullptr) {
		VarD.SetValue(var, c.Ansver());
	}
	if (VarI.Find(var) != nullptr) {
		VarI.SetValue(var, c.Ansver());
	}
}

void Text::forkIF(string s, Text::iterator& it)
{
	string formul, zn;
	int i = 0;
	while (s[i] != '(') i++;
	i++;
	while (s[i] != ')') {
		if (s[i] != '=' && s[i] != '<' && s[i] != '>' && s[i] != '!') formul += s[i];
		else {
			while (s[i] == '=' || s[i] == '<' || s[i] == '>' || s[i] == '!') {
				zn += s[i];
				i++;
			}
			formul += '-';
			continue;
		}
		i++;
	}
	calculator c(formul);
	map<string, double> tempVar = c.GetVar();
	for (auto& a : tempVar) {
		if (VarD.Find(a.first) != nullptr) {
			a.second = *(VarD.Find(a.first));
		}
		if (VarI.Find(a.first) != nullptr) {
			a.second = *(VarI.Find(a.first));
		}
		if (ConstD.Find(a.first) != nullptr) {
			a.second = *(ConstD.Find(a.first));
		}
		if (ConstI.Find(a.first) != nullptr) {
			a.second = *(ConstI.Find(a.first));
		}
	}
	c.SetVar(tempVar);
	double ans = c.Ansver();


	if (zn == "=" && ans > -0.0000000001 && ans < 0.0000000001) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	if (zn == ">" && ans > 0) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	if (zn == "<" && ans < 0) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	if (zn == ">=" && ans >= 0) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	if (zn == "<=" && ans <= 0) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	if (zn == "!=" && !(-0.0000000001 && ans < 0.0000000001)) {
		it.Next();
		if (it.node->pNext->pNext != nullptr)
			it.node->pNext->pNext->not_go = true;
		it.Down();
		return;
	}
	it.Next();
	it.Next();
}

void Text::CleatMemory()
{
	   stack<TextNode*> Stack;
	   Text::iterator it = begin();
	   while (it != end()) {
		   if (it.teck()->pDown != nullptr && it.teck()->pDown->text == "" && it.teck()->pDown != end()) {
			   delete it.teck()->pDown;
			   it.teck()->pDown = nullptr;
		   }
		   if (it.teck()->pNext != nullptr && it.teck()->pNext->text == "" && it.teck()->pNext != end()) {
			   delete it.teck()->pNext;
			   it.teck()->pNext = nullptr;
		   }
		   it++;
	   }
}


map<string, list<string>> Text::keyWords = {
		{"begin", {"end","end."}},
		{"const", {"var", "begin"}},
		{"var", {"const","begin"}}
};

vector<string> reservWords = {
	"program",
	"const",
	"var",
	"begin",
	"end",
	"end.",
	"Write",
	"Read",
	"if",
	"then",
	"else"
};