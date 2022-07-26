#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <vector>

using namespace std;

/** An enum specifying the type of the token */
enum TokenType {
  bool_or               = 0,
  bool_and              = 1,

  parenthesis_left      = 2,
  parenthesis_right     = 3,

  conditional_eq        = 4,
  conditional_neq       = 5,
  conditional_lt        = 6,
  conditional_gt        = 7,
  conditional_lte       = 8,
  conditional_gte       = 9,

  value_undefined_type  = 10,
  value_integer         = 11,
  value_floating        = 12,
  value_varchar         = 13,
  value_date            = 14,
  value_time            = 15,

  attribute_name        = 16
};

/**
 * A token is used in parsing to split a string into easy to use blocks of logic
 * associated with its purpose.
 */
typedef pair<TokenType, string> Token;

class Tokenizer {
public:

  /** Constructors takes the source string to be parsed */
  Tokenizer(string source);

  /** Returns a vector of tokens parsed from a where clause */
  vector<Token> tokenize();
private:
  char stream_get();
  void stream_unget(char c);
  static bool string_contains(string source, char target);

  vector<Token> tokens_;
  string stream_;
};


#endif