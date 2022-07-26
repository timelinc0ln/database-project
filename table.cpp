#include "table.h"

Table::Table() {
	records_ = deque<Record> ();
};

Table::Table(deque<Record> record_entry) {
	records_ = record_entry;
};


void add_column(string column_name, RecordType type) { 

};


void del_column(string column_name) {

};


void rename_column(string from, string to) {

};


ColumnList columns() const {

};

unsigned int index_for(string column_name) const {

};


void set_key(vector<string> column_names) {

};


int size() const {

};


void insert(const Record& record) {

};


TableIterator begin() const {

};

TableIterator end() const {

};


const Record& first() const {

};

const Record& last() const {

};

const Record& at(unsigned int i) const {

};


Table cross_join(const Table& other) const {

};


Table natural_join(const Table& other) const {

};


int count(string column_name) const {

};


template<typename T>
T sum(string column_name) const {

};


template<typename T>
T min(string column_name) const {

};


template<typename T>
T max(string column_name) const {
  
};