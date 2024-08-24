if (WIN32)
    # Patch CMake's 'add_test' to extend the DLL search path on Windows. Use the
    # target name associated with the test, if it is also a CMake target.

    macro(add_test)
        _add_test(${ARGV})

        cmake_parse_arguments("" "" "NAME" "" ${ARGV})
        if (TARGET ${_NAME})
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:${_NAME}>)
        else()
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:testTf>)
        endif()

        set_tests_properties(${_NAME}
            PROPERTIES ENVIRONMENT
            "PATH=$<JOIN:$<SHELL_PATH:${DLL_DIRS}>,\\;>")
    endmacro()
endif()

set(testWrapper "${Python_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/testWrapper.py" --verbose)
set(crashHandlerCmd "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfCrashHandler.py")

add_library(TestTfDl SHARED TestTfDl.cpp)

add_library(TestTfRegistryFunctionPlugin SHARED TestTfRegistryFunctionPlugin.cpp)
target_link_libraries(TestTfRegistryFunctionPlugin PUBLIC tf)

add_executable(testTf
    main.cpp
    anyUniquePtr.cpp
    atomicOfstreamWrapper.cpp
    bitUtils.cpp
    debug.cpp
    denseHashMap.cpp
    delegate.cpp
    dl.cpp
    enum.cpp
    error.cpp
    envSetting.cpp
    exception.cpp
    fastCompression.cpp
    fileUtils.cpp
    functionRef.cpp
    getenv.cpp
    hash.cpp
    iterator.cpp
    mallocTag.cpp
    meta.cpp
    notice.cpp
    pathUtils.cpp
    patternMatcher.cpp
    pointerAndBits.cpp
    preprocessorUtilsLite.cpp
    probe.cpp
    refPtr.cpp
    registryManager.cpp
    registryManagerUnload.cpp
    rwMutexes.cpp
    safeOutputFile.cpp
    scoped.cpp
    scopeDescription.cpp
    setenv.cpp
    smallVector.cpp
    stacked.cpp
    staticData.cpp
    stl.cpp
    stopwatch.cpp
    stringUtils.cpp
    staticTokens.cpp
    templateString.cpp
    token.cpp
    delegatedCountPtr.cpp
    type.cpp
    typeMultipleInheritance.cpp
    typeInfoMap.cpp
    unicodeUtils.cpp
    weakPtr.cpp)
target_link_libraries(testTf PUBLIC tf)

add_test(NAME TfAnyUniquePtr COMMAND testTf TfAnyUniquePtr)

add_test(NAME TfAtomicOfstreamWrapper COMMAND testTf TfAtomicOfstreamWrapper)

add_test(NAME TfBitUtils COMMAND testTf TfBitUtils)

add_test(NAME TfDl COMMAND testTf TfDl)

add_test(NAME TfDebug COMMAND testTf TfDebug)

add_test(NAME TfDebugFatal_1
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDebugFatal_1"
    --expected-return-code=134)

add_test(NAME TfDebugFatal_2
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDebugFatal_2"
    --expected-return-code=134)

add_test(NAME TfDebugFatal_3
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDebugFatal_3"
    --expected-return-code=134)

add_test(NAME TfDebugTestEnv
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDebugTestEnv"
    --env-var=TF_DEBUG="FOO* FLAM_* FLAM -FOOFLIMFLAM TF_DISCOVERY_D*"
    --stdout-redirect=debugTestEnv.out
    --diff-compare=debugTestEnv.out
    --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/TfDebugTestEnv)

add_test(NAME TfDebugTestEnvHelp
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDebugTestEnvHelp"
    --env-var=TF_DEBUG=help)

add_test(NAME TfDenseHashMap COMMAND testTf TfDenseHashMap)

add_test(NAME TfDelegate
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfDelegateAddRemove"
    --stdout-redirect=output.txt
    --diff-compare=output.txt
    --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/TfDelegate)

add_test(NAME TfError
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfError"
    --env-var=TF_FATAL_VERIFY=0)

add_test(NAME TfErrorThreadTransport COMMAND testTf TfErrorThreadTransport)

add_test(NAME TfEnvSetting
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfEnvSetting"
    --env-var=TF_TEST_BOOL_ENV_SETTING=1
    --env-var=TF_TEST_INT_ENV_SETTING=123
    --env-var=TF_TEST_STRING_ENV_SETTING=alpha
    --env-var=TF_ENV_SETTING_ALERTS_ENABLED=0
    --stdout-redirect=debugTfEnvSettingStderr.txt
    --diff-compare=debugTfEnvSettingStderr.txt
    --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/TfEnvSetting)

add_test(NAME TfException
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfException"
    --env-var=TF_FATAL_THROW=0)

add_test(NAME TfFileUtils COMMAND testTf TfFileUtils)

add_test(NAME TfFunctionRef COMMAND testTf TfFunctionRef)

add_test(NAME TfGetenv COMMAND testTf TfGetenv)

add_test(NAME TfHash COMMAND testTf TfHash)

add_test(NAME TfIterator COMMAND testTf TfIterator)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME TfMallocTag
        COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfMallocTag"
        --env-var=GLIBCXX_FORCE_NEW=1)
