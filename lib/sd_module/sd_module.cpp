#include "sd_module.h"

File sdFile;

void initSD()
{
  if (!SD.begin(5))
  {
    Serial.println("Erro ao inicializar cartão SD!");
    while (1)
      ;
  }
}

String generateLogFileName()
{
  DateTime now = getCurrentTime();
  char fileName[32];
  snprintf(fileName, sizeof(fileName), "/log_%04d-%02d-%02d.txt",
           now.year(), now.month(), now.day());
  return String(fileName);
}

String generateDataFileName()
{
  DateTime now = getCurrentTime();
  char fileName[32];
  snprintf(fileName, sizeof(fileName), "/data_%04d-%02d-%02d.txt",
           now.year(), now.month(), now.day());
  return String(fileName);
}

void logSerialSD(const char *format, ...)
{
  static char logMessage[256];
  va_list args;
  va_start(args, format);
  vsnprintf(logMessage, sizeof(logMessage), format, args);
  va_end(args);

  DateTime now = getCurrentTime();
  unsigned long milliseconds = millis() % 1000;
  char timestamp[32];
  snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03lu",
           now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), milliseconds);

  String logFileName = generateLogFileName();
  sdFile = SD.open(logFileName.c_str(), FILE_APPEND);
  if (sdFile)
  {
    sdFile.printf("[%s] %s\n", timestamp, logMessage);
    sdFile.close();
  }
  else
  {
    Serial.println("Erro ao abrir arquivo de log no cartão SD!");
  }

  Serial.printf("[%s] %s\n", timestamp, logMessage);
}

bool writeDataToSD(int16_t solar_irradiance)
{
  DateTime now = getCurrentTime();
  static char dataLine[64];
  snprintf(dataLine, sizeof(dataLine), "%02d-%02d-%04d %02d:%02d:%02d, %d",
           now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second(), solar_irradiance);

  String dataFileName = generateDataFileName();
  sdFile = SD.open(dataFileName.c_str(), FILE_APPEND);
  if (!sdFile)
  {
    return false;
  }

  sdFile.println(dataLine);
  sdFile.close();
  return true;
}