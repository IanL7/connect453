from PIL import Image
from math import sqrt
import colorsys
from picamera2 import Picamera2, Preview
from libcamera import Transform
import serial
from time import sleep
from math import floor

# center = [
#     [460, 529],[550, 527],[639, 526],[725, 525],[822, 518],[917, 517],[1014, 516],
#     [465, 451],[551, 451],[641, 447],[730, 443],[819, 439],[916, 435],[1013, 431],
#     [465, 380],[548, 376],[635, 371],[724, 368],[817, 366],[910, 359],[1004, 353],
#     [467, 305],[548, 299],[644, 298],[723, 294],[815, 289],[907, 283],[995, 278],
#     [470, 236],[550, 231],[635, 228],[722, 220],[806, 215],[897, 208],[992, 207],
#     [470, 166],[554, 161],[635, 153],[721, 150],[806, 146],[896, 138],[988, 133]
#     ]
    
    
center = [
#left column                                                                   right column
[1323, 1896],[1780, 1914],[2222, 1951],[2682, 1969],[3121, 1981],[3603, 2024],[4078, 2057], #bottom row
[1390, 1524],[1832, 1548],[2249, 1582],[2688, 1603],[3124, 1621],[3560, 1646],[4017, 1685], #2nd row
[1451, 1179],[1865, 1210],[2289, 1225],[2679, 1268],[3091, 1280],[3536, 1307],[3981, 1323], #3rd row
[1496, 877],[1920, 905],[2295, 941],[2694, 945],[3103, 978],[3463, 990],[3874, 1018],       #4th row
[1560, 618],[1960, 640],[2316, 673],[2694, 682],[3085, 704],[3460, 704],[3835, 725],        #5th row
[1624, 362],[1975, 378],[2338, 399],[2697, 417],[3060, 438],[3429, 448],[3798, 460]         #top row
]
radius = 7
colorArray = [[0] for _ in range(42)]
turnCount = 0

#Turn on camera
picam2 = Picamera2()
config = picam2.create_still_configuration(transform = Transform(vflip = True, hflip = True))
picam2.configure(config)
picam2.start()
photoCount = 0
def takePhoto():
    global picam2
    global photoCount
    picam2.capture_file(f"image_{photoCount}.jpg")
    image = Image.open(f"image_{photoCount}.jpg")
    #image = Image.open("IMG_1814.JPG")
    photoCount = (photoCount+1)%10
    global colorArray
    global turnCount
    for count in range(42):
        redCount = 0
        blueCount = 0
        greenCount = 0
        size = 4 * radius * radius
        centerX, centerY = center[count]
        for x in range(centerX - radius, centerX + radius):
            for y in range(centerY - radius, centerY + radius):
                #print(x,y)
                pixel = image.getpixel((x,y))
                redCount += pixel[0]
                greenCount += pixel[1]
                blueCount += pixel[2]
        averageRed = redCount / size
        averageGreen = greenCount / size
        averageBlue = blueCount / size

        averageHSV = colorsys.rgb_to_hsv(averageRed/255, averageGreen/255, averageBlue/255)
        angle = averageHSV[0] * 360
        saturation = averageHSV[1] * 100
        value = averageHSV[2] * 100
        if (angle>35) & (angle<120):
            colorArray[count] = 2
            #print("Yellow token at ", count)
        elif (angle>200) & (angle < 300):
            colorArray[count] = 1
            #print("Blue token at ", count)
        else:
            colorArray[count] = 0
    checkValidPiece()

def checkValidPiece():
    global colorArray
    columnZero = reduction([colorArray[0], colorArray[7], colorArray[14], colorArray[21], colorArray[28], colorArray[35]])
    columnOne = reduction([colorArray[1], colorArray[8], colorArray[15], colorArray[22], colorArray[29], colorArray[36]])
    columnTwo = reduction([colorArray[2], colorArray[9], colorArray[16], colorArray[23], colorArray[30], colorArray[37]])
    columnThree = reduction([colorArray[3], colorArray[10], colorArray[17], colorArray[24], colorArray[31], colorArray[38]])
    columnFour = reduction([colorArray[4], colorArray[11], colorArray[18], colorArray[25], colorArray[32], colorArray[39]])
    columnFive = reduction([colorArray[5], colorArray[12], colorArray[19], colorArray[26], colorArray[33], colorArray[40]])
    columnSix = reduction([colorArray[6], colorArray[13], colorArray[20], colorArray[27], colorArray[34], colorArray[41]])

    for i in range(42):
        if(i%7 == 0):
            colorArray[i] = columnZero[floor(i/7)]
        elif(i%7 == 1):
            colorArray[i] = columnOne[floor(i/7)]
        elif(i%7 == 2):
            colorArray[i] = columnTwo[floor(i/7)]
        elif(i%7 == 3):
            colorArray[i] = columnThree[floor(i/7)]
        elif(i%7 == 4):
            colorArray[i] = columnFour[floor(i/7)]
        elif(i%7 == 5):
            colorArray[i] = columnFive[floor(i/7)]
        elif(i%7 == 6):
            colorArray[i] = columnSix[floor(i/7)]

def reduction(col):
    newArray = [0,0,0,0,0,0]
    count = 0
    for i in col:
        if i!=0:
            newArray[count] = i
            count+=1
    return newArray

def controlLoop():
    # Configure the serial port
    ser = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=0, bytesize=serial.EIGHTBITS)
    global colorArray
    rxData = ""
    takePhoto()
    
    while True:
        takePhoto()
        
         # Convert each integer to a string
        str_arr = [str(num) for num in colorArray]
        # Join the strings to form a single string
        result = ''.join(str_arr)
        
        print("Sending: ", result)
        
        ser.write((f"{result}\n").encode())
        dataReceived = False
        result = ""
        
        sleep(.5)

controlLoop()
            
