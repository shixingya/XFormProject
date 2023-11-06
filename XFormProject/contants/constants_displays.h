#ifndef CONSTANTS_DISPLAYS_H
#define CONSTANTS_DISPLAYS_H
#include <QString>

namespace DisplayConstants {
const QString APP_NAME = "示例程序";
const QString APP_VERSION = "1.0.0";
const QString DEVELOPER_NAME = "雕爷优选";
const QString DEVELOPER_EMAIL = "shixingya@126.com";
const QString PROJECT_PATH = "/path/to/your/project";
const QString DEFAULT_FONT_FAMILY = "Arial";
const int DEFAULT_FONT_SIZE = 12;

// 系统状态
const QString SYSTEM_STATUS_CONNECTED = "正常";
const QString SYSTEM_STATUS_DISCONNECTED = "断开连接";
const QString SYSTEM_STATUS_ERROR = "警告";

// 文件路径
const QString DEFAULT_SAVE_PATH = "/path/to/save";
const QString LOG_FILE_PATH = "/path/to/log/file.txt";
// ...其他用户可见的常量
}
#endif // CONSTANTS_DISPLAYS_H
