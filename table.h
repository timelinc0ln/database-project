#ifndef TABLE_H_
#define TABLE_H_

#include <deque>
#include <string>
#include <vector>
using namespace std;

#include "exception.h"

class Record;

/**
 * A table.
 *
 * Tables consist of columns (each with a name and a type) and rows (records).
 *
 * A table may or may not be inside a database. Often it is used for result sets
 * from queries on a larger table.
 */
class EXPORT Table
{
public:
  /** An enum specifying the type of a column. */
  enum RecordType {
    undefined_type = 0, ///< Not for normal use
    integer        = 1, ///< 32-bit signed integer
    floating       = 2, ///< 32-bit floating point
    varchar        = 3, ///< Variable-length string
    date           = 4, ///< Date with no time
    time           = 5  ///< Time without time zone
  };

  /**
   * A const iterator over the records in a table.
   *
   * Do not depend on the underlying type of TableIterator in your code, but
   * rather a generic iterator interface.
   */
  typedef deque<Record>::const_iterator TableIterator;

  typedef vector<pair<string, RecordType> > ColumnList;

  /** Creates a table with no rows or columns. */
  Table();
  /**
   * Creates a table with the given column names and types and no rows.
   *
   * Example:
   * ~~~{.cpp}
   * Table::ColumnList columns;
   * columns.push_back(make_pair("id", Table::integer));
   * columns.push_back(make_pair("name", Table::varchar));
   * Table my_table(columns);
   * ~~~
   */
  Table(const ColumnList& columns);

  ~Table();

  /**
   * Adds a column to the end of the table.
   *
   * Existing entries will be NULL for this column.
   */
  void add_column(string column_name, RecordType type);

  /**
   * Deletes a column, erasing any associated data.
   *
   * Throws a \a ColumnDoesNotExistError if \a column_name doesn't exist.
   */
  void del_column(string column_name);

  /**
   * Renames a column, keeping the existing type and data.
   * Throws a \a ColumnDoesNotExistError if \a from doesn't exist.
   */
  void rename_column(string from, string to);

  /** Returns a list of columns and their types. */
  ColumnList columns() const;

  /**
   * Returns the index of the column specified in column_name, i.e. for accessing records.
   * Throws a \a ColumnDoesNotExistError if \a from doesn't exist.
   */
  unsigned int index_for(string column_name) const;

  /**
   * Defines the tuple of columns used as a key.
   * This function must be called before inserting any rows into the table.
   *
   * \param columns A list of column names that make up the key.
   *
   * Every row in the table must have a unique key. If a new row is inserted
   * with a key that already exists in the table, insertion will fail.
   *
   * Throws a \a ColumnDoesNotExistError if any of the \a column_names don't exist.
   * Throws an \a InvalidOperationError if called on a table with rows.
   */
  void set_key(vector<string> column_names);

  /**
   * Returns the column names that make up the key for this table.
   *
   * If there is no key, returns an empty vector.
   */
  vector<string> key() const;

  /** Returns the number of rows in the table. */
  int size() const;

  /**
   * Inserts a row at the end of the table.
   * Throws a \a KeyConflictError if this record would cause a primary key conflict.
   */
  void insert(const Record& record);

  /**
   * Returns an iterator to the first record in the table.
   *
   * A Table can be treated as a container. The *begin* and *end* functions
   * correspond to C++ STL container begin and end functions, as do *first*,
   * *last*, and *at*.
   *
   * \sa end(), first(), last(), at()
   */
  TableIterator begin() const;
  /**
   * Returns an iterator past the end of the last record in the table.
   * \sa begin()
   */
  TableIterator end() const;

  /**
   * Returns the first record in the table.
   * \sa last(), at()
   */
  const Record& first() const;
  /**
   * Returns the last record in the table.
   * \sa first(), at()
   */
  const Record& last() const;
  /**
   * Returns the *i*th record in the table, starting at 0.
   * Throws a \a RowDoesNotExistError if \a i is out of range (< 0 or >= size().)
   * \sa first(), last(), begin(), end()
   */
  const Record& at(unsigned int i) const;

  /**
   * Computes a cross join with another table.
   *
   * A cross join contains every possible combination of rows in the two
   * tables.
   */
  Table cross_join(const Table& other) const;

  /**
   * Computes a natural join with another table.
   *
   * The other table should have a key, and this table should have columns
   * matching that key.
   *
   * Throws an \a InvalidOperationError if the above conditions are not met.
   */
  Table natural_join(const Table& other) const;

  /**
   * Computes the number of non-NULL values in the given column in the table.
   * Throws a \a ColumnDoesNotExistError if \a column_name doesn't exist.
   */
  int count(string column_name) const;

  /**
   * Computes the sum of all values in the given column in the table.
   * Works for numeric column types only.
   *
   * Throws a \a ColumnDoesNotExistError if \a column_name doesn't exist.
   * Throws an \a InvalidOperationError if the column is not numeric.
   * Throws an \a InvalidTypeError if the column cannot be converted to type T.
   */
  template<typename T>
  T sum(string column_name) const;

  /**
   * Computes the smallest value of all values in the given column in the table.
   * Works for all column types.
   *
   * Throws a \a ColumnDoesNotExistError if \a column_name doesn't exist.
   * Throws an \a InvalidTypeError if the column cannot be converted to type T.
   */
  template<typename T>
  T min(string column_name) const;

  /**
   * Computes the largest value of all values in the given column in the table.
   * Works for all column types.
   *
   * Throws a \a ColumnDoesNotExistError if \a column_name doesn't exist.
   * Throws an \a InvalidTypeError if the column cannot be converted to type T.
   */
  template<typename T>
  T max(string column_name) const;

private:
  deque<Record> records_;
};

#endif  // TABLE_H_
