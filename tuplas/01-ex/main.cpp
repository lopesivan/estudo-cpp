#include <iostream>
#include <tuple>
using namespace std;

int main() {
   tuple<string, int, double> minhaTupla("exemplo", 42, 3.14);
   cout << get<0>(minhaTupla) << " " << get<1>(minhaTupla) << " " << get<2>(minhaTupla) << endl;
   return 0;
}

