import djitellopy
from time import sleep
from VCS import VideoCapture
import cv2

t1 = djitellopy.Tello("10.3.141.101")

t1.connect(False)
sleep(2)
t1.streamon()
t1.set_network_ports(8890, 11112)
webcam = VideoCapture("udp://10.3.141.67:11112")

counter = 0
for i in range(10):
    
    ret, frame = webcam.read()
    
    cv2.imwrite('./vid/' + str(counter) + '.jpg', frame)