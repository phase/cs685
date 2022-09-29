# Graduate Research Log

Week 2!

# 22 09 15

Questions for meeting:

In the AUM paper AUC is defined as ... while in the DAM papers they define AUC using the Wilcoxon-Mann-Whitney statistic. Are these different definitions or are they expressing the same thing? YES

Is minimizing AUM guarenteed to maximize AUC? YES

Is AUM differentiable? Can we make an approximation that is differentiable? YES

The "DAM margin-based min-max surrogate loss" paper says "Although the AUC square loss makes AUC maximization scalable, it has two issues that have been ignored by existing studies. In particular, it has adverse effect when trained with well-classified data (i.e., easy data), and is sensitive to noisily labeled data (i.e., noisy data)." Does AUM suffer from this?

# 22 09 12

Reading through Convex Optimizations has helped me understand these papers a lot better. I'm curious to see what Deep AUC Maximization has in store and see if AUM is applicable at all.

# 22 09 09

Going over the goals of the graudate research:

> Literature Review on AUC Optimization

- **Background**: what is the problem setting and data? For example, in regression the data is a n x p input/feature matrix, an n-vector of outputs/labels, problem is learning a function which takes a p-dimensional input/feature vector and returns a real-valued output/prediction.
- **Previous work**: what are the existing approaches for that problem, and what are their drawbacks that motivate a new algorithm? For example, in regression there are linear models, neural networks, boosting, etc.
- **Novelty**: what are the new ideas presented in the paper? Are they theoretical or empirical, or both? For example, the paper could use existing models/data and present a new proof about the optimality/speed of an existing algorithm, or it could use existing algorithms/data with a new neural network model architecture, or it could present new benchmark data sets for comparing various existing algorithms/models.
- **Results**: what comparisons were done to show that the new idea is interesting/useful in theory and/or in practice? For example, you could compare the test accuracy and computation time for different regression algorithms on various data sets.
- **Future work**: what are some of your ideas for new research papers that could be written as a follow up to this one? Justify why these ideas are sufficiently novel that they warrant a new paper describing them. Also describe what kinds of theoretical and empirical arguments you would need in that future paper. Please answer this point very specifically, because it will be helpful to define your first research project with me.

topics:

- AUC Maximization
- Compare exact line search vs grid search vs backtracking line search
- [LibAUC](https://libauc.org/publications/) research papers

- This is the paper we directly build from https://arxiv.org/abs/2107.01285
- This book gives intro to various optimization algorithms https://web.stanford.edu/~boyd/cvxbook/bv_cvxbook.pdf chapter 9
- Another way of optimizing ROC AUC: [Large-scale Robust Deep AUC Maximization: A New Surrogate Loss and Empirical Studies on Medical Image Classification](https://arxiv.org/pdf/2012.03173.pdf)
- [AUC Maximization in the Era of Big Data and AI: A Survey](https://arxiv.org/pdf/2203.15046.pdf)
- [Maximizing AUC with Deep Learning for Classification of Imbalanced Mammogram Datasets](https://diglib.eg.org/bitstream/handle/10.2312/vcbm20171246/131-135.pdf?sequence=1&isAllowed=y)

> Run computational cross-validation experiments which compare prediction accuracy of several different learning algorithms, including new exact line search algorithm.

- extend [max-generalized-auc](https://github.com/tdhock/max-generalized-auc)

> Create data visualizations which compare the prediction accuracy and speed of the different approaches, along with slides summarizing results and interpretation.

- Loss vs Step Size plot
- Get the [orange dots](https://github.com/tdhock/max-generalized-auc/blob/master/figure-aum-convexity-interactive.R#L68-L75) (non-differentiable points)
