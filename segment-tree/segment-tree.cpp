#include<iostream>
#include<algorithm>
#define NEG_INF -1000000009
using namespace std;

/**
 * A type of a segment tree that can add a given value to every cell inside an interval 
 * and can return the maximal value on any interval.
 */
class SegmentTree {
  private:
    int *vals;
    int *lazy;
    int d;
    int max_aux(int, int, int, int, int);
    void add_aux(int, int, int, int, int, int);
    void push(int);
  public:
    SegmentTree(int);
    int get_max(int, int);
    void add_val(int, int, int);
};

SegmentTree::SegmentTree(int n) {
  d = 2;
  while (d < n) d *= 2;
  vals = new int[2 * d]();
  lazy = new int[2 * d]();
}

int SegmentTree::get_max(int p, int q) {
  return max_aux(p, q, 0, d - 1, 1);
}

void SegmentTree::add_val(int p, int q, int val) {
  return add_aux(p, q, 0, d - 1, 1, val);
}

void SegmentTree::push(int v) {
  vals[v * 2] += lazy[v];
  vals[v * 2 + 1] += lazy[v];
  lazy[v * 2] += lazy[v];
  lazy[v * 2 + 1] += lazy[v];
  lazy[v] = 0;
}

int SegmentTree::max_aux(int p, int q, int a, int b, int v) {
  if (q < a || p > b) return NEG_INF;
  if (p <= a && q >= b) return vals[v];

  push(v);
  return max(
    max_aux(p, q, a, (a + b) / 2, 2 * v),
    max_aux(p, q, (a + b) / 2 + 1, b, 2 * v + 1)
  );
}

void SegmentTree::add_aux(int p, int q, int a, int b, int v, int val) {
  if (q < a || p > b) {
    return;
  } else if (p <= a && q >= b) {
    vals[v] += val;
    lazy[v] += val;
  } else {
    push(v);
    add_aux(p, q, a, (a + b) / 2, 2 * v, val);
    add_aux(p, q, (a + b) / 2 + 1, b, 2 * v + 1, val);
    vals[v] = max(vals[v * 2], vals[v * 2 + 1]);
  }
}

// example
int main() {
  SegmentTree *st = new SegmentTree(10);
  st->add_val(0, 3, 5);
  st->add_val(2, 5, 2);
  st->add_val(1, 2, -3);

  cout << st->get_max(2, 2); // Should be 4
  cout << st->get_max(0, 1); // Should be 5
  cout << st->get_max(3, 10); // Should be 7

  return 0;
}