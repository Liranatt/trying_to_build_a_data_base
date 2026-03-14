cmake_minimum_required(VERSION 3.10)
project(KVStore)

# מחייבים שימוש בתקן C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# מחפשים את ספריית ה-Threads של מערכת ההפעלה
find_package(Threads REQUIRED)

# מגדירים את קובץ ההרצה שלנו ואת הקבצים שמרכיבים אותו
add_executable(KVStore main.cpp KVStore.cpp
        Server.cpp
        Server.h)

# מקשרים את ספריית ה-Threads לקובץ ההרצה
target_link_libraries(KVStore Threads::Threads)