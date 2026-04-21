#ifndef LPNU_COMPAT_H
#define LPNU_COMPAT_H

#include <linux/types.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>

#include "../xnu-xnu-11215.1.10/bsd/sys/buf_internal.h"
#include "../xnu-xnu-11215.1.10/osfmk/vm/vm_page.h"

typedef struct buf *lpnu_buf_t;
typedef struct vm_page *lpnu_page_t;

#define LPNU_BREAD(bh, dev, block) bread((dev), (block), (bh)->b_size)
#define LPNU_BWRITE(bh) write_buffer((bh))
#define LPNU_BRELEASE(bh) brelse((bh))

#define LPNU_BP_GETFSPRIVATE(bp) ((bp)->b_fsprivate)
#define LPNU_BP_SETFSPRIVATE(bp, val) ((bp)->b_fsprivate = (val))

struct lpnu_vnode {
    struct inode vfs_inode;
    void *private_data;
    unsigned long vnode_id;
};

struct lpnu_file {
    struct file *filp;
    loff_t position;
};

int lpnu_vnode_getattr(struct lpnu_vnode *vp, void *attr);
int lpnu_vnode_setattr(struct lpnu_vnode *vp, void *attr);

int lpnu_kext_load(const char *path);
int lpnu_kext_unload(const char *identifier);

int lpnu_io_connect(struct file *filp);

#endif