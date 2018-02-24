/*------------------------------------------------------------------------*/
/* Sample code of OS dependent controls for FatFs                         */
/* (C)ChaN, 2012                                                          */
/*------------------------------------------------------------------------*/

#include <stdlib.h>		/* ANSI memory controls */
//#include <malloc.h>		/* ANSI memory controls */

#include "os.h"
#include "cmsis_os.h"

#include "../ff.h"


#if _FS_REENTRANT
/*------------------------------------------------------------------------*/
// Create a Synchronization Object
/*------------------------------------------------------------------------*/
/* This function is called by f_mount() function to create a new
/  synchronization object, such as semaphore and mutex. When a 0 is
/  returned, the f_mount() function fails with FR_INT_ERR.
*/

int ff_cre_syncobj (	/* 1:Function succeeded, 0:Could not create due to any error */
                    BYTE vol,			/* Corresponding logical drive being processed */
                    _SYNC_t* sobj		/* Pointer to return the created sync object */
                        )
{
    int ret;
    
    
    //	*sobj = CreateMutex(NULL, FALSE, NULL);		/* Win32 */
    //	ret = (int)(*sobj != INVALID_HANDLE_VALUE);
    
    //	*sobj = SyncObjects[vol];		/* uITRON (give a static created semaphore) */
    //	ret = 1;
    
    //	*sobj = OSMutexCreate(0, &err);	/* uC/OS-II */
    //	ret = (int)(err == OS_NO_ERR);
	
	//osMutexId a;
    osMutexDef(sob);
	*sobj = osMutexCreate(osMutex(sob));
	
   // sobj = xSemaphoreCreateMutex();	/* FreeRTOS */
	
	
    ret = (int)(*sobj != NULL);
    
    return ret;
	
	//return 1;
}



/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is
/  returned, the f_mount() function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to any error */
                    _SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
                        )
{
    int ret;
    
    
    	//ret = CloseHandle(sobj);	/* Win32 */
    
    	//ret = 1;					/* uITRON (nothing to do) */
    
    	//OSMutexDel(sobj, OS_DEL_ALWAYS, &err);	/* uC/OS-II */
    	//ret = (int)(err == OS_NO_ERR);
    //osMutexDelete
	osMutexDelete(sobj);
    //vSemaphoreDelete(sobj);		/* FreeRTOS */
    ret = 1;
    
    return ret;
}



/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a FALSE is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
                  _SYNC_t sobj	/* Sync object to wait */
                      )
{
    int ret;
    
    	//ret = (int)(WaitForSingleObject(sobj, _FS_TIMEOUT) == WAIT_OBJECT_0);	/* Win32 */
    
    	//ret = (int)(wai_sem(sobj) == E_OK);			/* uITRON */
    
    	//OSMutexPend(sobj, _FS_TIMEOUT, &err));		/* uC/OS-II */
    	//ret = (int)(err == OS_NO_ERR);
    
    //ret = (int)(xSemaphoreTake(sobj, _FS_TIMEOUT) == pdTRUE);	/* FreeRTOS */
	
	osMutexWait(sobj,_FS_TIMEOUT);
	ret = TRUE;
    
    return ret;
}



/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
                   _SYNC_t sobj	/* Sync object to be signaled */
                       )
{
    	//ReleaseMutex(sobj);		/* Win32 */
    
    	//sig_sem(sobj);			/* uITRON */
    
   // 	OSMutexPost(sobj);		/* uC/OS-II */
    
    osMutexRelease((void *)sobj);	/* FreeRTOS */
}

#endif




#if _USE_LFN == 3	/* LFN with a working buffer on the heap */
/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
/* If a NULL is returned, the file function fails with FR_NOT_ENOUGH_CORE.
*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block */
                   UINT msize		/* Number of bytes to allocate */
                       )
{
    return malloc(msize);
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree (
                 void* mblock	/* Pointer to the memory block to free */
                     )
{
    free(mblock);
}

#endif

#include <time.h>
DWORD get_fattime(void)
{
    /*
    bit31:25 Year origin from the 1980 (0..127)
    bit24:21 Month (1..12)
    bit20:16 Day of the month(1..31)
    bit15:11 Hour (0..23)
    bit10:5  Minute (0..59)
    bit4:0   Second / 2 (0..29)
    */
    time_t rawtime;
    struct tm * t;
    
    time (&rawtime);
    t = localtime (&rawtime);
    
    return ((t->tm_year - 80) << 25) | ((t->tm_mon) << 21) | (t->tm_mday << 16)
        | (t->tm_hour << 11) | (t->tm_min << 5) | (t->tm_sec/2);
}
