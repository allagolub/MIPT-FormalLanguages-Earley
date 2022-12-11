#include "EarleyAlgorithm.h"
void Grammar::InsertRule(char left_part, std::string right_part) {
  rules_[left_part].emplace_back(right_part);
}
void Grammar::AddRule(const std::string& rule) {
  char left_part = rule[0];
  if (IsTerminal(left_part)) {
    return; // error
  }
  std::string right_part;
  if (rule.size() <= 3) {
    return; //error
  }
  right_part = rule.substr(3, rule.size());
  if (right_part == "e") {
    right_part = "";
  }
  InsertRule(left_part, right_part);
  terminal_rules_.emplace_back(GrammarRules(left_part,right_part));
}
bool Grammar::IsTerminal(char symbol) {
  return std::islower(symbol);
}
bool EarleyAlgorithm::Scan(int index) {
  int old_size = list_of_situations_[index].size();
  if (index == 0) {
    return false;
  }
  for (auto &situation : list_of_situations_[index - 1]) {
    if (situation.right_part_.size() == 1) {
      continue;
    }
    char next_symbol = situation.right_part_[situation.index_point_ + 1];
    if (situation.index_point_ == situation.right_part_.size()) {
      continue;
    }
    if (((!std::isalpha(next_symbol)) && (next_symbol != '*')) || IsTerminal(next_symbol)) {
      if (next_symbol == word_[index - 1]) {
        char new_left_part = situation.left_part_;
        std::string new_right_part = situation.right_part_;
        new_right_part[situation.index_point_] = new_right_part[situation.index_point_ + 1];
        new_right_part[situation.index_point_ + 1] = '.';
        int new_index_read_str = situation.index_read_str_;
        int new_index_point = situation.index_point_ + 1;
        list_of_situations_[index].insert(Situation(new_left_part,
                                                    new_right_part,
                                                    new_index_read_str,
                                                    new_index_point));
      }
    }
  }
  return list_of_situations_[index].size() != old_size;
}
bool EarleyAlgorithm::Complete(int index) {
  int old_size = list_of_situations_[index].size();
  for (auto &situation : list_of_situations_[index]) {
    if ((situation.left_part_ == '*') || (situation.index_point_ != situation.right_part_.size() - 1)) {
      continue;
    }
    int new_index = situation.index_read_str_;
    for (auto &next_situation : list_of_situations_[new_index]) {
      if ((next_situation.index_point_ == next_situation.right_part_.size() - 1) ||
         (next_situation.right_part_[next_situation.index_point_ + 1] != situation.left_part_)){
        continue;
      }
      char new_left_part = next_situation.left_part_;
      std::string new_right_part = next_situation.right_part_;
      new_right_part[next_situation.index_point_] = new_right_part[next_situation.index_point_ + 1];
      new_right_part[next_situation.index_point_ + 1] = '.';
      int new_index_read_str = next_situation.index_read_str_;
      int new_index_point = next_situation.index_point_ + 1;
      list_of_situations_[index].insert(Situation(new_left_part, new_right_part, new_index_read_str, new_index_point));
    }
  }
  return list_of_situations_[index].size() != old_size;
}
bool EarleyAlgorithm::Predict(int index) {
  int old_size = list_of_situations_[index].size();
  for (auto &situation : list_of_situations_[index]) {
    if (situation.index_point_ == situation.right_part_.size() - 1) {
      continue;
    }
    for (auto &rules : terminal_rules_) {
      if (situation.right_part_[situation.index_point_ + 1] != rules.left_part_) {
        continue;
      }
      char new_left_part = rules.left_part_;
      std::string new_right_part;
      new_right_part = '.' + rules.right_part_;
      int new_index_read_str = index;
      int new_index_point = 0;
      list_of_situations_[index].insert(Situation(new_left_part, new_right_part, new_index_read_str, new_index_point));
    }
  }
  return list_of_situations_[index].size() != old_size;
}
bool EarleyAlgorithm::Earley() {
  int len = word_.size();
  list_of_situations_.resize(len + 1);
  list_of_situations_[0].insert(Situation('#', ".S", 0, 0));
  for (int j = 0; j <= len; ++j) {
    Scan(j);
    bool is_changed = true;
    while (is_changed) {
      is_changed = false;
      is_changed |= Complete(j);
      is_changed |= Predict(j);
    }
  }
  return list_of_situations_[len].find(Situation('#', "S.", 0, 1)) != list_of_situations_[len].end();
}
bool EarleyAlgorithm::EarleyResult(const std::string& word) {
  if (word == "e") {
    word_ = "";
  } else {
    word_ = word;
  }
  return Earley();
}