import RPi.GPIO as GPIO
import os
from threading import Thread
from time import sleep
from http.server import BaseHTTPRequestHandler, HTTPServer

#code adapted from https://github.com/e-tinkers/simple_httpserver/blob/master/simple_webserver2.py
#I learned how to implement threading from Jordan Ricks. Thank you Jordan!

host_name = '192.168.1.226'  # Change this to your Raspberry Pi IP address
host_port = 8000

blink_on = "off"
class MyServer(BaseHTTPRequestHandler):
    """ A special implementation of BaseHTTPRequestHander for reading data from
        and control GPIO of a Raspberry Pi
    """
    def blink(self):
        global blink_on
        while blink_on == "blinking":
            print("test")
            GPIO.output(20, GPIO.HIGH) #green on
            sleep(1)
            if blink_on != 'blinking':
                return
            GPIO.output(20, GPIO.LOW) #green off
            GPIO.output(16, GPIO.HIGH) #yellow on
            sleep(1)
            if blink_on != 'blinking':
                return
            GPIO.output(16, GPIO.LOW) #yellow off
            GPIO.output(21, GPIO.HIGH)#red on
            sleep(1)
            if blink_on != 'blinking':
                return
            GPIO.output(21, GPIO.LOW) #red off
            if blink_on != 'blinking':
                return
        
 
    def do_HEAD(self):
        """ do_HEAD() can be tested use curl command
            'curl -I http://server-ip-address:port'
        """
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        """ do_GET() can be tested using curl command
            'curl http://server-ip-address:port'
        """
        html = '''
            <html>
            <body style="width:960px; margin: 20px auto;">
            <h1>Traffic Light Loop</h1>
            <p>Turn LED: 
                  </br>
                  <a href="/red">Red</a>
                  </br>
                  <a href="/yellow">Yellow</a>
                  </br>
                  <a href="/green">Green</a>
                  </br>
                  <a href="/blink">Blink</a>
                  </br>
                  <a href="/off">Off</a>
                  </br>
            </body>
            </html>
        '''
        temp = os.popen("/opt/vc/bin/vcgencmd measure_temp").read()
        self.do_HEAD()
        status = ''
        global blink_on
        if self.path=='/':
            GPIO.setmode(GPIO.BCM)
            GPIO.setwarnings(False)
            GPIO.setup(20, GPIO.OUT) #green
            GPIO.setup(16, GPIO.OUT) #yellow
            GPIO.setup(21, GPIO.OUT) #red
            blink_on = "off"
        elif self.path=='/red':
            GPIO.output(21, GPIO.HIGH)#red on
            GPIO.output(20, GPIO.LOW) #green off 
            GPIO.output(16, GPIO.LOW) #yellow off
        elif self.path=='/yellow':
            GPIO.output(21, GPIO.LOW)#red off
            GPIO.output(20, GPIO.LOW) #green off 
            GPIO.output(16, GPIO.HIGH) #yellow on
            blink_on = "off"
        elif self.path=='/green':
            GPIO.output(21, GPIO.LOW)#red off
            GPIO.output(20, GPIO.HIGH) #green on
            GPIO.output(16, GPIO.LOW) #yellow off
            blink_on = "off"
        elif self.path=='/off':
            GPIO.output(20, GPIO.LOW) #green
            GPIO.output(16, GPIO.LOW) #yellow
            GPIO.output(21, GPIO.LOW) #red
            blink_on = "off"
        elif self.path=='/blink':
          blink_on = "blinking"
          t = Thread(target=self.blink)
          t.start()
            
        self.wfile.write(html.format(temp[5:], status).encode("utf-8"))


if __name__ == '__main__':
    http_server = HTTPServer((host_name, host_port), MyServer)
    print("Server Starts - %s:%s" % (host_name, host_port))

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()
