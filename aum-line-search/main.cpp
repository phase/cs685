#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

typedef struct Line {
    double slope;
    double intercept;
} Line;

typedef struct Point {
    double x;
    double y;
} Point;

Point intersect(Line a, Line b) {
    double x = (b.intercept - a.intercept) / (a.slope - b.slope);
    double y = a.intercept + a.slope * x;
    return Point{x, y};
}

typedef struct IntersectionData {
    Point point;
    int lineA;
    int lineB;
} IntersectionData;

bool operator<(IntersectionData a, IntersectionData b) {
    return a.point.x < b.point.x;
}

bool compareIntersections(IntersectionData &a, IntersectionData &b) {
    return a.point.x < b.point.x;
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
        std::vector<Line> lines
) {

    // Q is a list of indices of lines
    // Q is updated as we move across the X axis, passing intersection points changes the indices in Q
    std::vector<int> Q;
    Q.reserve(lines.size());
    for (int i = 0; i < lines.size(); i++) {
        Q.push_back(i);
    }

    std::multiset<IntersectionData> intersections;
    // start by queueing intersections of every line and the line after it
    for (int a = 0; a < lines.size() - 1; a++) {
        Point point = intersect(lines[a], lines[a + 1]);
        intersections.insert(IntersectionData{point, a, a + 1});
    }

    while (!intersections.empty()) {
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

        // create new intersections
        // "top" will actually be on top iff lineIndexA is after lineIndexB
        Point topIntersectionPoint = intersect(lines[Q[higherLineIndex]], lines[Q[lineIndexA]]);
        Point bottomIntersectionPoint = intersect(lines[Q[lowerLineIndex]], lines[Q[lineIndexB]]);

        IntersectionData topIntersection = IntersectionData{
                topIntersectionPoint, Q[higherLineIndex], Q[lineIndexA]
        };
        IntersectionData bottomIntersection = IntersectionData{
                bottomIntersectionPoint, Q[lowerLineIndex], Q[lineIndexB]
        };

        // queue these in the multiset
        intersections.insert(topIntersection);
        intersections.insert(bottomIntersection);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
