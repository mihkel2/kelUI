#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <elf.h>

struct FindData {
    const char* name;
    uintptr_t base;
};

static int find_lib_callback(struct dl_phdr_info* info, size_t size, void* data) {
    auto* d = (FindData*)data;
    if (info->dlpi_name && strstr(info->dlpi_name, d->name)) {
        d->base = info->dlpi_addr;
        return 1;
    }
    return 0;
}

void* patchPLTGOT(const char* soname, const char* symname, void* newfn) {
    FindData data = { soname, 0 };
    if (!dl_iterate_phdr(find_lib_callback, &data) || data.base == 0) return nullptr;

    // dlopen with NOLOAD is the safest way to get the local handle for symbol lookups
    void* handle = dlopen(soname, RTLD_NOW | RTLD_NOLOAD);
    if (!handle) return nullptr;

    // Use dlsym to get the target symbol's value (optional, but helps verification)
    void* target_sym = dlsym(handle, symname);
    dlclose(handle); // We don't need the handle anymore, just the base addr from phdr

    ElfW(Ehdr)* ehdr = (ElfW(Ehdr)*)data.base;
    ElfW(Phdr)* phdr = (ElfW(Phdr)*)(data.base + ehdr->e_phoff);
    ElfW(Dyn)* dynamic = nullptr;

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_DYNAMIC) {
            dynamic = (ElfW(Dyn)*)(data.base + phdr[i].p_vaddr);
            break;
        }
    }
    if (!dynamic) return nullptr;

    ElfW(Sym)* symtab = nullptr;
    const char* strtab = nullptr;
    ElfW(Addr) jmprel = 0;
    ElfW(Word) pltrelsz = 0;
    size_t relent = sizeof(ElfW(Rel));

    for (ElfW(Dyn)* dyn = dynamic; dyn->d_tag != DT_NULL; dyn++) {
        // On Android, these pointers in memory are often already absolute
        ElfW(Addr) val = dyn->d_un.d_ptr;
        if (dyn->d_tag == DT_SYMTAB) symtab = (ElfW(Sym)*)val;
        else if (dyn->d_tag == DT_STRTAB) strtab = (const char*)val;
        else if (dyn->d_tag == DT_JMPREL) jmprel = val;
        else if (dyn->d_tag == DT_PLTRELSZ) pltrelsz = dyn->d_un.d_val;
        else if (dyn->d_tag == DT_PLTREL && dyn->d_un.d_val == DT_RELA) relent = sizeof(ElfW(Rela));
    }

    // Re-adjust pointers if they were relative (rare on Android but possible)
    if ((uintptr_t)symtab < data.base) symtab = (ElfW(Sym)*)(data.base + (uintptr_t)symtab);
    if ((uintptr_t)strtab < data.base) strtab = (const char*)(data.base + (uintptr_t)strtab);
    if (jmprel < data.base) jmprel = (data.base + jmprel);

    if (!jmprel || !symtab || !strtab) return nullptr;

    for (size_t i = 0; i < pltrelsz / relent; i++) {
        auto* rel = (ElfW(Rel)*)(jmprel + i * relent);
#ifdef __LP64__
        size_t symidx = ELF64_R_SYM(rel->r_info);
#else
        size_t symidx = ELF32_R_SYM(rel->r_info);
#endif
        if (symidx && strcmp(symname, strtab + symtab[symidx].st_name) == 0) {
            void** address = (void**)(data.base + rel->r_offset);
            void* oldfn = *address;

            size_t pagesize = sysconf(_SC_PAGESIZE);
            uintptr_t page_start = (uintptr_t)address & ~(pagesize - 1);
            mprotect((void*)page_start, pagesize * 2, PROT_READ | PROT_WRITE); // Protect 2 pages to be safe

            *address = newfn;

            mprotect((void*)page_start, pagesize * 2, PROT_READ); // Reset to read-only
            return oldfn;
        }
    }
    return nullptr;
}
