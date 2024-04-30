#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

template <class Type>
class Table
{
	struct node
	{
		string name;
		string type;
		Type value;

	};
	vector<node> arr;
	int count = 0;
public:
	int size() { return count; }
	bool IsEmpty() { return count == 0; }
	Type* Find(string name) {
		for (auto& a : arr) {
			if (a.name == name) {
				return &a.value;
			}
		}
		return nullptr;
	}
	bool Insert(string name, string type, Type value) {
		if (Find(name) != nullptr) return false;
		arr.push_back({ name,type,value });
		count++;
		return true;
	}
	bool Delete(string name) {
		auto it = arr.begin();
		while (it != arr.end()) {
			if (it->name == name) {
				arr.erase(it);
				count--;
				return true;
			}
			it++;
		}
		return false;
	}
	void SetValue(string key, Type value) {
		for (auto& a : arr) {
			if (a.name == key) {
				a.value = value;
				break;
			}
		}
	}
	void Clear() {
		count = 0;
		arr.clear();
	}
	vector<node> getArr() { return arr; }
};

