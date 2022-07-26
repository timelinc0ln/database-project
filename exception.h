/**
 * \file exception.h
 * Defines the exception classes used by this database.
 *
 * Every exception inherits from std::exception, and a detailed message can be
 * obtained using *what()*.
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#define EXPORT __declspec(dllexport)

#include <stdexcept>

using namespace std;

class EXPORT ColumnDoesNotExistError : public exception {
public:
  ColumnDoesNotExistError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT RowDoesNotExistError : public exception {
public:
  RowDoesNotExistError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT TableDoesNotExistError : public exception {
public:
  TableDoesNotExistError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT InvalidOperationError : public exception {
public:
  InvalidOperationError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT KeyConflictError : public InvalidOperationError {
public:
  KeyConflictError(const string& what)
    : InvalidOperationError(what) {}
};

/** Thrown when the specified value cannot be converted to the requested type. */
class EXPORT InvalidTypeError : public exception {
public:
  InvalidTypeError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT QuerySyntaxError : public exception {
public:
  QuerySyntaxError(const string& what)
    : exception(what.c_str()) {}
};

class EXPORT IOError : public exception {
public:
  IOError(const string& what)
    : exception(what.c_str()) {}
};

#endif  // EXCEPTION_H_
