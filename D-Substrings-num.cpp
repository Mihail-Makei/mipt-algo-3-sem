#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


typedef struct {
    int index;
    int rank[2];
} suffix;

bool cmp(suffix suff1, suffix suff2) {
    if (suff1.rank[0] == suff2.rank[0])
        return suff1.rank[1] < suff2.rank[1];
    else
        return suff1.rank[0] < suff2.rank[0];
}

std::vector<int> buildSuffixArray(std::string str, int n) {
    suffix suffices[n];
    for (int i = 0; i < n; ++i) {
        suffices[i].index = i;
        suffices[i].rank[0] = str[i] - 'a';
        suffices[i].rank[1] = (((i+1) < n) ? (str[i + 1] - 'a') : -1);
    }

    std::sort(suffices, n + suffices, cmp);

    int ind[n];

    for (int k = 4; k < n * 2; k <<= 1) {
        int rank = 0;
        int prev_rank = suffices[0].rank[0];

        suffices[0].rank[0] = rank;
        ind[suffices[0].index] = 0;

        for (int i = 1; i < n; i++) {
            if ((suffices[i].rank[0] == prev_rank) && (suffices[i].rank[1] == suffices[i-1].rank[1])) {
                prev_rank = suffices[i].rank[0];
                suffices[i].rank[0] = rank;
            } else {
                prev_rank = suffices[i].rank[0];
                suffices[i].rank[0] = ++rank;
            }

            ind[suffices[i].index] = i;
        }

        for (int i = 0; i < n; ++i) {
            int nextIndex = suffices[i].index + k / 2;
            suffices[i].rank[1] = ((nextIndex < n) ? suffices[ind[nextIndex]].rank[0] : -1);
        }

        std::sort(suffices, suffices + n, cmp);
    }

    std::vector<int> SuffixArray;
    for (int i = 0; i < n; ++i)
        SuffixArray.push_back(suffices[i].index);

    return SuffixArray;
}



std::vector<int> buildLCP(std::string str, std::vector<int> SuffixArray) {
    size_t n = SuffixArray.size();
    std::vector<int> LCPArray(n, 0);
    std::vector<int> rSuffixArray(n, 0);

    for (size_t i = 0; i < n; ++i)
        rSuffixArray[SuffixArray[i]] = static_cast<int>(i);

    int k = 0;

    for (size_t i = 0; i < n; ++i) {
        if (rSuffixArray[i] == static_cast<int>(n) - 1) {
            k = 0;
            continue;
        }

        int j = SuffixArray[rSuffixArray[i] + 1];
        while ((i + k < n) && (j + k < n) && (str[i + k] == str[j + k])) {
            ++k;
        }

        LCPArray[rSuffixArray[i]] = k;

        if (k > 0)
            --k;
    }

    return LCPArray;
}

int countDistinctSubstr(const std::string& str) {
    int num = str.length();
    std::vector<int> SuffixArray = buildSuffixArray(str, num);
    std::vector<int> LCPArray = buildLCP(str, SuffixArray);
    int res = num - SuffixArray[0];

    for (int i = 1; i < LCPArray.size(); ++i)
        res += (num - SuffixArray[i]) - LCPArray[i - 1];

    ++res;

    return res;
}

int main() {
    std::string str;
    std::cin >> str;

    int answ = countDistinctSubstr(str);
    --answ;

    printf("%d", answ);
}
