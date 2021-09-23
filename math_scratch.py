# double KDE::pdf(double x) {
#   double p = 0.0;
#   size_t N = this->dataset.size();
#   for (double elem : this->dataset) {
#     double x1 = exp(-sqr(x - elem) / (2 * sqr(bw)));
#     x1 /= N * bw * sqrt(2 * M_PI);
#     p += x1;
#   }
#   return p;
# }

# --

import numpy as np

def g(dataset, x, bw):
  p = 0
  N = len(dataset)
  for elem in dataset:
    num = (x - elem) ** 2
    den = 2 * (bw ** 2)
    x1  = np.exp(-1 * num / den)
    x1 /= N * bw * np.sqrt(2 * np.pi)
    p += x1
  
  return p

def g2(dataset, x, bw):
  N   = len(dataset)
  z   = N * bw * np.sqrt(2 * np.pi)
  den = 2 * (bw ** 2)
  
  p = 0
  for elem in dataset:
    num = (x - elem) ** 2
    x1  = np.exp(-1 * num / den)
    p += x1
  
  p /= z
  return p


def g3(dataset, x, bw):
  N   = len(dataset)
  z   = (N * bw * np.sqrt(2 * np.pi))
  den = 2 * (bw ** 2)
  
  a = np.exp(-1 / den * dataset ** 2)
  b = np.exp(2 / den * dataset)
  
  c = np.exp(-1 / den * x ** 2)
  return (c / z) * (a @ (b ** x))


np.random.seed(123)

dataset = np.random.uniform(0, 1, 100)
x       = np.random.uniform(0, 1, 1)[0]
bw      = np.random.uniform(0, 1, 1)[0]
%timeit g(dataset, x, bw)
%timeit g2(dataset, x, bw)
%timeit g3(dataset, x, bw)

from scipy.stats import norm
g(dataset, x, bw)
%timeit norm.pdf(dataset, x, bw).mean()

g3(dataset, x, bw)
lookup = norm.pdf(np.linspace(0, 1, 100001), 0, bw)
lookup[(100000 * (dataset - x)).astype(np.int32)].mean()