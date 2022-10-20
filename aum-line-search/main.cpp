#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <unordered_set>

using namespace std;

struct Line {
    double slope;
    double intercept;
};

struct Point {
    double x;
    double y;
};

bool operator==(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

Point intersect(Line a, Line b) {
    double x = (b.intercept - a.intercept) / (a.slope - b.slope);
    double y = a.intercept + a.slope * x;
    return Point{x, y};
}

struct IntersectionData {
    Point point;
    // "low" line is the line below the other before the intersection point
    int lineLowBeforeIntercept;
    // "high" line is the line above the other after the intersection point
    int lineHighBeforeIntercept;
};

bool operator<(IntersectionData a, IntersectionData b) {
    return a.point.x < b.point.x;
}

bool operator==(IntersectionData a, IntersectionData b) {
    return a.point == b.point && (
            (a.lineHighBeforeIntercept == b.lineHighBeforeIntercept && a.lineLowBeforeIntercept == b.lineLowBeforeIntercept) ||
            (a.lineHighBeforeIntercept == b.lineLowBeforeIntercept && a.lineLowBeforeIntercept == b.lineHighBeforeIntercept)
    );
}

bool isFinite(Point point) {
    return isfinite(point.x) && isfinite(point.y);
}

bool compareIntersections(IntersectionData &a, IntersectionData &b) {
    return a < b;
}

/// custom compare function for testing the equality of two IntersectionData structs
/// to be used in sets
bool intersectionDataSetEquality(const IntersectionData &a, const IntersectionData &b) {
    return a < b;
}

// test function, not used in algorithm below
vector<IntersectionData> findIntersections(vector<Line> lines) {
    vector<IntersectionData> intersections;
    for (int a = 0; a < lines.size(); a++) {
        for (int b = a + 1; b < lines.size(); b++) {
            if (a != b) {
                Point point = intersect(lines[a], lines[b]);
                IntersectionData data = {point, a, b};
                intersections.push_back(data);
            }
        }
    }
    sort(intersections.begin(), intersections.end(), compareIntersections);
    return intersections;
}

struct CheckedLines {
    int low, high;
};

bool operator==(CheckedLines a, CheckedLines b) {
    return a.low == b.low && a.high == b.high;
}

namespace std {
    template<>
    struct hash<CheckedLines> {
        size_t operator()(const CheckedLines &data) const noexcept {
            return ((uint64_t) data.low) << 32 | ((uint64_t) data.high);
        }
    };
}


void queueIntersection(
        const Line *lines,
        multiset<IntersectionData> &intersections,
        unordered_set<CheckedLines> &checkedIntersections,
        int lowLine,
        int highLine
) {
    auto intersectionPoint = intersect(
            lines[lowLine],
            lines[highLine]
    );
    // intersection points with infinite values aren't real intersections
    if (isFinite(intersectionPoint)) {
        auto intersection = IntersectionData{
                intersectionPoint, lowLine, highLine
        };
        if (checkedIntersections.find(CheckedLines{.low = lowLine, .high = highLine}) != checkedIntersections.end()) {
            return;
        } else {
            cout << "already checkedIntersections" << endl;
            for (const auto &item : checkedIntersections) {
                cout << "  - " << item.low << " " << item.high << endl;
            }
        }

        cout << "queueing intersection point " << intersection.point.x << " " << intersection.point.y << " "
             << intersection.lineHighBeforeIntercept << " " << intersection.lineLowBeforeIntercept << endl;
        intersections.insert(intersection);
    }
}

/**
 * Line Search
 * @param lines lines array
 * @param lineCount size of the lines array
 * @param deltaFp input FP, assumed length == lineCount
 * @param deltaFn input FN, assumed length == lineCount
 * @param FP destination for FP, assumed length == lineCount
 * @param FN destination for FN, assumed length == lineCount
 * @param maxIterations
 */
void lineSearch(
        const Line *lines,
        int lineCount,
        const int *deltaFp,
        const int *deltaFn,
        long *FP,
        long *FN,
        long maxIterations
) {
    // a list of indices of lines
    // this is updated as we move across the X axis, passing intersection points changes the indices
    vector<int> sortedIndices;
    sortedIndices.reserve(lineCount);
    for (int i = 0; i < lineCount; i++) {
        sortedIndices.push_back(i);
    }

    unordered_set<CheckedLines> checkedIntersections;
    multiset<IntersectionData> intersections;
    // start by queueing intersections of every line and the line after it
    for (int a = 0; a < lineCount - 1; a++) {
        Point point = intersect(lines[a], lines[a + 1]);
        // parallel lines will be infinite
        if (isFinite(point)) {
            cout << "starting with intersection point " << point.x << " " << point.y << endl;
            int lineIndexLowBeforeIntercept;
            int lineIndexHighBeforeIntercept;
            if (lines[a].intercept < lines[a + 1].intercept) {
                // (a) is below (a + 1) before the intersection point
                lineIndexLowBeforeIntercept = a;
                lineIndexHighBeforeIntercept = a + 1;
            } else {
                // (a + 1) is below (a) before the intersection point
                lineIndexLowBeforeIntercept = a + 1;
                lineIndexHighBeforeIntercept = a;
            }
            intersections.insert(IntersectionData{point, lineIndexLowBeforeIntercept, lineIndexHighBeforeIntercept});
            checkedIntersections.insert(
                    CheckedLines{.low = lineIndexHighBeforeIntercept, .high = lineIndexLowBeforeIntercept});
        }
    }

    long iterations = 0;
    while (!intersections.empty() && iterations++ < maxIterations) {
        // TODO: handle multiple intersections with the same x coordinate at once
        auto intersection = intersections.begin();
        cout << "using intersection point " << intersection->point.x << " " << intersection->point.y << " "
             << intersection->lineHighBeforeIntercept << " " << intersection->lineLowBeforeIntercept << endl;
        // swap the indices of the lines that make this intersection point
        int lineIndexHighAfterIntercept = (int) distance(
                sortedIndices.begin(),
                find(sortedIndices.begin(), sortedIndices.end(), intersection->lineLowBeforeIntercept)
        );
        int lineIndexLowAfterIntercept = (int) distance(
                sortedIndices.begin(),
                find(sortedIndices.begin(), sortedIndices.end(), intersection->lineHighBeforeIntercept)
        );
        swap(sortedIndices[lineIndexHighAfterIntercept], sortedIndices[lineIndexLowAfterIntercept]);

        cout << " sorted indices: ";
        for (auto i: sortedIndices)
            cout << i << ' ';
        cout << endl;

        // indices of the next lines we want to find intersections for
        int higherLineIndex = intersection->lineHighBeforeIntercept - 1;
        int lowerLineIndex = intersection->lineLowBeforeIntercept + 1;

        cout << " 0higherLineIndex: " << higherLineIndex << " (line " << sortedIndices[higherLineIndex] << ")" << endl;
        cout << " 1lineHighAfterIntercept: " << lineIndexHighAfterIntercept << " (line "
             << intersection->lineLowBeforeIntercept << ")" << endl;
        cout << " 2lineLowAfterIntercept: " << lineIndexLowAfterIntercept << " (line "
             << intersection->lineHighBeforeIntercept << ")" << endl;
        cout << " 3lowerLineIndex: " << lowerLineIndex << " (line " << sortedIndices[lowerLineIndex] << ")" << endl;

        // (∆FP of top line) - (∆FP of bottom line)
        long deltaDeltaFp = deltaFp[sortedIndices[lineIndexHighAfterIntercept]] -
                            deltaFp[sortedIndices[lineIndexLowAfterIntercept]];
        long deltaDeltaFn = deltaFn[sortedIndices[lineIndexHighAfterIntercept]] -
                            deltaFn[sortedIndices[lineIndexLowAfterIntercept]];

        // update FP & FN
        FP[lineIndexHighAfterIntercept] = deltaDeltaFp;
        FN[lineIndexHighAfterIntercept] = deltaDeltaFn;

        checkedIntersections.insert(CheckedLines{.low = intersection->lineLowBeforeIntercept, .high = intersection->lineHighBeforeIntercept});
        intersections.erase(intersection);
        cout << "Intersections: " << intersections.size() << endl;

        // queue these in the multiset
        // this creates an intersection between "lineIndexHighAfterIntercept" and "higherLineIndex"
        // "lineIndexHighAfterIntercept" will now be the index of the low line before this new intersection point
        if (higherLineIndex > 0) {
            queueIntersection(lines, intersections, checkedIntersections,
                              lineIndexHighAfterIntercept, sortedIndices[higherLineIndex]);
        }
        if (lowerLineIndex < lineCount) {
            queueIntersection(lines, intersections, checkedIntersections,
                              lineIndexLowAfterIntercept, sortedIndices[lowerLineIndex]);
        }
    }
}

int main0() {
    Line lines[] = {
            {0, 3},
            {0, 2},
            {0, 1},
            {1, -1},
            {1, -2},
            {1, -3},
    };
    int deltaFp[] = {0, 0, 0, 0, 0, 0, 0};
    int deltaFn[] = {0, 0, 0, 0, 0, 0, 0};
    long FP[6];
    long FN[6];
    lineSearch(lines, 6, deltaFp, deltaFn, FP, FN, 10);
    return 0;
}

int main() {
    Line lines[] = {
            {1, 0},
            {2, -1},
            {3, -6},
    };
    int deltaFp[] = {0, 0, 0, 0, 0, 0, 0};
    int deltaFn[] = {0, 0, 0, 0, 0, 0, 0};
    long FP[6];
    long FN[6];
    lineSearch(lines, 3, deltaFp, deltaFn, FP, FN, 20);
    return 0;
}
