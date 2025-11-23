#ifndef PROGRAM_H
#define PROGRAM_H

void __common__start_program(int argc, char** argv);
void __common__end_program();
void __common__runtime_check_gl();
void __common__global_values_check();
double __common__get_program_duration();

#endif
