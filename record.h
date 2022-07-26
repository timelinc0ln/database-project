#ifndef _RECORD_H_
#define _RECORD_H_

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "exception.h"

/**
 * Allows for read and write access of field values.
 *
 * In the context of a table, fields are known as columns, but within a record
 * they are called fields.
 */
class EXPORT Record {
public:
  /**
   * A const iterator over the fields in a record.
   *
   * Each element accessed by the iterator is a pair. The first element is the
   * field name, the second is the value (in string form.)
   * \sa begin(), end()
   */
  typedef vector<pair<string, string> >::const_iterator RecordIterator;

  /**
    Create a record with no fields and no data.
    */
  Record();
  /**
    Create a record with existing entries
    \param entries a std::vector of pairs, with the first element in the pair
           being the field/column name, and the second element being the value.
   */
  Record(vector<pair<string, string> > entries);

  ~Record();

  /**
   * Returns an iterator to the first field in the record.
   *
   * Note that the RecordIterator gives you values in string form. If you want
   * automatic conversions, use *get* instead.
   *
   * \sa get(), end()
   */
  RecordIterator begin() const;
  /**
   * Returns an iterator past the end of the fields in the record.
   * \sa begin()
   */
  RecordIterator end() const;

  /**
    Get the value of a field by column name. The field is converted to the
    requested C++ type if possible (otherwise, an \a InvalidTypeError is thrown.)

    Example:
    ~~~{.cpp}
    myRecord.get<int>("age");
    string name = myRecord.get<string>("name");
    ~~~

    Throws a \a ColumnDoesNotExistError if \a field doesn't exist.

    \param T The expected type of the field
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  T get(string field);

  /**
    Set the value of a field by column name. The field is converted from the
    given C++ type if possible (otherwise, an \a InvalidTypeError is thrown.)

    Example:
    ~~~{.cpp}
    myRecord.set("age", 21);
    myRecord.set("name", "Abraham Lincoln");
    ~~~

    Throws a \a ColumnDoesNotExistError if \a field doesn't exist.

    \param T The type of *field*; usually inferred by the compiler.
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  void set(string field, T value);

private:
  vector<pair<string, string> > values_;
};

#endif
