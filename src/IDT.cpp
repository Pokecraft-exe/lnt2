#include "IDT.h"

IDTR idtTable;

__attribute__((aligned(0x10)))
IDT64 IDTs[256] = {};
  
void InitIDT(){
    idtTable.address = (uint64_t)&IDTs[0];
    idtTable.size = 0x0fff;

    asm("lidt %0" :: "m"(idtTable));
    asm ("sti");

    RemapPic(0x20,0b00000001);
}
  
void add_IRQ(uint8_t IRQ, void(*function)(interrupt_frame* frame), uint8_t gate) {
  	IRQ_clear_mask(IRQ);
	IDT64* newInterrupt = &IDTs[IRQ];
	newInterrupt->types_attr = 0b10000000 | gate;
	newInterrupt->codeseg = 0x8;
}

void add_IRQ(uint8_t IRQ, void* function, uint8_t gate) {
  	//IRQ_clear_mask(IRQ);	
	IDTs[IRQ].offset_low  = (uint16_t)(((uint64_t)function & 0x000000000000ffff));
	IDTs[IRQ].offset_mid  = (uint16_t)(((uint64_t)function & 0x00000000ffff0000) >> 16);
	IDTs[IRQ].offset_high = (uint32_t)(((uint64_t)function & 0xffffffff00000000) >> 32);
	IDTs[IRQ].types_attr = 0b10000000 | gate;
	IDTs[IRQ].codeseg = 0x8;
}

__attribute__((interrupt)) extern "C" void Schedule(interrupt_frame* frame) {
    DrawChar('a', 500, 500, 0x00FF, 2);
}

void print(char* str) {
	cls(0);
	DrawString(str, 100, 100, 0xff, 2);
}

__attribute__((interrupt)) void isr0(interrupt_frame* frame) {print("division by 0");while(1);};
__attribute__((interrupt)) void isr1(interrupt_frame* frame) {print("debug");while(1);};
__attribute__((interrupt)) void isr2(interrupt_frame* frame) {print("nmi");while(1);};
__attribute__((interrupt)) void isr3(interrupt_frame* frame) {print("breakpoint");while(1);};
__attribute__((interrupt)) void isr4(interrupt_frame* frame) {print("overflow");while(1);};
__attribute__((interrupt)) void isr5(interrupt_frame* frame) {print("table overload");while(1);};
__attribute__((interrupt)) void isr6(interrupt_frame* frame) {print("fake instruction");while(1);};
__attribute__((interrupt)) void isr7(interrupt_frame* frame) {print("FPU don't exist");while(1);};
__attribute__((interrupt)) void isr8(interrupt_frame* frame) {print("double fault");while(1);};
__attribute__((interrupt)) void isr9(interrupt_frame* frame) {print("coprocessor segment overrun");while(1);};
__attribute__((interrupt)) void isr10(interrupt_frame* frame) {print("invalid TSS");while(1);};
__attribute__((interrupt)) void isr11(interrupt_frame* frame) {print("segment not present");while(1);};
__attribute__((interrupt)) void isr12(interrupt_frame* frame) {print("stack-segment fault");while(1);};
__attribute__((interrupt)) void isr13(interrupt_frame* frame) {
	register uint64_t* rsp asm("rsp");
	uint64_t errorcode = *rsp;
	cls(0x0000FF);
	std::stdin cout;
	
	cout.color(0);
	cout << "General protection fault!\nError code: 0x" << std::hex(errorcode) << std::endl;
	if (rsp != 0) {
		limine_memmap_entry* last = memmap::memmap.entries[0];
		for (size_t i = 1; i < memmap::memmap.entry_count; i++) {
			if ((uint64_t*)last->base < rsp && (uint64_t*)memmap::memmap.entries[i]->base > rsp) {
				DrawStringBackground(memmap::get_memory_type(last), 0, 48, 2, 0xFFFFFF, 0x0000FF, 1);
				break;
			}
		}
	}
	if(*rsp & 1) {
		DrawStringBackground("This error is external to the processor", 0, 40, 2, 0xFFFFFF, 0x0000FF, 1);
	} else {
	    DrawStringBackground("This error is internal to the processor", 0, 40, 2, 0xFFFFFF, 0x0000FF, 1);
	}
	while(inb(0x60) != 0x5E);
	restart();
};
__attribute__((interrupt)) void isr14(interrupt_frame* frame) {
	cls(0x0000FF);
	std::stdin cout;
	cout << "\0m[\0\0\0]PageFault!" << std::endl;
	asm("mov %cr2, %rax");
	gregister(rax);
	uint64_t* memory = (uint64_t*)rax;
	cout << "cr2 = " << (void*)memory << std::endl;
	cout << "memory dump at " << (void*)memory << std::endl;
	cout << std::hex(*memory-8) << std::hex(*memory) << std::hex(*memory=8) << std::endl;
	while(1);
};
__attribute__((interrupt)) void isr16(interrupt_frame* frame) {print("float exception");while(1);};
__attribute__((interrupt)) void isr17(interrupt_frame* frame) {print("alignement check");while(1);};
__attribute__((interrupt)) void isr18(interrupt_frame* frame) {print("machine check");while(1);};
__attribute__((interrupt)) void isr19(interrupt_frame* frame) {print("SMID float exception");while(1);};
__attribute__((interrupt)) void isr20(interrupt_frame* frame) {print("virtualization exception");while(1);};
__attribute__((interrupt)) void isr21(interrupt_frame* frame) {print("control protection exception");while(1);};
__attribute__((interrupt)) void isr28(interrupt_frame* frame) {print("hypervisor injection exception");while(1);};
__attribute__((interrupt)) void isr29(interrupt_frame* frame) {print("VMM communication error");while(1);};
__attribute__((interrupt)) void isr30(interrupt_frame* frame) {print("Security exception");while(1);};

void* exception_handlers[32] = {
(void*)&isr0,
(void*)&isr1,
(void*)&isr2,
(void*)&isr3,
(void*)&isr4,
(void*)&isr5,
(void*)&isr6,
(void*)&isr7,
(void*)&isr8,
(void*)&isr9,
(void*)&isr10,
(void*)&isr11,
(void*)&isr12,
(void*)&isr13,
(void*)&isr14,
nullptr,
(void*)&isr16,
(void*)&isr17,
(void*)&isr18,
(void*)&isr19,
(void*)&isr20,
(void*)&isr21,
nullptr,
nullptr,
nullptr,
nullptr,
nullptr,
nullptr,
(void*)&isr28,
(void*)&isr29,
(void*)&isr30,
nullptr
};
