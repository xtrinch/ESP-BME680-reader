Import("env")

env.Append(CPPDEFINES=[
  ("SLEEP_SECONDS", 300), # how long the board will sleep between measurements in seconds
  ("WIFI_SSID", "YOUR-WIFI-SSID"),
	("WIFI_PASSWORD", "YOUR-WIFI-PASSWORD"),
	("SENSOR_ACCESS_TOKEN", "your sensor access token"), # sensor access token you get from the web dashboard
  ("SENSOR_DASHBOARD_URL", "sensor-dashboard.trina.si"),
  ("ONBOARD_WIFI_SSID", "ONBOARD-WIFI-SSID"), # onboard AP SSID during configuration
  ("ONBOARD_WIFI_PASSWORD", "ONBOARD-WIFI-PASSWORD") # onboard AP PASSWORD during configuration
])