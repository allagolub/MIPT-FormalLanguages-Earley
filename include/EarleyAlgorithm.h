#ifndef FORMALLANGUAGE
#define FORMALLANGUAGE
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cctype>
#include <algorithm>
#include <stack>
#include <utility>
struct Situation {
  char left_part_;
  std::string right_part_;
  int index_read_str_;
  int index_point_;
  friend bool operator<(const Situation& first, const Situation& second) {
    if (first.left_part_ != second.left_part_) {
      return (first.left_part_ < second.left_part_);
    }
    if (first.right_part_ != second.right_part_) {
      return (first.right_part_ < second.right_part_);
    }
    if (first.index_read_str_ != second.index_read_str_) {
      return (first.index_read_str_ < second.index_read_str_);
    }
    return (first.index_point_ < second.index_point_);
  }
  Situation(const char left_part, const std::string& right_part, const int index_read_str, const int index_point) {
    left_part_ = left_part;
    right_part_ = right_part;
    index_read_str_ = index_read_str;
    index_point_ = index_point;
  }
  Situation() = default;
};
struct GrammarRules {
  char left_part_;
  std::string right_part_;
  GrammarRules(const char left_part, const std::string& right_part) {
    left_part_ = left_part;
    right_part_ = right_part;
  }
  GrammarRules() = default;
};
class Grammar {
 public:
  void AddRule(const std::string& rule);
  std::vector<GrammarRules> terminal_rules_;
  std::map<char, std::vector<std::string>> rules_;

 protected:
  void InsertRule(char left_part, std::string right_part);
  bool IsTerminal(char symbol);
};

class EarleyAlgorithm : public Grammar {
 public:
  bool EarleyResult(const std::string& word);
  EarleyAlgorithm(Grammar &grammar) : rules_(std::move(grammar.rules_)), terminal_rules_(std::move(grammar.terminal_rules_)) {};
  EarleyAlgorithm() = default;

 protected:
  std::map<char, std::vector<std::string>> rules_;
  std::vector<GrammarRules> terminal_rules_;
  std::vector<std::set<Situation>> list_of_situations_;
  std::string word_;
  bool Earley();
  bool Scan(int index);
  bool Complete(int index);
  bool Predict(int index);
};
#endif //FORMALLANGUAGE