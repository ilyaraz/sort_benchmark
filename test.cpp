#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using std::copy;
using std::cout;
using std::endl;
using std::mt19937_64;
using std::random_device;
using std::sort;
using std::stable_sort;
using std::uniform_int_distribution;
using std::vector;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

double evaluate(size_t n, size_t num_attempts) {
  vector<uint32_t> a(n);
  random_device rd;
  mt19937_64 gen(rd());
  uniform_int_distribution<uint32_t> u;
  for (size_t i = 0; i < n; ++i) {
    a[i] = u(gen);
  }
  vector<uint32_t> b(n);
  uint32_t total = 0;
  auto t1 = high_resolution_clock::now();
  for (size_t it = 0; it < num_attempts; ++it) {
    copy(a.begin(), a.end(), b.begin());
    stable_sort(b.begin(), b.end());
    total += b[0];
  }
  auto t2 = high_resolution_clock::now();
  cout << total << endl;
  return duration_cast<duration<double>>(t2 - t1).count();
}

double evaluate(size_t n) {
  size_t num_attempts = 10;
  for (;;) {
    double t = evaluate(n, num_attempts);
    if (t >= 1.0) {
      return t / num_attempts;
    }
    num_attempts *= 2;
  }
}

int main() {
  cout << evaluate(1024) << endl;
  return 0;
}
