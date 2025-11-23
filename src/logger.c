#include "header/logger.h"
#include <stdarg.h>
#include <stdio.h>


struct Logger create_logger(const char* prefix)
{
	return (struct Logger) { ._prefix = prefix, .printf = 1 };
}

void log(struct Logger* _this, const char* _format, ...)
{
	if (_this->printf != 1) return;

	va_list list;

	va_start(list, _format);

		if (_this->start_prefix) printf(_this->start_prefix);
		printf("$ [%s] ", _this->_prefix);
		vprintf(_format, list);
		if (_this->end_prefix) printf(_this->end_prefix);
		printf("\n");

	va_end(list);
	
}
