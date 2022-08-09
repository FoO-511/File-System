#include "mbr.h"
uint32_t sector_loc = 0x00000000;

void push_pte2Q(EBR *ebr, std::queue<PTE> *Q)
{
    for (int i = 0; i < 4; i++)
    {
        PTE pte = ebr->pte_[i];
        if (pte.CHS_addr_ == 0x000000)
            continue;
        uchar type = pte.partition_type_;
        if (type == PTE::NTFS || type == PTE::FAT32_CHS || type == PTE::FAT32_LBA)
            printf("0x%X 0x%X 0x%lX\n", pte.partition_type_, sector_loc + pte.get_LBA_offset(), pte.get_size());
        if (type == PTE::EBR || type == PTE::EBR2)
            Q->push(pte);
    }
}

void usage()
{
    printf("syntax: work1 <filepath>\n");
    printf("sample: work1 ./mbr_128.dd\n");
}

bool parse(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage();
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (!parse(argc, argv))
        return -1;

    uint32_t ebr_base_loc = 0x00000000;
    uint32_t current_loc = 0x00000000;
    const char *filepath = argv[1];

    FILE *fp = NULL;
    fp = fopen(filepath, "rb");

    if (!fp)
    {
        fprintf(stderr, "fp open failed. \n");
        return -1;
    }

    MBR *mbr = (MBR *)malloc(sizeof(MBR));
    EBR *ebr = (EBR *)malloc(sizeof(EBR));
    Sector *buf = (Sector *)malloc(sizeof(Sector));

    fread(buf, sizeof(Sector), 1, fp);

    if (buf->signature() != MBR::SIGNATURE)
    {
        fprintf(stderr, "problem finding MBR. \n");
        return -1;
    }
    mbr = (MBR *)(buf);

    std::queue<PTE> Q;

    push_pte2Q(mbr, &(Q));

    while (!Q.empty())
    {
        PTE pte = Q.front();
        Q.pop();

        current_loc = ebr_base_loc + pte.get_LBA_offset();

        if (ebr_base_loc == 0)
            ebr_base_loc = pte.get_LBA_offset();

        fseek(fp, current_loc, SEEK_SET);
        fread(buf, sizeof(Sector), 1, fp);

        if (buf->signature() != MBR::SIGNATURE)
        {
            fprintf(stderr, "problem finding EBR. \n");
            return -1;
        }
        sector_loc = current_loc;

        ebr = (EBR *)(buf);

        push_pte2Q(ebr, &(Q));
    }

    return 0;
}