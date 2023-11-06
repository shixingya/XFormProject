#ifndef CONSTANTS_CODES_H
#define CONSTANTS_CODES_H
namespace ErrorCodes {
// 网络相关错误码（以1开头）
enum class NetworkErrors {
    CONNECTION_FAILED = 1001,
    TIMEOUT,
    INVALID_URL,
    // 其他网络错误码
};

// 存储相关错误码（以2开头）
enum class StorageErrors {
    FILE_NOT_FOUND = 2001,
    PERMISSION_DENIED,
    CORRUPTED_DATA,
    // 其他存储错误码
};

// 服务相关错误码（以3开头）
enum class ServiceErrors {
    UNAVAILABLE = 3001,
    INVALID_RESPONSE,
    SERVICE_TIMEOUT,
    // 其他服务错误码
};

// 接口相关错误码（以4开头）
enum class InterfaceErrors {
    INVALID_INPUT = 4001,
    NULL_POINTER,
    INVALID_ARGUMENT,
    // 其他接口错误码
};

//用户界面错误码（以5开头）
enum class UIErrors {
    INVALID_INPUT = 5001,
    WINDOW_NOT_FOUND,
    UI_COMPONENT_FAILURE,
    // 其他用户界面错误码
};

//加密和安全错误码（以6开头）
enum class SecurityErrors {
    ENCRYPTION_FAILURE = 6001,
    AUTHENTICATION_FAILED,
    SECURITY_BREACH,
    // 其他安全错误码
};

//数据库错误码（以7开头）
enum class DatabaseErrors {
    QUERY_FAILED = 7001,
    CONSTRAINT_VIOLATION,
    DATABASE_UNAVAILABLE,
    // 其他数据库错误码
};

//文件系统错误码（以8开头）
enum class FilesystemErrors {
    FILE_IO_ERROR = 8001,
    DIRECTORY_NOT_FOUND,
    FILE_LOCK_ERROR,
    // 其他文件系统错误码
};

//线程和并发错误码（以9开头）
enum class ConcurrencyErrors {
    THREAD_CREATION_FAILED = 9001,
    DEADLOCK_DETECTED,
    RESOURCE_CONFLICT,
    // 其他线程和并发错误码
};

//硬件和设备错误码（以10开头）
enum class HardwareErrors {
    DEVICE_NOT_RESPONDING = 10001,
    HARDWARE_FAILURE,
    INVALID_CONFIGURATION,
    // 其他硬件和设备错误码
};

//日志和记录错误码（以11开头）
enum class LogErrors {
    LOG_WRITE_FAILED = 11001,
    LOG_FILE_NOT_FOUND,
    LOG_CORRUPTION,
    // 其他日志和记录错误码
};

//权限和授权错误码（以12开头）
enum class PermissionErrors {
    ACCESS_DENIED = 12001,
    INSUFFICIENT_PRIVILEGES,
    PERMISSION_REVOKED,
    // 其他权限和授权错误码
};

//时间和日期错误码（以13开头）
enum class DateTimeErrors {
    INVALID_DATE = 13001,
    TIMEZONE_ERROR,
    DATE_CONVERSION_FAILED,
    // 其他时间和日期错误码
};

//内存错误码（以14开头）
enum class MemoryErrors {
    ALLOCATION_FAILED = 14001,
    MEMORY_LEAK_DETECTED,
    INVALID_POINTER,
    // 其他内存错误码
};

//算法和数学错误码（以15开头）
enum class MathErrors {
    DIVISION_BY_ZERO = 15001,
    INVALID_OPERATION,
    MATHEMATICAL_OVERFLOW,
    // 其他算法和数学错误码
};

//配置和设置错误码（以16开头）
enum class ConfigurationErrors {
    CONFIG_FILE_NOT_FOUND = 16001,
    INVALID_CONFIGURATION_SETTING,
    CONFIG_LOAD_FAILURE,
    // 其他配置和设置错误码
};

//输入输出错误码（以17开头）
enum class IOError {
    INPUT_ERROR = 17001,
    OUTPUT_ERROR,
    FILE_FORMAT_ERROR,
    // 其他输入输出错误码
};

//消息和通知错误码（以18开头）
enum class MessageErrors {
    MESSAGE_SEND_FAILED = 18001,
    MESSAGE_FORMAT_ERROR,
    MESSAGE_TIMEOUT,
    // 其他消息和通知错误码
};

//日历和事件错误码（以19开头）
enum class CalendarErrors {
    EVENT_NOT_FOUND = 19001,
    CALENDAR_SYNC_ERROR,
    INVALID_EVENT,
    // 其他日历和事件错误码
};
}

#endif // CONSTANTS_CODES_H
