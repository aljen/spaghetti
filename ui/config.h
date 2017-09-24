#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <QVector>

class Config {
 public:
  enum class Color {
    eFontName,
    eFontType,

    eNodeHeader,
    eNodeContent,

    eSocketBorder,
    eSocketInput,
    eSocketOutput,
    eSocketDrop,
    eSocketHover,

    eBoolSignalOff,
    eBoolSignalOn,
    eIntegerSignalOff,
    eIntegerSignalOn,
    eFloatSignalOff,
    eFloatSignalOn,

    eLink,

    eSelected,
    eCount
  };

  Config();

  static Config const &get();

  inline QColor getColor(Color aColor) const { return m_colors[static_cast<char>(aColor)]; }

 private:
  QVector<QColor> m_colors{};
};

#endif // CONFIG_H
