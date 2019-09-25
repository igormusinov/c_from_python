#include "test.h"
#include "struct.h"

static void
test_st_t_dealloc(test_st_t* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
test_st_t_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    test_st_t *self;

    self = (test_st_t *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->val1 = 0;
        self->val2 = 0.0;
        self->val3 = 0;
    }

    return (PyObject *)self;
}

static int
test_st_t_init(test_st_t *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"val1", "val2", "val3", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|idb", kwlist, &self->val1, &self->val2, &self->val3))
        return -1;

    return 0;
}

static PyMemberDef test_st_t_members[] = {
    {"val1", T_INT, offsetof(test_st_t, val1), 0, "int"},
    {"val2", T_DOUBLE, offsetof(test_st_t, val2), 0, "double"},
    {"val3", T_CHAR, offsetof(test_st_t, val3), 0, "char"},
    {NULL}
};

static PyMethodDef test_st_t_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject test_st_t_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_test.test_st_t",         /* tp_name */
    sizeof(test_st_t),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)test_st_t_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "test_st_t objects",       /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    test_st_t_methods,         /* tp_methods */
    test_st_t_members,         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)test_st_t_init,  /* tp_init */
    0,                         /* tp_alloc */
    test_st_t_new,             /* tp_new */
};

static PyModuleDef noddy2module = {
    PyModuleDef_HEAD_INIT,
    "noddy2",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

/*
PyMODINIT_FUNC
PyInit_noddy2(void)
{
    PyObject* m;

    if (PyType_Ready(&test_st_t_Type) < 0)
        return NULL;

    m = PyModule_Create(&noddy2module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&test_st_t_Type);
    PyModule_AddObject(m, "test_st_t", (PyObject *)&test_st_t_Type);
    return m;
}
*/
