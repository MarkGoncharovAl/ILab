#include "Errors.hpp"

namespace MLib_Error
{
    //Constructors
    ////////////////////////////////////
    ////////////////////////////////////
    #ifndef LOG_FILE
    Logging::Logging () :
        size_file_ (15) ,
        size_line_ (4) ,
        out_ ("debug.log") ,
        log_ (out_)
    {
        if (!out_.is_open ())
            throw std::runtime_error ("Can't create DEFAULT_LOG_FILE for logging");
    }

    #else //LOG_FILE was set
    Logging::Logging () :
        size_file_ (15) ,
        size_line_ (4) ,
        out_ () ,
        log_ (out_)
    {}
    #endif

    Log_stream::Log_stream (std::ofstream& out)
        : out_ (out)
    {}
    ////////////////////////////////////
    ////////////////////////////////////

    //Log_stream
    ////////////////////////////////////
    ////////////////////////////////////
    void Logging::PrintLevel (Logging_Level level)
    {
        std::string str;
        str.reserve (7);

        switch (level)
        {
        case Logging_Level::fatal:
            str = "FATAL ";
            break;
        case Logging_Level::error:
            str = "ERROR ";
            break;
        case Logging_Level::warning:
            str = "WARN  ";
            break;
        case Logging_Level::debug:
            str = "debug ";
            break;
        case Logging_Level::trace:
            str = "trace ";
            break;
        };

        out_ << str;
    }

    void Logging::PrintFileLine (const std::string& file , size_t line)
    {
        out_ << "File[" << std::setfill ('_') << std::setw (size_file_) << file << std::right
            << "] Line[" << std::setw (size_line_) << line << std::right
            << "] ";
    }

    void Logging::SetSize_NameFile (size_t new_size) noexcept
    {
        size_file_ = new_size;
    }
    void Logging::SetSize_NameLine (size_t new_size) noexcept
    {
        size_line_ = new_size;
    }
    ////////////////////////////////////
    ////////////////////////////////////

    //Logging
    ////////////////////////////////////
    ////////////////////////////////////
    void Logging::ChangeDirectory (const std::string& file) throw()
    {
        if (file.empty ())
        {
            LOG_error << "Can't change directory on empty file!";
            throw std::runtime_error ("empty file");
        }

        out_.close ();
        out_.open (file);
        if (!out_.is_open ())
        {
            LOG_error << "Can't properly open new logging directory " << file;
            throw std::runtime_error ("can't open");
        }
    }
}