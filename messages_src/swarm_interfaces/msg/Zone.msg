# This message is used to define a zone. 
# A zone is a 3D volume which is defined by its shape, its altitude and its limit in time.
# The zone is defined by a list of GeoPoints and a list of RelativePoints.
# The zone is considered active between the start_time and the end_time.

int64 alt_max
int64 alt_min
int64 end_time
GeoPoint[] geo_points
RelativePoint[] relative_points
int64 start_time
ZoneShapeEnum zone_shape
ZoneTypeEnum zone_type