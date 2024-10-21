# Guión de la Práctica 2

## Puntos clave a explicar de la primera parte de la práctica: **IMG_EQUALIZATION**:

- ¿Qué es ecualizar una imagen y como se realiza este proceso y sus ventajas?
- ¿Cómo se crea un histograma?
- ¿Qué significa normalizar un histograma?
- ¿Qué aplicaciones tiene un histograma acumulado?
- ¿Qué te ofrece el clipado de un histograma?
- Busqueda del valor de clipado
- ¿Qué es una LOOK UP TABLE y como se crean además de para que sirven? 


## Puntos clave a explicar de la primera parte de la práctica: **COLOR_BALANCE**:

- Proceso de reescalado de una imagen
- Concepto uso y aplicaciones de la técnica `Gray World`
- Conversión de RGB a Gris
- Calculo del percentil
- Explicar el proceso `White Patch`, concepto uso, aplicaciones.

Dentro del proceso `White Patch` explicar el `if(p == 0.0)` y el else:

- **Caso `p == 0`** en este caso se quiere utilizar el punto con mayor luminosidad, se localiza este punto y se aplica el reescalado.
- **Caso `p > 0`** en este caso se cogen aquellos puntos que sean un `p%` menos luminosos que el punto más luminoso de la imagen.


## Recaudando Información

1. ¿Qué es ecualizar una imagen?

