Import("env")

env.Append(CPPDEFINES=[
  ("WIFI_SSID", "YOUR WIFI SSID"),
	("WIFI_PASSWORD", "YOUR WIFI PASSWORD"),
	("SENSOR_ACCESS_TOKEN", "your sensor access token"),
  ("SENSOR_DASHBOARD_URL", "sensor-dashboard.trina.si")
])