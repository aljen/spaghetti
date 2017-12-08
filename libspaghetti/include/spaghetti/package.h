// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
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

#pragma once
#ifndef SPAGHETTI_PACKAGE_H
#define SPAGHETTI_PACKAGE_H

#include <concurrentqueue.h>
// clang-format off
#ifdef _MSC_VER
# include <unordered_map>
#else
# include <sparsepp/spp.h>
#endif
// clang-format on

#include <spaghetti/api.h>
#include <spaghetti/element.h>
#include <spaghetti/strings.h>

// clang-format off
#define PACKAGE_SPP_MAP 1
#define PACKAGE_STD_UNORDERED_MAP 2
#ifdef _MSC_VER
# define PACKAGE_MAP PACKAGE_STD_UNORDERED_MAP
#else
# define PACKAGE_MAP PACKAGE_SPP_MAP
#endif
// clang-format on

namespace spaghetti {

class SPAGHETTI_API Package final : public Element {
 public:
  using Elements = std::vector<Element *>;
  struct Connection {
    size_t from_id{};
    uint8_t from_socket{};
    size_t to_id{};
    uint8_t to_socket{};
  };

  using Connections = std::vector<Connection>;

  Package();
  ~Package() override;

  static constexpr char const *const TYPE{ "logic/package" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  std::string_view packageDescription() const { return m_packageDescription; }
  void setPackageDescription(std::string a_description) { m_packageDescription = a_description; }

  std::string_view packagePath() const { return m_packagePath; }
  void setPackagePath(std::string a_path) { m_packagePath = a_path; }

  std::string_view packageIcon() const { return m_packageIcon; }
  void setPackageIcon(std::string a_icon) { m_packageIcon = a_icon; }

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  Element *add(char const *const a_name) { return add(string::hash(a_name)); }
  Element *add(string::hash_t a_hash);

  void remove(Element *const a_element) { remove(a_element->id()); }
  void remove(size_t a_id);

  Element *get(size_t a_id) const;

  bool connect(size_t a_sourceId, uint8_t a_outputId, size_t a_targetId, uint8_t a_inputId);
  bool disconnect(size_t a_sourceId, uint8_t a_outputId, size_t a_targetId, uint8_t a_inputId);

  void dispatchThreadFunction();
  void updatesThreadFunction();

  void startDispatchThread();
  void quitDispatchThread();

  void elementChanged(size_t a_id);

  void setInputsPosition(double const a_x, double const a_y)
  {
    m_inputsPosition.x = a_x;
    m_inputsPosition.y = a_y;
  }
  void setInputsPosition(Vec2 const a_position) { m_inputsPosition = a_position; }
  Vec2 const &inputsPosition() const { return m_inputsPosition; }

  void setOutputsPosition(double const a_x, double const a_y)
  {
    m_outputsPosition.x = a_x;
    m_outputsPosition.y = a_y;
  }
  void setOutputsPosition(Vec2 const a_position) { m_outputsPosition = a_position; }
  Vec2 const &outputsPosition() const { return m_outputsPosition; }

  Elements const &elements() const { return m_data; }
  Connections const &connections() const { return m_connections; }

  void open(std::string const &a_filename);
  void save(std::string const &a_filename);

 private:
  bool tryDispatch();
  void dispatch(size_t a_id);

 private:
  std::string m_packageDescription{ "A package" };
  std::string m_packagePath{ "packages/unknown_package" };
  std::string m_packageIcon{ "icons/unknown.png" };
  Vec2 m_inputsPosition{};
  Vec2 m_outputsPosition{};
  Elements m_data{};
  Connections m_connections{};

  std::vector<size_t> m_free{};

  std::vector<Element *> m_updatables{};

  moodycamel::ConcurrentQueue<size_t> m_queue{};
  moodycamel::ConcurrentQueue<Element *> m_addUpdatableQueue{};
  moodycamel::ConcurrentQueue<Element *> m_removeUpdatableQueue{};

#if PACKAGE_MAP == PACKAGE_SPP_MAP
  using Callbacks = spp::sparse_hash_map<size_t, std::vector<size_t>>;
#elif PACKAGE_MAP == PACKAGE_STD_UNORDERED_MAP
  using Callbacks = std::unordered_map<size_t, std::vector<size_t>>;
#endif

  Callbacks m_dependencies{};
  std::thread m_dispatchThread{};
  std::thread m_updatesThread{};

  bool m_quit{};
};

} // namespace spaghetti

#endif // SPAGHETTI_PACKAGE_H
