/*
 * gcc -fPIC -shared -o libtest.so test.c
 */

#include <Python.h>

#include "test.h"
#include "struct.h"
#include <vector>


// Список функций модуля
static PyMethodDef methods[] = {
    {"func_hello", func_hello, METH_NOARGS, "func_hello"}, // Функция без аргументов
    {"func_ret_int", func_ret_int, METH_VARARGS, "func_ret_int"}, // Функция с аргументами
    {"func_ret_double", func_ret_double, METH_VARARGS, "func_ret_double"},
    {"func_ret_str", func_ret_str, METH_VARARGS, "func_ret_str"},
    {"func_many_args", func_many_args, METH_VARARGS, "func_many_args"},
    {"func_ret_struct", func_ret_struct, METH_VARARGS, "func_ret_struct"},
    {"add_to_counter", add_to_counter, METH_NOARGS, "add_to_counter"}, // изменяет counter в c коде

    {"get_pointer_list", get_pointer_list, METH_NOARGS, "get_pointer_list"},    
    {"get_list_by_pointer", get_list_by_pointer, METH_VARARGS, "get_list_by_pointer"},
    {"get_list", get_list, METH_NOARGS, "get_list"}, // изменяет counter в c коде
    {"set_list", set_list, METH_VARARGS, "set_list"}, // изменяет counter в c коде

    {NULL, NULL, 0, NULL}
};

// Описание модуля
static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT, "_test", "Test module", -1, methods
};

// Инициализация модуля
PyMODINIT_FUNC 
PyInit__test(void) {
    PyObject *mod = PyModule_Create(&module);

    // Добавляем глобальные переменные
    PyModule_AddObject(mod, "a", PyLong_FromLong(a)); // int
    PyModule_AddObject(mod, "b", PyFloat_FromDouble(b)); // double
    PyModule_AddObject(mod, "c", Py_BuildValue("b", c)); // char
    PyModule_AddObject(mod, "counter", PyLong_FromLong(counter)); // char

    // Добавляем структуру
    
    // Завершение инициализации структуры
    if (PyType_Ready(&test_st_t_Type) < 0)
        return NULL;
    
    Py_INCREF(&test_st_t_Type);
    PyModule_AddObject(mod, "test_st_t", (PyObject *) &test_st_t_Type);
    
    return mod;
}

/**
 * Тестовые функции, тестовые переменные.
 */

