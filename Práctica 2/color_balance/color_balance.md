# Explicación de las funciones

## `fsiv_color_rescaling`

### Descripción:
Esta función reescala los valores de color de una imagen de 3 canales (BGR) de acuerdo con dos valores escalares: `from` (color de entrada) y `to` (color deseado). Utiliza los métodos `cv::divide` y `cv::multiply` para calcular el factor de escala y aplicar la multiplicación a la imagen de entrada.

### Aplicación:
Se utiliza para ajustar los colores de una imagen basándose en un rango de color dado, lo que es útil en tareas como el balance de blancos o la corrección de colores.

---

## `fsiv_gray_world_color_balance`

### Descripción:
Implementa el balance de blancos usando el método de "Gray World". La función calcula el valor medio de los colores de la imagen (BGR) y lo ajusta para que ese valor medio sea igual a `(128, 128, 128)`.

### Aplicación:
Es útil para corregir las dominantes de color en imágenes en las que se asume que los colores promedio de la imagen deben ser neutros (grises).

---

## `fsiv_convert_bgr_to_gray`

### Descripción:
Convierte una imagen de color (BGR) a una imagen en escala de grises utilizando la función `cv::cvtColor`.

### Aplicación:
Esta conversión es esencial en muchas operaciones de procesamiento de imágenes, como la detección de bordes o la creación de histogramas, donde se necesita trabajar con una imagen monocromática.

---

## `fsiv_compute_image_histogram`

### Descripción:
Esta función calcula el histograma de una imagen en escala de grises de 8 bits (256 niveles de intensidad) utilizando `cv::calcHist`. El histograma muestra la distribución de los niveles de gris en la imagen.

### Aplicación:
El histograma es una herramienta fundamental en procesamiento de imágenes para analizar la distribución de intensidades y realizar ajustes como la ecualización de histograma o el balance de blancos.

---

## `fsiv_compute_histogram_percentile`

### Descripción:
Calcula el percentil en un histograma, es decir, encuentra el menor índice del histograma tal que la suma acumulada de los valores de los bins es mayor o igual a un porcentaje del área total del histograma.

### Aplicación:
Esta función es útil en el balance de blancos por el método "White Patch" cuando se desea ajustar el brillo basado en un percentil de los píxeles más brillantes de la imagen.

---

## `fsiv_white_patch_color_balance`

### Descripción:
Implementa el balance de blancos usando el método de "White Patch". Ajusta los colores de la imagen basándose en los píxeles más brillantes. Si el parámetro `p` es 0, ajusta al píxel más brillante; si `p` es mayor que 0, ajusta al percentil de brillo especificado.

### Aplicación:
Este método es ideal para corregir imágenes subexpuestas o sobreexpuestas, y mejora la fidelidad de color en situaciones donde el píxel más brillante debe ser blanco.

