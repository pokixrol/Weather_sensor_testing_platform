#include <ArduinoJson.h>

// Initializes lightning sensor
void lightning_init();
// Checks if lightning data is available
bool lightning_available();
// Reads lightning data and stores it into a JSON
void lightning_read(JsonDocument &doc);
