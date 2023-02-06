from controller import Robot, DistanceSensor, Motor
import numpy as np
import pickle
import socket



# time in [ms] of a simulation step
TIME_STEP = 64
MAX_SPEED = 10
RANGE = (1024 / 2)



        
# create the Robot instance.
robot = Robot()

# initialize devices
ps = []
ps_dist = []
psNames = [
    'ls0', 'ls1', 'ls2', 'ls3',
    'ls4', 'ls5', 'ls6', 'ls7'
]
ps_distNames =  [
    'ds0', 'ds1', 'ds2', 'ds3',
    'ds4', 'ds5', 'ds6', 'ds7'
]
for i in range(8):
    ps.append(robot.getDevice(psNames[i]))
    ps_dist.append(robot.getDevice(ps_distNames[i]))
    ps[i].enable(TIME_STEP)
    ps_dist[i].enable(TIME_STEP)


leftMotor = robot.getDevice('left wheel motor')
rightMotor = robot.getDevice('right wheel motor')
leftMotor.setPosition(float('inf'))
rightMotor.setPosition(float('inf'))
leftMotor.setVelocity(2.0)
rightMotor.setVelocity(2.0)





###
# Client dist
###
# Create a TCP/IP socket
server_dist_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a specific address and port
server_dist_address = ('localhost', 8889)
server_dist_socket.bind(server_dist_address)

# Listen for incoming connections
server_dist_socket.listen(1)
print(f'Server is listening on {server_dist_address}')



# Create a TCP/IP socket
server_light_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a specific address and port
server_light_address = ('localhost', 8887)
server_light_socket.bind(server_light_address)

# Listen for incoming connections
server_light_socket.listen(1)
print(f'Server is listening on {server_light_address}')


# Create a TCP/IP socket
#server_coord_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a specific address and port
#server_coord_address = ('localhost', 8886)
#server_coord_socket.bind(server_coord_address)

# Listen for incoming connections
#server_coord_socket.listen(1)
#print(f'Server is listening on {server_coord_address}')



while robot.step(32) != -1:
        # Wait for a connection
        connection_dist, client_address_dist = server_dist_socket.accept()
        connection_light, client_address_light = server_light_socket.accept()
        #connection_coord, client_address_coord = server_coord_socket.accept()


        vitesse_moteur_gauche = vitesse_moteur_droit = 0
   
   
        #Calcul des vitesse
        valeur_capteur_light=[]
        valeur_capteur_dist=[]
        for i in range(8):
            valeur_capteur_light.append(ps[i].getValue())
        for i in range(8):
            valeur_capteur_dist.append(ps_dist[i].getValue())
        data=pickle.dumps(valeur_capteur_dist)
        connection_dist.send(data)
        data_receive = pickle.loads(connection_dist.recv(1024))
        print(data_receive)
        
        data_light = pickle.dumps(valeur_capteur_light)
        connection_light.send(data_light)
        data_light_receive = pickle.loads(connection_light.recv(1024))
        print(data_light_receive)
        
        #data_coord = [valeur_capteur_dist,valeur_capteur_light]
        #data_coord = pickle.dumps(data_coord)
        #connection_coord.send(data_coord)
        #data_coord_receive = pickle.loads(connection_coord.recv(1024))
        
        
        if np.sum(np.array(valeur_capteur_dist))>0:
            vitesse_moteur_gauche = data_receive[0]
            vitesse_moteur_droit = data_receive[1]
        else :
            vitesse_moteur_gauche = data_light_receive[0]
            vitesse_moteur_droit = data_light_receive[1] 
           
       
        print("gauche",vitesse_moteur_gauche) 
        print("droit",vitesse_moteur_droit)
        leftMotor.setVelocity(vitesse_moteur_gauche)
        rightMotor.setVelocity(vitesse_moteur_droit)
        connection_dist.close()

