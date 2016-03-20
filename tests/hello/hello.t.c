#include "hello.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool hello_me_test() {
	assert((strcmp(hello_me(), "Hello world to me!") == 0) && "hello_me() didn't return Hello world to me!\n");
	return EXIT_SUCCESS;
}

int main() {

	/* You may uncomment the following line to test valgrind's integration. */
	/* void *p = malloc(16); */

	hello_me_test();
	return EXIT_SUCCESS;
}
