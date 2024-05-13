from kivy.config import Config
Config.set('input', 'mouse', 'mouse,multitouch_on_demand')
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.properties import NumericProperty, ReferenceListProperty
from kivy.vector import Vector
from kivy.uix.gridlayout import GridLayout
from kivy.graphics import *
from kivy.properties import ObjectProperty
from kivy.clock import Clock
from kivy.logger import Logger, LOG_LEVELS
import kivy.logger
import threading
import os
os.environ["KIVY_NO_CONSOLELOG"] = "1"

turn = 0
column = 0
unseen = False

def handler():
    global unseen
    global column
    
    with lock:
        if(unseen):
            print("Placed a piece in column ", column)
            unseen = False
            return column
        else:
            return -1

yellow = [1,1,0,1]
blue = [0,0,1,1]
empty = [0,0,0,0]
lock = threading.Lock()
colorArr = [[0] * 4 for _ in range(42)]
stopThreads = False
class Piece(Widget):
    r = NumericProperty(0)
    g = NumericProperty(0)
    b = NumericProperty(0)
    a = NumericProperty(0)
    text = ObjectProperty("Placeholder Text")

headerText = "The game is starting"
c = 0
class GridWidget(Widget):
    piece00 = ObjectProperty(None)
    piece01 = ObjectProperty(None)
    piece02 = ObjectProperty(None)
    piece03 = ObjectProperty(None)
    piece04 = ObjectProperty(None)
    piece05 = ObjectProperty(None)
    piece06 = ObjectProperty(None)
    piece10 = ObjectProperty(None)
    piece11 = ObjectProperty(None)
    piece12 = ObjectProperty(None)
    piece13 = ObjectProperty(None)
    piece14 = ObjectProperty(None)
    piece15 = ObjectProperty(None)
    piece16 = ObjectProperty(None)
    piece20 = ObjectProperty(None)
    piece21 = ObjectProperty(None)
    piece22 = ObjectProperty(None)
    piece23 = ObjectProperty(None)
    piece24 = ObjectProperty(None)
    piece25 = ObjectProperty(None)
    piece26 = ObjectProperty(None)
    piece30 = ObjectProperty(None)
    piece31 = ObjectProperty(None)
    piece32 = ObjectProperty(None)
    piece33 = ObjectProperty(None)
    piece34 = ObjectProperty(None)
    piece35 = ObjectProperty(None)
    piece36 = ObjectProperty(None)
    piece40 = ObjectProperty(None)
    piece41 = ObjectProperty(None)
    piece42 = ObjectProperty(None)
    piece43 = ObjectProperty(None)
    piece44 = ObjectProperty(None)
    piece45 = ObjectProperty(None)
    piece46 = ObjectProperty(None)
    piece50 = ObjectProperty(None)
    piece51 = ObjectProperty(None)
    piece52 = ObjectProperty(None)
    piece53 = ObjectProperty(None)
    piece54 = ObjectProperty(None)
    piece55 = ObjectProperty(None)
    piece56 = ObjectProperty(None)
    

    def placePiece(self, instance, columnNumber):
        global headerText
        global turn
        if turn == 1:
            headerText = "Your piece is being placed!"
        global column
        global unseen
        with lock:
            column = columnNumber

            unseen = True;

        #TODO: send data to PSoC 6 via BLE4.2
    def threadHandler(self, instance):
        global stopThreads
        with lock:
            stopThreads = True
    
    def start(self):
        self.piece00.r = colorArr[0][0]
        self.piece00.g = colorArr[0][1]
        self.piece00.b = colorArr[0][2]
        self.piece00.a = colorArr[0][3]

        self.piece01.r = colorArr[1][0]
        self.piece01.g = colorArr[1][1]
        self.piece01.b = colorArr[1][2]
        self.piece01.a = colorArr[1][3]

        self.piece02.r = colorArr[2][0]
        self.piece02.g = colorArr[2][1]
        self.piece02.b = colorArr[2][2]
        self.piece02.a = colorArr[2][3]

        self.piece03.r = colorArr[3][0]
        self.piece03.g = colorArr[3][1]
        self.piece03.b = colorArr[3][2]
        self.piece03.a = colorArr[3][3]

        self.piece04.r = colorArr[4][0]
        self.piece04.g = colorArr[4][1]
        self.piece04.b = colorArr[4][2]
        self.piece04.a = colorArr[4][3]

        self.piece05.r = colorArr[5][0]
        self.piece05.g = colorArr[5][1]
        self.piece05.b = colorArr[5][2]
        self.piece05.a = colorArr[5][3]

        self.piece06.r = colorArr[6][0]
        self.piece06.g = colorArr[6][1]
        self.piece06.b = colorArr[6][2]
        self.piece06.a = colorArr[6][3]

        self.piece10.r = colorArr[7][0]
        self.piece10.g = colorArr[7][1]
        self.piece10.b = colorArr[7][2]
        self.piece10.a = colorArr[7][3]

        self.piece11.r = colorArr[8][0]
        self.piece11.g = colorArr[8][1]
        self.piece11.b = colorArr[8][2]
        self.piece11.a = colorArr[8][3]

        self.piece12.r = colorArr[9][0]
        self.piece12.g = colorArr[9][1]
        self.piece12.b = colorArr[9][2]
        self.piece12.a = colorArr[9][3]

        self.piece13.r = colorArr[10][0]
        self.piece13.g = colorArr[10][1]
        self.piece13.b = colorArr[10][2]
        self.piece13.a = colorArr[10][3]

        self.piece14.r = colorArr[11][0]
        self.piece14.g = colorArr[11][1]
        self.piece14.b = colorArr[11][2]
        self.piece14.a = colorArr[11][3]

        self.piece15.r = colorArr[12][0]
        self.piece15.g = colorArr[12][1]
        self.piece15.b = colorArr[12][2]
        self.piece15.a = colorArr[12][3]

        self.piece16.r = colorArr[13][0]
        self.piece16.g = colorArr[13][1]
        self.piece16.b = colorArr[13][2]
        self.piece16.a = colorArr[13][3]

        self.piece20.r = colorArr[14][0]
        self.piece20.g = colorArr[14][1]
        self.piece20.b = colorArr[14][2]
        self.piece20.a = colorArr[14][3]

        self.piece21.r = colorArr[15][0]
        self.piece21.g = colorArr[15][1]
        self.piece21.b = colorArr[15][2]
        self.piece21.a = colorArr[15][3]

        self.piece22.r = colorArr[16][0]
        self.piece22.g = colorArr[16][1]
        self.piece22.b = colorArr[16][2]
        self.piece22.a = colorArr[16][3]

        self.piece23.r = colorArr[17][0]
        self.piece23.g = colorArr[17][1]
        self.piece23.b = colorArr[17][2]
        self.piece23.a = colorArr[17][3]

        self.piece24.r = colorArr[18][0]
        self.piece24.g = colorArr[18][1]
        self.piece24.b = colorArr[18][2]
        self.piece24.a = colorArr[18][3]

        self.piece25.r = colorArr[19][0]
        self.piece25.g = colorArr[19][1]
        self.piece25.b = colorArr[19][2]
        self.piece25.a = colorArr[19][3]

        self.piece26.r = colorArr[20][0]
        self.piece26.g = colorArr[20][1]
        self.piece26.b = colorArr[20][2]
        self.piece26.a = colorArr[20][3]

        self.piece30.r = colorArr[21][0]
        self.piece30.g = colorArr[21][1]
        self.piece30.b = colorArr[21][2]
        self.piece30.a = colorArr[21][3]

        self.piece31.r = colorArr[22][0]
        self.piece31.g = colorArr[22][1]
        self.piece31.b = colorArr[22][2]
        self.piece31.a = colorArr[22][3]

        self.piece32.r = colorArr[23][0]
        self.piece32.g = colorArr[23][1]
        self.piece32.b = colorArr[23][2]
        self.piece32.a = colorArr[23][3]

        self.piece33.r = colorArr[24][0]
        self.piece33.g = colorArr[24][1]
        self.piece33.b = colorArr[24][2]
        self.piece33.a = colorArr[24][3]

        self.piece34.r = colorArr[25][0]
        self.piece34.g = colorArr[25][1]
        self.piece34.b = colorArr[25][2]
        self.piece34.a = colorArr[25][3]

        self.piece35.r = colorArr[26][0]
        self.piece35.g = colorArr[26][1]
        self.piece35.b = colorArr[26][2]
        self.piece35.a = colorArr[26][3]

        self.piece36.r = colorArr[27][0]
        self.piece36.g = colorArr[27][1]
        self.piece36.b = colorArr[27][2]
        self.piece36.a = colorArr[27][3]

        self.piece40.r = colorArr[28][0]
        self.piece40.g = colorArr[28][1]
        self.piece40.b = colorArr[28][2]
        self.piece40.a = colorArr[28][3]

        self.piece41.r = colorArr[29][0]
        self.piece41.g = colorArr[29][1]
        self.piece41.b = colorArr[29][2]
        self.piece41.a = colorArr[29][3]

        self.piece42.r = colorArr[30][0]
        self.piece42.g = colorArr[30][1]
        self.piece42.b = colorArr[30][2]
        self.piece42.a = colorArr[30][3]

        self.piece43.r = colorArr[31][0]
        self.piece43.g = colorArr[31][1]
        self.piece43.b = colorArr[31][2]
        self.piece43.a = colorArr[31][3]

        self.piece44.r = colorArr[32][0]
        self.piece44.g = colorArr[32][1]
        self.piece44.b = colorArr[32][2]
        self.piece44.a = colorArr[32][3]

        self.piece45.r = colorArr[33][0]
        self.piece45.g = colorArr[33][1]
        self.piece45.b = colorArr[33][2]
        self.piece45.a = colorArr[33][3]

        self.piece46.r = colorArr[34][0]
        self.piece46.g = colorArr[34][1]
        self.piece46.b = colorArr[34][2]
        self.piece46.a = colorArr[34][3]

        self.piece50.r = colorArr[35][0]
        self.piece50.g = colorArr[35][1]
        self.piece50.b = colorArr[35][2]
        self.piece50.a = colorArr[35][3]


        self.piece51.r = colorArr[36][0]
        self.piece51.g = colorArr[36][1]
        self.piece51.b = colorArr[36][2]
        self.piece51.a = colorArr[36][3]

        self.piece52.r = colorArr[37][0]
        self.piece52.g = colorArr[37][1]
        self.piece52.b = colorArr[37][2]
        self.piece52.a = colorArr[37][3]

        self.piece53.r = colorArr[38][0]
        self.piece53.g = colorArr[38][1]
        self.piece53.b = colorArr[38][2]
        self.piece53.a = colorArr[38][3]

        self.piece54.r = colorArr[39][0]
        self.piece54.g = colorArr[39][1]
        self.piece54.b = colorArr[39][2]
        self.piece54.a = colorArr[39][3]

        self.piece55.r = colorArr[40][0]
        self.piece55.g = colorArr[40][1]
        self.piece55.b = colorArr[40][2]
        self.piece55.a = colorArr[40][3]

        self.piece56.r = colorArr[41][0]
        self.piece56.g = colorArr[41][1]
        self.piece56.b = colorArr[41][2]
        self.piece56.a = colorArr[41][3]
    def update(self, dt):
        self.piece00.text = headerText
        self.piece00.r = colorArr[0][0]
        self.piece00.g = colorArr[0][1]
        self.piece00.b = colorArr[0][2]
        self.piece00.a = colorArr[0][3]

        self.piece01.r = colorArr[1][0]
        self.piece01.g = colorArr[1][1]
        self.piece01.b = colorArr[1][2]
        self.piece01.a = colorArr[1][3]

        self.piece02.r = colorArr[2][0]
        self.piece02.g = colorArr[2][1]
        self.piece02.b = colorArr[2][2]
        self.piece02.a = colorArr[2][3]

        self.piece03.r = colorArr[3][0]
        self.piece03.g = colorArr[3][1]
        self.piece03.b = colorArr[3][2]
        self.piece03.a = colorArr[3][3]

        self.piece04.r = colorArr[4][0]
        self.piece04.g = colorArr[4][1]
        self.piece04.b = colorArr[4][2]
        self.piece04.a = colorArr[4][3]

        self.piece05.r = colorArr[5][0]
        self.piece05.g = colorArr[5][1]
        self.piece05.b = colorArr[5][2]
        self.piece05.a = colorArr[5][3]

        self.piece06.r = colorArr[6][0]
        self.piece06.g = colorArr[6][1]
        self.piece06.b = colorArr[6][2]
        self.piece06.a = colorArr[6][3]

        self.piece10.r = colorArr[7][0]
        self.piece10.g = colorArr[7][1]
        self.piece10.b = colorArr[7][2]
        self.piece10.a = colorArr[7][3]

        self.piece11.r = colorArr[8][0]
        self.piece11.g = colorArr[8][1]
        self.piece11.b = colorArr[8][2]
        self.piece11.a = colorArr[8][3]

        self.piece12.r = colorArr[9][0]
        self.piece12.g = colorArr[9][1]
        self.piece12.b = colorArr[9][2]
        self.piece12.a = colorArr[9][3]

        self.piece13.r = colorArr[10][0]
        self.piece13.g = colorArr[10][1]
        self.piece13.b = colorArr[10][2]
        self.piece13.a = colorArr[10][3]

        self.piece14.r = colorArr[11][0]
        self.piece14.g = colorArr[11][1]
        self.piece14.b = colorArr[11][2]
        self.piece14.a = colorArr[11][3]

        self.piece15.r = colorArr[12][0]
        self.piece15.g = colorArr[12][1]
        self.piece15.b = colorArr[12][2]
        self.piece15.a = colorArr[12][3]

        self.piece16.r = colorArr[13][0]
        self.piece16.g = colorArr[13][1]
        self.piece16.b = colorArr[13][2]
        self.piece16.a = colorArr[13][3]

        self.piece20.r = colorArr[14][0]
        self.piece20.g = colorArr[14][1]
        self.piece20.b = colorArr[14][2]
        self.piece20.a = colorArr[14][3]

        self.piece21.r = colorArr[15][0]
        self.piece21.g = colorArr[15][1]
        self.piece21.b = colorArr[15][2]
        self.piece21.a = colorArr[15][3]

        self.piece22.r = colorArr[16][0]
        self.piece22.g = colorArr[16][1]
        self.piece22.b = colorArr[16][2]
        self.piece22.a = colorArr[16][3]

        self.piece23.r = colorArr[17][0]
        self.piece23.g = colorArr[17][1]
        self.piece23.b = colorArr[17][2]
        self.piece23.a = colorArr[17][3]

        self.piece24.r = colorArr[18][0]
        self.piece24.g = colorArr[18][1]
        self.piece24.b = colorArr[18][2]
        self.piece24.a = colorArr[18][3]

        self.piece25.r = colorArr[19][0]
        self.piece25.g = colorArr[19][1]
        self.piece25.b = colorArr[19][2]
        self.piece25.a = colorArr[19][3]

        self.piece26.r = colorArr[20][0]
        self.piece26.g = colorArr[20][1]
        self.piece26.b = colorArr[20][2]
        self.piece26.a = colorArr[20][3]

        self.piece30.r = colorArr[21][0]
        self.piece30.g = colorArr[21][1]
        self.piece30.b = colorArr[21][2]
        self.piece30.a = colorArr[21][3]

        self.piece31.r = colorArr[22][0]
        self.piece31.g = colorArr[22][1]
        self.piece31.b = colorArr[22][2]
        self.piece31.a = colorArr[22][3]

        self.piece32.r = colorArr[23][0]
        self.piece32.g = colorArr[23][1]
        self.piece32.b = colorArr[23][2]
        self.piece32.a = colorArr[23][3]

        self.piece33.r = colorArr[24][0]
        self.piece33.g = colorArr[24][1]
        self.piece33.b = colorArr[24][2]
        self.piece33.a = colorArr[24][3]

        self.piece34.r = colorArr[25][0]
        self.piece34.g = colorArr[25][1]
        self.piece34.b = colorArr[25][2]
        self.piece34.a = colorArr[25][3]

        self.piece35.r = colorArr[26][0]
        self.piece35.g = colorArr[26][1]
        self.piece35.b = colorArr[26][2]
        self.piece35.a = colorArr[26][3]

        self.piece36.r = colorArr[27][0]
        self.piece36.g = colorArr[27][1]
        self.piece36.b = colorArr[27][2]
        self.piece36.a = colorArr[27][3]

        self.piece40.r = colorArr[28][0]
        self.piece40.g = colorArr[28][1]
        self.piece40.b = colorArr[28][2]
        self.piece40.a = colorArr[28][3]

        self.piece41.r = colorArr[29][0]
        self.piece41.g = colorArr[29][1]
        self.piece41.b = colorArr[29][2]
        self.piece41.a = colorArr[29][3]

        self.piece42.r = colorArr[30][0]
        self.piece42.g = colorArr[30][1]
        self.piece42.b = colorArr[30][2]
        self.piece42.a = colorArr[30][3]

        self.piece43.r = colorArr[31][0]
        self.piece43.g = colorArr[31][1]
        self.piece43.b = colorArr[31][2]
        self.piece43.a = colorArr[31][3]

        self.piece44.r = colorArr[32][0]
        self.piece44.g = colorArr[32][1]
        self.piece44.b = colorArr[32][2]
        self.piece44.a = colorArr[32][3]

        self.piece45.r = colorArr[33][0]
        self.piece45.g = colorArr[33][1]
        self.piece45.b = colorArr[33][2]
        self.piece45.a = colorArr[33][3]

        self.piece46.r = colorArr[34][0]
        self.piece46.g = colorArr[34][1]
        self.piece46.b = colorArr[34][2]
        self.piece46.a = colorArr[34][3]

        self.piece50.r = colorArr[35][0]
        self.piece50.g = colorArr[35][1]
        self.piece50.b = colorArr[35][2]
        self.piece50.a = colorArr[35][3]


        self.piece51.r = colorArr[36][0]
        self.piece51.g = colorArr[36][1]
        self.piece51.b = colorArr[36][2]
        self.piece51.a = colorArr[36][3]

        self.piece52.r = colorArr[37][0]
        self.piece52.g = colorArr[37][1]
        self.piece52.b = colorArr[37][2]
        self.piece52.a = colorArr[37][3]

        self.piece53.r = colorArr[38][0]
        self.piece53.g = colorArr[38][1]
        self.piece53.b = colorArr[38][2]
        self.piece53.a = colorArr[38][3]

        self.piece54.r = colorArr[39][0]
        self.piece54.g = colorArr[39][1]
        self.piece54.b = colorArr[39][2]
        self.piece54.a = colorArr[39][3]

        self.piece55.r = colorArr[40][0]
        self.piece55.g = colorArr[40][1]
        self.piece55.b = colorArr[40][2]
        self.piece55.a = colorArr[40][3]

        self.piece56.r = colorArr[41][0]
        self.piece56.g = colorArr[41][1]
        self.piece56.b = colorArr[41][2]
        self.piece56.a = colorArr[41][3]
    

# we are defining the Base Class of our Kivy App
def getData():
    return stopThreads

class myApp(App):
    
    def updateData(self, input):
        global headerText
        global colorArr
        global c
        global turn

        with lock:
            for x in range(42):
                if input[x] == 0:
                    colorArr[x] = empty
                elif input[x] == 1:
                    colorArr[x] = blue
                elif input[x] == 2:
                    colorArr[x] = yellow
            if input[42] == 0:
                headerText = "It is the opponents turn!"
                turn = 0
            elif input[42] == 1:
                headerText = "It is your turn!"
                turn = 1
            elif input[42] == 2:
                headerText = "The game has not started"
            elif input[42] == 3:
                headerText = "Error: feed empty"
            elif input[42] == 4:
                headerText = "Game over: you win!"
            elif input[42] == 5:
                headerText = "Game over: you lose :("
            else:
                headerText = f"Unknown error, input: {input[42]}"


    def build(self):
        global colorArr
        
        renderer = GridWidget()
        renderer.start()
        Clock.schedule_interval(renderer.update, 1/60)
        return renderer
    
if __name__ == '__main__':
    # Here the class MyApp is initialized
    myApp().run()
