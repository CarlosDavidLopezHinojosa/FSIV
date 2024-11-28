# Lo que he hecho

_Describe de forma honesta qué funcionalidades has realizado de forma total o parcial, indicando en este caso qué problema has tenido para no poder implementarla de forma completa._

Todo
# Lo que no he hecho

# Enlace al vídeo descriptivo

[Enlace al video](https://www.youtube.com/watch?v=hkIJsioI5r0)

_No olvides poner en la descripción del vídeo los capítulos de la forma:_
'
Capítulos:
00:00 fsiv_xxxx
01:30 fsiv_xxxx
03:00 Ejemplo de ejecución con parámetro X y justificación.
'
./calibrate -c --size=0.04 --rows=5 --cols=6 ../data/logitech.xml 0

./undistort ../data/elp.xml ../data/undist_elp_8.png undist.png

./calibrate -c --size=0.04 --rows=5 --cols=6 --save_frames=../frames/carlos ../mis_parametros.xml 0

./undistort ../data/elp.xml ../data/elp_4.png elp4_undist.png
