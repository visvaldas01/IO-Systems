#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 1 << 12

int main(int argc, char *argv[]) {
    FILE *dev_file = fopen("/dev/var5", "r+");
    FILE *proc_file = fopen("/proc/var5", "r");

    if (dev_file != NULL && proc_file != NULL)  {

    	printf("Hff4\n");
    	fputs("Hff4\n", dev_file);

    	printf("OK\n");
    	fclose(dev_file);

    	char message[BUFFER_SIZE];
    	while (feof(proc_file) == 0) {
    		if (fgets(message, BUFFER_SIZE, proc_file) == 0) {
    			break;
    		}

    		if (message == NULL) {
    			printf("Error\n");
    			break;
    		}

    		printf("Test: %s\n", message);
    	}

        
        fclose(proc_file);
    } else {
        printf("file is not found.\n");
    }

    return 0;
}
