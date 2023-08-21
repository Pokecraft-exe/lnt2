#pragma once
#ifndef IO
#define IO

#include "typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA (uint16_t)0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA (uint16_t)0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
#define StackP 0x7bff

extern void push_registers();
extern void pop_registers();

struct CPUState
    {
    	uint64_t rax;
        uint64_t rbx;
        uint64_t rcx;
        uint64_t rdx;
        
        uint32_t eax;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
        
        uint16_t ax;
        uint16_t bx;
        uint16_t cx;
        uint16_t dx;
        
        uint8_t al;
        uint8_t bl;
        uint8_t cl;
        uint8_t dl;

        uint64_t rsi;
        uint64_t rdi;
        uint64_t rbp;
        
        uint64_t cr0;
        uint64_t cr2;
        uint64_t cr3;
        uint64_t cr4;
        uint64_t cr8;
    
        uint64_t error;

        uint64_t rip;
        uint16_t cs;
        uint16_t ds;
		uint16_t es;
		uint16_t fs;
		uint16_t gs;
        uint64_t rflags;
        uint64_t rsp;
        uint16_t ss; 
    } __attribute__((packed));
    
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);
void RemapPic(uint8 master, uint8 slave);
void IRQ_set_mask(unsigned char IRQline);
void IRQ_clear_mask(unsigned char IRQline);
void restart();

class Port8Bit{
private:
    uint16_t portnumber;
public:
    Port8Bit(uint16_t port){
        portnumber = port;
    }
    void Write(uint8_t data){
        asm volatile ("outb %0, %1" : : "a"(data), "Nd"(portnumber));
    }
    uint8_t Read(){
        uint8_t returnVal;
        asm volatile ("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(portnumber));
        return returnVal;
    }
};

class Port8BitSlow{
private:
    uint16_t portnumber;
public:
    Port8BitSlow(uint16_t port){
        portnumber = port;
    }
    void Write(uint8_t data){
        asm volatile ("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a"(data), "Nd"(portnumber));
    }
    uint8_t Read(){
        uint8_t returnVal;
        asm volatile ("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(portnumber));
        return returnVal;
    }
};

class Port16Bit{
private:
    uint16_t portnumber;
public:
    Port16Bit(uint16_t port){
        portnumber = port;
    }
    void Write(uint16_t data){
        asm volatile ("outw %0, %1" : : "a"(data), "Nd"(portnumber));
    }
    uint16_t Read(){
        uint16_t returnVal;
        asm volatile ("inw %1, %0"
        : "=a"(returnVal)
        : "Nd"(portnumber));
        return returnVal;
    }
};

class Port32Bit{
private:
    uint16_t portnumber;
public:
    Port32Bit(uint16_t port){
        portnumber = port;
    }
    void Write(uint32_t data){
        asm volatile ("outl %0, %1" : : "a"(data), "Nd"(portnumber));
    }
    uint32_t Read(){
        uint32_t returnVal;
        asm volatile ("inl %1, %0"
        : "=a"(returnVal)
        : "Nd"(portnumber));
        return returnVal;
    }
};

#endif
