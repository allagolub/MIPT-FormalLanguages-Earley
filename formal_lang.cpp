#include <iostream>
#include "include/EarleyAlgorithm.h"

int main() {
  int quantity_rules;
  std::cin >> quantity_rules;
  int64_t quantity_words;
  std::cin >> quantity_words;
  Grammar grammar;
  for (int64_t i = 0; i < quantity_rules; ++i) {
    std::string rule;
    std::cin >> rule;
    grammar.AddRule(rule);
  }
  EarleyAlgorithm earley_algorithm;
  for (int64_t i = 0; i < quantity_words; ++i) {
    std::string word;
    std::cin >> word;
    if (earley_algorithm.EarleyResult(word)) {
      std::cout << "YES";
    } else {
      std::cout << "NO";
    }
  }
}