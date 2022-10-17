#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

struct Line {
    double slope;
    double intercept;
};

struct Point {
    double x;
    double y;
};

Point intersect(Line a, Line b) {
    double x = (b.intercept - a.intercept) / (a.slope - b.slope);
    double y = a.intercept + a.slope * x;
    return Point{x, y};
}

struct IntersectionData {
    Point point;
    // "low" line is the line below the other before the intersection point
    int lineIndexLowBeforeIntercept;
    // "high" line is the line above the other after the intersection point
    int lineIndexHighBeforeIntercept;
};

void queueIntersection(const std::vector<int> &sortedIndices, std::multiset<IntersectionData> &intersections,
                       long lowLineIndex, long highLineIndex, Point &intersectionPoint);

bool operator<(IntersectionData a, IntersectionData b) {
    return a.point.x < b.point.x;
}

bool isFinite(Point point) {
    return std::isfinite(point.x) && std::isfinite(point.y);
}

bool compareIntersections(IntersectionData &a, IntersectionData &b) {
    return a < b;
}

// test function, not used in algorithm below
std::vector<IntersectionData> findIntersections(std::vector<Line> lines) {
    std::vector<IntersectionData> intersections;
    for (int a = 0; a < lines.size(); a++) {
        for (int b = a + 1; b < lines.size(); b++) {
            if (a != b) {
                Point point = intersect(lines[a], lines[b]);
                IntersectionData data = {point, a, b};
                intersections.push_back(data);
            }
        }
    }
    std::sort(intersections.begin(), intersections.end(), compareIntersections);
    return intersections;
}

void test(
        const std::vector<Line> &lines,
        const std::vector<int> &deltaFp,
        const std::vector<int> &deltaFn,
        long maxIterations
) {
    // a list of indices of lines
    // this is updated as we move across the X axis, passing intersection points changes the indices
    std::vector<int> sortedIndices;
    sortedIndices.reserve(lines.size());
    for (int i = 0; i < lines.size(); i++) {
        sortedIndices.push_back(i);
    }

    std::vector<long> FP;
    FP.reserve(lines.size());

    std::vector<long> FN;
    FN.reserve(lines.size());

    std::multiset<IntersectionData> intersections;
    // start by queueing intersections of every line and the line after it
    for (int a = 0; a < lines.size() - 1; a++) {
        Point point = intersect(lines[a], lines[a + 1]);
        // parallel lines will be infinite
        if (isFinite(point)) {
            intersections.insert(IntersectionData{point, a, a + 1});
        }
    }

    long iterations = 0;
    while (!intersections.empty() && iterations++ < maxIterations) {
        // TODO: handle multiple intersections with the same x coordinate at once
        auto intersection = intersections.begin();
        // swap the indices of the lines that make this intersection point
        long lineIndexHighAfterIntercept = distance(
                sortedIndices.begin(),
                find(sortedIndices.begin(), sortedIndices.end(), intersection->lineIndexLowBeforeIntercept)
        );
        long lineIndexLowAfterIntercept = distance(
                sortedIndices.begin(),
                find(sortedIndices.begin(), sortedIndices.end(), intersection->lineIndexHighBeforeIntercept)
        );
        std::swap(sortedIndices[lineIndexHighAfterIntercept], sortedIndices[lineIndexLowAfterIntercept]);

        // indices of the next lines we want to find intersections for
        long higherLineIndex = lineIndexHighAfterIntercept + 1;
        long lowerLineIndex = lineIndexLowAfterIntercept - 1;

        // (∆FP of top line) - (∆FP of bottom line)
        long deltaDeltaFp = deltaFp[sortedIndices[lineIndexHighAfterIntercept]] -
                            deltaFp[sortedIndices[lineIndexLowAfterIntercept]];
        long deltaDeltaFn = deltaFn[sortedIndices[lineIndexHighAfterIntercept]] -
                            deltaFn[sortedIndices[lineIndexLowAfterIntercept]];

        // update FP & FN
        FP.insert(FP.begin() + lineIndexHighAfterIntercept, deltaDeltaFp);
        FN.insert(FN.begin() + lineIndexHighAfterIntercept, deltaDeltaFn);

        // create new intersections
        Point topIntersectionPoint = intersect(
                lines[sortedIndices[higherLineIndex]],
                lines[sortedIndices[lineIndexHighAfterIntercept]]
        );
        Point bottomIntersectionPoint = intersect(
                lines[sortedIndices[lowerLineIndex]],
                lines[sortedIndices[lineIndexLowAfterIntercept]]
        );

        // queue these in the multiset
        // this creates an intersection between "lineIndexHighAfterIntercept" and "higherLineIndex"
        // "lineIndexHighAfterIntercept" will now be the index of the low line before this new intersection point
        queueIntersection(sortedIndices, intersections,
                          lineIndexHighAfterIntercept, higherLineIndex,
                          topIntersectionPoint);
        queueIntersection(sortedIndices, intersections,
                          lowerLineIndex, lineIndexLowAfterIntercept,
                          bottomIntersectionPoint);
    }
}

void queueIntersection(const std::vector<int> &sortedIndices, std::multiset<IntersectionData> &intersections,
                       long lowLineIndex, long highLineIndex, Point &intersectionPoint) {
    // intersection points with infinite values aren't real intersections
    if (isFinite(intersectionPoint)) {
        auto intersection = IntersectionData{
                intersectionPoint, sortedIndices[lowLineIndex], sortedIndices[highLineIndex]
        };
        intersections.insert(intersection);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
