#include "gpt.h"

int main(int argc, char *argv[])
{

    FILE *fp = NULL;
    const char *filepath = argv[1];
    fp = fopen(filepath, "rb");

    GPT_HDR *gpt_hdr = (GPT_HDR *)malloc(sizeof(GPT_HDR));
    fseek(fp, 512, SEEK_SET);
    fread(gpt_hdr, sizeof(GPT_HDR), 1, fp);

    if (gpt_hdr->signature_ != GPT_HDR::EFI_PART)
    {
        printf("Not GPT\n");
        return -1;
    }

    GPT_PTEs *GPT_ptes = (GPT_PTEs *)malloc(sizeof(GPT_PTEs));
    Partition_Hdr *partition_hdr = (Partition_Hdr *)malloc(sizeof(Partition_Hdr));

    fseek(fp, 1024, SEEK_SET);
    fread(GPT_ptes, sizeof(GPT_PTEs), 1, fp);

    for (int i = 0; i < 128; i++)
    {
        GPT_PTE *pte = &(GPT_ptes->pte[i]);
        if (pte->first_LBA_ == 0)
            continue;
        pte->print_guid();
        printf(" %lld %lld\n", pte->first_LBA(), pte->get_size());
    }
}