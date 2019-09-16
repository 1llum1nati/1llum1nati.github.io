#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main()
{
    /*unordered_set<int> a;
    for (int i = 0; i < 1000000; i++) {
        a.insert(i);
    }
    */
    vector<int> a;
    for (int i = 0; i < 1000000; i++) {
        a.push_back(i);
    }
    //for (auto &b: a)
        //cout << b << endl;
    if(find(a.begin(), a.end(), 799960) != a.end())
        cout << "found";
    //if (a.find(799960) != a.end())
        //cout << "found";
}
