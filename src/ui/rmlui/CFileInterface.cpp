#include "CFileInterface.h"

Rml::FileHandle ui::CFileInterface::Open(const Rml::String& path)
{
    if(!std::filesystem::exists(path))
    {
        SPDLOG_ERROR("File not found: {}", path);
        return 0;
    }

    SPDLOG_INFO("Opening file: {}", path);

    return (Rml::FileHandle)new std::ifstream(path, std::ios::binary | std::ios::in);
}

void ui::CFileInterface::Close(Rml::FileHandle file)
{
    const auto std_file = reinterpret_cast<std::ifstream*>(file);
    SPDLOG_INFO("Closing file: {:x}", file);
    delete std_file;
}

size_t ui::CFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
    SPDLOG_INFO("Reading file: {:x} {}", file, size);
    const auto std_file = reinterpret_cast<std::ifstream*>(file);
    std_file->read((char*)buffer, size);
    return std_file->gcount();
}

bool ui::CFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
    SPDLOG_INFO("Seeking file: {:x} {} {}", file, offset, origin);
    const auto std_file = reinterpret_cast<std::ifstream*>(file);
    const auto pos = Tell(file);
    const auto len = Length(file);
    switch(origin)
    {
    case SEEK_END: //end
    case SEEK_SET: //set
        if(offset < 0 || offset >= len)
            return false;
        break;
    case SEEK_CUR: //cur
        if (pos + offset < 0 || pos + offset >= len)
            return false;
        break;
    }
    std_file->seekg(offset, static_cast<std::ios_base::seekdir>(origin));
    return true;
}

size_t ui::CFileInterface::Tell(Rml::FileHandle file)
{
    SPDLOG_INFO("Telling file: {:x}", file);
    const auto std_file = reinterpret_cast<std::ifstream*>(file);
    return std_file->tellg();
}

size_t ui::CFileInterface::Length(Rml::FileHandle file)
{
    const auto std_file = reinterpret_cast<std::ifstream*>(file);
    const auto current_pos = std_file->tellg();
    std_file->seekg(0, std::ios_base::end);
    const auto file_size = std_file->tellg();
    std_file->seekg(current_pos);
    SPDLOG_INFO("Get length of file: {:x} {}", file, file_size);
    return file_size;
}

bool ui::CFileInterface::LoadFile(const Rml::String& path, Rml::String& out)
{
    SPDLOG_INFO("Load file: {}", path);
    if (const auto file = Open(path))
    {
        const auto len = Length(file);
        out.resize(len);
        Read(out.data(), len, file);
        return true;
    }
    return false;
}
