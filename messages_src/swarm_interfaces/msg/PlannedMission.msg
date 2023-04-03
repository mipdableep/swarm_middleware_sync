# Planned mission
# This message is sent to the vehicle to start a mission
# The mission is defined by a list of waypoints and a list of zones
# The vehicle will fly to each waypoint in order and then loiter in each zone
# The vehicle will exit the mission when it has reached the last waypoint and loitered in the last zone

Drone[] drones
int64 flight_altitude
GeoPoint initial_location
MissionTypeEnum mission_type
MissionTargetData[] target_data
Zone[] zones
