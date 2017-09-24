#include "config.h"

Config::Config()
{
  m_colors.resize(static_cast<size_t>(Color::eCount));

  m_colors[static_cast<size_t>(Color::eFontName)] = QColor(255, 245, 238, 255);
  m_colors[static_cast<size_t>(Color::eFontType)] = QColor(169, 169, 169, 255);

  m_colors[static_cast<size_t>(Color::eNodeHeader)] = QColor(105, 105, 105, 64);
  m_colors[static_cast<size_t>(Color::eNodeContent)] = QColor(105, 105, 105, 128);

  m_colors[static_cast<size_t>(Color::eSocketBorder)] = QColor(54, 54, 54, 255);
  m_colors[static_cast<size_t>(Color::eSocketInput)] = QColor(255, 99, 71, 255);
  m_colors[static_cast<size_t>(Color::eSocketOutput)] = QColor(135, 206, 235, 255);
  m_colors[static_cast<size_t>(Color::eSocketDrop)] = QColor(173, 255, 47, 255);
  m_colors[static_cast<size_t>(Color::eSocketHover)] = QColor(255, 215, 0, 255);

  m_colors[static_cast<size_t>(Color::eBoolSignalOff)] = QColor(244, 53, 64, 255);
  m_colors[static_cast<size_t>(Color::eBoolSignalOn)] = QColor(75, 173, 88, 255);
  m_colors[static_cast<size_t>(Color::eIntegerSignalOff)] = QColor(62, 84, 174, 255);
  m_colors[static_cast<size_t>(Color::eIntegerSignalOn)] = QColor(0, 170, 238, 255);
  m_colors[static_cast<size_t>(Color::eFloatSignalOff)] = QColor(111, 80, 96, 255);
  m_colors[static_cast<size_t>(Color::eFloatSignalOn)] = QColor(232, 0, 99, 255);

  //  QColor color(244, 53, 64, 255); // RED
  //  QColor color(232, 0, 99, 255); // PINK
  //  QColor color(154, 41, 169, 255); // PURPLE
  //  QColor color(101, 63, 176, 255); // DEEP PURPLE
  //  QColor color(62, 84, 174, 255); // INDIGO
  //  QColor color(29, 151, 237, 255); // BLUE
  //  QColor color(0, 170, 238, 255); // LIGHT BLUE
  //  QColor color(0, 188, 208, 255); // CYAN
  //  QColor color(0, 149, 134, 255); // TEAL
  //  QColor color(75, 173, 88, 255); // GREEN
  //  QColor color(137, 193, 86, 255); // LIGHT GREEN
  //  QColor color(203, 217, 81, 255); // LIME
  //  QColor color(254, 231, 86, 255); // YELLOW
  //  QColor color(254, 187, 59, 255); // AMBER
  //  QColor color(254, 144, 50, 255); // ORANGE
  //  QColor color(254, 74, 53, 255); // DEEP ORANGE
  //  QColor color(120, 83, 74, 255); // BROWN
  //  QColor color(156, 156, 156, 255); // GREY
  //  QColor color(95, 124, 136, 255); // BLUE GREY
  //  QColor color(58, 66, 71, 255); // DARK GREY
  //  QColor color(82, 55, 77, 255); // PURPLE TAUPE
  //  QColor color(111, 80, 96, 255); // WENGE
  //  m_colors[static_cast<size_t>(Color::eSocketHover)] = color;

  m_colors[static_cast<size_t>(Color::eLink)] = QColor(165, 165, 165, 64);

  m_colors[static_cast<size_t>(Color::eSelected)] = QColor(255, 255, 255, 255);
}

Config const &Config::get()
{
  static Config s_config{};
  return s_config;
}
