import socket
import pickle
import numpy as np


TIME_STEP = 64
MAX_SPEED = 10
RANGE = (1024 / 2)


matrice_braitenberg_droite=np.array([4,5,7,-6,-4,-2,-0.5,-0.5])
matrice_braitenberg_gauche=np.array([-2,-3,-7,7,5,4,-0.5,-0.5])


def run_client():
    vitess_moteur_droit=moteur_gauche=0
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the server
    server_address = ('host.docker.internal', 8889)
    client_socket.connect(server_address)
  
    # Receive data from the server
    data = client_socket.recv(1024)
    data = pickle.loads(data)

    vitesse_moteur_gauche=np.dot(1-(np.array(data)/RANGE),matrice_braitenberg_gauche)
    vitesse_moteur_droit=np.dot(1-(np.array(data)/RANGE),matrice_braitenberg_droite)
    vitesse_moteur_gauche=np.max(vitesse_moteur_gauche)
    vitesse_moteur_droit=np.max(vitesse_moteur_droit)
    # Send data to the server
    send_data = [vitesse_moteur_gauche, vitesse_moteur_droit]
    message = pickle.dumps(send_data) 
    client_socket.sendall(message)
        
    
    # Clean up the connection
    client_socket.close()

if __name__ == '__main__':
    while True:
        run_client()