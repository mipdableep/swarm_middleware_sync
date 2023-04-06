import djitellopy
from time import sleep
from VCS import VideoCapture
import cv2

t1 = djitellopy.Tello("192.168.43.120")

t1.connect(False)
sleep(2)
t1.streamon()
t1.set_network_ports(8890, 11112)
webcam = VideoCapture("udp://192.168.43.120:11112")

sleep(2)

counter = 0
for i in range(10):
    
    ret, frame = webcam.read()
    
    cv2.imwrite('./vid/' + str(i) + '.jpg', frame)
    sleep(0.5)