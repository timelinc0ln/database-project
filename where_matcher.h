#ifndef WHEREMATCHER_H_
#define WHEREMATCHER_H_

#include "Record.h"
#include "Tokenizer.h"

#include <vector>
#include <string>
using namespace std;

class WhereMatcher {
public:
  WhereMatcher(string where_clause);
  bool does_match(Record record);
private:
  vector<Token> tokens_;
  Record record_;

  template <typename T>
  T parse_value();
  bool parse_conditional();
  bool parse_or();
  bool parse_and();

  Token stream_get();
  void stream_unget(Token token);
};

#endif