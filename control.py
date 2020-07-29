import cv2
#import pyttsx3
import time
import playsound
import serial

#robot_mouth = pyttsx3.init()
eye_cascPath = r'..\Makerthon_Mori\haarcascade_eye_tree_eyeglasses.xml'  #eye detect model
face_cascPath = r'..\Makerthon_Mori\haarcascade_frontalface_alt.xml'  #face detect model
faceCascade = cv2.CascadeClassifier(face_cascPath)
eyeCascade = cv2.CascadeClassifier(eye_cascPath)
cap = cv2.VideoCapture(0)
countClosedEyes = 0
start = time.time()
end = 0
arduinoData = serial.Serial('COM3', 9600)

arduinoData.write(b'1')
playsound.playsound('welcome.mp3')
arduinoData.write(b'1')

while 1:
    ret, img = cap.read()
    
    if ret:
        frame = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Detect faces in the image
        faces = faceCascade.detectMultiScale(
            frame,
            scaleFactor = 1.2,
            minNeighbors = 5,
            minSize=(30, 30),
            flags = cv2.CASCADE_SCALE_IMAGE
        )

        if len(faces) > 0:

            # Detect eyes in the image
            eyes = eyeCascade.detectMultiScale(
                frame,
                scaleFactor = 1.2,
                minNeighbors = 5,
                minSize = (30, 30),
                flags = cv2.CASCADE_SCALE_IMAGE
            )
            if len(eyes) == 0:  #Can't detect the eyes

                start = time.time()
                # Draw a red rectangle around the faces
                for (x, y, w, h) in faces:
                    cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)

                if countClosedEyes < 4: #Normal
                    arduinoData.write(b'1')
                    countClosedEyes += 1
                    start = time.time()
                    time.sleep(1)
                elif countClosedEyes == 4: #Level 1
                    playsound.playsound('level1.mp3')
                    time.sleep(1)
                    playsound.playsound('music.mp3')

                    start = time.time()
                    countClosedEyes += 1
                    time.sleep(1)
                elif countClosedEyes == 8: #Level 2
                    playsound.playsound('level2.mp3')
                    arduinoData.write(b'2')
                    time.sleep(8)
                    start = time.time()
                    countClosedEyes += 1
                    time.sleep(1)
                elif countClosedEyes >= 12: #Level 3
                    playsound.playsound('level3.mp3')
                    arduinoData.write(b'3')
                    time.sleep(8)
                    start = time.time()
                    countClosedEyes += 1
                    time.sleep(1)
                else:
                    countClosedEyes += 1
                    time.sleep(1)

            else:
                # Draw a green rectangle around the faces
                for (x, y, w, h) in faces:
                    cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

                end = time.time()
                if (end - start) > 30:
                    countClosedEyes = 0
            #frame_tmp = cv2.resize(frame_tmp, (400, 400), interpolation=cv2.INTER_LINEAR)
            cv2.imshow('Face Recognition', img)
            print("You close eyes %d times" % countClosedEyes)
        waitkey = cv2.waitKey(1)
        if waitkey == ord('q') or waitkey == ord('Q'):
            cv2.destroyAllWindows()  
            break
