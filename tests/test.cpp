#include "test.h"

TEST(Test, TestResult) {
  Grammar grammar;
  grammar.AddRule("S->T+S");
  grammar.AddRule("S->T");
  grammar.AddRule("T->F*T");
  grammar.AddRule("T->F");
  grammar.AddRule("F->(S)");
  grammar.AddRule("F->a");
  TestEarleyAlgorithm test_earley(grammar);
  EXPECT_TRUE(test_earley.GetEarleyResult("(a+a)"));
  EXPECT_FALSE(test_earley.GetEarleyResult("B"));
  EXPECT_FALSE(test_earley.GetEarleyResult("aa"));
  EXPECT_FALSE(test_earley.GetEarleyResult("(a+A)"));
}
TEST(Test, TestAddRule) {
  Grammar grammar;
  grammar.AddRule("S->T+S");
  grammar.AddRule("S->T");
  grammar.AddRule("T->F*T");
  grammar.AddRule("T->F");
  grammar.AddRule("F->(S)");
  grammar.AddRule("F->a");
  TestEarleyAlgorithm test_grammar(grammar);
  EXPECT_EQ(6, test_grammar.GetQuantityRules());
  std::map<char, std::vector<std::string>> rules = test_grammar.GetRules();
  std::vector<std::string> right_part({"T+S", "T"});
  EXPECT_EQ(right_part, rules['S']);
  right_part = {"F*T", "F"};
  EXPECT_EQ(right_part, rules['T']);
  right_part = {"(S)", "a"};
  EXPECT_EQ(right_part, rules['F']);
}
TEST(Test, TestResultE) {
  Grammar grammar;
  grammar.AddRule("S->e");
  grammar.AddRule("s->e");
  grammar.AddRule("S->");
  TestEarleyAlgorithm test_earley(grammar);
  EXPECT_TRUE(test_earley.GetEarleyResult("e"));
  EXPECT_FALSE(test_earley.GetEarleyResult("(a+A)"));
}
TEST(Test, TestPredict) {
  Grammar grammar;
  grammar.AddRule("S->T+S");
  grammar.AddRule("S->T");
  grammar.AddRule("T->F*T");
  grammar.AddRule("T->F");
  grammar.AddRule("F->(S)");
  grammar.AddRule("F->a");
  TestEarleyAlgorithm test_earley_algorithm(grammar);
  std::vector<std::set<Situation>> list;
  list.resize(2);
  list[0].insert({'#', ".S", 0, 0});
  test_earley_algorithm.SetListSituations(list);
  bool is_change = true;
  test_earley_algorithm.SetWord("(a+a)");
  while (is_change) {
    is_change = false;
    is_change |= test_earley_algorithm.TestPredict(0);
  }
  list = test_earley_algorithm.GetListSituations();
  EXPECT_EQ(7, list[0].size());
  EXPECT_TRUE(list[0].find({'S', ".T+S", 0, 0}) != list[0].end());
  EXPECT_TRUE(list[0].find({'T', ".F", 0, 0}) != list[0].end());
  EXPECT_FALSE(list[0].find({'F', ".a", 1, 0}) != list[0].end());
}
TEST(Test, TestComplete) {
  Grammar grammar;
  grammar.AddRule("S->T+S");
  grammar.AddRule("S->T");
  grammar.AddRule("T->F*T");
  grammar.AddRule("T->F");
  grammar.AddRule("F->(S)");
  grammar.AddRule("F->a");
  TestEarleyAlgorithm test_earley_algorithm(grammar);
  std::vector<std::set<Situation>> list;
  list.resize(2);
  list[0].insert({'#', ".S", 0, 0});
  test_earley_algorithm.SetWord("(a+a)");
  test_earley_algorithm.SetListSituations(list);
  bool is_change = true;
  while (is_change) {
    is_change = false;
    is_change |= test_earley_algorithm.TestPredict(0);
  }
  is_change = test_earley_algorithm.TestComplete(1);
  EXPECT_TRUE(!is_change);
  list = test_earley_algorithm.GetListSituations();
  EXPECT_EQ(0, list[1].size());
  EXPECT_TRUE(list[1].find({'F', "(.S)", 2, 1}) == list[1].end());
}
TEST(Test, TestScan) {
  Grammar grammar;
  grammar.AddRule("S->T+S");
  grammar.AddRule("S->T");
  grammar.AddRule("T->F*T");
  grammar.AddRule("T->F");
  grammar.AddRule("F->(S)");
  grammar.AddRule("F->a");
  TestEarleyAlgorithm test_earley_algorithm(grammar);
  test_earley_algorithm.SetWord("(a+a)");
  std::vector<std::set<Situation>> list;
  list.resize(3);
  list[1].insert({'F', "(.S)", 1, 1});
  list[1].insert({'S', ".T+S", 1, 0});
  list[1].insert({'S', ".T", 1, 0});
  list[1].insert({'T', ".F*T", 1, 0});
  list[1].insert({'T', ".F", 1, 0});
  list[1].insert({'F', ".(S)", 1, 0});
  list[1].insert({'F', ".a", 1, 0});
  test_earley_algorithm.SetListSituations(list);
  bool is_change = test_earley_algorithm.TestScan(2);
  EXPECT_TRUE(is_change);
  list = test_earley_algorithm.GetListSituations();
  EXPECT_EQ(1, list[2].size());
  EXPECT_TRUE(list[1].find({'F', "a.", 2, 1}) != list[2].end());
  list[1].insert({'F', "a.", 1, 2});
  test_earley_algorithm.SetListSituations(list);
  is_change = test_earley_algorithm.TestScan(2);
  EXPECT_FALSE(is_change);
}