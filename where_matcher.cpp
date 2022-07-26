#include "where_matcher.h"

#include <sstream>

WhereMatcher::WhereMatcher(string where_clause) {
  Tokenizer tokenizer(where_clause);
  tokens_ = tokenizer.tokenize();
  tokens_ = vector<Token>(tokens_.rbegin(), tokens_.rend());
}

bool WhereMatcher::does_match(Record record) {
  record_ = record;
  return parse_and();
}

// TODO: Handle parenthesis
template <typename T>
T WhereMatcher::parse_value() {
  Token t = stream_get();

  if(t.first == attribute_name) {
    return record_.get<T>(t.second);
  } else {
    // will not handle dates or times currently
    stringstream ss;
    ss << t.second;

    T value;
    ss >> value;

    return value;
  }
}

bool WhereMatcher::parse_conditional() {
  Token left_token = stream_get();
  Token op_token = stream_get();
  TokenType value_type = left_token.first;

  // the left side is an attribute_name, we need to look ahead for the type
  if(value_type == attribute_name) {
    Token right_token = stream_get();
    value_type = right_token.first;
    stream_unget(right_token);
  }

  stream_unget(left_token);

  void* left;

  // In C++ you can't declare variables of the same name, even if they were
  // never actually declared, this is a really ugly fix, but if there is a way
  // to emulate some sort of dynamic typing C++, we could make this a lot better
  switch(op_token.first) {
    case conditional_eq:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() == parse_value<int>();
        case value_floating:
          return parse_value<float>() == parse_value<float>();
        case value_varchar:
          return parse_value<string>() == parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() == parse_value<date>();
        // case value_time:
        //   return parse_value<time>() == parse_value<time>();
      }
    case conditional_neq:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() != parse_value<int>();
        case value_floating:
          return parse_value<float>() != parse_value<float>();
        case value_varchar:
          return parse_value<string>() != parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() != parse_value<date>();
        // case value_time:
        //   return parse_value<time>() != parse_value<time>();
      }
    case conditional_lt:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() < parse_value<int>();
        case value_floating:
          return parse_value<float>() < parse_value<float>();
        case value_varchar:
          return parse_value<string>() < parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() < parse_value<date>();
        // case value_time:
        //   return parse_value<time>() < parse_value<time>();
      }
    case conditional_gt:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() > parse_value<int>();
        case value_floating:
          return parse_value<float>() > parse_value<float>();
        case value_varchar:
          return parse_value<string>() > parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() > parse_value<date>();
        // case value_time:
        //   return parse_value<time>() > parse_value<time>();
      }
      break;
    case conditional_lte:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() <= parse_value<int>();
        case value_floating:
          return parse_value<float>() <= parse_value<float>();
        case value_varchar:
          return parse_value<string>() <= parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() <= parse_value<date>();
        // case value_time:
        //   return parse_value<time>() <= parse_value<time>();
      }
      break;
    case conditional_gte:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() >= parse_value<int>();
        case value_floating:
          return parse_value<float>() >= parse_value<float>();
        case value_varchar:
          return parse_value<string>() >= parse_value<string>();
        // not worrying about dates or times yet
        // case value_date:
        //   return parse_value<date>() >= parse_value<date>();
        // case value_time:
        //   return parse_value<time>() >= parse_value<time>();
      }
      break;
    default:
      stream_unget(left_token);
      // error
  }
}

bool WhereMatcher::parse_or() {
  bool left = parse_conditional();
  Token t = stream_get();

  if(t.first == bool_or) {
    return left || parse_conditional();
  } else {
    stream_unget(t);
    return left;
  }
}

bool WhereMatcher::parse_and() {
  bool left = parse_or();
  Token t = stream_get();

  if(t.first == bool_and) {
    return left && parse_or();
  } else {
    stream_unget(t);
    return left;
  }
}

Token WhereMatcher::stream_get() {
  Token token = *tokens_.end();
  tokens_.pop_back();

  return token;
}

void WhereMatcher::stream_unget(Token token) {
  tokens_.push_back(token);
}