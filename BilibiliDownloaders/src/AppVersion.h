#pragma once
#pragma execution_character_set("utf-8")

#define _STR(A) #A
#define STR(A) _STR(A)

#define APP_MAJOR 1
#define APP_MINOR 0
#define APP_PATCH 0

#define GIT_HASH   1

#define APP_VERSION 10000*APP_MAJOR + 100*APP_MINOR + APP_PATCH
#define APP_VERSION_STR APP_MAJOR.#APP_MINOR#.APP_PATCH
#define APP_VERSION_STR_WITH_GIT APP_VERSION_STR.#GIT_HASH

#define APP_BUILD_YEAR       2022
#define APP_BUILD_MONTH      05
#define APP_BUILD_DAY        08

#define APP_BUILD_DATE APP_BUILD_YEAR.#APP_BUILD_MONTH#.APP_BUILD_DAY

#define APP_BUILD_HOUR       10
#define APP_BUILD_MINTUE     18
#define APP_BUILD_SECOND     56

#define APP_BUILD_TIME APP_BUILD_HOUR.#APP_BUILD_MINTUE#.APP_BUILD_SECOND

#define APP_BUILD_STR APP_BUILD_DATE.#APP_BUILD_TIME