// mac937@nyu.edu
// hw14 question 1

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

template<class T>
vector<T> findBounds(const vector<T>& values, size_t startInclusive, size_t endInclusive) {

}

template <class T>
vector<T> findBounds(const vector<T>& values) {
    if (values.empty()) {
        return vector<T>();
    }
    return findBounds(values, 0, values.size() - 1);
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
    assertBoundsEqual(bounds, expectedMin, expectedMax);
}

void testFindBounds() {
    vector<int> emptyInts = vector<int>();
    vector<int> emptyIntsBounds = findBounds(emptyInts);
    assert(emptyIntsBounds.empty());
    vector<double> emptyDoubles = vector<double>();
    vector<double> emptyDoublesBounds = findBounds(emptyDoubles);
    assert(emptyDoublesBounds.empty());
    vector<int> singletonInt({1});
    vector<int> singletonIntBounds = findBounds(singletonInt);
    assertBoundsEqual(singletonIntBounds, 1, 1);
    vector<double> singletonDouble({2.5});
    vector<double> singletonDoubleBounds = findBounds(singletonDouble);
    assertBoundsEqual(singletonDoubleBounds, 2.5, 2.5);
    testFindBounds<int>({1, 2, 3, 4});
    testFindBounds<int>({-3, 39, -193, 0, 0, 38, 11, 7, 3, 66, 64, 23});
    testFindBounds<double>({-1.0, 5.5, -2.9, -2.99, 85239899.12, 1.3e12 -2.999, 0, 0.11});
}

int main() {
    testFindBounds();
    return 0;
}
