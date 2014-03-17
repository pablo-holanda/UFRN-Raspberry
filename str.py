#sudo apt-get update
#sudo apt-get install python-dev
#sudo apt-get install python-rpi.gpio
from socket import *
import thread
from array import *
import time
import threading
import RPi.GPIO as GPIO #Importa a biblioteca das GPIO

GPIO.setmode(GPIO.BOARD) #Configura o modo de definicao de pinos como BOARD (contagem de pinos da placa)

GPIO.setwarnings(False) #Desativa os avisos

GPIO.setup(12, GPIO.OUT) #Configura o pino da placa como saida
GPIO.setup(16, GPIO.OUT)
GPIO.setup(18, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)

pwm_led1 = GPIO.PWM(12, 500) #Cria um objeto pwm_ledN para PWM na porta N com 500 Hertz
pwm_led2 = GPIO.PWM(16, 500)
pwm_led3 = GPIO.PWM(18, 500)
pwm_led4 = GPIO.PWM(22, 500)

pwm_led1.start(0) #Inicia o PWM em 0%
pwm_led2.start(0)
pwm_led3.start(0)
pwm_led4.start(0)

brilho = array('i', [0,0,0,0]) #array para armazenar os valores de brilho de cada led
frequencia = array('i', [0,0,0,0]) #array para armazenar os valores de frequencia de cada led

BUFF = 1024 #buff de leitura
HOST = '127.0.0.1' #Para trabalhar em local host sete 127.0.0.1 ja na rede o seu ip de rede
PORT = 56414 #Porta que vamos ouvir

def controle_led1(id): #Funcao para controlar o LED
    while 1:
        pwm_led1.ChangeDutyCycle(brilho[id]) #Altera o PWM do LED
        time.sleep(frequencia[id]/3) #Sleep
		pwm_led1.ChangeDutyCycle(0)
        time.sleep(frequencia[id]/3)

def controle_led2(id):
    while 1:
    	pwm_led2.ChangeDutyCycle(brilho[id])
    	time.sleep(frequencia[id]/3)
        pwm_led2.ChangeDutyCycle(0)
    	time.sleep(frequencia[id]/3)

def controle_led3(id):
    while 1:
    	pwm_led3.ChangeDutyCycle(brilho[id])
    	time.sleep(frequencia[id]/3)
        pwm_led3.ChangeDutyCycle(0)
    	time.sleep(frequencia[id]/3)

def controle_led4(id):
    while 1:
        pwm_led4.ChangeDutyCycle(brilho[id])
        time.sleep(frequencia[id]/3)
		pwm_led4.ChangeDutyCycle(0)
        time.sleep(frequencia[id]/3)

def response(key):
    try:
        thread.start_new_thread(controle_led1, (0,)) #Thread para controlar o led
        thread.start_new_thread(controle_led2, (1,))
        thread.start_new_thread(controle_led3, (2,))
        thread.start_new_thread(controle_led4, (3,))
    except:
        print "Error: Unable to start new thread"
    return 'Server response: ' + key

def handler(clientsock,addr):
    while 1:
        data = clientsock.recv(BUFF)
        if not data: break
        print repr(addr) + ' recv:' + repr(data)
        clientsock.send(response(data))
        gambis = len(data) #gambiarra para contornar o erro no programa java
        if gambis > 1:
            j = int(data[0:2],2) - 1 #Saber qual LED que vai alterar o valor
            op = int(data[2:4],2) #Saber a operacao que vai realizar
            if op == 0: #ON/OFF
                brilho[j] = int(data[4:12],2)
        
            if op == 1:  #Altera o brilho
                brilho[j] = int(data[4:12],2)
        
            if op == 2: #Altera a frequencia
                frequencia[j] = int(data[4:12],2)

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
    
    while 1:
    	print 'Ouvindo a porta: ', PORT
    	clientsock, addr = serversock.accept()
    	print '->Nova conexao: ', addr
    	thread.start_new_thread(handler, (clientsock, addr))

while 1:
    pass