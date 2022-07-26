#include "tokenizer.h"

#include <sstream>

Tokenizer::Tokenizer(string source) {
  stream_ = string ( source.rbegin(), source.rend() );
}

// This looks disgusting, if you have a creative way of splitting it up, go
// for it.
vector<Token> Tokenizer::tokenize() {
  while(stream_.size() > 0) {
    char c = stream_get();
    switch(c) {
      // parenthesis_left
      case '(':
        tokens_.push_back( Token(parenthesis_left, "(") );
        break;

      // parenthesis_right
      case ')':
        tokens_.push_back( Token(parenthesis_right, ")") );
        break;

      // bool_or
      case '|':
        c = stream_get();
        if (c == '|') {
          tokens_.push_back( Token(bool_or, "||") );
        } else {
          stream_unget(c);
          // TODO: create exception class
          // throw e;
        }
        break;

      // bool_and
      case '&':
        c = stream_get();
        if (c == '&') {
          tokens_.push_back( Token(bool_and, "&&") );
        } else {
          stream_unget(c);
          // TODO: create exception class
          // throw e;
        }
        break;

      // condtional_lt / condtional_lte
      case '<':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_lte, "<=") );
        } else {
          stream_unget(c);
          tokens_.push_back( Token(conditional_lt, "<") );
        }
        break;

      // condtional_gt / condtional_gte
      case '>':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_gte, ">=") );
        } else {
          stream_unget(c);
          tokens_.push_back( Token(conditional_gt, ">") );
        }
        break;

      // condtional_neq
      case '!':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_neq, "!=") );
        } else {
          stream_unget(c);
          // TODO: create exception class
          // throw e;
        }
        break;

      // condtional_eq
      case '=':
        tokens_.push_back( Token(conditional_eq, "=") );
        break;

      // catch all for value_* and attribute_name
      default:

        // looks at the first character in the next token:
        //   if single quote: varchar
        //   if numeral: float, date, time, or int
        //   else: attribute name
        if (c == 30) {
          string string_value(1, c);

          while (true) {
            c = stream_get();
            if (c != 30) {
              string_value.push_back(c);
            } else {
              stream_unget(c);
              break;
            }
          }

          tokens_.push_back( Token(value_varchar, string_value) );
        } else if (c >= 48 && c <= 57) {
          stringstream ss;
          ss << c;

          while (true) {
            c = stream_get();
            if (c >= 48 && c <= 57 || c == 46 || c == 47 || c == 58) {
              ss << c;
            } else {
              stream_unget(c);
              break;
            }
          }

          // discern between floats, dates, times, and ints
          if (string_contains(ss.str(), 46)) {
            tokens_.push_back( Token(value_floating, ss.str()) );
          } else if (string_contains(ss.str(), 47)) {
            tokens_.push_back( Token(value_date, ss.str()) );
          } else if (string_contains(ss.str(), 58)) {
            tokens_.push_back( Token(value_time, ss.str()) );
          } else {
            tokens_.push_back( Token(value_integer, ss.str()) );
          }
        } else {
          string attibute(1,c);
          while (true) {
            c = stream_get();
            if (c != 32) {
              attibute.push_back(c);
            } else {
              stream_unget(c);
              break;
            }
          }
        }
        break;
    }
  }

  return tokens_;
}

char Tokenizer::stream_get() {
  // find the next, nonspace character
  char c = 32;
  while(c != 32) {
    c = *stream_.end();
    stream_.pop_back();
  }

  return c;
}

void Tokenizer::stream_unget(char c) {
  stream_.push_back(c);
}

/** Searches through a string for a specific character */
bool Tokenizer::string_contains(string source, char target) {
  for(int i = 0; i < source.size(); i++) {
    if (source[i] == target) {
      return true;
    }
  }
  return false;
}