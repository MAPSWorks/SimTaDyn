//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef LOGGER_HPP_
#  define LOGGER_HPP_

#  include "Singleton.tpp"
#  include "ILogger.hpp"
//#  include "TextColor.hpp"

class Logger: public FileLogger, public LazySingleton<Logger>
{
  friend class LazySingleton<Logger>;

public:

  Logger();
  Logger(std::string const& logfile);
  virtual ~Logger();
  void changeLog(std::string const& logfile);
  ILogger& operator<<(const logger::LoggerSeverity& severity);
  ILogger& operator<<(const char *msg);

  // FIXME: en double avec File::fileName
  inline static std::string fileName(std::string const& path)
  {
    std::string::size_type pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
      return path.substr(pos + 1, std::string::npos);
    return path;
  }

protected:

  virtual void open(std::string const& filename) override;
  virtual void close() override;

private:

  inline virtual void write(std::string const& message) override
  {
    write(message.c_str());
  }

  virtual void write(const char *message, const int length = -1) override;
  virtual void header() override;
  virtual void footer() override;
  virtual void beginLine() override;

private:

  std::ofstream m_file;
};

#define SHORT_FILENAME Logger::fileName(__FILE__).c_str()

//! \brief Log C++ like. Example:  CPP_LOG(logger::Fatal) << "test\n";
#define CPP_LOG(severity, ...)                                          \
  Logger::instance() << Logger::instance().strtime();                   \
  Logger::instance() << severity << '[' << SHORT_FILENAME << "::" << __LINE__ << "] "


//! \brief Basic log without severity or file and line information. 'B' for Basic.
#define LOGB(...)         Logger::instance().log(nullptr, logger::None, __VA_ARGS__)
//! \brief Information Log.
#define LOGI(format, ...) Logger::instance().log(nullptr, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Debug Log.
#define LOGD(format, ...) Logger::instance().log(nullptr, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Warning Log.
#define LOGW(format, ...) Logger::instance().log(nullptr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Failure Log.
#define LOGF(format, ...) Logger::instance().log(nullptr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Error Log.
#define LOGE(format, ...) Logger::instance().log(nullptr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Fatal Log.
#define LOGX(format, ...) Logger::instance().log(nullptr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#define LOGIS(format, ...) Logger::instance().log(&std::cout, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGDS(format, ...) Logger::instance().log(&std::cout, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGWS(format, ...) Logger::instance().log(&std::cerr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGFS(format, ...) Logger::instance().log(&std::cerr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGES(format, ...) Logger::instance().log(&std::cerr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGXS(format, ...) Logger::instance().log(&std::cerr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#endif /* LOGGER_HPP_ */
