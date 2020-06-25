# include <iostream>
# include <vector>
# include <queue>

const int max_char = 26;

using namespace std;

struct trie_node {
  trie_node* next[max_char];
  string* out;
  trie_node* failure;
  trie_node(): next{nullptr}, out(nullptr), failure(nullptr) {}
};

inline int code(char letter) {
  return letter - 'a';
} 

trie_node* build_tree(vector<string> &patterns) {
  trie_node* root = new trie_node();

  trie_node* current_state;
  trie_node** new_node;

  for (string &pattern: patterns) {
    current_state = root;

    for (const char &letter: pattern) {
      new_node = &(current_state->next[code(letter)]);
      if (!*new_node)
        *new_node = new trie_node();

      current_state = *new_node;
    }
    current_state->out = &pattern;
  }
  return root;
}

trie_node* find_failure(trie_node* current, int letter_code) {
  if (current->next[letter_code])
    return current->next[letter_code];
  else if (current == current->failure)
    return current;
  else
    return find_failure(current->failure, letter_code);
}

void fill_failure(trie_node* root) {
  trie_node* current;
  queue<trie_node*> bfs_queue;

  root->failure = root;
  for (int i = 0; i < max_char; i++) {
    if (root->next[i]) {
      root->next[i]->failure = root;
      bfs_queue.push(root->next[i]);
    } else {
      root->next[i] = root;  
    }
  }
  while (!bfs_queue.empty()) {
    current = bfs_queue.front();
    bfs_queue.pop();
    for (int i = 0; i < max_char; i++) {
      if (current->next[i]) {
        current->next[i]->failure = find_failure(current->failure, i);
        bfs_queue.push(current->next[i]);
      } else {
        current->next[i] = find_failure(current->failure, i);
      }
    }
  }
}

void aho_corasick(string text, vector<string> &patterns, trie_node *node) {
  for (const char &letter: text) {
    node = node->next[code(letter)];
    if (node->out) {
      cout << *(node->out) << " ";
    }
  }
}

void debug(trie_node *node, string word) {
  cout << word << (node->out ? "*" : "") << endl;
  for (int i = 0; i < max_char; i++) {
    if (node->next[i]) {
      string ch(1, char(i + 'a'));
      debug(node->next[i], " " + word + ch);
    }
  }
}

int main () {
  string text = "abcacabcabacbcbbbcacba";
  vector<string> patterns{"abc", "abb", "cab", "bac"};

  trie_node* root = build_tree(patterns);
  // debug(root, "-");
  fill_failure(root);

  aho_corasick(text, patterns, root);

  return 0;
}