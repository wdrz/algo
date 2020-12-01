#include<iostream>
#include<algorithm>
#include<vector>
#include<functional>
#define NEG_INF -1000000009
using namespace std;

/**
 * A type of a segment tree that can add a given value to every cell inside an interval 
 * and can return the maximal value on any interval.
 */
class SegmentTree {
  private:
    int d;
    vector<int> vals;
    vector<int> lazy;
    void push(int);
  public:
    SegmentTree(int);
    int get_max(int, int);
    void add_val(int, int, int);
};

SegmentTree::SegmentTree(int n) : d{
  [n]() -> int{
    int m = 2;
    while (m < n) m *= 2;
    return m;
  }()}, vals{2 * d}, lazy{2 * d} {
    vals.resize(2 * d);
    lazy.resize(2 * d);
  }

int SegmentTree::get_max(int p, int q) {
  std::function<int(int, int, int, int, int)> max_func;
  max_func = [&](int p, int q, int a, int b, int v) -> int {
    if (q < a || p > b) return NEG_INF;
    if (p <= a && q >= b) return vals[v];

    push(v);
    return max(
      max_func(p, q, a, (a + b) / 2, 2 * v),
      max_func(p, q, (a + b) / 2 + 1, b, 2 * v + 1)
    );
  };

  return max_func(p, q, 0, d - 1, 1);
}

void SegmentTree::add_val(int p, int q, int val) {
  std::function<void(int, int, int, int, int, int)> add_func;
  add_func = [&](int p, int q, int a, int b, int v, int val) -> void {
    if (q < a || p > b) {
      return;
    } else if (p <= a && q >= b) {
      vals[v] += val;
      lazy[v] += val;
    } else {
      push(v);
      add_func(p, q, a, (a + b) / 2, 2 * v, val);
      add_func(p, q, (a + b) / 2 + 1, b, 2 * v + 1, val);
      vals[v] = max(vals[v * 2], vals[v * 2 + 1]);
    }
  };

  return add_func(p, q, 0, d - 1, 1, val);
}

void SegmentTree::push(int v) {
  vals[v * 2] += lazy[v];
  vals[v * 2 + 1] += lazy[v];
  lazy[v * 2] += lazy[v];
  lazy[v * 2 + 1] += lazy[v];
  lazy[v] = 0;
}

// example
int main() {
  SegmentTree st{10};
  st.add_val(0, 3, 5);
  st.add_val(2, 5, 2);
  st.add_val(1, 2, -3);

  cout << st.get_max(2, 2); // Should be 4
  cout << st.get_max(0, 1); // Should be 5
  cout << st.get_max(3, 10); // Should be 7

  return 0;
}