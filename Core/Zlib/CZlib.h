#ifndef ZLIB_PIPE_H
#define ZLIB_PIPE_H

#include <stdafx.h>

class CZlib{
	public:
		int DecompStream(FILE* source, FILE* dest);

};

#endif