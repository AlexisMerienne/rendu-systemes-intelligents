import socket
import pickle
import numpy as np


TIME_STEP = 64
MAX_SPEED = 10
RANGE = (1024 / 2)



def run_client():
    vitess_moteur_droit=moteur_gauche=0
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the server
    server_address = ('host.docker.internal', 8887)
    client_socket.connect(server_address)
  
    # Receive data from the server
    data = client_socket.recv(1024)
    data = pickle.loads(data)
    max_sensors_value = np.argmax(data)
    if max_sensors_value==0:
        vitesse_moteur_gauche=0
        vitesse_moteur_droit=MAX_SPEED
    elif max_sensors_value==1:
        vitesse_moteur_gauche=MAX_SPEED/2
        vitesse_moteur_droit=-MAX_SPEED
    elif max_sensors_value==2:
        vitesse_moteur_gauche=MAX_SPEED-0.2
        vitesse_moteur_droit=MAX_SPEED
    elif max_sensors_value==3:
        vitesse_moteur_gauche=MAX_SPEED
        vitesse_moteur_droit=MAX_SPEED-0.2
    elif max_sensors_value==4:
        vitesse_moteur_gauche=MAX_SPEED
        vitesse_moteur_droit=MAX_SPEED/2
    elif max_sensors_value==5:
        vitesse_moteur_gauche=MAX_SPEED
        vitesse_moteur_droit=0
    elif max_sensors_value==6:
        vitesse_moteur_gauche=-MAX_SPEED
        vitesse_moteur_droit=0
    elif max_sensors_value==7:
        vitesse_moteur_gauche=0
        vitesse_moteur_droit=-MAX_SPEED
        


    # Send data to the server
    send_data = [vitesse_moteur_gauche, vitesse_moteur_droit]
    message = pickle.dumps(send_data) 
    client_socket.sendall(message)
        
    
    # Clean up the connection
    client_socket.close()

if __name__ == '__main__':
    while True:
        run_client()