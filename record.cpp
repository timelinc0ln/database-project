#include "record.h"

#include <sstream>

Record::Record() {
  values_ = vector<pair<string, string > >();
}

Record::Record(vector<pair<string, string> > entries) {
  values_ = entries;
}

Record::~Record() {
  delete &values_;
}

Record::RecordIterator Record::begin() const {
  return values_.begin();
}

Record::RecordIterator Record::end() const {
  return values_.end();
}

template <typename T>
T Record::get(string field) {
	for(int i = 0; i < values_.size(); i++) {
    if(values_[i].first = field) {
      stringstream ss;
      ss << values_[i].second;

      T value;
      ss >> value;

      return value;
    }
	}

  T test;
  // if not found, throw exception
  // throw new ColumnDoesNotExistError(field);
  return test;
}

template <typename T>
void Record::set(string field, T new_value) {
  for(int i = 0; i < values_.size(); i++) {
    if(values_[i].first = field) {
      stringstream ss;
      ss << new_value;

      string string_value;
      ss >> string_value;

      values_[i].second = string_value;
    }
  }

  // if not found, throw exception
  // throw new ColumnDoesNotExistError(field);
}
