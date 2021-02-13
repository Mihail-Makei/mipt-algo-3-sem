#include <iostream>
#include <vector>
#include <string>

const int ALPHABET_SIZE = 200;

std::vector<int> suffix_array(const std::string& str) {
    int len = str.length();

    std::vector<int> count(std::max(ALPHABET_SIZE, len), 0);

    for (int i = 0; i < len; ++i) {
        ++count[str[i]];
    }

    for (int i = 1; i < ALPHABET_SIZE; ++i) {
        count[i] += count[i - 1];
    }

    std::vector<int> suffix_arr(len);

    for (int i = 0; i < len; ++i) {
        --count[str[i]];
        suffix_arr[count[str[i]]] = i;
    }

    std::vector<int> equiv_classes(len);

    equiv_classes[suffix_arr[0]] = 0;
    int classes_num = 1;

    for (int i = 1; i < len; ++i) {
        if (str[suffix_arr[i]] != str[suffix_arr[i - 1]]) {
            ++classes_num;
        }

        equiv_classes[suffix_arr[i]] = classes_num - 1;
    }

    std::vector<int> perm(len);
    std::vector<int> classes_temp(len);

    for (int deg = 0; (1 << deg) < len; ++deg) {
        for (int i = 0; i < len; ++i) {
            perm[i] = suffix_arr[i] - (1 << deg);
            if (perm[i] < 0) {
                perm[i] += len;
            }
        }

        fill(count.begin(), count.begin() + classes_num, 0);

        for (int i = 0; i < len; ++i) {
            ++count[equiv_classes[perm[i]]];
        }

        for (int i = 1; i < classes_num; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = len - 1; i >= 0; i--) {
            --count[equiv_classes[perm[i]]];
            suffix_arr[count[equiv_classes[perm[i]]]] = perm[i];
        }

        classes_temp[suffix_arr[0]] = 0;
        classes_num = 1;

        for (int i = 1; i < len; ++i) {
            if (equiv_classes[suffix_arr[i]] != equiv_classes[suffix_arr[i-1]] ||
                                equiv_classes[(suffix_arr[i] + (1 << deg)) % len] !=
                                equiv_classes[(suffix_arr[i-1] + (1 << deg)) % len]) {
                ++classes_num;
            }
            classes_temp[suffix_arr[i]] = classes_num - 1;
        }

        for (int i = 0; i < len; ++i) {
            equiv_classes[i] = classes_temp[i];
        }
    }

    return suffix_arr;
}


std::vector<int> lcp_array(const std::string& str, const std::vector<int>& suffix_arr) {
    int len = suffix_arr.size();

    std::vector<int> lcp_arr(len, 0);
    std::vector<int> inv_suffix_arr(len, 0);

    for (int i = 0; i < len; ++i) {
        inv_suffix_arr[suffix_arr[i]] = i;
    }

    int k = 0;
    for (int i = 0; i < len; ++i) {
        if (k > 0) {
            --k;
        }

        if (inv_suffix_arr[i] == len - 1) {
            lcp_arr[len - 1] = -1;
            k = 0;
            continue;
        }

        int temp = suffix_arr[inv_suffix_arr[i] + 1];
        while (std::max(i + k, temp + k) < len && str[i + k] == str[temp + k]) {
            ++k;
        }
        lcp_arr[inv_suffix_arr[i]] = k;
    }

    return lcp_arr;
}


int main() {
    int num = 0;
    std::cin >> num;

    std::string str;
    std::cin >> str;

    str += '$';

    std::vector<int> suffix_arr = suffix_array(str);
    std::vector<int> lcp_arr = lcp_array(str, suffix_arr);

    int maxlen = lcp_arr[0];
    for (int i = 1; i < lcp_arr.size(); ++i) {
        if (lcp_arr[i] > maxlen) {
            maxlen = lcp_arr[i];
        }
    }

    std::cout << maxlen << std::endl;

    return 0;
}
