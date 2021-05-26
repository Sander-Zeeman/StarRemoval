#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/statistics/univariate_statistics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

float skewTest(std::vector<float> data)
{
    int n = data.size();

    float b2 = boost::math::statistics::skewness(data);
    float y = b2 * sqrt(
        ((n + 1) * (n + 3))
            /
        (6.0 * (n - 2))
    );

    float beta2 = (
        (3.0 * (n*n + 27*n - 70) * (n + 1) * (n + 3))
            /
        ((n - 2.0) * (n + 5) * (n + 7) * (n + 9))
    );

    float w2 = sqrt(2 * (beta2 - 1)) - 1;
    float delta = 1 / sqrt(0.5 * log(w2));
    float alpha = sqrt(2.0 / (w2 - 1));
    y = (y == 0 ? 1 : y);

    float coeff = y / alpha;
    float Z = delta * log(coeff + sqrt(coeff*coeff + 1));
    return Z;
}

float kurtosisTest(std::vector<float> data)
{
    int n = data.size();

    float b2 = boost::math::statistics::kurtosis(data);
    float E = 3.0 * (n - 1) / (n + 1);
    float varb2 =
        (24.0 * n * (n - 2) * (n - 3))
            /
        ((n + 1.0) * (n + 1) * (n + 3) * (n + 5));
    float x = (b2 - E) / sqrt(varb2);

    float sqrtbeta1 =
        6.0 * (n*n - 5*n + 2)
            /
        ((n+7) * (n + 9))
            *
        sqrt(
            (6.0 * (n + 3) * (n + 5))
                /
            (n * (n - 2) * (n - 3))
        );

    float A = 6.0 + (8.0 / sqrtbeta1) * (
        (2.0 / sqrtbeta1) + sqrt(
            1 + (4.0 / (sqrtbeta1 * sqrtbeta1))
        )
    );

    float term1 = 1 - 2 / (9.0 * A);
    float denom = 1 + x * sqrt(
        2 / (A - 4.0)
    );
    float term2 = cbrt((1 - (2.0 / A)) / denom);

    float Z = (term1 - term2) / sqrt(2 / (9.0 * A));
    return Z;
}

void KSquared(std::vector<float> data)
{
    float s, k, k_2, p;

    s = skewTest(data);
    k = kurtosisTest(data);
    k_2 = s*s + k*k;
    boost::math::chi_squared dist(2);
    p = boost::math::cdf(boost::math::complement(dist, k_2));

    std::cout << "SkewnessTest: " << s << std::endl;
    std::cout << "KurtosisTest: " << k << std::endl;
    std::cout << "KSquared val: " << k_2 << std::endl;
    std::cout << "P Value: " << p << std::endl;
}

int main()
{
    std::vector<float> data = {
        0.1, 0.2, 0.3, 0.4, 0.45,0.48, 0.49, 0.5, 0.53, 0.55,
        0.57, 0.58, 0.59, 0.595, 0.6, 0.595, 0.59, 0.58, 0.57, 0.55,
        0.53, 0.5, 0.48, 0.45, 0.42, 0.4, 0.37, 0.33, 0.3, 0.24,
        0.22, 0.2, 0.18, 0.17, 0.16, 0.155, 0.15, 0.13, 0.11, 0.1
    };

    std::cout << "Size: " << data.size() << std::endl;
    std::cout << "Mean: " << boost::math::statistics::mean(data) << std::endl;
    std::cout << "Variance: " << boost::math::statistics::variance(data) << std::endl;
    std::cout << "Median: " << boost::math::statistics::median(data) << std::endl;
    std::cout << "Skewness: " << boost::math::statistics::skewness(data) << std::endl;
    std::cout << "Kurtosis: " << boost::math::statistics::kurtosis(data) <<
    std::endl;
    KSquared(data);
}