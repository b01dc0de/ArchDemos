#include "Utils.h"

namespace Arch
{
    const char* GetBaseDirectory()
    {
        static string BaseDir{};
        static const char* BaseDirName = "ArchDemos";
        if (BaseDir.empty() && Arch_ArgCount > 0)
        {
            BaseDir = string(Arch_ArgVals[0]);
            string::size_type DirPos = BaseDir.find(BaseDirName);
            if (DirPos != string::npos) { BaseDir = BaseDir.substr(0, DirPos + strlen(BaseDirName) + 1); }
            // TODO: Clean this up, this is unsafe (+ naive!) right now
        }
        return BaseDir.c_str();
    }

    string GetRelativeFilename(const char* Filename)
    {
        return string{ string{GetBaseDirectory()} + string{Filename} };
    }

    const char* ReadFileContents(const char* Filename)
    {
        char* Result = nullptr;

        std::string FullFilename = GetRelativeFilename(Filename);

        FILE* FileHandle = nullptr;
        fopen_s(&FileHandle, FullFilename.c_str(), "rb");
        if (FileHandle)
        {
            size_t FileSize = 0;
            fseek(FileHandle, 0, SEEK_END);
            FileSize = ftell(FileHandle);
            fseek(FileHandle, 0, SEEK_SET);

            Result = (char*)malloc(FileSize + 1);
            fread(Result, FileSize, 1, FileHandle);
            Result[FileSize] = '\0';

            fclose(FileHandle);
        }

        return Result;
    }

}