endif()

add_test(NAME TfRefPtr COMMAND testTf TfRefPtr)

add_test(NAME TfEnum
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfEnum"
    --stdout-redirect=enum.out
    --diff-compare=enum.out
    --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/TfEnum)

add_test(NAME TfNotice
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfNotice"
    --stdout-redirect=notice.out
    --diff-compare=notice.out
    --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/TfNotice)

add_test(NAME TfPathUtils COMMAND testTf TfPathUtils)

add_test(NAME TfPatternMatcher COMMAND testTf TfPatternMatcher)

add_test(NAME TfPointerAndBits COMMAND testTf TfPointerAndBits)

add_test(NAME TfPreprocessorUtilsLite COMMAND testTf TfPreprocessorUtilsLite)

add_test(NAME TfProbe COMMAND testTf TfProbe)

add_test(NAME TfRWMutexes COMMAND testTf TfRWMutexes)

add_test(NAME TfRegistryManager COMMAND testTf TfRegistryManager)

if(BUILD_SHARED_LIBS)
    add_test(NAME TfRegistryManagerUnload COMMAND testTf TfRegistryManagerUnload)
endif()

add_test(NAME TfRegTest
    COMMAND ${testWrapper} $<TARGET_FILE:testTf> --expected-return-code=2)

add_test(NAME TfRegTest_TfScoped
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfScoped no args expected"
    --expected-return-code=2)

add_test(NAME TfRegTest_TfUndefinedTest
    COMMAND ${testWrapper} "$<TARGET_FILE:testTf> TfUndefinedTest"
    --expected-return-code=3)

add_test(NAME TfSafeOutputFile COMMAND testTf TfSafeOutputFile)

add_test(NAME TfScopeDescription COMMAND testTf TfScopeDescription)

add_test(NAME TfScoped COMMAND testTf TfScoped)

add_test(NAME TfScopedVar COMMAND testTf TfScopedVar)

add_test(NAME TfSetenv COMMAND testTf TfSetenv)

add_test(NAME TfSmallVector COMMAND testTf TfSmallVector)

add_test(NAME TfStacked COMMAND testTf TfStacked)

add_test(NAME TfStaticData COMMAND testTf TfStaticData)

add_test(NAME TfStaticTokens COMMAND testTf TfStaticTokens)

add_test(NAME TfStl COMMAND testTf TfStl)

add_test(NAME TfStopwatch COMMAND testTf TfStopwatch)

add_test(NAME TfStringUtils COMMAND testTf TfStringUtils)

add_test(NAME TfTemplateString COMMAND testTf TfTemplateString)

add_test(NAME TfToken COMMAND testTf TfToken)

add_test(NAME TfDelegatedCountPtr COMMAND testTf TfDelegatedCountPtr)

add_test(NAME TfType COMMAND testTf TfType)

add_test(NAME TfTypeInfoMap COMMAND testTf TfTypeInfoMap)

add_test(NAME TfType_MultipleInheritance COMMAND testTf TfType_MultipleInheritance)

add_test(NAME TfUnicodeUtils COMMAND testTf TfUnicodeUtils)

add_test(NAME TfWeakPtr COMMAND testTf TfWeakPtr)

add_test(NAME TfWeakPtrConversion COMMAND testTf TfWeakPtrConversion)

add_executable(testTfSIGSEGV SIGSEGV.cpp)
target_link_libraries(testTfSIGSEGV PUBLIC tf)
add_test(NAME testTfSIGSEGV
    COMMAND ${testWrapper} "${crashHandlerCmd} $<TARGET_FILE:testTfSIGSEGV> SIGSEGV")

add_executable(testTfSIGFPE SIGFPE.cpp)
target_link_libraries(testTfSIGFPE PUBLIC tf)
add_test(NAME testTfSIGFPE
    COMMAND ${testWrapper} "${crashHandlerCmd} $<TARGET_FILE:testTfSIGFPE> SIGFPE")

add_executable(testTfCxxCast testTfCast.cpp)
target_link_libraries(testTfCxxCast PUBLIC tf)
add_test(NAME testTfCxxCast COMMAND testTfCxxCast)

add_executable(testTfSpan testTfSpan.cpp)
target_link_libraries(testTfSpan PUBLIC tf)
add_test(NAME testTfSpan COMMAND testTfSpan)

