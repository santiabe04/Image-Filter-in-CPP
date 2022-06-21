import streamlit as st
import pandas as pd
import imageio
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
    img = imageio.imread("../out/salida.ppm")
    plt.imshow(img)
    st.pyplot()

    if 'img' not in st.session_state:
        st.session_state['img'] = img

def run_filters(c, b, cp, rows, columns, zoom, multi, shar, bw, coloredge):

    ps = ""
    ps3 = ""
    filters = ""

    if cp:
        filters += 'crop '
        ps += str(rows) + ' '
        ps3 += str(columns) + ' '
    
    if coloredge:
        filters += 'colorEdge '
        ps += '1 '

    if shar:
        filters += 'sharpen '
        ps += '1 '
    
    filters += 'contrast '
    ps += str(c) + ' '

    filters += 'brightness '
    ps += str(b / 100) + ' '

    if bw:
        filters += 'blackWhite '
        ps += '1 '

    if zoom:
        filters += 'zoom '
        ps += str(multi) + ' '

    ps = ps[:-1]
    ps3 = ps3[:-1]

    cmd = '..\main "' + filters + '" 1 "' + ps + '" ../imgs/ashitaka.ppm ../out/salida.ppm '
    if ps3 != "":
        cmd += '"' + ps3 + '"'
    os.system(cmd)

# Sidebar
st.sidebar.header('Parámetros')

c = st.sidebar.slider('Contraste',-100, 100, 0, 1, '%d')
b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')

cp = st.sidebar.checkbox('Crop',value = False)
if cp:
    rows = st.sidebar.number_input("Filas", 1)
    columns = st.sidebar.number_input("Columnas", 1)
zoom = st.sidebar.checkbox('Zoom',value = False)
if zoom:
    multi = st.sidebar.number_input("Multiplicador", 2)
shar = st.sidebar.checkbox('Sharpen',value=False)
bw = st.sidebar.checkbox('BlackWhite',value=False)
coloredge = st.sidebar.checkbox('ColorEdge',value=False)


if st.sidebar.button('Aplicar'):
    run_filters(c, b, cp, rows, columns, zoom, multi, shar, bw, coloredge)


# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

show_image()
