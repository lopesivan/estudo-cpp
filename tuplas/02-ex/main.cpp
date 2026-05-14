#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
   unordered_map<string, int> minhaHashtable = {
        {"um", 1}, {"dois", 2}, {"tres", 3}
    };
   cout << minhaHashtable["um"] 
        << " " 
        << minhaHashtable["dois"] 
        << " " 
        << minhaHashtable["tres"] 
        << endl;
   return 0;
}

