# Using of OpenCL - Findings patterns #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1.  [Установка]()
2.  [Интерфейс]()

Установка
=========
* Скачать содержимое репозитория в удобную Вам папку на диске
>git clone https://github.com/MarkGoncharovAl/ILab
* Установить OpenCL. Она использовалась при написании кода. 
Здесь может быть множество ошибок, как минимум установаите пакеты ниже
>sudo apt install opencl-c-headers opencl-clhpp-headers
* Для запуска тестов необходимо доставить Google tests
>sudo apt install libgtest-dev
* Если не установлен cmake:
>sudo apt install cmake
* Сoздать и войти в папку, в которой будет собрана игра
>Открывем терминал в папке ILab/OpenCL

>mkdir build

>cd build
* Собрать cmake проект (а именно main, e2e тесты, тесты функций tests) 
>cmake -DMAIN=ON

>make
* Запускаем проект. При запуске main обратите внимание на аргументы командной строки
Запуск ниже подскажет, что делать. 
>./main --help

Интерфейс
=========
@size_base@ @basis string@

@size tests@

@size of test string@ @test string@
-//-