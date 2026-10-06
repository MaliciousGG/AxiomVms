target_sources(
    AxiomVms.Core
    PRIVATE
        # Json Core Implementation
        Private/Core/Json/JsonParser.cpp
        Private/Core/Json/JsonValue.cpp

        # Logging Core Implementation
        Private/Core/Logging/ConsoleLogger.cpp
        Private/Core/Logging/Log.cpp

    PUBLIC
        # Main Core Headers
        Public/Core/Api.h
        Public/Core/Assert.h
        Public/Core/Compiler.h
        Public/Core/Platform.h
        Public/Core/Types.h
        Public/CoreMinimal.h

        # Json Core Headers
        Public/Core/Json/JsonParseError.h
        Public/Core/Json/JsonParser.h
        Public/Core/Json/JsonSourceLocation.h
        Public/Core/Json/JsonValue.h

        # Logging Core Headers
        Public/Core/Logging/ConsoleLogger.h
        Public/Core/Logging/ILogger.h
        Public/Core/Logging/Log.h
        Public/Core/Logging/LogLevel.h
)