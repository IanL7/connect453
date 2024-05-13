from connect4 import *
from time import sleep
import threading
import asyncio
from bleak import BleakScanner
from bleak import BleakClient

app = myApp()
keepRunning = True

def controlLoop():
    print("******************************************")
    print("********ENTERING GUI CONTROL LOOP*********")
    print("******************************************")
    global keepRunning
    asyncio.run(ble_handler())

async def ble_handler():
    d = await BleakScanner.find_device_by_name("ECE453-Connect453")
    #print(d)
    async with BleakClient(d) as client:
        while True:
            # read board array
            while True:
                try:
                    with lock:
                        array = await client.read_gatt_char("0d36242b-792a-4204-bfc1-1b3a8bd3407d")
                except TimeoutError:
                    continue
                app.updateData(array)
                if array[42] == 1:
                    break
                sleep(0.5)


            while True:
                array = await client.read_gatt_char("0d36242b-792a-4204-bfc1-1b3a8bd3407d")
                app.updateData(array)
                column = handler()
                if column >= 0 and column < 7:
                    await client.write_gatt_char("d22d4953-d396-4880-88bc-904a49758cee", str(column).encode('ascii'), True)
                    break
                        


if __name__ == '__main__':
    #run in loop waiting for ble data

    task1 = threading.Thread(target=controlLoop, name='Bluetooth')
    task2 = threading.Thread(target=app.run, name='GUI')

    task1.start()
    app.run()
    



