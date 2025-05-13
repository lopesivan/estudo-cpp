#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main()
{
    std::vector<int> source(10);
    std::iota(source.begin(), source.end(), 0);

    std::vector<int> destination;

    std::ranges::copy(source.begin(), source.end(),
                      std::back_inserter(destination));
// or, alternatively,
//  std::vector<int> destination(source.size());
//  std::ranges::copy(source.begin(), source.end(), destination.begin());
// either way is equivalent to
//  std::vector<int> destination = source;

    std::cout << "destination contains: ";

    std::ranges::copy(destination, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    std::cout << "odd numbers in destination are: ";

    std::ranges::copy_if(destination, std::ostream_iterator<int>(std::cout, " "),
                         [](int x) { return (x % 2) == 1; });
    std::cout << '\n';
}
