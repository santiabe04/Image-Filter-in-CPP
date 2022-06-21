import streamlit as st
import pandas as pd
import imageio.v2 as imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)


def show_image():
    img = imageio.imread('../out/salida.ppm')
    plt.imshow(img)
    st.pyplot()

    if 'img' not in st.session_state:
        st.session_state['img'] = img

def run_filters(c, b, blur, bw, sh, ed, s, cp, rows, columns, fr, color, margin, zoom, multi):

    ps = ""
    ps3 = ""
    filters = ""

    if cp:
        filters += 'crop '
        ps += str(rows) + ' '
        ps3 += str(columns) + ' '

    if blur:
        filters += 'boxBlur '
        ps += '0 '
    
    if ed:
        filters += 'edgeDetection '
        ps += '0 '

    if sh:
        filters += 'sharpen '
        ps += '0 '
    
    filters += 'contrast '
    ps += str(c) + ' '

    filters += 'brightness '
    ps += str(b / 100) + ' '

    if bw:
        filters += 'shades '
        ps += str(s) + ' '

    if fr:
        filters += 'frame '
        ps += str(color) + ' '
        ps3 += str(margin) + ' '

    if zoom:
        filters += 'zoom '
        ps += str(multi) + ' '

    ps = ps[:-1]
    ps3 = ps3[:-1]

    cmd = '..\main "' + filters + '" 1 "' + ps + '" ../imgs/ashitaka.ppm ../out/salida.ppm '
    if ps3 != "":
        cmd += '"' + ps3 + '"'
    os.system(cmd)

if 'ed' in st.session_state:

    if st.session_state['ed']:
        st.session_state['bw'] = False
        st.session_state['blur'] = False

    elif st.session_state['bw'] or st.session_state['blur']:
        st.session_state['ed'] = False

   
# Sidebar
st.sidebar.header('Parámetros')

c = st.sidebar.slider('Contraste',-255, 255, 0, 1, '%d')
b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')

sh = st.sidebar.checkbox('Sharpen',value = False, key='sh')
ed = st.sidebar.checkbox('EdgeDetection',value = False, key='ed')
blur = st.sidebar.checkbox('Box blur',value = False, key='blur')
bw = st.sidebar.checkbox('BlackWhite',value = False, key='bw')

s = 0
rows = 0
columns = 0
color = 0
margin = 0
multi = 0

if bw:
    s = st.sidebar.slider('Shades', 2, 64, 2, 1, '%d')

st.sidebar.header('Otros')

cp = st.sidebar.checkbox('Crop',value = False, key='cp')

if cp:
    rows = st.sidebar.number_input("Filas", 1)
    columns = st.sidebar.number_input("Columnas", 1)

fr = st.sidebar.checkbox('Frame',value = False, key='fr')

if fr:
    color = st.sidebar.number_input("Escala de gris", 0, 255)
    margin = st.sidebar.number_input("Tamaño del marco", 1)

zoom = st.sidebar.checkbox('Zoom',value = False, key='zoom')

if zoom:
    multi = st.sidebar.number_input("Multiplicador", 2)

if st.sidebar.button('Aplicar'):
    run_filters(c, b, blur, bw, sh, ed, s, cp, rows, columns, fr, color, margin, zoom, multi)

if st.sidebar.button('Reset'):
    imageio.imwrite('../out/salida.ppm', st.session_state['img'])

# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

show_image()