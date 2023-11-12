# -*- coding: utf-8 -*-
"""
Created on Sun Nov 12 11:14:49 2023

@author: antoi
"""

import serial

# Configuration du port série
ser = serial.Serial('COM7', baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

# Convertir -100 en octet signé
byte_to_send = (97).to_bytes(1, byteorder='big', signed=True)


# Envoyer l'octet sur le port série
ser.write(byte_to_send)

# Fermer le port série
ser.close()
