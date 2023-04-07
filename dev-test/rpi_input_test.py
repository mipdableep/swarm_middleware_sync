import cv2
from time import sleep
cap = cv2.VideoCapture()
cap.open(0)
save_dir = './vid'

for i in range(1):
    save_str = f"{save_dir}/{str(i)}.jpg"
    cv2.imwrite(save_str, cap.read()[1])
    sleep(0.5)

cap.release()