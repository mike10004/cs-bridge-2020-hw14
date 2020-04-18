// mac937@nyu.edu
// hw14

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const bool VERBOSE = false;

void divide(size_t start, size_t length, size_t& outStart1, size_t& outLen1, size_t& outStart2, size_t& outLen2) {
    outStart1 = start;
    outLen1 = length / 2;
    outStart2 = start + outLen1;
    outLen2 = length - outLen1;
}

template<class T>
vector<T> findBounds(const vector<T>& values, size_t start, size_t length) {
    if (length == 0) {
        return vector<T>();
    }
    if (length == 1) {
        return vector<T>({values[start], values[start]});
    }
    if (length == 2) {
        int minOffset = values[start] < values[start + 1] ? 0 : 1;
        int maxOffset = minOffset == 0 ? 1 : 0;
        return vector<T>({values[start + minOffset], values[start + maxOffset]});
    }
    size_t leftStart, leftLength, rightStart, rightLength;
    divide(start, length, leftStart, leftLength, rightStart, rightLength);
    assert(leftLength + rightLength >= 3);
    vector<T> leftBounds = findBounds(values, leftStart, leftLength);
    vector<T> rightBounds = findBounds(values, rightStart, rightLength);
    vector<T> bounds({
        min(leftBounds[0], rightBounds[0]),
        max(leftBounds[1], rightBounds[1]),
    });
    return bounds;
}

template <class T>
vector<T> findBounds(const vector<T>& values) {
    return findBounds(values, 0, values.size());
}

template <class T>
void assertBoundsEqual(const vector<T>& bounds, T expectedMin, T expectedMax) {
    assert(bounds.size() == 2);
    if (expectedMin != bounds[0] || expectedMax != bounds[1]) {
        cerr << "expect [" << expectedMin << ", " << expectedMax << "] but got [" << bounds[0] << ", " << bounds[1] << "]" << endl;
    }
    assert(expectedMin == bounds[0]);
    assert(expectedMax == bounds[1]);
}

template<class T>
void testFindBounds(const vector<T>& values) {
    assert(!values.empty());
    bool pastFirst = false;
    T expectedMin = T();
    T expectedMax = T();
    for (const T& value : values) {
        if (!pastFirst || value < expectedMin) {
            expectedMin = value;
        }
        if (!pastFirst || value > expectedMax) {
            expectedMax = value;
        }
        pastFirst = true;
    }
    vector<T> bounds = findBounds(values);
    if (VERBOSE) {
        cout << "expect [" << expectedMin << ", " << expectedMax << "]; actual is [" << bounds[0] << ", " << bounds[1] << "]" << endl;
        cout << "values: ";
        for (T value : values) {
            cout << value << ' ';
        }
        cout << endl;
    }
    assertBoundsEqual(bounds, expectedMin, expectedMax);
}

#pragma clang diagnostic push                       // stage: cut
#pragma ide diagnostic ignored "cert-msc51-cpp"     // stage: cut
#pragma ide diagnostic ignored "cert-msc50-cpp"     // stage: cut
void testFindBounds() {
    vector<int> emptyInts = vector<int>();
    assert(findBounds(emptyInts).empty());
    vector<double> emptyDoubles = vector<double>();
    assert(findBounds(emptyDoubles).empty());
    assertBoundsEqual(findBounds<int>({1}), 1, 1);
    assertBoundsEqual(findBounds<double>({2.5}), 2.5, 2.5);
    assertBoundsEqual(findBounds<int>({-61, -8, 33, 1, 17}), -61, 33);
    int randIntMin = -100, randIntMax = 100;
    int numTrials = 100;
    unsigned int seed = time(nullptr);
    // stage: cut start
    char* seedStr = getenv("HW14_SEED");
    if (seedStr != nullptr) {
        seed = (int) strtol(seedStr, nullptr, 10);
    }
    // stage: cut stop
    srand(seed);
    for (int i = 2; i <= 10; i++) {
        for (int t = 0; t < numTrials; t++) {
            vector<int> intValues;
            vector<double> doubleValues;
            for (int j = 0; j < i; j++) {
                intValues.push_back(rand() % (randIntMax - randIntMin + 1) + randIntMin);
                doubleValues.push_back((double) rand() / RAND_MAX * 2.0 - 1.0);
            }
            testFindBounds(intValues);
            testFindBounds(doubleValues);
        }
    }
    testFindBounds<int>({5, 6});
    testFindBounds<int>({-2, 109, 16});
    testFindBounds<int>({1, 2, 3, 4});
    testFindBounds<int>({1, -2, -3, 4, -5});
    testFindBounds<int>({-2, 1, -2, -3, 4, -5});
    testFindBounds<int>({-2, 1, 7, -2, -3, 4, -5});
    testFindBounds<int>({-3, 39, -193, 0, 0, 38, 11, 7, 3, 66, 64, 23});
    testFindBounds<double>({-1.0, 5.5, -2.9, -2.99, 85239899.12, 1.3e12 -2.999, 0, 0.11});
}
#pragma clang diagnostic pop                        // stage: cut

void testDivide() {
    size_t s1, l1, s2, l2;
    divide(0, 0, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 0 && s2 == 0 && l2 == 0);
    divide(0, 1, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 0 && s2 == 0 && l2 == 1);
    divide(0, 2, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 1 && s2 == 1 && l2 == 1);
    divide(0, 3, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 1 && s2 == 1 && l2 == 2);
    divide(0, 4, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 2 && s2 == 2 && l2 == 2);
    divide(0, 5, s1, l1, s2, l2);
    assert(s1 == 0 && l1 == 2 && s2 == 2 && l2 == 3);
    // start at 10 instead of 0
    divide(10, 0, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 0 && s2 == 10 && l2 == 0);
    divide(10, 1, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 0 && s2 == 10 && l2 == 1);
    divide(10, 2, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 1 && s2 == 11 && l2 == 1);
    divide(10, 3, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 1 && s2 == 11 && l2 == 2);
    divide(10, 4, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 2 && s2 == 12 && l2 == 2);
    divide(10, 5, s1, l1, s2, l2);
    assert(s1 == 10 && l1 == 2 && s2 == 12 && l2 == 3);
}

int main() {
    testDivide();
    testFindBounds();
    return 0;
}
