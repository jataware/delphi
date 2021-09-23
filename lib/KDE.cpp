#include "KDE.hpp"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/numeric.hpp>
#include <random>

using namespace std;
using namespace delphi::utils;
using boost::irange, boost::adaptors::transformed, boost::lambda::_1;

double sample_from_normal(
    std::mt19937 gen,
    double mu = 0.0, /**< The mean of the distribution.*/
    double sd = 1.0  /**< The standard deviation of the distribution.*/
) {
  normal_distribution<> d{mu, sd};
  return d(gen);
}

KDE::KDE(std::vector<double> v) : dataset(v) {

  // Compute the bandwidth using Silverman's rule
  mu = mean(v);
  auto X = v | transformed(_1 - mu);

  // Compute standard deviation of the sample.
  size_t N = v.size();
  double stdev = sqrt(inner_product(X, X, 0.0) / (N - 1));
  bw = pow(4 * pow(stdev, 5) / (3 * N), 1 / 5);
}

vector<double> KDE::resample(int n_samples,
                             std::mt19937& gen,
                             uniform_real_distribution<double>& uni_dist,
                             normal_distribution<double>& norm_dist) {
  vector<double> samples(n_samples);

  for (int i : irange(0, n_samples)) {
    double element = select_random_element(dataset, gen, uni_dist);

    // Transform the sampled values using a Gaussian distribution
    // ~ ( sampled value, bw)
    // We sample from a standard Gaussian and transform that sample
    // to the desired Gaussian distribution by
    // μ + σ * standard Gaussian sample
    samples[i] = element + bw * norm_dist(gen);
  }

  return samples;
}

// double exp1(double x) { return (6+x*(6+x*(3+x)))*0.16666666f;                                                                    }
// double exp2(double x) { return (24+x*(24+x*(12+x*(4+x))))*0.041666666f;                                                          }
// double exp3(double x) { return (120+x*(120+x*(60+x*(20+x*(5+x)))))*0.0083333333f;                                                }
// double exp4(double x) { return 720+x*(720+x*(360+x*(120+x*(30+x*(6+x))))))*0.0013888888f;                                        }
// double exp5(double x) { return (5040+x*(5040+x*(2520+x*(840+x*(210+x*(42+x*(7+x)))))))*0.00019841269f;                           }
// double exp6(double x) { return (40320+x*(40320+x*(20160+x*(6720+x*(1680+x*(336+x*(56+x*(8+x))))))))*2.4801587301e-5;             }
// double exp7(double x) { return (362880+x*(362880+x*(181440+x*(60480+x*(15120+x*(3024+x*(504+x*(72+x*(9+x)))))))))*2.75573192e-6; }

double KDE::pdf(double x) {
  // !! Inlining makes a big difference, could probably optimize further (vectorization, refactoring math)
  // !! If we can tolerate lower accuracy (experimentally), can use wacky tricks above, from
  //       https://stackoverflow.com/questions/10552280
  //    Note these are only accuracy over a certain range -- see post for more details
  
  double p = 0.0;
  size_t N = this->dataset.size();
  for (double elem : this->dataset) {
    double x1 = exp(-sqr(x - elem) / (2 * sqr(bw)));
    x1 /= N * bw * sqrt(2 * M_PI);
    p += x1;
  }
  return (double)p;
}

vector<double> KDE::pdf(vector<double> v) {
  vector<double> values;
  for (double elem : v) {
    values.push_back(pdf(elem));
  }
  return values;
}

double KDE::logpdf(double x) { return log(pdf(x)); }
