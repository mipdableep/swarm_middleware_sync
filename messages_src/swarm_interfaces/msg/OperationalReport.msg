# The following is a ROS2 Message file for an OperationalReport. 
# The message includes fields for failID, spraying, findTarget, targets, and tracking.

int64 fail_id
bool spraying
bool found_target
Target[] targets
bool tracking