int a = 5;
double b = 5.12345;
char c = 'X'; // 88
int counter = 0;
std::vector<int> list{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int intlist[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static PyObject *
get_pointer_list(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    return Py_BuildValue("L", &list);
}

static PyObject *
get_list_by_pointer(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    long long pointer;

    if (!PyArg_ParseTuple(args, "L", &pointer)) 
        Py_RETURN_NONE;
    std::intptr_t ptr = pointer;
    std::vector<int> *_vec = reinterpret_cast<std::vector<int> *>(ptr);
    std::vector<int> &vec = *_vec;

    int size = vec.size();
    PyObject* python_val = PyList_New(size);
    for (int i = 0; i < size; ++i)
    {
        PyObject* python_int = Py_BuildValue("i", vec[i]);
        PyList_SetItem(python_val, i, python_int);
    }
    return python_val;
}

static PyObject *
get_list(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    int size = list.size();
    PyObject* python_val = PyList_New(size);
    for (int i = 0; i < size; ++i)
    {
        PyObject* python_int = Py_BuildValue("i", list[i]);
        PyList_SetItem(python_val, i, python_int);
    }
    return python_val;
}

static PyObject *
set_list(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    PyObject *obj;
    long long pointer;

    if (!PyArg_ParseTuple(args, "OL", &obj, &pointer)) 
        Py_RETURN_NONE;
    printf("Here is address: %lld\n", pointer);
    std::intptr_t ptr = pointer;
    std::vector<int> *_vec = reinterpret_cast<std::vector<int> *>(ptr);
    std::vector<int> &vec = *_vec;

    PyObject *iter = PyObject_GetIter(obj);
    if (!iter) {
        // error not iterator
        Py_RETURN_NONE;
    }

    int i = 0;
    while (true) {
        PyObject *next = PyIter_Next(iter);
        if (!next) {
            // nothing left in the iterator
            break;
        }

        if (!PyLong_Check(next)) {
            // error, we were expecting a int value
            Py_RETURN_NONE;
        }

        int foo = PyLong_AsLong(next);
        // do something with foo

        vec[i] = foo;
        i++;
    }

    Py_RETURN_NONE;
}

static PyObject *
add_to_counter(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    counter++;
    return Py_BuildValue("i", counter);
}

static PyObject *
func_hello(PyObject *self, PyObject *args) { // Можно без args, но будет warning при компиляции.
    puts("Hello!");
    Py_RETURN_NONE;
}

/**
 * Получение значения переменной содержащей значение типа int и возврат его.
 */
static PyObject *
func_ret_int(PyObject *self, PyObject *args) {
    int val;

    // Проверка кол-ва аргументов
    if (PyTuple_Size(args) != 1) {
        PyErr_SetString(self, "func_ret_int args error");
    }

    PyArg_ParseTuple(args, "i", &val);
    /* 
     * Альтернативный вариант.
     * 
    // Получаем аргумент
    PyObject *obj = PyTuple_GetItem(args, 0);
    // Проверяем его на тип int/long
    if (PyLong_Check(obj)) {
        PyErr_Print();
    }
    // Приводим (PyObject *) к int
    val = _PyLong_AsInt(obj);
     */
    printf("C get func_ret_int: %d\n", val);
    return Py_BuildValue("i", val);
}

/**
 * Получение значения переменной содержащей значение типа double и возврат его.
 */
static PyObject *
func_ret_double(PyObject *self, PyObject *args) {
    double val;

    if (PyTuple_Size(args) != 1) {
        PyErr_SetString(self, "func_ret_double args error");
    }

    PyArg_ParseTuple(args, "d", &val);

    printf("C get func_ret_double: %f\n", val);
    return Py_BuildValue("f", val);
}

/**
 * Получение string и возврат его.
 */
static PyObject *
func_ret_str(PyObject *self, PyObject *args) {
    char *val;

    if (PyTuple_Size(args) != 1) {
        PyErr_SetString(self, "func_ret_str args error");
    }

    PyArg_ParseTuple(args, "s", &val);
    /* 
     * Альтернативный вариант.
     * 
    PyObject *obj = PyTuple_GetItem(args, 0);
    
    PyObject* pResultRepr = PyObject_Repr(obj);
    val = PyBytes_AS_STRING(PyUnicode_AsEncodedString(pResultRepr, "utf-8", "ERROR"));
     */
    printf("C get func_ret_str: %s\n", val);
    return Py_BuildValue("s", val);
}

/**
 * Получение значения переменных содержащих значения типа int, double, char *.
 */
static PyObject *
func_many_args(PyObject *self, PyObject *args) {
    int val1;
    double val2;
    char *val3;

    if (PyTuple_Size(args) != 3) {
        PyErr_SetString(self, "func_ret_str args error");
    }

    PyArg_ParseTuple(args, "ids", &val1, &val2, &val3);

    printf("C get func_many_args: int - %d, double - %f, string - %s\n", val1, val2, val3);
    return Py_BuildValue("ifs", val1, val2, val3);
}

static PyObject *
func_ret_struct(PyObject *self, PyObject *args) {
    
    test_st_t *st;
    
    // Получаем структуру из Python
    if (!PyArg_ParseTuple(args, "O", &st)) // O - объект данных
        Py_RETURN_NONE;
    
    printf("C get test_st: val1 - %d, val2 - %f, val3 - %d\n", st->val1++, st->val2++, st->val3++);

    return Py_BuildValue("O", st);
}
