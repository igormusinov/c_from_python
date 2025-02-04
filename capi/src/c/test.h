#ifndef _TEST_H_
#define	_TEST_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <Python.h>
    
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cstdint>

typedef struct test_st_s test_st_t;

extern int a;
extern double b;
extern char c;
extern int counter;

static PyObject *func_hello(PyObject *self, PyObject *args);
static PyObject *func_ret_int(PyObject *self, PyObject *args);
static PyObject *func_ret_double(PyObject *self, PyObject *args);
static PyObject *func_ret_str(PyObject *self, PyObject *args);
static PyObject *func_many_args(PyObject *self, PyObject *args);
static PyObject *func_ret_struct(PyObject *self, PyObject *args);

static PyObject *add_to_counter(PyObject *self, PyObject *args);

static PyObject *get_pointer_list(PyObject *self, PyObject *args);
static PyObject *get_list_by_pointer(PyObject *self, PyObject *args);
static PyObject *get_list(PyObject *self, PyObject *args);
static PyObject *set_list(PyObject *self, PyObject *args);

struct test_st_s {
    PyObject_HEAD // Макрос объявления нового типа, объекта фиксированного размера
    int val1;
    double val2;
    char val3;
};

#ifdef	__cplusplus
}
#endif

#endif	/* _TEST_H_ */
