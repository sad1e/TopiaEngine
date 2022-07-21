#include "StringUtils.h"

#include <locale>

std::string topia::ToLower(const std::string& str)
{
    std::string lower_case = str;
    std::locale loc;
    for (char& s : lower_case)
        s = std::tolower(s, loc);
    return lower_case;
}

std::wstring topia::ToLower(const std::wstring& str)
{
    std::wstring lower_case = str;
    std::locale loc;
    for (wchar_t& s : lower_case)
        s = std::tolower(s, loc);
    return lower_case;
}

std::string topia::GetBasePath(const std::string& filePath)
{
    size_t lastSlash;
    if ((lastSlash = filePath.rfind('/')) != std::string::npos)
        return filePath.substr(0, lastSlash + 1);
    else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
        return filePath.substr(0, lastSlash + 1);
    else
        return "";
}

std::wstring topia::GetBasePath(const std::wstring& filePath)
{
    size_t lastSlash;
    if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
        return filePath.substr(0, lastSlash + 1);
    else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
        return filePath.substr(0, lastSlash + 1);
    else
        return L"";
}

std::string topia::RemoveBasePath(const std::string& filePath)
{
    size_t lastSlash;
    if ((lastSlash = filePath.rfind('/')) != std::string::npos)
        return filePath.substr(lastSlash + 1, std::string::npos);
    else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
        return filePath.substr(lastSlash + 1, std::string::npos);
    else
        return filePath;
}

std::wstring topia::RemoveBasePath(const std::wstring& filePath)
{
    size_t lastSlash;
    if ((lastSlash = filePath.rfind(L'/')) != std::string::npos)
        return filePath.substr(lastSlash + 1, std::string::npos);
    else if ((lastSlash = filePath.rfind(L'\\')) != std::string::npos)
        return filePath.substr(lastSlash + 1, std::string::npos);
    else
        return filePath;
}

std::string topia::GetFileExtension(const std::string& filePath)
{
    std::string fileName = RemoveBasePath(filePath);
    size_t extOffset = fileName.rfind('.');
    if (extOffset == std::wstring::npos)
        return "";

    return fileName.substr(extOffset + 1);
}

std::wstring topia::GetFileExtension(const std::wstring& filePath)
{
    std::wstring fileName = RemoveBasePath(filePath);
    size_t extOffset = fileName.rfind(L'.');
    if (extOffset == std::wstring::npos)
        return L"";

    return fileName.substr(extOffset + 1);
}

std::string topia::RemoveExtension(const std::string& filePath)
{
    return filePath.substr(0, filePath.rfind("."));
}

std::wstring topia::RemoveExtension(const std::wstring& filePath)
{
    return filePath.substr(0, filePath.rfind(L"."));
}
