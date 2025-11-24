# Jar To exe

простой инструмент для упаковки jar в динамическую библиотеку и динамическую библиотеку в исполняемый файл

## возможности


- **JAR → DLL** - преобразование Java архивов в динамические библиотеки
- **DLL → EXE** - упаковка DLL в самостоятельные исполняемые файлы
- **Dev Interface** - dev interface, за счет работы через аргументы и файл конфигурации

## требование:
- **Компилятор C++**: `g++` или `clang++` (рекомендуется через [MSYS2](https://www.msys2.org/))
- **Windows SDK**

## Использование:
### 1. Установка
```bash
# Скачайте и распакуйте готовый релиз
jar-to-dll-x64-windows.zip
```
### 2 Настройка конфигурации
```bash
# Основные параметры
input=my_application.jar
output=my_program.dll
class=com/company/Main
method=main

# Дополнительные настройки, log_dir нужно создать самому
log_dir=logs

# если у вас нету в Environment path компилятора g++, 
# то укажите путь до него вручную
compiler=path/to/g++.exe 
```
### 3. Запуск конвертации
```bash
# Конвертация JAR в DLL
JarToDll.exe dll config=config.txt

# Создание EXE из DLL  
JarToDll.exe exe config=config.txt

# Режим отладки
JarToDll.exe dll config=config.txt --debug
```
на выходе вы получите my_program.dll из output

## Сборка
- ```bash git clone https://github.com/megantcs/Jar-To-Exe.git ```
- ```bash mkdir build```
- ```bash cd build```
- ```bash cmake ..```
- ```bash cmake --build .```
