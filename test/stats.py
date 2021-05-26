import numpy as np
import scipy.stats as stats

data = [
    0.1, 0.2, 0.3, 0.4, 0.45,0.48, 0.49, 0.5, 0.53, 0.55,
    0.57, 0.58, 0.59, 0.595, 0.6, 0.595, 0.59, 0.58, 0.57, 0.55,
    0.53, 0.5, 0.48, 0.45, 0.42, 0.4, 0.37, 0.33, 0.3, 0.24,
    0.22, 0.2, 0.18, 0.17, 0.16, 0.155, 0.15, 0.13, 0.11, 0.1
]

ks, p = stats.normaltest(data);
x,_ = stats.skewtest(data);
y,_ = stats.kurtosistest(data);

print("Size: " + str(len(data)));
print("Mean: " + str(np.mean(data)));
print("Variance: " + str(np.var(data)));
print("Median: " + str(np.median(data)));
print("Skewness: " + str(stats.skew(data)));
print("Kurtosis: " + str(stats.kurtosis(data, fisher=False)));
print("Skewnesstest: " + str(x));
print("Kurtosistest: " + str(y));
print("K squared: " + str(ks));
print("P value: " + str(p));