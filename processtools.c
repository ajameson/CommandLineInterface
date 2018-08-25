
/**
 * @file processtools.c
 * @brief Tools to manage processes
 * 
 * Manages structure PROCESSINFO
 * 
 * @author Olivier Guyon
 * @date 24 Aug 2018
 */




#define _GNU_SOURCE


/* =============================================================================================== */
/* =============================================================================================== */
/*                                        HEADER FILES                                             */
/* =============================================================================================== */
/* =============================================================================================== */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <malloc.h>
#include <sys/mman.h> // mmap()

#include <time.h>

#include <unistd.h>    // getpid()
#include <sys/types.h>

#include <CommandLineInterface/CLIcore.h>




/* =============================================================================================== */
/* =============================================================================================== */
/*                                      DEFINES, MACROS                                            */
/* =============================================================================================== */
/* =============================================================================================== */




/* =============================================================================================== */
/* =============================================================================================== */
/*                                  GLOBAL DATA DECLARATION                                        */
/* =============================================================================================== */
/* =============================================================================================== */














/* =============================================================================================== */
/* =============================================================================================== */
/*                                    FUNCTIONS SOURCE CODE                                        */
/* =============================================================================================== */
/* =============================================================================================== */



//
// Create processinfo in shared memory
//
PROCESSINFO* processinfo_shm_create(char *pname)
{
    size_t sharedsize = 0; // shared memory size in bytes
    int SM_fd; // shared memory file descriptor
    PROCESSINFO *pinfo;
    
    
    sharedsize = sizeof(PROCESSINFO);

	char  SM_fname[200];
    pid_t PID;

    
    PID = getpid();

    
    sprintf(SM_fname, "%s/%s.%d.proc.shm", SHAREDMEMDIR, pname, (int) PID);    
    SM_fd = open(SM_fname, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (SM_fd == -1) {
        perror("Error opening file for writing");
        exit(0);
    }

    int result;
    result = lseek(SM_fd, sharedsize-1, SEEK_SET);
    if (result == -1) {
        close(SM_fd);
        fprintf(stderr, "Error calling lseek() to 'stretch' the file");
        exit(0);
    }

    result = write(SM_fd, "", 1);
    if (result != 1) {
        close(SM_fd);
        perror("Error writing last byte of the file");
        exit(0);
    }

    pinfo = (PROCESSINFO*) mmap(0, sharedsize, PROT_READ | PROT_WRITE, MAP_SHARED, SM_fd, 0);
    if (pinfo == MAP_FAILED) {
        close(SM_fd);
        perror("Error mmapping the file");
        exit(0);
    }

	printf("created processinfe entry at %s\n", SM_fname);
    printf("shared memory space = %ld bytes\n", sharedsize); //TEST

	clock_gettime(CLOCK_REALTIME, &pinfo->createtime);
	

    return pinfo;
}






//
// Remove processinfo in shared memory
//
int processinfo_shm_rm(char *pname)
{
	return 0;
}