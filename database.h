#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <string>
#include <vector>
#include <map>

#include "record.h"
#include "table.h"
#include "exception.h"

/** The entry point for creating tables, deleting records,
    and running queries.
 */
class EXPORT Database {
public:
  /** Creates an empty database */
  Database();

  /**
    Add a table to the database.

    Ownership of the table is permanently transferred to the database, and it will be
    destroyed when the database is destroyed. You MUST allocate the table with
    *new*. Do NOT destroy the table after adding it to the database!

    ~~~{.cpp}
    Database db;
    Table *table = new Table;
    db.add_table("my_table", table);
    ~~~

    Throws an \a InvalidOperationError if \a name already exists in the database.

    \param name what to call the table in the database
    \param table the Table to be inserted into the database
    \sa Table
   */
  void add_table(string name, Table* table);

  /**
    Remove a table from the database.

    The table is destroyed with *delete* when this function is called.

    Throws a \a TableDoesNotExistError if \a name does not exist in the database.

    \param name which table to remove from the database
    \returns A pointer to the Table, which can now be destroyed.
    \sa table_names()
   */
  void drop_table(string name);

  /**
    Returns a list of all the tables currently in the database
   */
  vector<string> table_names();

  /**
    Returns the table named *table_name* in the database.
    Throws a \a TableDoesNotExistError if \a table_name does not exist in the database.
    */
  Table* table(string table_name);

  /**
    Returns the table named *table_name* in the database.
    Returns NULL if \a table_name does not exist in the database.
    */
  Table* table_if_exists(string table_name);

  /**
    Perform a query on the database. Here are some examples of possible queries:

    ~~~{.cpp}
    myDatabase.query("name", "students", "gender = 'male'");
    myDatabase.query("name", "students", "(gender = 'male' AND age > 21) OR age > 65");
    myDatabase.query("name", "students", "gender = 'female' AND id IN good_students");
    myDatabase.query("*", "students", "gpa >= ALL(good_student_gpas)");
    myDatabase.query("id", "students", "gpa >= ANY(good_student_gpas) AND NOT (id IN good_students)");
    myDatabase.query("*", "students", "EXISTS(good_students)");
    ~~~

    Throws a \a TableDoesNotExistError if \a from does not exist.
    Throws a \a QuerySyntaxError if \a select or \a where have a syntax error.

    \param select which columns to include in the returned Table
    \param from which table to query from
    \param where the conditions for the query to match
    \return A pointer to Table with all of the records that match the query
   */
  Table* query(string select, string from, string where);

  /**
    Delete all records that match the query.

    Throws a \a TableDoesNotExistError if \a from does not exist.
    Throws a \a QuerySyntaxError if \a where has a syntax error.

    \param from which table to query from
    \param where the conditions for the query to match
   */
  void delete_from(string from, string where);

  /**
    Mass modify records in table. Examples:

    ~~~{.cpp}
    myDatabase.update("students", "gender = 'male'", "gender = 'm'");

    myDatabase.update("students", "gender = 'male'", "gender = 'm', school = 'A&M'");

    myDatabase.update("students", "gender = 'female'", "age = age * 2");
    ~~~

    Throws a \a TableDoesNotExistError if \a table does not exist.
    Throws a \a QuerySyntaxError if \a where or \a set have a syntax error.

    \param table name of the table to update records in
    \param where a SQL where clause to find records in the table
    \param set a SQL set clause
   */
  void update(string table, string where, string set);

  /**
    Save the database to a file
    Throws an \a IOError on failture.
    \param filename the output file
   */
  void save(string filename);

  /**
    Load a database from a file, this will clear any existing records
    Throws an \a IOError on failture.
    \param filename the input file
   */
  void load(string filename);

  /**
    Merge another database into this one.
    Tables in this database are overwritten by tables in \a database.
    \param database The database that you want to merge into this one.
   */
   void merge(const Database& database);

  /**
    Make a copy of this database
    \returns a one-for-one copy / clone of this database
   */
  Database copy();
};

#endif
