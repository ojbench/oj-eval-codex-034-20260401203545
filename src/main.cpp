#include <bits/stdc++.h>
#include "skip_list.hpp"

using namespace std;

static inline bool is_integer_token(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

static inline string lower(const string& s){ string t=s; for(char& c:t) c=tolower((unsigned char)c); return t; }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SkipList<long long> sl;

    string tok;
    long long Q = -1;
    if (!(cin >> tok)) return 0;
    if (is_integer_token(tok)) {
        Q = stoll(tok);
    } else {
        // process this as op, then continue until EOF
        string op = lower(tok);
        long long x; if(!(cin>>x)) return 0; 
        if (op=="i"||op=="ins"||op=="insert"||op=="+"||op=="add") sl.insert(x);
        else if (op=="d"||op=="del"||op=="delete"||op=="erase"||op=="-") sl.deleteItem(x);
        else if (op=="s"||op=="search"||op=="find"||op=="?"||op=="q"||op=="query") cout<<(sl.search(x)?"true":"false")<<'\n';
        // Now stream rest
        while (cin >> op >> x) {
            op = lower(op);
            if (op=="i"||op=="ins"||op=="insert"||op=="+"||op=="add") sl.insert(x);
            else if (op=="d"||op=="del"||op=="delete"||op=="erase"||op=="-") sl.deleteItem(x);
            else if (op=="s"||op=="search"||op=="find"||op=="?"||op=="q"||op=="query") cout<<(sl.search(x)?"true":"false")<<'\n';
        }
        return 0;
    }

    for (long long i=0;i<Q;++i){
        string op; long long x; if(!(cin>>op>>x)) break; op=lower(op);
        if (op=="i"||op=="ins"||op=="insert"||op=="+"||op=="add") sl.insert(x);
        else if (op=="d"||op=="del"||op=="delete"||op=="erase"||op=="-") sl.deleteItem(x);
        else if (op=="s"||op=="search"||op=="find"||op=="?"||op=="q"||op=="query") cout<<(sl.search(x)?"true":"false")<<'\n';
    }
    return 0;
}
