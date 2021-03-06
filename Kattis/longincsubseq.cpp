#include <iostream>
#include <vector>
#include <set>

using namespace std;

template<class T>
int longest_increasing_subsequence(vector<T>& a, vector<int>& seq) {
	vector<int> lis(a.size(), 0), pre(a.size(), -1);
	int L = 0;
	for (int i = 0; i < a.size(); ++i) {
		int l = 1, r = L;
//	cerr << "BS with l=" << l << ", r=" << r << endl;
		while (l <= r) {
			int m = (l + r + 1) / 2;
			if (a[lis[m - 1]] < a[i])
				l = m + 1;
			else
				r = m - 1;
		}
//	cerr << "l is now " << l << ", lis[l - 2] is " << lis[l - 2] << endl;
	
		pre[i] = (l > 1 ? lis[l - 2] : -1);
		lis[l - 1] = i;
		if (l > L) L = l;
	}

	seq.assign(L, -1);
	int j = lis[L - 1];
	for (int i = L - 1; i >= 0; --i) {
		seq[i] = j;
		j = pre[j];
	}
	return L;
}

template<class T>
int longest_increasing_subsequence(vector<T>& a) {
	set<T> st;
	typename set<T>::iterator it;
	for (int i = 0; i < a.size(); ++i) {
		it = st.lower_bound(a[i]);
		if (it != st.end()) st.erase(it);
		st.insert(a[i]);
	}
	return st.size();
}

int main() {

	int n;
	vector<int> a, seq;
	while (EOF != scanf(" %d", &n)) {
		a.assign(n, -1);
		for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
		
		printf("%d\n", longest_increasing_subsequence(a, seq));
		for (int i = 0; i < int(seq.size()) - 1; ++i) {
			printf("%d ", seq[i]);
		}
		printf("%d\n", seq.back());
	}

	return 0;
}

