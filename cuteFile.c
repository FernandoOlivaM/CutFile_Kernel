#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

asmlinkage long cutFile(char *source, char *target)
{


	  int segment_Actual;
	  char aux_buf[1];
	 
	  mm_segment_t segment_Previous = get_fs();
	  set_fs(KERNEL_DS);

      int j = 0;
      char file_buf[1024000000];
      
	  segment_Actual = sys_open(source, O_RDONLY, 0);
	  if (segment_Actual >= 0) {
	    while (sys_read(segment_Actual, aux_buf, 1) == 1){
	        file_buf[j] = aux_buf[0];
		    j++; 
            }
	    sys_close(segment_Actual);
	  }else {
		  return -1;
  	  }
	  set_fs(segment_Previous);	


	  struct file *file;
	  loff_t position = 0;
	  int newMemSeg;

	  mm_segment_t segment_Last = get_fs();
	  set_fs(KERNEL_DS);

	  segment_New = sys_open(target, O_WRONLY|O_CREAT, 0644);
	  if (segment_New >= 0) {
	    sys_write(segment_New, file_buf, strlen(file_buf));
	    file = fget(segment_New);
	    if (file) {
	      vfs_write(file, file_buf, strlen(file_buf), &position);
	      fput(file);
	    }
	    sys_close(segment_New);
	  }
	  else {
		return -1;	
       	}

	  set_fs(segment_Last);
    return 0;
} 