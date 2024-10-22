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

Ecualizar una imagen es un proceso de procesamiento de imágenes que se utiliza para mejorar el contraste y la distribución de tonos en una imagen en escala de grises o color. El objetivo es distribuir uniformemente los niveles de brillo o tonos en la imagen, lo que puede ayudar a:

- Mejorar la visibilidad de detalles en la imagen
- Reducir la saturación de tonos extremos (blanco o negro)
- Aumentar la percepción de textura y detalles en la image

2. ¿Como se realiza el proceso de la ecualización?

    1. Se obtiene el histograma de la imagen (Se pueden usar técnicas de recorte)
    2. Se obtiene el histograma acumulado
    3. Se obtiene el histograma normalizado



3. ¿Que es una LookUp Table?

Es básicamente una tabla hash para tranformar un valor de pixel en otro, este está precalculado por ejemplo que los pixeles con valor 231 pasen a ser 114, no es necesario realizar la operación ya que se tiene el valor asociado a cada pixel.

4. ¿Que es el Clipping Level o Nivel de Recorte?

Es el nivel el cual se empieza a recortar el histograma ya que por encima de este valor la generación de una ecualización contendría mucho ruido 
y los blancos solamente se verian más brillantes no cumpliendo el objetivo.

5. ¿Como se calcula el CL?

Se realiza una busqueda binaria.

```cpp
float fsiv_compute_actual_clipping_histogram_value(const cv::Mat &h, float s)
{
    CV_Assert(h.type() == CV_32FC1);
    CV_Assert(h.rows == 256 && h.cols == 1);

    int n = static_cast<int>(h.rows);
    int CL = s * cv::sum(h)[0] / n;

    // TODO: coded the algorithm show in the practical assign description.
    int top = CL;
    int bottom = 0;
    int middle = 0;
    int R;

    while ((top - bottom) > 1.0)
    {
        middle = (top + bottom) / 2;
        R = 0;

        for (int i = 0; i < n; i++)
            if (h.at<float>(i) > middle)
                R += (h.at<float>(i) - middle);
        
        if (R > ((CL - middle) * n))
            top = middle;
        else
            bottom = middle;
    }
    //

    return bottom;
}
```

Un valor optimo pafa `s` esta entre 3 y 6.

Un valor superior a 6 puede generar ruido en la imagen y quemar los blancos.
Un valor inferior a 3 puede generar una imagen muy oscura con perdida de información en estos.

6. ¿Que es el Gray World?

El Gray World es un algoritmo de balance de color que se basa en la suposición de que la media de los valores de los canales de color de una imagen es gris. Por lo tanto, el algoritmo Gray World ajusta los valores de los canales de color de una imagen para que la media de los valores de los canales de color sea gris.

Esta técnica se utiliza para corregir el balance de color en una imagen. El algoritmo Gray World es útil para corregir el balance de color en imágenes que tienen un balance de color incorrecto.


7. Calculo del Percentil

El percentil es una medida estadística que indica el valor por debajo del cual se encuentra un porcentaje dado de observaciones en un conjunto de datos. En el contexto de la corrección de color, el percentil se utiliza para identificar los puntos menos luminosos de una imagen en relación con el punto más luminoso.

```cpp
float fsiv_compute_histogram_percentile(cv::Mat const &hist, float p_value)
{
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.cols == 1);
    CV_Assert(0.0 <= p_value && p_value <= 1.0);

    int p = 0;

    // TODO
    // Remember: find the smaller index 'p' such that
    //           sum(h[0], h[1], ... , h[p]) >= p_value*area(hist)
    // Hint: use cv::sum() to compute the histogram area.

    cv::Scalar area = cv::sum(hist);

    float sum = 0;
    size_t i = 0;

    while (sum < p_value * area[0])
        sum += hist.at<float>(i++);
    
    p = i - 1;
    //

    CV_Assert(0 <= p && p < hist.rows);
    return p;
}
```

8. Pasar de RGB a Gris

Se basa en esta fórmula:

$$
Gris = 0.299 * R + 0.587 * G + 0.114 * B
$$

9. ¿Que es el White Patch?

El White Patch es un algoritmo de balance de color que se basa en la suposición de que el punto más brillante de una imagen es blanco. Por lo tanto, el algoritmo White Patch ajusta los valores de los canales de color de una imagen para que el punto más brillante de la imagen sea blanco.

Se puede aplicar de 2 maneras:

- Considerando el punto más brillante de la imagen como el punto blanco.
- Considerando el punto más brillante de la imagen como el punto blanco y aplicando un reescalado a los puntos que sean un `p%` menos luminosos que el punto más brillante de la imagen.

Ventajas de White Patch: en un solo punto (El más brillante)
1. **Corrección del Balance de Blancos**: Ajusta el balance de blancos, haciendo que los colores se vean más naturales.
2. **Simplicidad**: Relativamente fácil de implementar al identificar el punto más blanco.
3. **Mejora Visual**: Aumenta la percepción visual, con colores más vivos.
4. **Aplicación Rápida**: Método rápido y adecuado para aplicaciones en tiempo real.
5. **Efectividad en Escenas Bien Iluminadas**: Funciona bien en escenas donde el punto más blanco es realmente blanco.

Desventajas de White Patch: en un solo punto (El más brillante)
1. **Dependencia de la Iluminación**: Puede distorsionar colores si el punto más blanco no es realmente blanco.
2. **Pérdida de Detalles**: Posible pérdida de detalles en áreas brillantes si se ajusta incorrectamente.
3. **Efecto No Deseado en Escenas Complejas**: Puede no representar adecuadamente el balance de color en imágenes con múltiples fuentes de luz.
4. **Inconsistencia**: Resultados inconsistentes al aplicar en imágenes con diferentes iluminaciones.
5. **No Considera la Iluminación Global**: Basado solo en un punto específico, sin tomar en cuenta el contexto de toda la imagen.


Ventajas de White Patch: en los `p%` menos luminosos
1. **Reducción de Errores de Color**: Minimiza el riesgo de seleccionar un punto que no sea un verdadero blanco, reduciendo la distorsión de color.
2. **Mejora del Contexto General**: Permite una evaluación más holística del color al considerar otros puntos representativos de la iluminación general.
3. **Menor Sensibilidad al Ruido**: Proporciona una corrección de color más robusta al ser menos sensible al ruido o reflejos no deseados.
4. **Corrección de Imágenes con Iluminación Irregular**: Útil en situaciones de iluminación irregular, reflejando mejor la variación de iluminación en la escena.
5. **Más Control sobre el Rango de Ajuste**: Permite más control sobre la cantidad de corrección aplicada, resultando en ajustes más sutiles.

Desventajas de White Patch: en los `p%` menos luminosos
1. **Complejidad Adicional**: Complica el proceso de corrección de color, requiriendo más cálculos y una implementación más compleja.
2. **Posible Pérdida de Detalles**: Un porcentaje demasiado alto puede resultar en pérdida de detalles en áreas brillantes que se pretenden mantener.
3. **Inconsistencia**: Los resultados pueden variar dependiendo del porcentaje seleccionado, llevando a inconsistencias en el color.
4. **Dificultad en la Selección de `p%`**: Determinar el porcentaje óptimo puede ser subjetivo y depender del contenido específico de la imagen.
5. **Puede No Ser Efectivo en Todas las Situaciones**: En escenas con alto contraste, considerar un rango de puntos menos brillantes puede no ser suficiente para un balance adecuado.
