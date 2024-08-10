#include "jie_alloc.h"
#include <vector>
#include <iostream>

int main() {

    uint i;
    int ia[5] = { 0, 1, 2, 3, 4 };
    std::vector<int, JJ::allocator<int>> iv(ia, ia+5);

    for (int i=0; i<iv.size(); ++i) {
        std::cout << iv[i] << " ";
    }
    std::cout << std::endl;
}