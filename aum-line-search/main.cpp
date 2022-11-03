#include "aumLineSearch.h"

using namespace std;

int main() {
    Line lines[] = {
            {3, -6},
            {2, -1},
            {1, 0},
    };
    double deltaFp[] = {3, -2, 1};
    double deltaFn[] = {2, 2, -1};
    double FP[3] = {0};
    double FN[3] = {0};
    double M[3] = {0};
    double stepSizeVec[12] = {0};
    double aumVec[12] = {0};
    lineSearch(lines, 3, deltaFp, deltaFn, FP, FN, M, 20, stepSizeVec, aumVec);

    cout << "FP: ";
    for (auto i: FP) { cout << i << " "; }
    cout << endl;
    cout << "FN: ";
    for (auto i: FN) { cout << i << " "; }
    cout << endl;
    return 0;
}

int main2() {
    Line lines[] = {
            {3, -5},
            {2, -2},
            {0.5, 1},
            {-1, 2},
    };
    double deltaFp[] = {3, -2, 1, 0};
    double deltaFn[] = {2, 2, -1, 0};
    double FP[4] = {0};
    double FN[4] = {0};
    double M[4] = {0};
    double stepSizeVec[12] = {0};
    double aumVec[12] = {0};
    lineSearch(lines, 4, deltaFp, deltaFn, FP, FN, M, 20, stepSizeVec, aumVec);

    cout << "FP: ";
    for (auto i: FP) { cout << i << " "; }
    cout << endl;
    cout << "FN: ";
    for (auto i: FN) { cout << i << " "; }
    cout << endl;
    return 0;
}
