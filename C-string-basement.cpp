#include <iostream>
#include <string>
#include <vector>

std::vector<int> prefix_function(const std::string& str) {
    std::vector<int> prefix_func(str.length());
    prefix_func[0] = 0;

    for (int i = 1; i < str.length(); ++i) {
        int k = prefix_func[i - 1];

        while (k > 0 && str[i] != str[k]) {
            k = prefix_func[k - 1];
        }

        if (str[i] == str[k]) {
            ++k;
        }

        prefix_func[i] = k;
    }

    return prefix_func;
}

int main() {
    std::string str;
    std::cin >> str;

    std::vector<int> prefix_func = prefix_function(str);

    std::cout << str.length() - prefix_func[str.length() - 1] << std::endl;

    return 0;
}
