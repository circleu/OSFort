#include "efi.h"
#include "efilib.h"
#include "../inc/elf.h"
#include "../inc/gpt.h"
#include "../inc/ext2.h"


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    EFI_STATUS Status;

    /*********************** Locate GOP **********************/
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    {
        Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GOP);
        if (EFI_ERROR(Status)) {
            Print(L"Could not locate GOP.\r\n");
            return Status;
        }

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
        UINTN info_size, mode_count, native_mode;

        Status  = uefi_call_wrapper(GOP->QueryMode, 4, GOP, GOP->Mode == NULL ? 0 : GOP->Mode->Mode, &info_size, &info);
        if (Status == EFI_NOT_STARTED) {
            Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, 0);
            if (EFI_ERROR(Status)) {
                Print(L"Could not set graphic mode.\r\n");
                return Status;
            }
        }

        native_mode = GOP->Mode->Mode;
        mode_count = GOP->Mode->MaxMode;

        for (UINTN i = 0; i < mode_count; i++) {
            Status = uefi_call_wrapper(GOP->QueryMode, 4, GOP, i, &info_size, &info);
            if (EFI_ERROR(Status)) {
                Print(L"Could not get graphic mode.\r\n");
                return Status;
            }

            if (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080) {
                Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, i);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not set graphic mode.\r\n");
                    return Status;
                }
                break;
            }
        }
    }
    /*********************************************************/

    /*********************** Open Disk ***********************/
    EFI_FILE_HANDLE BootLoader;
    {
        EFI_HANDLE* block_ios;
        UINTN block_io_num;
        Status = uefi_call_wrapper(BS->LocateHandleBuffer, 5, ByProtocol, &gEfiBlockIo2ProtocolGuid, NULL, &block_io_num, &block_ios);
        if (EFI_ERROR(Status)) {
            Print(L"Could not locate BlockIOProtocol.\r\n");
            return Status;
        }

        EFI_BLOCK_IO_PROTOCOL* block_io;
        UINT32 media_id;
        UINTN block_size = 512;
        GPT_HEADER* gpt_header = NULL;
        BOOLEAN disk_found = 0;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, block_size, (VOID**)&gpt_header);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for BlockIOProtocol.\r\n");
            return Status;
        }

        for (UINTN i = 0; i < block_io_num; i++) {
            Status = uefi_call_wrapper(BS->HandleProtocol, 3, block_ios[i], &gEfiBlockIoProtocolGuid, (VOID**)&block_io);
            if (EFI_ERROR(Status)) continue;

            if ((block_io->Media->LogicalPartition)) continue;

            media_id = block_io->Media->MediaId;

            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, 1, block_size, (VOID*)gpt_header);
            if (EFI_ERROR(Status)) continue;

            if (strncmpa(gpt_header->sign, "EFI PART", 8) == 0) {
                Print(L"Found Disk.\r\n");
                disk_found = 1;
                break;
            }
        }
        if (!(disk_found)) {
            Print(L"Could not locate BlockIOProtocol for disk.\r\n");
            return EFI_DEVICE_ERROR;
        }

        EXT2_SUPERBLOCK* primary_part = NULL;
        GPT_PARTITION_ENTRY* part_tbl = NULL;
        UINT64 part_tbl_start = gpt_header->part_tbl_start_lba;
        UINT32 part_entry_num = gpt_header->part_entry_num;
        UINT8 tbl_size = 0x80;
        BOOLEAN part_found;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, tbl_size * part_entry_num, (VOID**)&part_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for GPT_PARTITION_ENTRY.\r\n");
            return Status;
        }

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, sizeof(EXT2_SUPERBLOCK), (VOID**)&primary_part);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for primary partition.\r\n");
            return Status;
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, part_tbl_start, tbl_size * part_entry_num, (VOID*)part_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not read partition table.\r\n");
            return Status;
        }

        for (UINTN i = 0; i < part_entry_num; i++) {
            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, part_tbl[i].part_start_lba + 2, sizeof(EXT2_SUPERBLOCK), (VOID*)primary_part); // SuperBlock of ext2 locates at LBA 2.
            if (EFI_ERROR(Status)) continue;

            if (primary_part->magic == 0xef53) {
                Print(L"Found Primary Partition.\r\n");
                part_found = 1;
                break;
            }
        }
        if (!(part_found)) {
            Print(L"Could not find primary partition.\r\n");
            return EFI_DEVICE_ERROR;
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)part_tbl);
        if (EFI_ERROR(Status)) return Status;

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)gpt_header);
        if (EFI_ERROR(Status)) return Status;

        

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)primary_part);
        if (EFI_ERROR(Status)) return Status;
    }
    /*********************************************************/



    while (1);

    return EFI_SUCCESS;
}