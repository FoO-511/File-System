#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#pragma once

typedef struct _MBR
{
    u_char bytes_[512];
} __attribute__((packed)) MBR;

typedef struct _GPT_HDR
{
    uint64_t signature_;
    uint32_t revision_;
    uint32_t hdr_size_;
    uint32_t CRC32_of_hdr_;
    uint32_t reserved_;

    uint64_t current_LBA_;
    uint64_t backup_LBA_;
    uint64_t first_usable_LBA_; // usually 34
    uint64_t last_usable_LBA_;

    u_char disk_guid[16];

    uint64_t partition_entries_starting_LBA_;
    uint32_t number_of_partition_entries_;
    uint32_t size_of_partition_entry_;
    uint32_t CRC32_of_partition_array;
    // u_char paddings[420];

    enum : uint64_t
    {
        EFI_PART = 0x5452415020494645
    };

} __attribute__((packed)) GPT_HDR;

typedef struct _GPT_GUID
{
    uint32_t g1_;
    uint16_t g2_;
    uint16_t g3_;
    uint16_t g4_;
    u_char g5_[6];

    void print()
    {
        printf("%08X", g1_);
        printf("%04X", g2_);
        printf("%04X", g3_);
        printf("%04X", g4_);
        for (int i = 0; i < 6; i++)
            printf("%02X", g5_[i]);
    }

} __attribute__((packed)) GPT_GUID;

typedef struct _GPT_PTE
{
    GPT_GUID partition_type_guid_;
    GPT_GUID unique_partition_guid_;

    uint64_t first_LBA_;
    uint64_t last_LBA_;
    uint64_t attribute_flags_;

    unsigned long long int first_LBA() { return first_LBA_ * 0x200; };
    unsigned long long int last_LBA() { return last_LBA_ * 0x200; };
    unsigned long long int get_size() { return (last_LBA_ - first_LBA_ + 1) * 0x200; };
    void print_guid()
    {
        partition_type_guid_.print();
        // unique_partition_guid_.print_formatted();
    }

    char partition_name_[72]; // UTF-16LE

} __attribute__((packed)) GPT_PTE;

typedef struct _GPT_PTEs
{
    GPT_PTE pte[128];
} __attribute__((packed)) GPT_PTEs;

typedef struct _Partition_Hdr
{
    u_char jump_instruction_[3];
    u_char file_system_type_[8];
    u_char others_[501];

    void print_type_hex()
    {
        for (int i = 0; i < 8; i++)
            printf("%02X", file_system_type_[i]);
    }

} __attribute__((packed)) Partition_Hdr;