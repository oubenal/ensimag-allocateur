#include <python3.4m/Python.h>
#include "mem.h"

static PyObject *
mempy_alloc(PyObject *self, PyObject *args) {
  unsigned long taille;
  unsigned long addr;

  if (!PyArg_ParseTuple(args, "k", &taille))
        return NULL;
  addr = (unsigned long) mem_alloc(taille);
  return PyLong_FromUnsignedLong(addr);
}

static PyObject *
mempy_free(PyObject *self, PyObject *args) {
  unsigned long taille;
  unsigned long addr;

  if (!PyArg_ParseTuple(args, "kk", & addr, &taille))
        return NULL;
  long ret = (long) mem_free((void*) addr, taille);
  return PyLong_FromLong(ret);
}

static PyObject *
mempy_init(PyObject *self, PyObject *args) {
  long ret;

  ret = (long) mem_init();
  return PyLong_FromLong(ret);
}

static PyObject *
mempy_destroy(PyObject *self, PyObject *args) {
  long ret = (long) mem_destroy();
  return PyLong_FromLong(ret);
}

static PyMethodDef EnsiAllocMethods[] = {
    {"alloc",  mempy_alloc, METH_VARARGS,
     "allocate a bloc of the argument size, return the pointer value"},
    {"free",  mempy_free, METH_VARARGS,
     "free a bloc with the argument pointer value of the argument size"},
    {"init",  mempy_init, METH_VARARGS,
     "init allocator"},
    {"destroy",  mempy_destroy, METH_VARARGS,
     "free memory used by the allocator"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};



static struct PyModuleDef mempymodule = {
   PyModuleDef_HEAD_INIT,
   "ensialloc",   /* name of module */
   NULL, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   EnsiAllocMethods
};

PyMODINIT_FUNC
PyInit_mempy(void)
{
    return PyModule_Create(&mempymodule);
}
