#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <WiFi.h>
#include "credentials.h"

void initWiFi();
bool isWiFiConnected();

#endif