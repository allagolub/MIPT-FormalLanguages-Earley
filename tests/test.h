#ifndef FORMALLANGUAGE_PRACTIE_TEST_TEST_H_
#define FORMALLANGUAGE_PRACTIE_TEST_TEST_H_
#include <gtest/gtest.h>
#include "../include/EarleyAlgorithm.cpp"
class TestEarleyAlgorithm : public EarleyAlgorithm {
 public:
  TestEarleyAlgorithm() : EarleyAlgorithm() {};
  explicit TestEarleyAlgorithm(Grammar& grammar) : EarleyAlgorithm(grammar) {}
  int64_t GetQuantityRules() {
    return terminal_rules_.size();
  }
  std::map<char, std::vector<std::string>> GetRules() {
    return rules_;
  }
  bool GetEarleyResult(const std::string& word) {
    return EarleyResult(word);
  }
  void SetListSituations(const std::vector<std::set<Situation>>& list) {
    list_of_situations_ = list;
  }
  void SetWord(const std::string & word) {
    word_ = word;
  }
  std::vector<std::set<Situation>> GetListSituations() {
    return list_of_situations_;
  }
  bool TestScan(int index) {
    return Scan(index);
  }
  bool TestPredict(int index) {
    return Predict(index);
  }
  bool TestComplete(int index) {
    return Complete(index);
  }
};
#endif //FORMALLANGUAGE_PRACTIE_TEST_TEST_H_
