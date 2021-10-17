Для выполнения задачи использовались features2d.hpp и xfeatures2d.hpp, которые входят в contrib OpenCV.
Что бы они были необходимо скачать:

1. https://github.com/opencv/opencv/archive/refs/tags/4.5.4.zip
2. https://github.com/opencv/opencv_contrib
3. CMake

Далее нужно сделать:

1. Открыть в GUI CMake папку OpenCV
2. Сделать конфигурацию под х64
3. Изменить конфиг, что бы поле OPENCV_EXTRA_MODULES_PATH = {папка contrib}/modules
4. Поставить OPENCV_ENABLE_NONFREE = true, BUILD_opencv_world = true
5. По желанию отключить BUILD_opencv_python2, BUILD_opencv_python3, BUILD_opencv_java
6. Добавить в конфиг string OpenCV_RUNTIME = vc16
7. Нажать "Configure" и "Generate" 
8. Собрать релиз или дебаг:
   cmake.exe --build . --config Debug --target INSTALL
   cmake.exe --build . --config Release --target INSTALL
8. Добавить {папка OpenCV}/build/install/x64/vc16/bin в PATH

В проекте Visual Studio установлено:

Project -> Properties -> Configuration Properties -> VC++ Directories -> Include Directories добавить {папка OpenCV}/build/install/include

Project -> Properties -> Configuration Properties -> VC++ Directories -> Library Directories добавить {папка OpenCV}/build/install/x64/vc16/lib

Project -> Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies добавить opencv_world454d.dll
