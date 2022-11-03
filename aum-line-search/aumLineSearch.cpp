#include "aumLineSearch.h"

using namespace std;

bool Point::operator==(const Point other) const {
    // return x == other.x && y == other.y;
    // TODO double check this is correct
    return fabs(x - other.x) < 1e-5 && fabs(y - other.y) < 1e-5;
}

bool Point::isFinite() {
    return isfinite(x) && isfinite(y);
}

Point intersect(Line a, Line b) {
    double x = (b.intercept - a.intercept) / (a.slope - b.slope);
    double y = a.intercept + a.slope * x;
    return Point{x, y};
}

bool operator<(IntersectionData a, IntersectionData b) {
    return a.point.x < b.point.x;
}

bool operator==(IntersectionData a, IntersectionData b) {
    return a.point == b.point && (
            (a.lineHighBeforeIntersect == b.lineHighBeforeIntersect &&
             a.lineLowBeforeIntersect == b.lineLowBeforeIntersect) ||
            (a.lineHighBeforeIntersect == b.lineLowBeforeIntersect &&
             a.lineLowBeforeIntersect == b.lineHighBeforeIntersect)
    );
}

void queueIntersection(
        const Line *lines,
        multiset<IntersectionData> &intersections,
        unordered_set<Point> &checkedIntersections,
        int lowLine,
        int highLine
) {
    auto intersectionPoint = intersect(
            lines[lowLine],
            lines[highLine]
    );

    // intersection points with infinite values aren't real intersections
    if (intersectionPoint.isFinite() && intersectionPoint.x >= 0) {
        auto intersection = IntersectionData{
                intersectionPoint, lowLine, highLine
        };
        if (checkedIntersections.find(intersectionPoint) != checkedIntersections.end()) {
            return;
        }

        //cout << "queueing intersection point " << intersection.point.x << " " << intersection.point.y << " "
        //     << intersection.lineHighBeforeIntersect << " " << intersection.lineLowBeforeIntersect << endl;
        intersections.insert(intersection);
    }
}

