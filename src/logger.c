#include "header/logger.h"
#include <stdarg.h>
#include <stdio.h>


struct Logger create_logger(const char* prefix)
{
	return (struct Logger) { ._prefix = prefix };
}

void log(struct Logger* _this, const char* _format, ...)
{
	va_list list;

	va_start(list, _format);

		printf("$ [%s] ", _this->_prefix);
		vprintf(_format, list);
		printf("\n");

	va_end(list);
	
}
