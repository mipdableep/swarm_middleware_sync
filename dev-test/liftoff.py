import djitellopy
from time import sleep


t1 = djitellopy.Tello("192.168.87.44")

t1.connect(False)

sleep(2)

t1.takeoff()

t1.send_rc_control(0, 0, 0, 0)

t1.move_up(100)
t1.send_rc_control(0, 0, 0, 0)
sleep(1)
t1.send_rc_control(0, 0, -30, 300)
sleep(5)
t1.land()