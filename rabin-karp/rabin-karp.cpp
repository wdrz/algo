#include <iostream>
#include <algorithm>

using namespace std;

const int large_prime = 10000019;
const int small_prime = 149;

inline int code(char letter) {
  if (letter == ' ') return 1;
  else return letter - 'a' + 2;
}

int hash_string(const string &word, int n) {
  int hash = 0;
  int power = 1;
  for_each(word.rbegin() + word.size() - n, word.rend(),
    [&hash, &power](char letter) {
      hash += code(letter) * power;
      hash %= large_prime;
      power *= small_prime;
      power %= large_prime;
    }
  );
  return hash;
}

int high_power(int n) {
  int res = 1;
  while (n > 1) {
    n--;
    res = (res * small_prime) % large_prime;
  }
  return res;
}

int hash_next(long long int &old_hash, char char_removed, char char_added, int power) {
  old_hash = (old_hash - power * code(char_removed) % large_prime) + large_prime;
  old_hash = (old_hash * small_prime + code(char_added)) % large_prime;
}

bool verify_match(const string &text, const string &pattern, int index) {
  int n = pattern.size();

  for (int i = 0; i < n; i++)
    if (text[index - i] != pattern[n - 1 - i])
      return false;

  return true;
}

// it returns a vector filled with indices of first letters of all matches 
vector<int> rabin_karp(const string &text, const string &pattern) {
  int n = pattern.size();
  int power = high_power(n);
  long long int pattern_hash = hash_string(pattern, n);
  long long int current_hash = hash_string(text, n);
  vector<int> res;
  if (pattern_hash == current_hash && verify_match(text, pattern, n - 1)) 
    res.push_back(0);

  for (int i = n; i < text.size(); i++) {
    hash_next(current_hash, text[i - n], text[i], power);
    if (pattern_hash == current_hash && verify_match(text, pattern, i)) 
      res.push_back(i - n + 1);
  }

  return res;
}

int main() {
  string text = "in this not too orginal text you can look for a pattern";
  string pattern = "oo";
  vector<int> res = rabin_karp(text, pattern);
  for (int ind: res) {
    cout << ind << " ";
  }
  return 0;
}