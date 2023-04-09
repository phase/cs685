# Monsoon experiments

![test](../img/multi.datasets.aum.png)
Testing running all of these algorithms Ignore `hybrid` here, it's not the same algorithm as the ones below.

![test](../img/monsoon.aum.comparison.png)
Minimizing AUM. `hybrid` performs as well as `exactq` here.
![test](../img/monsoon.auc.png)


![test](../img/monsoon.time.comparison.png)
Time elapsed per iteration of gradient descent. `hybrid` starts out as slow as `grid`, but then drops down to a little above `exact`.

![test](../img/monsoon.total.time.png)
`hybrid` is as fast as `exact` while getting AUM/AUC values similar to `exactq`.
