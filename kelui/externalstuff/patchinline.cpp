#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

// patch for hooking openxrlib without dobby you can still change this later
static void* patchPLTGOT(const char* soname, const char* symname, void* newfn) {
    void* handle = dlopen(soname, RTLD_NOW | RTLD_NOLOAD);
    if (!handle) return nullptr;

    struct link_map* lm = nullptr;
    if (dlinfo(handle, RTLD_DI_LINK_MAP, &lm) != 0 || !lm) return nullptr;
//ignore table fixed by ai just because im lazy XD
    ElfW(Sym)* symtab = nullptr;
    const char* strtab = nullptr;
    ElfW(Addr) jmprel = 0;
    ElfW(Word) pltrelsz = 0;
    size_t relent = sizeof(ElfW(Rel));

    for (ElfW(Dyn)* dyn = lm->l_ld; dyn->d_tag != DT_NULL; dyn++) {
        if (dyn->d_tag == DT_SYMTAB) symtab = (ElfW(Sym)*)(lm->l_addr + dyn->d_un.d_ptr);
        else if (dyn->d_tag == DT_STRTAB) strtab = (const char*)(lm->l_addr + dyn->d_un.d_ptr);
        else if (dyn->d_tag == DT_JMPREL) jmprel = lm->l_addr + dyn->d_un.d_ptr;
        else if (dyn->d_tag == DT_PLTRELSZ) pltrelsz = dyn->d_un.d_val;
        else if (dyn->d_tag == DT_PLTREL && dyn->d_un.d_val == DT_RELA) relent = sizeof(ElfW(Rela));
    }

    for (size_t i = 0; i < pltrelsz / relent; i++) {
        auto* rel = (ElfW(Rel)*)(jmprel + i * relent);
#ifdef __LP64__
        size_t symidx = ELF64_R_SYM(rel->r_info);
#else
        size_t symidx = ELF32_R_SYM(rel->r_info);
#endif
        if (symidx && strtab && symtab && strcmp(symname, strtab + symtab[symidx].st_name) == 0) {
            void** address = (void**)(lm->l_addr + rel->r_offset);
            void* oldfn = *address;
            size_t pagesize = sysconf(_SC_PAGESIZE);
            uintptr_t page_start = (uintptr_t)address & ~(pagesize - 1);
            mprotect((void*)page_start, pagesize, PROT_READ | PROT_WRITE);
            *address = newfn;
            return oldfn;
        }
    }
    return nullptr;
}


