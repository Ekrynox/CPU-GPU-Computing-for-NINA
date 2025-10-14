#pragma once

#ifdef CGPUNINA_EXPORTS
#define CGPUNINA_API __declspec(dllexport)
#else
#define CGPUNINA_API __declspec(dllimport)
#endif