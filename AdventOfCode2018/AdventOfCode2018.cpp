

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
using namespace std;

template <int dayNumber, int part = 1> void day();

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
	day<8>();
    return 0;
}

