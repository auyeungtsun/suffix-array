#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

// Structure to hold the Suffix Array and LCP Array
struct SuffixArrayResult {
    vector<int> sa;  // Suffix Array: sorted indices of suffixes
    vector<int> lcp; // LCP Array: lcp[i] is LCP of sa[i] and sa[i-1]
};

/**
 * @brief Builds the Suffix Array and Longest Common Prefix (LCP) array for a given text.
 *
 * @param text The input string for which to build the suffix array and LCP array.
 * @return SuffixArrayResult A struct containing the Suffix Array and LCP array.
 *                           Returns empty vectors for an empty input string.
 *
 * @note Time Complexity: O(N log N) for building the Suffix Array (due to sorting).
 * @note Space Complexity: O(N) for storing Suffix Array, LCP Array, and intermediate rank arrays.
 */
SuffixArrayResult buildSuffixArrayAndLCP(const string& text) {
    int n = text.length();

    if (n == 0) {
        return {{}, {}};
    }

    vector<int> sa(n);          // Suffix array (stores original indices)
    vector<int> rank_array(n);  // rank_array[i] = rank of suffix starting at i
    vector<int> temp_rank_array(n); // Temporary array for ranks in next step

    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank_array[i] = text[i]; // Using char ASCII values as initial ranks
    }

    // 'gap' represents 2^k, the length of prefixes being considered for sorting
    for (int gap = 1; ; gap *= 2) {
        auto compare_suffixes = [&](int i, int j) {
            if (rank_array[i] != rank_array[j]) {
                return rank_array[i] < rank_array[j];
            }
            int ri_next = (i + gap < n) ? rank_array[i + gap] : -1;
            int rj_next = (j + gap < n) ? rank_array[j + gap] : -1;
            return ri_next < rj_next;
        };

        sort(sa.begin(), sa.end(), compare_suffixes);

        temp_rank_array[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            temp_rank_array[sa[i]] = temp_rank_array[sa[i-1]] +
                                     (compare_suffixes(sa[i-1], sa[i]) ? 1 : 0);
        }

        for (int i = 0; i < n; ++i) {
            rank_array[i] = temp_rank_array[i];
        }

        if (rank_array[sa[n-1]] == n - 1) {
            break;
        }
    }

    // Construct LCP array using Kasai's algorithm (O(n))
    vector<int> lcp(n, 0);
    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank_array[i] > 0) {
            int prev_sa_idx = sa[rank_array[i] - 1];

            // Calculate LCP between text[i...] and text[prev_sa_idx...]
            while (i + h < n && prev_sa_idx + h < n && text[i+h] == text[prev_sa_idx+h]) {
                h++;
            }
            lcp[rank_array[i]] = h;

            if (h > 0) {
                h--;
            }
        }
    }

    return {sa, lcp};
}

void testBuildSuffixArrayAndLCP() {
    string test_str;
    SuffixArrayResult result;

    // Test case 1: "banana"
    test_str = "banana";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({5, 3, 1, 0, 4, 2}));
    assert(result.lcp == vector<int>({0, 1, 3, 0, 0, 2}));
    cout << "Test case 1 passed." << endl;

    // Test case 2: "ababa"
    test_str = "ababa";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({4, 2, 0, 3, 1}));
    assert(result.lcp == vector<int>({0, 1, 3, 0, 2}));
    cout << "Test case 2 passed." << endl;

    // Test case 3: "aaaaa"
    test_str = "aaaaa";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({4, 3, 2, 1, 0}));
    assert(result.lcp == vector<int>({0, 1, 2, 3, 4}));
    cout << "Test case 3 passed." << endl;

    // Test case 4: "abcde"
    test_str = "abcde";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({0, 1, 2, 3, 4}));
    assert(result.lcp == vector<int>({0, 0, 0, 0, 0}));
    cout << "Test case 4 passed." << endl;

    // Test case 5: "" (empty string)
    test_str = "";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({}));
    assert(result.lcp == vector<int>({}));
    cout << "Test case 5 passed." << endl;

    // Test case 6: "a"
    test_str = "a";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({0}));
    assert(result.lcp == vector<int>({0}));
    cout << "Test case 6 passed." << endl;
    
    // Test case 7: "mississippi"
    test_str = "mississippi";
    result = buildSuffixArrayAndLCP(test_str);
    assert(result.sa == vector<int>({10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2}));
    assert(result.lcp == vector<int>({0, 1, 1, 4, 0, 0, 1, 0, 2, 1, 3}));
    cout << "Test case 7 passed." << endl;

    cout << "\nAll tests passed successfully!" << endl;
}

void runBuildSuffixArrayAndLCPSample() {
    string sample_text = "banana";
    cout << "Building Suffix Array and LCP for: \"" << sample_text << "\"" << endl;
    SuffixArrayResult result = buildSuffixArrayAndLCP(sample_text);

    cout << "Suffix Array (SA): ";
    for (int index : result.sa) {
        cout << index << " ";
    }
    cout << endl;

    cout << "LCP Array (LCP): ";
    for (int lcp_val : result.lcp) {
        cout << lcp_val << " ";
    }
    cout << endl;

    cout << "\nSuffixes in Lexicographical Order:" << endl;
    for (int i = 0; i < result.sa.size(); ++i) {
        cout << "SA[" << i << "]=" << result.sa[i] << ", LCP[" << i << "]=" << result.lcp[i] << ": "
             << sample_text.substr(result.sa[i]) << endl;
    }
    cout << endl;
}

int main() {
    testBuildSuffixArrayAndLCP();
    runBuildSuffixArrayAndLCPSample();
    return 0;
}