#include <iostream>
 
int main() {
    std::cout.sync_with_stdio(0);
    unsigned n;
    std::cin >> n;
    auto revBit = [&n]() -> int
    {
    	unsigned r;
    	for (r = 0; n; n >>= 1)
        {
           r <<= 1;
           r |= n & 1;
        }
        return r;
    };
    std::cout << revBit();
}
