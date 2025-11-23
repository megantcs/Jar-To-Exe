#ifndef LOGGER_H
#define LOGGER_H

struct Logger
{
	const char* _prefix;

	char start_prefix[255];
	char end_prefix[255];

	int printf;
};

struct Logger create_logger(const char* prefix);

void log(struct Logger* _this, const char* _format, ...);

#endif