# LPNU Architecture

## Machine Land (Hardware Layer)
- Instruction translation (x86_64 ↔ ARM64)
- Memory management
- Device emulation

## Kernel Land (OS Layer)
- linux-7.0/ - Linux kernel base
- fs/apfs/ - APFS driver (linux-apfs-rw)
- fs/hfsplus/ - HFS+ driver
- fs/lpnu/ - LPNU compatibility layer
- fs/binfmt_mach_o.c - Mach-O binary format handler

## User Land (Userspace Layer)
- maloader/ - Mach-O to ELF converter (shinh/maloader)
- apfs-fuse/ - APFS-FUSE for DMG mounting
- ld-mac - Loader for macOS binaries

## Stack

```
┌─────────────────────────────────┐
│      User Applications           │
├─────────────────────────────────┤
│   ld-mac (maloader)             │
│   - Mach-O → ELF conversion    │
│   - macOS syscall emulation   │
├─────────────────────────────────┤
│   Linux Kernel 7.0             │
│   - APFS/HFS+ drivers          │
│   - binfmt_mach_o              │
│   - LPNU compatibility        │
├─────────────────────────────────┤
│   Machine Land                 │
│   - Instruction translation   │
│   - Memory management        │
└─────────────────────────────────┘
```

## Building
```bash
# Kernel space
make -C linux-7.0 defconfig
make -C linux-7.0

# User space
make -C maloader release

# All
./build-all.sh
```