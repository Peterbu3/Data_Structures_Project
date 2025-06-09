#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <numeric>

using namespace std;
using namespace std::chrono;

const int TESTS = 100;
const int TABLE_SIZE = 100007; // Liczba pierwsza

int hashFunc(int key) {
    return abs(key) % TABLE_SIZE;
}

int hashFunc2(int key) {
    return abs(key * 5) % TABLE_SIZE;
}

class ChainingHash {
    vector<list<int>> table;
public:
    ChainingHash() : table(TABLE_SIZE) {}

    void insert(int key) {
        table[hashFunc(key)].push_back(key);
    }

    bool remove(int key) {
        auto& chain = table[hashFunc(key)];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (*it == key) {
                chain.erase(it);
                return true;
            }
        }
        return false;
    }
};

class OpenHash {
    vector<int> table;
public:
    OpenHash() : table(TABLE_SIZE, -1) {}

    void insert(int key) {
        int index = hashFunc(key);
        while (table[index] != -1) {
            index = (index + 1) % TABLE_SIZE;
        }
        table[index] = key;
    }

    bool remove(int key) {
        int index = hashFunc(key);
        while (table[index] != -1) {
            if (table[index] == key) {
                table[index] = -1;
                return true;
            }
            index = (index + 1) % TABLE_SIZE;
        }
        return false;
    }
};

class ChainingTwoChoiceHash {
    vector<list<int>> table;
public:
    ChainingTwoChoiceHash() : table(TABLE_SIZE) {}

    void insert(int key) {
        int index1 = hashFunc(key);
        int index2 = hashFunc2(key);

        // Wybierz mniej obciążony koszyk
        int selected_index = (table[index1].size() <= table[index2].size()) ? index1 : index2;
        table[selected_index].push_back(key);
    }

    bool remove(int key) {
        // Sprawdź oba możliwe koszyki
        int index1 = hashFunc(key);
        auto& chain1 = table[index1];
        for (auto it = chain1.begin(); it != chain1.end(); ++it) {
            if (*it == key) {
                chain1.erase(it);
                return true;
            }
        }

        int index2 = hashFunc2(key);
        auto& chain2 = table[index2];
        for (auto it = chain2.begin(); it != chain2.end(); ++it) {
            if (*it == key) {
                chain2.erase(it);
                return true;
            }
        }

        return false;
    }
};

vector<int> genOptData(int n) {
    vector<int> data(n);
    for (int i = 0; i < n - 1; i++)
        data[i] = i + 1; // Wartości 1,2,3,...,n-1
    data.back() = n * 10; // Ostatni element: 10,100,1000,10000,100000
    return data;
}

vector<int> genAvgData(int n) {
    vector<int> data(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, n * 10);

    // Wypełniamy losowymi wartościami
    for (int i = 0; i < n - 1; i++) {
        data[i] = dist(gen);
    }

    data.back() = 100012;
    return data;
}

vector<int> genPesData(int n) {
    vector<int> data(n, 5); // Wszystkie elementy równe 5
    data.back() = 100012;
    return data;
}

void testCase(const string& name, int hashType, const vector<int>& data) {
    vector<long long> insertTimes, removeTimes;

    for (int i = 0; i < TESTS; i++) {
        if (hashType == 0) { // Separate Chaining
            ChainingHash ht;
            for (size_t j = 0; j < data.size() - 1; j++)
                ht.insert(data[j]);

            auto start = high_resolution_clock::now();
            ht.insert(data.back());
            auto mid = high_resolution_clock::now();
            ht.remove(data.back());
            auto end = high_resolution_clock::now();

            insertTimes.push_back(duration_cast<nanoseconds>(mid - start).count());
            removeTimes.push_back(duration_cast<nanoseconds>(end - mid).count());
        }
        else if (hashType == 1) { // Open Addressing
            OpenHash ht;
            for (size_t j = 0; j < data.size() - 1; j++)
                ht.insert(data[j]);

            auto start = high_resolution_clock::now();
            ht.insert(data.back());
            auto mid = high_resolution_clock::now();
            ht.remove(data.back());
            auto end = high_resolution_clock::now();

            insertTimes.push_back(duration_cast<nanoseconds>(mid - start).count());
            removeTimes.push_back(duration_cast<nanoseconds>(end - mid).count());
        }
        else { // Two-Choice Hashing
            ChainingTwoChoiceHash ht;
            for (size_t j = 0; j < data.size() - 1; j++) ht.insert(data[j]);

            auto start = high_resolution_clock::now();
            ht.insert(data.back());
            auto mid = high_resolution_clock::now();
            ht.remove(data.back());
            auto end = high_resolution_clock::now();

            insertTimes.push_back(duration_cast<nanoseconds>(mid - start).count());
            removeTimes.push_back(duration_cast<nanoseconds>(end - mid).count());
        }
    }

    long long avgInsert = accumulate(insertTimes.begin(), insertTimes.end(), 0LL) / TESTS;
    long long avgRemove = accumulate(removeTimes.begin(), removeTimes.end(), 0LL) / TESTS;

    cout << "  " << name << ":\n";
    cout << "    Insert: " << avgInsert << " ns\n";
    cout << "    Remove: " << avgRemove << " ns\n";
}

void testAllSizes(int n) {
    cout << "\n=== Testing " << n << " elements ===\n";

    cout << "SEPARATE CHAINING:\n";
    testCase("Optimistic", 0, genOptData(n));
    testCase("Average", 0, genAvgData(n));
    testCase("Pessimistic", 0, genPesData(n));

    cout << "OPEN ADDRESSING:\n";
    testCase("Optimistic", 1, genOptData(n));
    testCase("Average", 1, genAvgData(n));
    testCase("Pessimistic", 1, genPesData(n));

    cout << "TWO-CHOICE HASHING:\n";
    testCase("Optimistic", 2, genOptData(n));
    testCase("Average", 2, genAvgData(n));
    testCase("Pessimistic", 2, genPesData(n));
}

int main() {
    vector<int> sizes = { 10, 100, 1000, 10000, 100000 };

    for (int size : sizes) {
        testAllSizes(size);
    }

    return 0;
}