void lineSearch(
        const Line *lines,
        int lineCount,
        const double *deltaFp,
        const double *deltaFn,
        double *FP,
        double *FN,
        double *M,
        int maxIterations,
        double *stepSizeVec,
        double *aumVec
) {
    // a list of indices of lines
    // this is updated as we move across the X axis, passing intersection points changes the indices
    vector<int> sortedIndices;
    // map from line number to index of line
    vector<int> backwardsIndices;
    sortedIndices.reserve(lineCount);
    backwardsIndices.reserve(lineCount);
    for (int i = 0; i < lineCount; i++) {
        sortedIndices.push_back(i);
        backwardsIndices.push_back(i);
    }

    unordered_set<Point> checkedIntersections;
    multiset<IntersectionData> intersections;
    // start by queueing intersections of every line and the line after it
    for (int a = 0; a < lineCount - 1; a++) {
        Point point = intersect(lines[a], lines[a + 1]);
        // parallel lines will be infinite
        if (point.isFinite() && point.x >= 0) {
            int lineIndexLowBeforeIntersect;
            int lineIndexHighBeforeIntersect;
            if (lines[a].intercept < lines[a + 1].intercept) {
                // (a) is below (a + 1) before the intersection point
                lineIndexLowBeforeIntersect = a;
                lineIndexHighBeforeIntersect = a + 1;
            } else {
                // (a + 1) is below (a) before the intersection point
                lineIndexLowBeforeIntersect = a + 1;
                lineIndexHighBeforeIntersect = a;
            }
            cout << "starting with intersection point: high line=" << lineIndexHighBeforeIntersect << " low line=" << lineIndexLowBeforeIntersect << " x=" << point.x << " y=" << point.y << endl;
            intersections.insert(IntersectionData{point, lineIndexLowBeforeIntersect, lineIndexHighBeforeIntersect});
            checkedIntersections.insert(point);
        }
    }

    double intercept = 0; // TODO this should be AUM at step size 0
    double aumSlope = 0.0;
    int iterations = 0;
    while (!intersections.empty() && iterations++ < maxIterations) {
        auto intersection = intersections.begin();
        cout << "using intersection point " << intersection->point.x << " " << intersection->point.y << " "
             << intersection->lineHighBeforeIntersect << " " << intersection->lineLowBeforeIntersect << endl;

        // swap the indices of the lines that make this intersection point
        // the names of these variables look backwards because they get swapped
        int lineIndexLowAfterIntersect = backwardsIndices[intersection->lineLowBeforeIntersect];
        int lineIndexHighAfterIntersect = backwardsIndices[intersection->lineHighBeforeIntersect];
        swap(backwardsIndices[intersection->lineLowBeforeIntersect], backwardsIndices[intersection->lineHighBeforeIntersect]);
        swap(sortedIndices[lineIndexHighAfterIntersect], sortedIndices[lineIndexLowAfterIntersect]);

        // indices of the next lines we want to find intersections for
        int higherLineIndex = lineIndexHighAfterIntersect + 1;
        int lowerLineIndex = lineIndexLowAfterIntersect - 1;

        cout << "sortedIndices = ";
        for (int i : sortedIndices) {
            cout << i << " ";
        }
        cout << endl;

        cout << "backwardsIndices = ";
        for (int i : backwardsIndices) {
            cout << i << " ";
        }
        cout << endl;

        cout << " 0higherLineIndex: " << higherLineIndex << " (line " << sortedIndices[higherLineIndex] << ")" << endl;
        cout << " 1lineHighAfterIntersect: " << lineIndexHighAfterIntersect << " (line "
             << intersection->lineLowBeforeIntersect << ")" << endl;
        cout << " 2lineLowAfterIntersect: " << lineIndexLowAfterIntersect << " (line "
             << intersection->lineHighBeforeIntersect << ")" << endl;
        cout << " 3lowerLineIndex: " << lowerLineIndex << " (line " << sortedIndices[lowerLineIndex] << ")" << endl;

        // (∆FP of top line) - (∆FP of bottom line)
        double deltaFpDiff = deltaFp[lineIndexHighAfterIntersect] - deltaFp[lineIndexLowAfterIntersect];
        double deltaFnDiff = deltaFn[lineIndexHighAfterIntersect] - deltaFn[lineIndexLowAfterIntersect];

        // b ∈ {2, . . . , B} is the index of the function which is larger before intersection point
        int b = intersection->lineHighBeforeIntersect;
        // current step size we're at
        double stepSize = intersection->point.x;

        // update FP & FN
        FP[b] += deltaFpDiff;
        FN[b] += deltaFnDiff;
        double minBeforeIntersection = M[b];
        M[b] = min(FP[b], FN[b]);

        checkedIntersections.insert(intersection->point);
        // queue the next intersections in the multiset
        // this creates an intersection between "lineIndexHighAfterIntersect" and "higherLineIndex"
        // "lineIndexHighAfterIntersect" will now be the index of the low line before this new intersection point
        if (higherLineIndex < lineCount) {
            queueIntersection(lines, intersections, checkedIntersections,
                              intersection->lineLowBeforeIntersect, sortedIndices[higherLineIndex]);
        }
        if (lowerLineIndex >= 0) {
            queueIntersection(lines, intersections, checkedIntersections,
                              sortedIndices[lowerLineIndex], intersection->lineHighBeforeIntersect);
        }

        // update aum slope
        double slopeDiff = lines[intersection->lineHighBeforeIntersect].slope - lines[intersection->lineLowBeforeIntersect].slope;
        // this is the D^(k+1) update rule in the paper
        aumSlope += (slopeDiff) * (M[b - 1] + M[b + 1] - M[b] - minBeforeIntersection);
        // this is Z^k + s*D^k in the paper
        double aum = intercept + stepSize * aumSlope;
        stepSizeVec[iterations] = stepSize;
        aumVec[iterations] = aum;
        cout << "stepSize: " << stepSize
             << " aum: " << aum
             << " slopeDiff: " << slopeDiff
             << " b: " << b
             << " minBeforeIntersection: " << minBeforeIntersection
             << " M[b]: " << M[b]
             << endl;

        cout << "FP: ";
        for (int i = 0; i < lineCount; i++) { cout << FP[i] << " "; }
        cout << endl;
        cout << "FN: ";
        for (int i = 0; i < lineCount; i++) { cout << FN[i] << " "; }
        cout << endl;
        cout << "M: ";
        for (int i = 0; i < lineCount; i++) { cout << M[i] << " "; }
        cout << endl;

        intersections.erase(intersection);
    }
}
