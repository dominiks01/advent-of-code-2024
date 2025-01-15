#include <stdlib.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::string line;
    std::vector<int> first_one{};
    std::unordered_map<int, int> second_one{};
    std::ifstream input("input.txt");
    int a, b;

    while (std::getline(input, line)) {
        std::stringstream ss(line);

        ss >> a >> b;

        first_one.push_back(a);
        auto result = second_one.try_emplace(b, 1);

        if (!result.second) {
            result.first->second += 1;
        }
    }

    int result = 0;

    for (int i = 0; i < first_one.size(); i++) {
        result += std::abs(first_one[i] * second_one[first_one[i]]);
    }

    std::cout << result << "\n";
    return 0;
}
