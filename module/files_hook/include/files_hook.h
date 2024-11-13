#ifndef FILES_HOOK_H
#define FILES_HOOK_H

#include <linux/types.h>  // For __u32, __u64, etc.

/* Define structure for 64-bit linux_dirent64 */
struct linux_dirent64 {
    __u64 d_ino;       // Inode number
    __s64 d_off;       // Offset to the next linux_dirent
    unsigned short d_reclen;   // Length of this record
    char d_name[];             // Filename (variable length)
};

/* Define structure for 32-bit linux_dirent */
struct linux_dirent {
    __u32 d_ino;       // Inode number
    __s32 d_off;       // Offset to the next linux_dirent
    unsigned short d_reclen;   // Length of this record
    char d_name[];             // Filename (variable length)
};

#endif /* FILES_HOOK_H */
