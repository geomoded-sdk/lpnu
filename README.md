# LPNU Kernel

**Linux with XNU/macOS Compatibility Layer**

## Credits

### Based On

- **Linux Kernel 7.0** - https://github.com/torvalds/linux
  - Main kernel base
  - APFS driver (linux-apfs-rw): https://github.com/linux-apfs/linux-apfs-rw
  - HFS+ driver (in-tree)

- **Maloader** - https://github.com/shinh/maloader
  - Mach-O to ELF converter
  - macOS syscall emulation in userspace
  - Created byShin Y

- **XNU Kernel** - https://github.com/apple/darwin-xnu
  - Reference for macOS kernel APIs
  - Source: xnu-11215.1.10

- **APFS-FUSE** - https://github.com/sgan81/apfs-fuse
  - DMG mounting support

## Architecture

```
┌─────────────────────────────────────┐
│     User Applications (macOS)        │
├─────────────────────────────────────┤
│  ld-mac (maloader)                  │
│  - Mach-O → ELF conversion         │
│  - macOS syscall emulation        │
├─────────────────────────────────────┤
│  Linux Kernel 7.0                 │
│  - APFS/HFS+ drivers              │
│  - binfmt_mach_o                  │
│  - LPNU compatibility layer     │
├─────────────────────────────────────┤
│  Machine Layer                    │
│  - Hardware emulation           │
└─────────────────────────────────────┘
```

## Features

- APFS filesystem support (read/write)
- HFS+ filesystem support
- Mach-O binary loader
- KEXT (.kext) compatibility
- DMG mounting via APFS-FUSE
- Metal API compatibility layer
- Full macOS API emulation

## Building

### Kernel Modules
```bash
cd linux-7.0/fs/lpnu
make KERNEL_DIR=/path/to/kernel/headers
```

### Maloader
```bash
cd maloader
make release
```

### Full Build (via GitHub Actions)
```bash
gh workflow run build.yml
```

## Usage

### Load Kernel Modules (Linux)
```bash
sudo insmod lpnu.ko
sudo insmod apfs.ko
```

### Mount APFS
```bash
mount -t apfs /dev/sdX /mnt/apfs
```

### Run Mach-O Binary
```bash
./ld-mac /path/to/mach_o_binary
```

## License

- Linux Kernel: GPLv2
- Maloader: Simplified BSD / GPLv3
- XNU: Apple Public Source License
- LPNU Components: GPLv2

## Documentation

- [Architecture](docs/architecture.md)
- [Building](docs/building.md)

## Links

- GitHub: https://github.com/geomoded-sdk/lpnu
- Issues: https://github.com/geomoded-sdk/lpnu/issues