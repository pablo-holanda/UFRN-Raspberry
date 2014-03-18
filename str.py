#sudo apt-get update
#sudo apt-get install python-dev
#sudo apt-get install python-rpi.gpio
from socket import *
import thread
from array import *
import time
import threading
import RPi.GPIO as GPIO #Importa a biblioteca das GPIO

GPIO.setmode(GPIO.BCM)

GPIO.setwarnings(False) #Desativa os avisos

GPIO.setup(25, GPIO.OUT)# set GPIO 25 as output for white led  
GPIO.setup(24, GPIO.OUT)# set GPIO 24 as output for red led 
GPIO.setup(23, GPIO.OUT)
GPIO.setup(18, GPIO.OUT)

pwm_led1 = GPIO.PWM(25, 100) #Cria um objeto pwm_ledN para PWM na porta N com 500 Hertz
pwm_led2 = GPIO.PWM(24, 100)
pwm_led3 = GPIO.PWM(23, 100)
pwm_led4 = GPIO.PWM(18, 100)

pwm_led1.start(0) #Inicia o PWM eom 0%
pwm_led2.start(0)
pwm_led3.start(0)
pwm_led4.start(0)

controle = array('i', [0,0,0,0]) #array para controle on/off
freqcontrol = array('i', [0,0,0,0]) #controle da frequencia
brilho = array('i', [0,0,0,0]) #array para armazenar os valores de brilho de cada led
frequencia = array('i', [0,0,0,0]) #array para armazenar os valores de frequencia de cada led
teste = 0

BUFF = 1024 #buff de leitura
HOST = '192.168.0.11' #Para trabalhar em local host sete 127.0.0.1 ja na rede o seu ip de rede
PORT = 56414 #Porta que vamos ouvir

def controle_led1(id): #Funcao para controlar o LED
	while 1:
   	     if controle[id] == 1 and freqcontrol[id] == 1 and brilho[id] != 0:
	          pwm_led1.ChangeDutyCycle(0) #Altera o PWM do LED
		  time.sleep(frequencia[id]/14)
	   	  pwm_led1.ChangeDutyCycle(brilho[id])
	  	  time.sleep(frequencia[id]/14)
	     elif brilho[id] == 0:
		  pwm_led1.ChangeDutyCycle(0)
		  freqcontrol[id] = 0
	     else:
		  pwm_led1.ChangeDutyCycle(brilho[id])
		  controle[id] = 1

def controle_led2(id):
	while 1:
	     if controle[id] == 1 and freqcontrol[id] == 1 and brilho[id] != 0:
                  pwm_led2.ChangeDutyCycle(0) #Altera o PWM do LED
                  time.sleep(frequencia[id]/14)
                  pwm_led2.ChangeDutyCycle(brilho[id])
                  time.sleep(frequencia[id]/14)
             elif brilho[id] == 0:
                  pwm_led2.ChangeDutyCycle(0)
                  freqcontrol[id] = 0
             else:
                  pwm_led2.ChangeDutyCycle(brilho[id])
                  controle[id] = 1

def controle_led3(id):
	while 1:
             if controle[id] == 1 and freqcontrol[id] == 1 and brilho[id] != 0:
                  pwm_led3.ChangeDutyCycle(0) #Altera o PWM do LED
                  time.sleep(frequencia[id]/14)
                  pwm_led3.ChangeDutyCycle(brilho[id])
                  time.sleep(frequencia[id]/14)
             elif brilho[id] == 0:
                  pwm_led3.ChangeDutyCycle(0)
                  freqcontrol[id] = 0
             else:
                  pwm_led3.ChangeDutyCycle(brilho[id])
                  controle[id] = 1
          
def controle_led4(id):
	while 1:
             if controle[id] == 1 and freqcontrol[id] == 1 and brilho[id] != 0:
                  pwm_led4.ChangeDutyCycle(0) #Altera o PWM do LED
                  time.sleep(frequencia[id]/14)
                  pwm_led4.ChangeDutyCycle(brilho[id])
                  time.sleep(frequencia[id]/14)
             elif brilho[id] == 0:
                  pwm_led4.ChangeDutyCycle(0)
                  freqcontrol[id] = 0
             else:
                  pwm_led4.ChangeDutyCycle(brilho[id])
                  controle[id] = 1
	 
def response(key):
    return 'Server response: ' + key

def handler(clientsock,addr):
    while 1:
        data = clientsock.recv(BUFF)
        if not data: break
        print repr(addr) + ' recv:' + repr(data)
        clientsock.send(response(data))
        gambis = len(data) #gambiarra para contornar o erro no programa java
        if gambis > 1 and gambis < 13:
            j = int(data[0:2],2) #Saber qual LED que vai alterar o valor
            op = int(data[2:4],2) #Saber a operacao que vai realizar
            if op == 0: #ON/OFF
                brilho[j] = int(data[4:12],2)

            if op == 1:  #Altera o brilho
                brilho[j] = int(data[4:12],2)
        
            if op == 2: #Altera a frequencia
                frequencia[j] = int(data[4:12],2)
		freqcontrol[j] = 1

        #print repr(addr) + ' sent:' + repr(response(data))
        if "close" == data.rstrip(): break # type 'close' on client console to close connection from the server side
    
    clientsock.close()
    #print addr, "- closed connection" #log on console

if __name__=='__main__':
    ADDR = (HOST, PORT)
    serversock = socket(AF_INET, SOCK_STREAM)
    serversock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    serversock.bind(ADDR)
    serversock.listen(5)

try:
    thread.start_new_thread(controle_led1,(0,))
    thread.start_new_thread(controle_led2,(1,))
    thread.start_new_thread(controle_led3,(2,))
    thread.start_new_thread(controle_led4,(3,))
    while 1:
    	print 'Ouvindo a porta: ', PORT
    	clientsock, addr = serversock.accept()
    	print '->Nova conexao: ', addr
    	thread.start_new_thread(handler, (clientsock, addr))

except: 
    print "Error: Unable to start new thread" 
