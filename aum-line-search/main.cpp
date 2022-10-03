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
    int lineA;
    int lineB;
};

bool operator<(IntersectionData a, IntersectionData b) {
    return a.point.x < b.point.x;
}

bool isFinite(Point point) {
    return std::isfinite(point.x) && std::isfinite(point.y);
}

bool compareIntersections(IntersectionData &a, IntersectionData &b) {
    return a < b;
}

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
        const std::vector<int> &deltaFn
) {

    // Q is a list of indices of lines
    // Q is updated as we move across the X axis, passing intersection points changes the indices in Q
    std::vector<int> Q;
    Q.reserve(lines.size());
    for (int i = 0; i < lines.size(); i++) {
        Q.push_back(i);
    }

    std::vector<long> FP;
    FP.reserve(lines.size());

    std::vector<long> FN;
    FN.reserve(lines.size());

    std::multiset<IntersectionData> intersections;
    // start by queueing intersections of every line and the line after it
    for (int a = 0; a < lines.size() - 1; a++) {
        Point point = intersect(lines[a], lines[a + 1]);
        intersections.insert(IntersectionData{point, a, a + 1});
    }

    while (!intersections.empty()) {
        // TODO: handle multiple intersections with the same x coordinate at once
        auto intersection = intersections.begin();
        // swap the indices of the lines that make this intersection point
        long lineIndexA = distance(Q.begin(), find(Q.begin(), Q.end(), intersection->lineA));
        long lineIndexB = distance(Q.begin(), find(Q.begin(), Q.end(), intersection->lineB));
        std::swap(Q[lineIndexA], Q[lineIndexB]);

        // will be 1 if lineIndexA is after lineIndexB
        // will be -1 if lineIndexA is before lineIndexB
        long diff = lineIndexA - lineIndexB;
        // indices of the lines we want to find intersections for
        long higherLineIndex = lineIndexA + diff;
        long lowerLineIndex = lineIndexB - diff;

        // if A > B: ((∆FP of A) * 1) − ((∆FP of B) * 1)
        // if A < B: ((∆FP of A) * -1) − ((∆FP of B) * -1)
        long deltaDeltaFp = (deltaFp[Q[lineIndexA]] * diff) - (deltaFp[Q[lineIndexB]] * diff);
        long deltaDeltaFn = (deltaFn[Q[lineIndexA]] * diff) - (deltaFn[Q[lineIndexB]] * diff);

        // update FP & FN
        auto updateIndex = std::max(lineIndexA, lineIndexB);
        FP.insert(FP.begin() + updateIndex, deltaDeltaFp);
        FN.insert(FN.begin() + updateIndex, deltaDeltaFn);

        // create new intersections
        // "top" will actually be on top iff lineIndexA is after lineIndexB
        Point topIntersectionPoint = intersect(lines[Q[higherLineIndex]], lines[Q[lineIndexA]]);
        Point bottomIntersectionPoint = intersect(lines[Q[lowerLineIndex]], lines[Q[lineIndexB]]);

        // queue these in the multiset
        // intersection points with infinite values aren't real intersections
        if (isFinite(topIntersectionPoint)) {
            auto topIntersection = IntersectionData{
                    topIntersectionPoint, Q[higherLineIndex], Q[lineIndexA]
            };
            intersections.insert(topIntersection);
        }

        if (isFinite(bottomIntersectionPoint)) {
            auto bottomIntersection = IntersectionData{
                    bottomIntersectionPoint, Q[lowerLineIndex], Q[lineIndexB]
            };
            intersections.insert(bottomIntersection);
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
