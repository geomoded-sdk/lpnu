# LPNU Kernel
Linux with XNU compatibility - APFS, HFS+, Mach-O, KEXT support

## Based On
- Linux Kernel 7.0
- XNU (xnu-11215.1.10) for macOS compatibility
- linux-apfs-rw for APFS filesystem driver
- apfs-fuse for DMG/APFS support

## Structure
- `linux-7.0/` - Modified Linux kernel base
- `xnu-xnu-11215.1.10/` - XNU source for reference
- `lpnu/` - LPNU compatibility layer
  - `lpnu_mach_o.c` - Mach-O binary loader
  - `lpnu_kext.c` - KEXT compatibility
  - `lpnu_vfs_compat.c` - VFS compatibility
- `linux-7.0/fs/apfs/` - APFS driver (from linux-apfs-rw)
- `linux-7.0/fs/hfsplus/` - HFS+ driver (in-tree)
- `lpnu/apfs-fuse/` - APFS-FUSE for DMG support

## Building
```bash
# Build kernel
cd linux-7.0
make defconfig
make -j$(nproc)

# Build APFS module
cd linux-7.0/fs/apfs
make
```

## Features
- APFS filesystem support (read/write)
- HFS+ filesystem support
- Mach-O binary loading
- KEXT (.kext) compatibility
- DMG mounting support via APFS-FUSE
- Metal API compatibility layer
- Full macOS API emulation