#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <queue>
#pragma once

typedef unsigned char uchar;

typedef struct Sector_
{
    uchar bytes[510];
    uint16_t signature_;

    uint16_t signature() { return ntohs(signature_); }

} Sector;

typedef struct _NTFS
{
    uchar jump_instruction_[3];
    uint32_t OEM_ID_NTFS_; // NTFS -- 4byte
    uint32_t OEM_ID_;      // 20 20 20 20 -- 4byte
    uchar others[499];
    uint16_t signature_;

    uint32_t OEM_ID_NTFS() { return ntohl(OEM_ID_NTFS_); }

    enum : uint32_t
    {
        NTFS = 0x4E544653
    };

} __attribute__((packed)) NTFS;

typedef struct _PTE
{
    uchar status_;
    uchar CHS_addr_[3];
    uchar partition_type_;
    uchar CHS_addr2_[3];
    uint32_t LBA_addr_of_start_;
    uint32_t number_of_sectors_;

    uint32_t get_LBA_offset() { return LBA_addr_of_start_ * 0x200; }
    uint64_t get_size() { return number_of_sectors_ * 0x200; }

    /* flag */
    enum : uchar
    {
        ACTIVE = 0X80,
        INACTIVE = 0X00,

    };
    /* partition type
    0x07 : NTFS
    0x0B : (CHS FAT32)
    0x0C : (LBA FAT32)
    0x05, 0x06 : 확장 파티션
     */
    enum : uchar
    {
        NTFS = 0X07,
        FAT32_CHS = 0x0B,
        FAT32_LBA = 0x0c,
        EBR = 0X05,
        EBR2 = 0X06
    };

} PTE;

typedef struct _MBR
{
    uchar boot_code_[446];
    PTE pte_[4];
    uint16_t signature_;
    enum : uint16_t
    {
        SIGNATURE = 0x55AA
    };
} __attribute__((packed)) MBR;

typedef struct _MBR EBR;