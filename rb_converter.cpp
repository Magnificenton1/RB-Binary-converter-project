#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// konwersja rb na binarny - metoda tradycyjna
vector<int> convertConventional(const vector<int>& rb) {
    // tworzenie wektorów x+ i x-
    vector<int> x_plus(rb.size(), 0);
    vector<int> x_minus(rb.size(), 0);
    
    for (int i = 0; i < rb.size(); i++) {
        if (rb[i] == 1) {
            x_plus[i] = 1;
        } else if (rb[i] == -1) {
            x_minus[i] = 1;
        }
    }
    
    // odejmowanie x+ - x- z przeniesieniem
    vector<int> result(rb.size() + 1, 0);
    int borrow = 0;
    
    for (int i = 0; i < rb.size(); i++) {
        int diff = x_plus[i] - x_minus[i] - borrow;
        
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result[i] = diff;
    }
    
    // bit znaku
    result[rb.size()] = borrow;
    
    // odwrócenie kolejności bitów
    reverse(result.begin(), result.end());
    
    return result;
}

// konwersja rb na binarny - metoda z papieru (z C_i)
vector<int> convertProposed(const vector<int>& rb) {
    vector<int> binary(rb.size() + 1, 0);
    int C = 0; // początkowy stan C
    
    for (int i = 0; i < rb.size(); i++) {
        int S, D;
        
        // kodowanie cyfry rb na (S,D)
        if (rb[i] == 0) {
            S = 0; D = 0;
        } else if (rb[i] == 1) {
            S = 0; D = 1;
        } else { // -1
            S = 1; D = 1;
        }
        
        // obliczenie bitu i następnego C
        binary[i] = D ^ C;
        C = S | ((!D) & C);
    }
    
    // ostatni bit to C_n
    binary[rb.size()] = C;
    
    // odwrócenie kolejności bitów
    reverse(binary.begin(), binary.end());
    
    return binary;
}

int main() {
    srand(time(0));
    const int N = 100000;
    vector<int> rb(N);
    int zero = 0, one = 0, minone = 0;
    int current_streak = 1, longest_streak = 1;
    int prev = -2;

    // generowanie losowego rb
    for (int i = 0; i < N; ++i) {
        int r = rand() % 3 - 1;
        if (r == -1) minone++;
        else if (r == 0) zero++;
        else one++;
        if (prev != -2) {  // Not first iteration
            if (r == prev) {
                current_streak++;
                if (current_streak > longest_streak) {
                    longest_streak = current_streak;
                }
            } else {
                current_streak = 1;
            }
        }
        prev = r;
        rb[i] = r;
    }

    auto start1 = chrono::high_resolution_clock::now();
    auto conv1 = convertConventional(rb);
    auto end1 = chrono::high_resolution_clock::now();

    auto start2 = chrono::high_resolution_clock::now();
    auto conv2 = convertProposed(rb);
    auto end2 = chrono::high_resolution_clock::now();

    cout << fixed << setprecision(3);
    cout << "Time (Conventional): " 
         << chrono::duration<double, micro>(end1 - start1).count() << " us" << endl;

    cout << "Time (Proposed):     " 
         << chrono::duration<double, micro>(end2 - start2).count() << " us" << endl;

    cout << "\n 0 count: " << zero
    << "\n 1 count: " << one
    << "\n-1 count: " << minone << endl;

    cout << "\nThere was a streak of: " << longest_streak << "\nrepeating numbers in a row!\n";
    // porownanie
    bool match = true;
    for (int i = 0; i < conv1.size(); ++i) {
        if (conv1[i] != conv2[i]) {
            match = false;
            cout << "Mismatch at bit " << i << ": conv1 = "
                 << conv1[i] << ", conv2 = " << conv2[i] << endl;
            break;
        }
    }

    cout << "Output Match:        " << (match ? "YES" : "NO") << endl;

    return 0;
}
