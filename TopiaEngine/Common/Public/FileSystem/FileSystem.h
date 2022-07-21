#pragma once

namespace topia
{
	class FileSystem
	{
	public:

	private:
		std::wstring AssetPath;
	};

	TOPIA_INLINE void GetAssetsPath(WCHAR* Path, u32 PathSize)
	{
		if (Path == nullptr)
		{
			throw std::exception("GetAssetsPath, but the parameter **Path** is a null pointer.");
		}

		DWORD Size = ::GetModuleFileName(nullptr, Path, PathSize);
		if (Size == 0 || Size == PathSize)
		{
			throw std::exception("GetAssetsPath, but the Size is truncated.");
		}

		WCHAR* LastSlash = wcsrchr(Path, L'\\');
		if (LastSlash)
		{
			*(LastSlash + 1) = L'\0';
		}
	}

	TOPIA_INLINE std::wstring GetAssetFullPath(LPCWSTR AssetName);
}