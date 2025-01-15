#include <stdlib.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    std::vector<int> first_one{};
    std::vector<int> second_one{};
    std::ifstream input("input.txt");
    int a, b;

    while (std::getline(input, line)) {
        std::stringstream ss(line);

        ss >> a >> b;

        first_one.push_back(a);
        second_one.push_back(b);
    }

    std::sort(first_one.begin(), first_one.end());
    std::sort(second_one.begin(), second_one.end());

    int result = 0;

    for (int i = 0; i < first_one.size(); i++) {
        result += std::abs(first_one[i] - second_one[i]);
    }

    std::cout << result << "\n";
    return 0;
}
