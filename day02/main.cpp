#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool check_line(std::vector<int> line) { return true; }

int main() {
    std::ifstream input_file("input.txt");
    std::string line;
    int result = 0;
    int a = 0, b = 0;

    while (std::getline(input_file, line)) {
        std::stringstream ss(line);
        // bool decreasing = true, valid = true;

        int increasing_no = 0;
        int decreasing_no = 0;
        int invalid_counter = 0;

        ss >> a >> b;
        if (a == b || std::abs(a - b) > 3) {
            invalid_counter++;
        } else if (b > a) {
            increasing_no++;
            a = b;
        } else {
            decreasing_no++;
            a = b;
        }

        for (; ss >> b;) {
            /* if ((b == a || std::abs(b - a) > 3) ||
                (decreasing == true && b < a) ||
                (decreasing == false && b > a)) {
                valid = false;
                break;
            } */

            if (b > a && !(b == a || std::abs(b - a) > 3)) {
                increasing_no++;
                a = b;
            } else if ((b < a) && !(b == a || std::abs(b - a) > 3)) {
                decreasing_no++;
                a = b;
            } else if (b == a || std::abs(b - a) > 3) {
                invalid_counter++;
            }
        }

        if ((increasing_no == 0 || decreasing_no == 0) &&
            invalid_counter <= 1) {
            result++;
        } else if (invalid_counter == 0 &&
                   (increasing_no == 1 || decreasing_no == 1)) {
            result++;
        }
    }

    printf("Taks 01: %d\n", result);
    return 0;
}
