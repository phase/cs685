Algorithm is done!?

Notes 11/3:
- [ ] build speed tests
- prediction vector optimization vs linear weights
  - pred vec opt: doesnt learning anything
  - weight opt: does actual learning
- [this](https://github.com/tdhock/max-generalized-auc/blob/d7308ad6d650ffc785cef7a33d1d317c4f179499/figure-aum-optimized-data.R#L87-L96) can be replaced with my algorithm, figuring out the exact step sizes the aum changes at
- create visualization for traversing line intersections

12/6:
- exact, grid, step halving, constant step size
- analyze wall time?
- will it always be quadratic?
- Create data visualizations which compare the prediction accuracy and speed of the different approaches, along with slides summarizing results and interpretation.
  - use cross validation code (example 5)
- check kink value `.I/.N` which tells you how far along the minimum value was (`1.0` means it chose the last value - the maxIterations can be incremented to lower the amount of steps taken (which increases how many steps inside the algorithm are done)).
- try finding spots where the intersection point is at a step size of zero, or where multiple intersection points are at the same step size, or when there are 3 lines all intersecting with each other
