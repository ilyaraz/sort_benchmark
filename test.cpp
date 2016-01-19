#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using std::copy;
using std::cout;
using std::endl;
using std::fill;
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
  //vector<uint32_t> b(n);
  vector<vector<uint32_t>> buf(2, vector<uint32_t>(n));
  vector<uint32_t> cnt(256);
  vector<uint32_t> start(257);
  uint32_t total = 0;
  auto t1 = high_resolution_clock::now();
  for (size_t it = 0; it < num_attempts; ++it) {
    /*
    copy(a.begin(), a.end(), b.begin());
    stable_sort(b.begin(), b.end());
    total += b[0];
    */
    copy(a.begin(), a.end(), buf[0].begin());
    for (size_t b = 0; b < 4; ++b) {
      int layer = b % 2;
      int shift = 8 * b;
      fill(cnt.begin(), cnt.end(), 0);
      for (size_t i = 0; i < n; ++i) {
	uint32_t val = buf[layer][i];
	++cnt[(val >> shift) & 255];
      }
      start[0] = 0;
      for (size_t i = 0; i < 256; ++i) {
	start[i + 1] = start[i] + cnt[i];
      }
      for (size_t i = 0; i < n; ++i) {
	uint32_t val = buf[layer][i];
	buf[1 - layer][start[(val >> shift) & 255]++] = val;
      }
    }
    total += buf[0][0];
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
  cout << evaluate(1 << 20) << endl;
  return 0;
}
