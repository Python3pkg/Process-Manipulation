#include <Windows.h>
#include <Python.h>
#include <psapi.h>

static PyObject * ProcList(PyObject * self, PyObject * args) {
	DWORD pids;
	DWORD pid[1024];
	EnumProcesses(pid, sizeof(pid), &pids);
	pids /= sizeof(DWORD);
	PyObject * result = PyList_New(pids);
	for (int i = 0; i < pids; ++i) {
		PyList_SET_ITEM(result, i, Py_BuildValue("I", pid[i]));
	}
	return result;
}

static PyObject * ProcName(PyObject * self, PyObject * args) {
	HMODULE module = 0;
	DWORD modules = 0;
	char name[256] = {};
	int pid = 0;
	PyArg_ParseTuple(args, "I", &pid);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (!EnumProcessModules(process, &module, sizeof(module), &modules)) {
		return Py_BuildValue("s", "");
	}
	GetModuleBaseName(process, module, name, sizeof(name));
	CloseHandle(process);
	return Py_BuildValue("s", name);
}

static PyObject * ProcBase(PyObject * self, PyObject * args) {
	MODULEINFO info = {};
	HMODULE module = 0;
	DWORD modules = 0;
	int pid = 0;
	PyArg_ParseTuple(args, "I", &pid);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	EnumProcessModules(process, &module, sizeof(module), &modules);
	GetModuleInformation(process, module, &info, sizeof(info));
	CloseHandle(process);
	return Py_BuildValue("I", info.lpBaseOfDll);
}

static PyObject * ProcRead(PyObject * self, PyObject * args) {
	void * address = 0;
	int size = 0;
	int pid = 0;
	PyArg_ParseTuple(args, "Iii", &pid, &address, &size);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	char * data = (char *)malloc(size);
	memset(data, 0, size);
	ReadProcessMemory(process, address, data, size, 0);
	PyObject * result = PyBytes_FromStringAndSize(data, size);
	CloseHandle(process);
	free(data);
	return result;
}

static PyObject * ProcWrite(PyObject * self, PyObject * args) {
	void * address = 0;
	char * data = 0;
	int size = 0;
	DWORD old = 0;
	int pid = 0;
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	PyArg_ParseTuple(args, "Iiy#", &pid, &address, &data, &size);
	VirtualProtectEx(process, address, size, PAGE_EXECUTE_READWRITE, &old);
	WriteProcessMemory(process, address, data, size, 0);
	PyObject * result = PyBytes_FromStringAndSize(data, size);
	CloseHandle(process);
	return Py_None;
}

static PyMethodDef methods[] = {
	{"List", ProcList, METH_VARARGS, 0},
	{"Name", ProcName, METH_VARARGS, 0},
	{"Base", ProcBase, METH_VARARGS, 0},
	{"Read", ProcRead, METH_VARARGS, 0},
	{"Write", ProcWrite, METH_VARARGS, 0},
	{0, 0},
};

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, "ProcMod", 0, -1, methods, 0, 0, 0, 0};

extern "C" {
	PyObject * PyInit_ProcMod();
}

PyObject * PyInit_ProcMod() {
	return PyModule_Create(&moduledef);
}
