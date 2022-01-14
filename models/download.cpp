#include "download.h"
#include <stdexcept>
#include <cstdio>
#include <array>
#include "../helpers/filetypehelpers.h"

namespace NickvisionTubeConverter::Models
{
    using namespace NickvisionTubeConverter::Helpers;

    Download::Download(const std::string& videoUrl, FileType fileType, const std::string& saveFolder, const std::string& newFilename) : m_videoUrl(videoUrl), m_fileType(fileType), m_path(saveFolder + "/" + newFilename)
    {

    }

    const std::string& Download::getVideoUrl() const
    {
        return m_videoUrl;
    }

    FileType Download::getFileType() const
    {
        return m_fileType;
    }

    std::string Download::getPath() const
    {
        return m_path + FileTypeHelpers::toDotExtension(m_fileType);
    }

    bool Download::download() const
    {
        std::string cmdDownload = "";
        if(FileTypeHelpers::isVideo(m_fileType))
        {
            cmdDownload = "yt-dlp --format " + FileTypeHelpers::toString(m_fileType) + " " + m_videoUrl + " -o \"" + m_path + ".%(ext)s\"";
        }
        else
        {
            cmdDownload = "yt-dlp -f 'ba' -x --audio-format " + FileTypeHelpers::toString(m_fileType) + " " + m_videoUrl + " -o \"" + m_path + ".%(ext)s\"";
        }
        std::string cmdOutput = "";
        std::array<char, 128> buffer;
        FILE* pipe = popen(cmdDownload.c_str(), "r");
        if(!pipe)
        {
            return false;
        }
        while (!feof(pipe))
        {
            if (fgets(buffer.data(), 128, pipe) != nullptr)
            {
                cmdOutput += buffer.data();
            }
        }
        int resultCode = pclose(pipe);
        return resultCode == EXIT_SUCCESS;
    }
}
