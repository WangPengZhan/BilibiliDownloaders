#pragma once

#define _STR(A) #A
#define STR(A) _STR(A)

#define ARIA_MAJOR 1
#define ARIA_MINOR 0
#define ARIA_PATCH 0

#define GIT_HASH   1

#define ARIA_VERSION 10000*ARIA_MAJOR + 100*ARIA_MINOR + ARIA_PATCH
#define ARIA_VERSION_STR ARIA_MAJOR.#ARIA_MINOR#.ARIA_PATCH
#define ARIA_VERSION_STR_WITH_GIT ARIA_VERSION_STR.#GIT_HASH

#define ARIA_ARIA_BUILD_YEAR
#define ARIA_ARIA_BUILD_MONTH
#define ARIA_BUILD_DAY

#define ARIA_BUILD_DATE ARIA_BUILD_YEAR.#ARIA_BUILD_MONTH#.ARIA_BUILD_DAY

#define ARIA_BUILD_HOUR
#define ARIA_BUILD_MINTUE
#define ARIA_BUILD_SECOND

#define ARIA_BUILD_TIME ARIA_BUILD_HOUR.#ARIA_BUILD_MINTUE#.ARIA_BUILD_SECOND

#define ARIA_BUILD_STR ARIA_BUILD_DATE.#ARIA_BUILD_TIME
