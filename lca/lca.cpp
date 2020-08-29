// Lowest common ancestor

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class Tree {
  private:
    int d, n, root;
    int** anc;
    int *depth;
    vector<int>* child;
    void dfs(int, int);
    void fill_depth(int, int);
  public:
    Tree(int, int*);
    int lca(int, int);
};

void Tree::dfs(int v, int k) {
  if (v == root) anc[v][k] = root;
  else anc[v][k] = anc[ anc[v][k - 1] ][k - 1];
  for (const int& son: child[v]) dfs(son, k);
}

void Tree::fill_depth(int v, int d) {
  depth[v] = d;
  for (const int& son: child[v]) fill_depth(son, d + 1);
}

int Tree::lca(int a, int b) {
  a--;
  b--;
  if (depth[a] > depth[b]) swap(a, b);
  
  for (int i = d - 1; i >= 0 && depth[a] < depth[b]; i--)
    if (depth[anc[b][i]] >= depth[a])
      b = anc[b][i];

  if (a == b) return a + 1;

  for (int i = d - 1; i >= 0; i--)
    if (anc[a][i] != anc[b][i]) {
      a = anc[a][i];
      b = anc[b][i];
    }

  return anc[a][0] + 1;
}

Tree::Tree(int n, int* parents) {
  this->n = n;
  child = new vector<int>[n];

  d = 2;
  for (int t = 2; t < n; t *= 2) d++;

  anc = new int*[n];
  depth = new int[n];

  for (int i = 0; i < n; i++) {
    anc[i] = new int[d];
    
    anc[i][0] = parents[i] - 1;
    if (i != anc[i][0]) child[ anc[i][0] ].push_back(i);
    root = anc[i][0] == i ? i : root;
  }
  fill_depth(root, 0);
  for (int i = 1; i < d; i++) dfs(root, i);
}

// for testing purposes

int main() {
  int n = 7;
  int parents[n] = {1, 1, 1, 2, 3, 3, 6};
  /*

    1
   / \
  2   3
  |  / \
  4 5   6
        |
        7

  */
  Tree *tree = new Tree(n, parents);
  cout << tree->lca(2, 3) << endl; // 1
  cout << tree->lca(5, 7) << endl; // 3
  cout << tree->lca(4, 2) << endl; // 2
  cout << tree->lca(2, 5) << endl; // 1
  cout << tree->lca(7, 2) << endl; // 1
  cout << tree->lca(7, 2) << endl; // 1
}