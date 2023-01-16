include(CTest)

add_executable(CmdArgumentTester test_CmdArgumentTester.cpp)
target_link_libraries(CmdArgumentTester HDV_SRC)

add_test(NAME CmdArgumentTesterBuild COMMAND $<TARGET_FILE:CmdArgumentTester>)
add_test(NAME SingleOption COMMAND $<TARGET_FILE:CmdArgumentTester> "--example_valid")
add_test(NAME InvalidOption COMMAND $<TARGET_FILE:CmdArgumentTester> "--example_invalid")
add_test(NAME SingleOptionWithArguments COMMAND $<TARGET_FILE:CmdArgumentTester> "--example_multiple" "arg1" "arg2")

add_executable(HDVApplicationTester test_HDVApplicationTester.cpp)
target_link_libraries(HDVApplicationTester HDV_SRC)

add_test(NAME HDVApplicationTesterBuild COMMAND $<TARGET_FILE:HDVApplicationTester>)
add_test(NAME OutputDirectory COMMAND $<TARGET_FILE:HDVApplicationTester> "--output-directory" "ExampleDir/Sub1/Sub2/")
add_test(NAME OutputFile COMMAND $<TARGET_FILE:HDVApplicationTester> "--output-name" "ExampleOutput")
add_test(NAME Dimensions COMMAND $<TARGET_FILE:HDVApplicationTester> "--dimensions" "4")
add_test(NAME ParticleCount COMMAND $<TARGET_FILE:HDVApplicationTester> "--particle-count" "200")
add_test(NAME SearchRadius COMMAND $<TARGET_FILE:HDVApplicationTester> "--search-radius" "5")
add_test(NAME Speed COMMAND $<TARGET_FILE:HDVApplicationTester> "--speed" "6")
add_test(NAME RandomRotation COMMAND $<TARGET_FILE:HDVApplicationTester> "--random-rotation" "3")
add_test(NAME RandomPeriod COMMAND $<TARGET_FILE:HDVApplicationTester> "--random-period" "11")
add_test(NAME RecordPeriod COMMAND $<TARGET_FILE:HDVApplicationTester> "--record-period" "13")
add_test(NAME Config COMMAND $<TARGET_FILE:HDVApplicationTester> "--config" "TestConfig" WORKING_DIRECTORY "./")
