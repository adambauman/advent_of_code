#pragma once

#include <Windows.h>
#include <string>

namespace VirtualDisk {
	
	enum class OverwriteExisting : bool {
		yes = true,
		no = false
	};
	namespace Vhd {
		[[nodiscard]] HANDLE create_vhdx(
			const std::wstring& path, uint64_t max_allocation, 
			const OverwriteExisting overwrite_existing
		);
	}
}