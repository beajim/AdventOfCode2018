

#include "stdafx.h"

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <inttypes.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <sstream>
#include <tuple>
#include <memory>
#include <assert.h>
using namespace std;

template <int dayNumber, int part = 1> void day();

template<> void day<4>() {
	fstream file("d4.txt");
	struct datetime {
		int month, day, hour, minute;
	};
	struct entry {
		datetime time;
		int data;
		void operator<<(istream& s) {
			char str[19];
			s.read(str, 19);
			int year;
			sscanf_s(str, "[%d-%d-%d %d:%d] ", &year, &time.month, &time.day, &time.hour, &time.minute);
			switch (s.get()) {
			case 'G':
				s.ignore(6);
				s >> data;
				break;
			case 'w':
				data = -1;
				break;
			case 'f':
				data = -2;
				break;
			}
		}
	};
	vector<entry> in;
	string s;
	while (getline(file, s)) {
		in.emplace_back();
		in.back() << istringstream(s);
	}
	sort(begin(in), end(in), [](const entry& a, const entry& b) {
		return tie(a.time.month, a.time.day, a.time.hour, a.time.minute) < tie(b.time.month, b.time.day, b.time.hour, b.time.minute);
	});
	struct instant {
		int currentGuard;
		bool awake;
	};
	struct myhash
	{
		std::size_t operator()(const datetime& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:
			/*hash_tuple::
			return hash_tuple:: hash<tuple<int, int, int, int>>{}(tie(k.month, k.day, k.hour, k.minute));
			return ((hash<string>()(k.first)
				^ (hash<string>()(k.second) << 1)) >> 1)
				^ (hash<int>()(k.third) << 1);*/
		}
	};

	unordered_map<datetime, instant, myhash> all;

}

template<> void day<7, 1>() {
	fstream file("d7.txt");
	set<char> t;
	map<char, set<char>> d;
	string s;
	while (getline(file, s)) {
		char a = s[5], b = s[36];
		t.insert(a);
		t.insert(b);
		d[b].insert(a);
	}
	while (!t.empty()) {
		for (auto& task : t) {
			if (d[task].empty()) {
				cout << task;
				for (auto& pair : d) {
					pair.second.erase(task);
				}
				t.erase(task);
				break;
			}
		}
	}
	cin.get();
}

template<> void day<7, 2>() {
	fstream file("d7.txt");
	set<char> t;
	map<char, set<char>> d;
	map<char, int> l;
	string s;
	while (getline(file, s)) {
		char a = s[5], b = s[36];
		t.insert(a);
		t.insert(b);
		d[b].insert(a);
	}
	for (char c : t) { l[c] = 61 + c - 'A'; }
	while (!t.empty()) {
		for (auto& task : t) {
			if (d[task].empty()) {
				cout << task;
				for (auto& pair : d) {
					pair.second.erase(task);
				}
				t.erase(task);
				break;
			}
		}
	}
	cin.get();
}

template<> void day<8>() {
	fstream file("d8.txt");
	struct node {
		vector<node> children;
		vector<int> metadata;
		void operator<<(istream& s) {
			int c, m;
			s >> c >> m;
			for (int i = 0; i < c; i++) { children.emplace_back(); children.back() << s; }
			for (int i = 0; i < m; i++) { metadata.emplace_back(); s >> metadata.back(); }
		};
		int value = 0;
		bool hasValue = false;
		int getValue() {
			if (!hasValue) {
				hasValue = true;
				value = [this](){
					if (children.empty()) return accumulate(begin(metadata), end(metadata), 0);
					else {
						int total = 0;
						for (int m : metadata) {
							int i = m - 1;
							if (i >= 0 && i < children.size()) {
								total += children[i].getValue();
							}
						}
						return total;
					}
				}();
			}
			return value;
		}
	};
	node root;
	root << file;
	function<void(node&, const function<void(node&)>&)> forEachNode;
	forEachNode = [&forEachNode](node& root, const function<void(node&)>& f) {
		f(root);
		for (auto& node : root.children) {
			forEachNode(node, f);
		}
	};
	int total = 0;
	forEachNode(root, [&total](node& n) {
		for (int i : n.metadata) {
			total += i;
		}
	});
	int value = root.getValue();
	cout << total << '\n';
	cout << value << '\n';
	cin.get();
}

int main()
{
	day<7, 2>();
    return 0;
}

