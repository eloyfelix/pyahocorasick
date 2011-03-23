static PyObject*
pymod_get_string(PyObject* obj, char** word, ssize_t* wordlen, bool* unicode) {
	if (PyBytes_Check(obj)) {
		*word = (char*)PyBytes_AS_STRING(obj);
		*wordlen = PyBytes_GET_SIZE(obj);
		Py_INCREF(obj);
		*unicode = false;
		return obj;
	}
	else if (PyUnicode_Check(obj)) {
		*word = (char*)PyUnicode_AS_UNICODE(obj);
		*wordlen = PyUnicode_GET_SIZE(obj);
		Py_INCREF(obj);
		*unicode = true;
		return obj;
	}
	else {
		PyErr_SetString(PyExc_ValueError, "string or bytes object expected");
		return NULL;
	}
}


static PyObject*
pymod_get_string_from_tuple(PyObject* tuple, int index, char** word, ssize_t* wordlen, bool* unicode) {
	PyObject* obj;

	obj = PyTuple_GetItem(tuple, index);
	if (obj)
		return pymod_get_string(obj, word, wordlen, unicode);
	else
		return NULL;
}

/**
	[start, [end]]
*/
static int
pymod_parse_start_end(
	PyObject* args,
	int idx_start, int idx_end,
	const ssize_t min, const ssize_t max,
	ssize_t* Start, ssize_t* End
) {
	PyObject* obj;
#define start (*Start)
#define end (*End)

	start	= min;
	end		= max;

	// first argument
	obj = PyTuple_GetItem(args, idx_start);
	if (obj == NULL) {
		PyErr_Clear();
		return 0;
	}

	obj = PyNumber_Index(obj);
	if (obj == NULL)
		return -1;

	start = PyNumber_AsSsize_t(obj, PyExc_IndexError);
	if (start == -1 and PyErr_Occurred())
		return -1;

	if (start < 0)
		start = max + start;

	if (start < min or start >= max) {
		PyErr_Format(PyExc_IndexError, "start index not in range %d..%d", min, max);
		return -1;
	}

	// second argument
	obj = PyTuple_GetItem(args, idx_end);
	if (obj == NULL) {
		PyErr_Clear();
		return 0;
	}

	obj = PyNumber_Index(obj);
	if (obj == NULL)
		return -1;

	end = PyNumber_AsSsize_t(obj, PyExc_IndexError);
	if (end == -1 and PyErr_Occurred())
		return -1;

	if (end < 0)
		end = max - 1 + end;

	if (end < min or end >= max) {
		PyErr_Format(PyExc_IndexError, "end index not in range %d..%d", min, max);
		return -1;
	}

	return 0;

#undef start
#undef end
}

