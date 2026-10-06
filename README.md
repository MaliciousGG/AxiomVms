# **AxiomVms**

**AxiomVms** is a C++ video management system project built as a way to learn and apply modern C++ concepts in a practical system.

The project started as a portfolio project focused on building something closer to real world infrastructure software instead of leetcode style exercises. The goal is to work through areas such as camera management, stream handling, testing, serialization, error handling, CMake, and architecture while learning why those systems are designed the way they are.

This project is still in active development and will continue to change as new systems are added and existing ones are refactored.

## Contact

- **GitHub: [MaliciousGG](https://github.com/MaliciousGG)**
- **Discord: `Malicious_`**

## **Goals**

The main goals of **AxiomVms** are:

- Build a functional video management system from the ground up using as few libraries as possible
- Improve understanding of modern C++
- Practice designing maintainable systems and interfaces
- Learn how to structure and test a larger C++ project
- Work with real video and streaming technologies such as GStreamer
- Build experience with CMake and cross-platform development
- Document design decisions and lessons learned throughout development

## **Current Features**

**AxiomVms** currently includes or is working toward:

- Camera management
- Camera session lifecycle
- Stream abstraction through interfaces and factories
- GStreamer integration
- JSON value representation and parsing
- Structured logging
- Platform and compiler utilities
- Unit testing with GoogleTest
- CMake-based project configuration

## **Project Structure**

```text
AxiomVms/
├── Source/
│   ├── AxiomVms.Application/
│   │   ├── Private/
│   │   │   └── AxiomApplication.cpp
│   │   ├── Public/
│   │   │   └── AxiomApplication.h
│   │   ├── AxiomVms.cpp
│   │   └── CMakeLists.txt
│   │
│   ├── AxiomVms.Core/
│   │   ├── CMake/
│   │   │   ├── Cameras.cmake
│   │   │   ├── Core.cmake
│   │   │   └── Streaming.cmake
│   │   ├── Private/
│   │   │   ├── Cameras/
│   │   │   │   ├── CameraManager.cpp
│   │   │   │   └── CameraSession.cpp
│   │   │   ├── Core/
│   │   │   │   ├── Json/
│   │   │   │   │   ├── JsonParser.cpp
│   │   │   │   │   └── JsonValue.cpp
│   │   │   │   └── Logging/
│   │   │   │       ├── ConsoleLogger.cpp
│   │   │   │       └── Log.cpp
│   │   │   └── Streaming/
│   │   │       ├── GStreamerCameraStream.cpp
│   │   │       ├── GStreamerCameraStreamFactory.cpp
│   │   │       └── NullCameraStreamFactory.cpp
│   │   ├── Public/
│   │   │   ├── Cameras/
│   │   │   │   ├── Camera.h
│   │   │   │   ├── CameraManager.h
│   │   │   │   └── CameraSession.h
│   │   │   ├── Core/
│   │   │   │   ├── Json/
│   │   │   │   │   ├── JsonParseError.h
│   │   │   │   │   ├── JsonParser.h
│   │   │   │   │   ├── JsonSourceLocation.h
│   │   │   │   │   └── JsonValue.h
│   │   │   │   ├── Logging/
│   │   │   │   │   ├── ConsoleLogger.h
│   │   │   │   │   ├── ILogger.h
│   │   │   │   ├── Log.h
│   │   │   │   └── LogLevel.h
│   │   │   │   ├── Api.h
│   │   │   │   ├── Assert.h
│   │   │   │   ├── Compiler.h
│   │   │   │   ├── Platform.h
│   │   │   │   └── Types.h
│   │   │   ├── Streaming/
│   │   │   │   ├── CameraStreamStats.h
│   │   │   │   ├── GStreamerCameraStream.h
│   │   │   │   ├── GStreamerCameraStreamFactory.h
│   │   │   │   ├── ICameraStream.h
│   │   │   │   ├── ICameraStreamFactory.h
│   │   │   │   ├── NullCameraStream.h
│   │   │   │   └── NullCameraStreamFactory.h
│   │   │   └── CoreMinimal.h
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
│
├── Tests/
│   ├── Core/
│   │   ├── Cameras/
│   │   │   ├── CameraManagerTests.cpp
│   │   │   ├── CameraSessionTests.cpp
│   │   └── Json/
│   │       ├── JsonParserTests.cpp
│   │       └── JsonValueTests.cpp
│   └── CMakeLists.txt
├── Docs/
└── CMakeLists.txt
```
The exact structure may change as the project grows.

### **Tests**

Tests cover cameras, sessions, JSON parsing, required GStreamer elements, and a
synthetic pipeline. No camera is required. These presets cover Windows; Linux
builds have not been validated.

## **Development Notes**
This repository also contains working documentation and development notes. These are intentionally kept alongside the project to document:

- Design decisions
- Problems encountered during development
- Changes in architecture
- Concepts learned while implementing systems
- Reasons behind certain implementation choices

Some of these documents may be working notes rather than finalized documentation.

## **Status**
AxiomVms is currently under active development. The project is being built incrementally, with individual systems implemented and tested before moving on to larger features.

## **Planned Work**

Some areas planned for future development include:

- RTSP camera streams
- Improved GStreamer integration
- Recording and playback
- Camera health monitoring
- Storage management
- Configuration loading
- Improved JSON serialization and parsing
- Networking and API support
- Application UI

## **Technologies**
- C++20
- CMake
- GoogleTest
- GStreamer
- Git / GitHub
- GitHub Actions

## License

This project is licensed under the MIT License.

You are free to use, modify, distribute, and incorporate this software into other projects, including commercial projects, provided that the original copyright notice and license are included.

Credit is appreciated when using or adapting AxiomVms, especially in public or derivative projects.

See the [LICENSE](LICENSE) file for the full license text.