// Copyright (c) 2017-2022 Fuego Developers
// Copyright (c) 2018-2019 Conceal Network & Conceal Devs
// Copyright (c) 2016-2019 The Karbowanec developers
// Copyright (c) 2012-2018 The CryptoNote developers
//
// This file is part of Fuego.
//
// Fuego is free software distributed in the hope that it
// will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. You can redistribute it and/or modify it under the terms
// of the GNU General Public License v3 or later versions as published
// by the Free Software Foundation. Fuego includes elements written
// by third parties. See file labeled LICENSE for more details.
// You should have received a copy of the GNU General Public License
// along with Fuego. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <vector>
#include <Common/IOutputStream.h>
#include "ISerializer.h"
#include "MemoryStream.h"

namespace CryptoNote {

class KVBinaryOutputStreamSerializer : public ISerializer {
public:

  KVBinaryOutputStreamSerializer();
  virtual ~KVBinaryOutputStreamSerializer() {}

  void dump(Common::IOutputStream& target);

  virtual ISerializer::SerializerType type() const override;

  virtual bool beginObject(Common::StringView name) override;
  virtual void endObject() override;

  virtual bool beginArray(size_t& size, Common::StringView name) override;
  virtual void endArray() override;

  virtual bool operator()(uint8_t& value, Common::StringView name) override;
  virtual bool operator()(int16_t& value, Common::StringView name) override;
  virtual bool operator()(uint16_t& value, Common::StringView name) override;
  virtual bool operator()(int32_t& value, Common::StringView name) override;
  virtual bool operator()(uint32_t& value, Common::StringView name) override;
  virtual bool operator()(int64_t& value, Common::StringView name) override;
  virtual bool operator()(uint64_t& value, Common::StringView name) override;
  virtual bool operator()(double& value, Common::StringView name) override;
  virtual bool operator()(bool& value, Common::StringView name) override;
  virtual bool operator()(std::string& value, Common::StringView name) override;
  virtual bool binary(void* value, size_t size, Common::StringView name) override;
  virtual bool binary(std::string& value, Common::StringView name) override;

  template<typename T>
  bool operator()(T& value, Common::StringView name) {
    return ISerializer::operator()(value, name);
  }

private:

  void writeElementPrefix(uint8_t type, Common::StringView name);
  void checkArrayPreamble(uint8_t type);
  void updateState(uint8_t type);
  MemoryStream& stream();

  enum class State {
    Root,
    Object,
    ArrayPrefix,
    Array
  };

  struct Level {
    State state;
    std::string name;
    size_t count;

    Level(Common::StringView nm) :
      name(nm), state(State::Object), count(0) {}

    Level(Common::StringView nm, size_t arraySize) :
      name(nm), state(State::ArrayPrefix), count(arraySize) {}

    Level(Level&& rv) {
      state = rv.state;
      name = std::move(rv.name);
      count = rv.count;
    }

  };

  std::vector<MemoryStream> m_objectsStack;
  std::vector<Level> m_stack;
};

}
