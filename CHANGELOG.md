# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 🔖 [[0.2.1]](https://github.com/OpCoSim/OpCoSerializer/releases/tag/v0.2.0 "v0.2.1 Release")

### 🐛 Fixed

- Adding missing `typename` keywords for clang support.

## 🔖 [[0.2.0]](https://github.com/OpCoSim/OpCoSerializer/releases/tag/v0.2.0 "v0.2.0 Release")

### ✨ Added

- `std::string` support in JSON serializer.
- Native `enum class` support in JSON serializer.

### 🙌 Improvements

- Types with OpCoSerializer properties are now automatically detected and serialized to objects.
- Renames `properties` to `SerializerProperties` for clearer intent on consuming types.

## 🔖 [[v0.1.0]](https://github.com/OpCoSim/OpCoSerializer/releases/tag/v0.1.0 "v0.1.0 Release")

### ✨ Added

- Initial project 🎂
- JSON serializer implementation.
- Extendable type serialization.
- Extendable serializer interface.

### 👷 Build

- Added build and test GitHub action.

### 📚 Third Party Libraries

- Added [rapidjson](https://github.com/Tencent/rapidjson "rapidjson repository link.").
