#pragma once

#ifdef NINAPP_EXPORTS
#define NINAPP_API __declspec(dllexport)
#else
#define NINAPP_API __declspec(dllimport)
#endif



NINAPP_API void test();