if(BUILD_PYTHON_BINDINGS)
    set(PATH_SEP $<IF:$<BOOL:${WIN32}>,$<SEMICOLON>,:>)
    set(testPyWrapper
        ${testWrapper}
        "--env-var=PYTHONPATH=$<TARGET_FILE_DIR:pyTf>/../..${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}")

    add_test(NAME testTfPython
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPython.py")

    add_executable(testTfPyFunction testTfPyFunction.cpp)
    target_link_libraries(testTfPyFunction PUBLIC tf Boost::python)
    add_test(NAME testTfPyFunction
        COMMAND ${testPyWrapper} $<TARGET_FILE:testTfPyFunction>)

    add_test(NAME testTfPyNotice
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPyNotice.py")

    add_executable(testTfPyInterpreter testTfPyInterpreter.cpp)
    target_link_libraries(testTfPyInterpreter PUBLIC tf)
    add_test(NAME testTfPyInterpreter
        COMMAND ${testPyWrapper} $<TARGET_FILE:testTfPyInterpreter>)

    add_executable(testTfPyInvoke testTfPyInvoke.cpp)
    target_link_libraries(testTfPyInvoke PUBLIC tf)
    add_test(NAME testTfPyInvoke
        COMMAND ${testPyWrapper} $<TARGET_FILE:testTfPyInvoke>)

    add_executable(testTfPyLock testTfPyLock.cpp)
    target_link_libraries(testTfPyLock PUBLIC tf Boost::python)
    add_test(NAME testTfPyLock
        COMMAND ${testPyWrapper} $<TARGET_FILE:testTfPyLock>)

    add_executable(testTfPyResultConversions testTfPyResultConversions.cpp)
    target_link_libraries(testTfPyResultConversions PUBLIC tf Boost::python)
    add_test(NAME testTfPyResultConversions
        COMMAND ${testPyWrapper} $<TARGET_FILE:testTfPyResultConversions>)

    add_test(NAME testTfPyScopeDescription
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPyScopeDescription.py")

    add_test(NAME testTfScriptModuleLoader
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfScriptModuleLoader.py"
        --stdout-redirect=scriptModuleLoader.out
        --diff-compare=scriptModuleLoader.out
        --baseline-dir=${CMAKE_CURRENT_SOURCE_DIR}/baseline/testTfScriptModuleLoader)

    add_test(NAME TfFileUtils_Python
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfFileUtils.py")

    add_test(NAME TfStringUtils_Python
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfStringUtils.py")

    add_test(NAME TfMallocTagReport
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfMallocTagReport.py")

    add_test(NAME TfNamedTemporaryFile
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfNamedTemporaryFile.py")

    add_test(NAME TfPathUtils_Python
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPathUtils.py")

    add_test(NAME testTfTemplateString
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfTemplateString.py")

    add_test(NAME testTfType
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfType.py")

    add_test(NAME testTf_PyContainerConversions
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTf_PyContainerConversions.py")

    add_test(NAME testTf_PyOptional
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPyOptional.py")

    add_test(NAME testTf_PyStaticTokens
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPyStaticTokens.py")
endif()

if (WIN32)
    # Add a test that WindowsImportWrapper adds entries on the PATH in the correct order, to ensure
    # that the correct dlls needed by python modules are loaded

    add_library(testTfPyDllLinkImplementationGood SHARED
        testTfPyDllLinkImplementationGood.cpp)

    set_target_properties(testTfPyDllLinkImplementationGood
        PROPERTIES
            OUTPUT_NAME "testTfPyDllLinkImplementation"
            RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/ImplementationGood"
            ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/ImplementationGood")

    add_library(testTfPyDllLinkImplementationBad SHARED
        testTfPyDllLinkImplementationBad.cpp)

    set_target_properties(testTfPyDllLinkImplementationBad
        PROPERTIES
            OUTPUT_NAME "testTfPyDllLinkImplementation"
            RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/ImplementationBad"
            ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/ImplementationBad")

    # Create the _testTfPyDllLinkModule.pyd compiled python module

    add_library(testTfPyDllLinkModule SHARED
        testTfPyDllLinkModule.c)

    set_target_properties(testTfPyDllLinkModule
        PROPERTIES
            PREFIX ""
            SUFFIX ".pyd"
            OUTPUT_NAME "_testTfPyDllLinkModule"
            RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/testenv/testTfPyDllLinkModule"
            LIBRARY_OUTPUT_DIRECTORY "$<CONFIG>/testenv/testTfPyDllLinkModule"
            ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/testenv/testTfPyDllLinkModule")

    target_include_directories(testTfPyDllLinkModule
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

    target_link_directories(testTfPyDllLinkModule
        PRIVATE
            $<TARGET_FILE_DIR:testTfPyDllLinkImplementationGood>)

    target_link_libraries(testTfPyDllLinkModule
        PRIVATE
            testTfPyDllLinkImplementation
            Python::Python)

    add_custom_command(
        TARGET testTfPyDllLinkModule POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/TfPyDllLinkTestEnv/testTfPyDllLinkModule/__init__.py"
            "$<TARGET_FILE_DIR:testTfPyDllLinkModule>/"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<CONFIG>/ImplementationGood"
            "$<TARGET_FILE_DIR:testTfPyDllLinkModule>/ImplementationGood"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<CONFIG>/ImplementationBad"
            "$<TARGET_FILE_DIR:testTfPyDllLinkModule>/ImplementationBad"
    )

    add_dependencies(testTfPyDllLinkModule testTfPyDllLinkImplementationGood)

    # Create test

    add_test(NAME testTfPyDllLink
        COMMAND ${testPyWrapper}
        "${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testTfPyDllLink.py"
        "--testenv-dir=$<TARGET_FILE_DIR:testTfPyDllLinkModule>/..")
endif()