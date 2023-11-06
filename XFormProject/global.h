#ifndef GLOBAL_H
#define GLOBAL_H
//避免悬空指针问题
#define SAFE_DELETE(p) do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p) do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#include <QtWidgets>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "contants/constants.h"

#endif // GLOBAL_H
