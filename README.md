# exactoSim
 Simulator with Bullet and UE4:

-- For building objects using constraints of Bullet or another physics engine

# Build

Build Bullet from https://github.com/bulletphysics/bullet3

Build Instructions

-- Run the CMake UI

--It doesn’t matter where you choose for “Where to build the binaries”

Check the USE_MSVC_RUNTIME_LIBRARY_DLL option
Check the USE_MSVC_RELEASE_RUNTIME_ALWAYS option
Pick a location for LIBRARY_OUTPUT_PATH that’s inside your UE project
e.g. UEProject/ThirdParty/lib

--Press the Configure button

--Press Generate

--Press Open Project

In VS, Build > Batch Build
Check App_HelloWorld in Debug, Release and RelWithDbgInfo configs
Hit Build
Once done you should have static libraries in LIBRARY_OUTPUT_PATH/Debug|Release|RelWithDebInfo.

Specify path in Source/exactoSim/exactoSim.Build.cs:

Use your own ThirdPartyPath

More details ur can find in https://www.stevestreeting.com/2020/07/26/using-bullet-for-physics-in-ue4/

# Сборка

-- Скачайте bullet https://github.com/bulletphysics/bullet3

-- Запустите Cmake GUI

-- Укажите пункты:

USE_MSVC_RUNTIME_LIBRARY_DLL

USE_MSVC_RELEASE_RUNTIME_ALWAYS

Укажите LIBRARY_OUTPUT_PATH так как вам удобно 
 UEProject/ThirdParty/lib

-- Нажмите Конфигурировать

-- Нажмите Генерировать

-- Откройте проект в VS и соберите его со следующими опциями: Debug|Release|RelWithDebInfo

-- В файле Source/exactoSim/exactoSim.Build.cs укажите путь к сгенерированным библиотекам 

# Назначение

В физическом мире предметы постоянно взаимодействуют. В результате столкновений предметы преобретают или теряют линейную или угловую скорость. Все это сказывается на других предметах, и оно крайне интересуют людей: на вышеописанное устанавливается ярлык "события".


Поехала машина -- событие. Приземлился самолет -- событие.


Пытаясь научить созданные человеческим трудом датчики понимать, что такое "события", люди воспроизводят их в ограниченном объеме в контролируемых условиях и называют все это "экспериментом".


Эксперименты в реальной жизни крайне важны, однако потребуется бесконечное количество времени, чтобы организовать эксперименты, полно описывающие весь спектр взамодействий объектов. У обычных людей этого времени нет. 


И вот тут нам поможет виртуально моделирование! 

# Описание версии 0.1

Моделирование чувствительной плиты, на которую падают различные предметы. Плита "виртуально" оснащена акселерометрами и гироскопами. Это значит, что линейные ускорения и угловая скорость, которые она испытывает в определенной степени передается датчикам.
