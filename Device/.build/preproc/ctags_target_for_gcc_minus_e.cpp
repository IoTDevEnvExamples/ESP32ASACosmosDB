# 1 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
# 1 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
/**
 * A simple Azure IoT example for sending telemetry to Iot Hub.
 */

# 6 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 2
# 7 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 2

#define INTERVAL 10000
#define DEVICE_ID "Esp32Device"
#define MESSAGE_MAX_LEN 256
// Please input the SSID and password of WiFi
const char* ssid = "aaaa";
const char* password = "chenyiwen123";

/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=t-yiwche-test2-iothub.azure-devices.net;DeviceId=device1;SharedAccessKey=w3gnxvu17gZJaAyuJhB2y2ZIWp1VjDnvtVtuzeZd6mA=";
const char *messageData = "{\"deviceId\":\"%s\", \"messageId\":%d, \"Temperature\":%f, \"Humidity\":%f}";
static bool hasIoTHub = false;
static bool hasWifi = false;
int messageCount = 1;
static bool messageSending = true;
static uint64_t send_interval_ms;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  Serial.println("Message callback:");
  Serial.println(payLoad);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == 
# 43 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 3 4
             __null
# 43 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
                 )
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  // Display Twin message.
  Serial.println(temp);
  free(temp);
}

static int DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 56 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 3 4
 __null
# 56 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
 ) l(AZ_LOG_INFO, "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino", __func__, 56, 0x01, "Try to invoke method %s", methodName); }; }while((void)0,0);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 62 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 3 4
   __null
# 62 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino", __func__, 62, 0x01, "Start sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 67 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 3 4
   __null
# 67 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino", __func__, 67, 0x01, "Stop sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = false;
  }
  else
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 72 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino" 3 4
   __null
# 72 "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-yiwche\\vscode-iot-workbench\\test\\asa-test1\\Device\\device.ino", __func__, 72, 0x01, "No method %s found", methodName); }; }while((void)0,0);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}



void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Device");
  Serial.println("Initializing...");
  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");

  delay(10);
  WiFi.mode(WIFI_MODE_AP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    hasWifi = false;
  }
  hasWifi = true;

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" > IoT Hub");
  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString, true))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  Serial.println("Start sending events.");
  randomSeed(analogRead(0));
  send_interval_ms = millis();

}

void loop() {
if (hasWifi && hasIoTHub)
  {
    if (messageSending &&
        (int)(millis() - send_interval_ms) >= 10000)
    {
      // Send teperature data
      char messagePayload[256];
      float temperature = (float)random(0,500)/10;
      float humidity = (float)random(0, 1000)/10;
      snprintf(messagePayload,256, messageData, "Esp32Device", messageCount++, temperature,humidity);
      Serial.println(messagePayload);
      EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
      Esp32MQTTClient_SendEventInstance(message);
      send_interval_ms = millis();
    }
    else
    {
      Esp32MQTTClient_Check();
    }
  }
  delay(10);
}
