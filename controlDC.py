import cv2
import hand as htm
import math
from cvzone.SerialModule import SerialObject

arduino = SerialObject("COM8")

cap = cv2.VideoCapture(0)
detector = htm.handDetector(detectionCon=1)

while True:
    ret, frame = cap.read()
    frame = detector.findHands(frame)
    lmList = detector.findPosition(frame, draw=False)

    if len(lmList) != 0:
        x1, y1 = lmList[4][1], lmList[4][2]
        x2, y2 = lmList[8][1], lmList[8][2]

        cv2.circle(frame, (x1,y1), 5, (255, 255, 255), -1)
        cv2.circle(frame, (x2,y2), 5, (255, 255, 255), -1)
        cv2.line(frame,(x1,y1),(x2,y2),(255,255,255),3)

        length = int(math.hypot(x2-x1,y2-y1))
        if length < 20:
            length = 20
        if length > 120:
            length = 120
        arduino.sendData([length])
    cv2.imshow("Nguyen Duy Tuan Nguyen", frame)
    if cv2.waitKey(1) == ord(" "):
        break
cap.release()
cv2.destroyAllWindows()
