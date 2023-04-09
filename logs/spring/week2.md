# 1/31/23

Graphs are looking a lot nicer.

Presentation:
1. Introduce AUC
2. Introduce non-convex ROC curves
3. Introduce Area Under Min
4. How do we minimize this?
5. Iterations of Gradient Descent
   1. Differentialble points
6. My research: building an exact line search to optimize AUM
7. Reconstructing the AUM line using breakpoints / kinks in the curve
   1. exploiting the piecewise nature of the curve
8. Results
   1. Major result: algorithm works as expected
9.  Future plans
   1. comparison between exact, grid, halving, constant
   2. comparisons between many different datasets

Questions for 1-on-1:
- Step halving starts at 1 and halves until what stopping point? I'm currently using the `factor` to set a custom one. Does this reset between iterations of gradient descent?
- why is grid search faster in some cases?

meeting notes:
- go slowly with slides, show different graphs. don't show massive matrix of graphs
- see how average time per step changes with max iterations of exact search / number of grid points
- test another dataset
