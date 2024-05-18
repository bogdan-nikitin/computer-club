# Тестовое задание YADRO IMPULSE 2024

Сборка производится с использованием CMake.
Решение включает в себя тесты на GTest.

Тестовый пример находится в папке test_files.

## Сборка

Пример сборки и запуска решения:
```bash
cmake --preset Release -DTREAT_WARNINGS_AS_ERRORS=ON -Dtest=ON -S .
cmake --build cmake-build-SanitizedDebug
./cmake-build-SanitizedDebug/main test_files/example.txt
```

Пример сборки и запуска тестов с использованием vcpkg (вместо `$YOUR_VCPKG_PATH` нужно указать путь до vcpkg):
```bash
cmake "-DCMAKE_TOOLCHAIN_FILE=$YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake" --preset SanitizedDebug -DTREAT_WARNINGS_AS_ERRORS=ON -Dtest=ON -S .
cmake --build cmake-build-SanitizedDebug
./cmake-build-SanitizedDebug/tests
```
