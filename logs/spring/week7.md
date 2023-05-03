# Week #Something let's say 7

https://github.com/tdhock/aum/issues/5#issuecomment-1503747062 maybe remake this graph with `exact` & `hybrid`?

need to compare against constant step sizes

maybe try going until validation loss increases

What kind of plot(s) should be in the paper?

plot sum of iterations for `exactL` & `exactQ`

instead of `X.finite` or `X.subtrain`, use https://github.com/phase/max-generalized-auc/blob/aum-exact-line-search/figure-line-grid-search-interactive.R#L136-L146 `diffs.list$subtrain`

geom_ribbon !!! x, ymin, ymax where x=diffs.list$subtrain

exactL & exactQ

box plot (maybe different ones for small, big data regimes?)

- x: max validation AUC, time
- y: exact L, exactQ, grid, hybrid, constant
- fix 1 dataset & fold, average over different seeds
