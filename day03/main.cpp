#include <fstream>
#include <iostream>
#include <print>
#include <regex>
#include <string>

int main() {
    std::ifstream file("input.txt");

    std::string line{};
    std::regex multiplication_regex{
        "mul\\(\\d*\\,\\d*\\)|do\\(\\)|don\\'t\\(\\)"};

    std::smatch m{};
    int result_1 = 0, result_2 = 0;
    bool second_task_flag = true;

    while (getline(file, line)) {
        std::string::const_iterator start = line.begin();
        std::string::const_iterator end = line.end();

        while (std::regex_search(start, end, m, multiplication_regex)) {
            if (m[0].str() == "do()") {
                second_task_flag = true;
            } else if (m[0].str() == "don't()") {
                second_task_flag = false;
            } else {
                int a = 0, b = 0;
                std::string x = m[0].str();
                std::sscanf(x.c_str(), "mul(%d, %d)", &a, &b);

                result_1 += a * b;
                if (second_task_flag) result_2 += a * b;
            }

            start = m[0].second;
        }
    }

    std::printf("first task result: %d\n", result_1);
    std::printf("second task result: %d\n", result_2);

    return 0;
}
