// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef UI_EXPANDER_WIDGET_H
#define UI_EXPANDER_WIDGET_H

#include <QTreeWidget>
#include <QIcon>

class ExpanderWidget : public QTreeWidget {
  Q_OBJECT
  Q_PROPERTY(int count READ count)

 public:
  explicit ExpanderWidget(QWidget *const a_parent = nullptr);

  int addItem(QWidget *const a_widget, QIcon const &a_iconSet, QString const &a_text);
  int addItem(QWidget *const a_widget, QString const &a_text);

  int insertItem(int const a_index, QWidget *const a_widget, QIcon const &a_iconSet, QString const &a_text);
  int insertItem(int const a_index, QWidget *const a_widget, QString const &a_text);

  int count() const;

  int indexOf(QWidget *const a_widget) const;

  QWidget *widget(int const a_index) const;
  QIcon itemIcon(int const a_index) const;
  QString itemText(int const a_index) const;

  void removeItem(int const a_index);

  void setItemIcon(int const a_index, QIcon const &a_icon);
  void setItemText(int const a_index, QString const &a_text);

 private:
  void onItemPressed(QTreeWidgetItem *const a_item, int const a_column);

 private:
  struct WidgetContainer {
    QString text{};
    QIcon icon{};
    QTreeWidgetItem *category{};
    QWidget *widget{};
  };
  QList<WidgetContainer> m_widgets{};
};

#endif // UI_EXPANDER_WIDGET_H
