#include "VirtualDisk.h"

#pragma comment(lib, "VirtDisk.lib")

#ifndef WINVER
#define WINVER _WIN32_WINNT_WIN10
#endif

#include <cassert>

#include <virtdisk.h>
#include <Objbase.h>


[[nodiscard]] HANDLE VirtualDisk::Vhd::create_vhdx(
    const std::wstring& path, uint64_t max_allocation, const OverwriteExisting overwrite_existing) {

    assert(!path.empty());
    
    {
        const auto file_attributes{ ::GetFileAttributesW(path.c_str()) };
        if (file_attributes != INVALID_FILE_ATTRIBUTES && OverwriteExisting::no == overwrite_existing) {

            throw(std::exception("Check if file exists when overwrite is not allowed"));
        }
        else if (file_attributes != INVALID_FILE_ATTRIBUTES) {

            auto delete_result{ ::DeleteFile(path.c_str()) };
            if (FALSE == delete_result) { throw(std::exception("Delete existing files")); }
        }
    }

    constexpr auto sector_size{ 512 };
    // Max allocation must be a multiple of sector size.
    if (0 != max_allocation % sector_size) { for (; 0 != max_allocation % sector_size; ++max_allocation); }

    GUID unique_id;
    {
        auto guid_result = ::CoCreateGuid(&unique_id);
        if (FAILED(guid_result)) { throw(std::exception("Create unique GUID")); }
    }

    VIRTUAL_STORAGE_TYPE virtual_storage_type;
    virtual_storage_type.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHDX;
    virtual_storage_type.VendorId = GUID_NULL;

    CREATE_VIRTUAL_DISK_PARAMETERS create_virtual_disk_parameters;
    create_virtual_disk_parameters.Version = CREATE_VIRTUAL_DISK_VERSION_2;
    create_virtual_disk_parameters.Version2.UniqueId = unique_id;
    create_virtual_disk_parameters.Version2.MaximumSize = max_allocation;
    create_virtual_disk_parameters.Version2.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
    create_virtual_disk_parameters.Version2.SectorSizeInBytes = sector_size;
    create_virtual_disk_parameters.Version2.PhysicalSectorSizeInBytes = sector_size;
    create_virtual_disk_parameters.Version2.ParentPath = nullptr;
    create_virtual_disk_parameters.Version2.SourcePath = nullptr;
    create_virtual_disk_parameters.Version2.OpenFlags = OPEN_VIRTUAL_DISK_FLAG_NONE;
    //ResiliencyGUID? No clue.

    HANDLE vhd_handle{ nullptr };
    auto result = ::CreateVirtualDisk(
        &virtual_storage_type,
        path.c_str(),
        VIRTUAL_DISK_ACCESS_NONE,
        nullptr,
        CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION,
        0,
        &create_virtual_disk_parameters,
        nullptr,
        &vhd_handle
    );
    if (ERROR_SUCCESS != result) { throw(std::exception("Create VHD file")); }

    return(vhd_handle);
}
