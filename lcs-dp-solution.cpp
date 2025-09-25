// LCS - Longest Common Subsequence
// Programação Dinâmica
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // std::remove_if
#include <cctype> // std::isspace
#include <chrono>

using namespace std;
using namespace std::chrono;

int lcs_dp(string a, string b, int x, int y) {
    // Dynamically allocate a 2D array (DP table)
    int** dp = new int*[x + 1];
    for (int i = 0; i <= x; i++) {
        dp[i] = new int[y + 1];
    }

    for (int i = 0; i <= x; i++) {
        for (int j = 0; j <= y; j++) {
            if (i == 0 || j == 0) { // f(0, :) = f(:, 0) = 0
                dp[i][j] = 0;
            } else if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int result = dp[x][y];

    // Free allocated memory
    for (int i = 0; i <= x; i++) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}


int main() {

    string fileName = "Strings100.txt";
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo: " << fileName << endl;
        return 1;
    }
    string a, b;
    // Ler as duas strings do arquivo
    getline(file, a);
    getline(file, b);

    // Remover espaços em branco
    a.erase(std::remove_if(a.begin(), a.end(), ::isspace), a.end());
    b.erase(std::remove_if(b.begin(), b.end(), ::isspace), b.end());

    int x = a.length();
    int y = b.length();

    lcs_dp(a, b, x, y);
   
    return 0;
}