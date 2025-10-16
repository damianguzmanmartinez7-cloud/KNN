#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <ctype.h> // isspace
#include <errno.h>
#include <math.h>

typedef struct
{
    double x, y;
    int c;
} xyc;

typedef struct
{
    double x, y;
} xy;

typedef struct
{
    double d;
    int c;
} dc;

int lineaVacia(const char *s)
{
    while (*s != '\0')
    {
        if (!isspace((unsigned char)*s))
        {
            return 0;
        }
        s++;
    }
    return 1;
}

void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

double calculoKNN(xy *arrPuntos, xyc *arrClase0, xyc *arrClase1, int linasConDatos, int cantidadPuntos, dc **arrPorPuntos, int cantidadDatosEnClase, xyc *knn)
{
    int mitad = linasConDatos / 2;

    int a = 0;
    double distancia;
    for (int i = 0; i < mitad; i++) // CLASE 0
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase0[i].x - arrPuntos[j].x;
            double y = arrClase0[i].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = arrClase0[i].c;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase0[i].x, arrClase0[i].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    int b = -1;
    for (int i = mitad; i < linasConDatos; i++) // CLASE 1
    {
        b++;
        for (int j = 0; j < cantidadPuntos; j++)
        {
            double x = arrClase1[b].x - arrPuntos[j].x;
            double y = arrClase1[b].y - arrPuntos[j].y;

            distancia = hypot(x, y);

            arrPorPuntos[j][i].d = distancia;
            arrPorPuntos[j][i].c = 1;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase1[b].x, arrClase1[b].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntos[j][i].d, arrPorPuntos[j][i].c);
        }
        a++;
    }

    printf("\n");

    // Ordenar cada fila de arrPorPuntos por distancia (d), de menor a mayor
    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < linasConDatos - 1; j++)
        {
            for (int k = 0; k < linasConDatos - j - 1; k++)
            {
                if (arrPorPuntos[i][k].d > arrPorPuntos[i][k + 1].d)
                {
                    // Intercambiar toda la estructura dc, no solo el valor de d
                    dc temp = arrPorPuntos[i][k];
                    arrPorPuntos[i][k] = arrPorPuntos[i][k + 1];
                    arrPorPuntos[i][k + 1] = temp;
                }
            }
        }
    }

    int k = 1; // número de vecinos
    for (int i = 0; i < cantidadPuntos; i++)
    {
        // printf("\nPunto %d - %d vecinos más cercanos:\n", i, k);
        for (int j = 0; j < k; j++)
        {
            // printf("\tPunto: (%lf, %lf) Clase: %d\n", arrPuntos[i].x, arrPuntos[i].y, arrPorPuntos[i][j].c);
            knn[i].x = arrPuntos[i].x;
            knn[i].y = arrPuntos[i].y;
            knn[i].c = arrPorPuntos[i][j].c;
        }
    }
    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("\tPunto: (%lf, %lf) Clase: %d\n", knn[i].x, knn[i].y, knn[i].c);
    }
}

double calcularPromedioX(xyc *arr, int cantidadDatosEnClase)
{
    double suma = 0;
    double promedio;

    for (int i = 0; i < cantidadDatosEnClase; i++)
    {
        suma += arr[i].x;
    }

    promedio = (double)suma / cantidadDatosEnClase;
    return promedio;
}

double calcularPromedioY(xyc *arr, int cantidadDatosEnClase)
{
    double suma = 0;
    double promedio;

    for (int i = 0; i < cantidadDatosEnClase; i++)
    {
        suma += arr[i].y;
    }

    promedio = (double)suma / cantidadDatosEnClase;
    return promedio;
}

double calcularsumaXX(xyc *arr, int n)
{
    double suma = 0;
    // double sumaX = 0;
    // double sumaY = 0;

    for (int i = 0; i < n; i++)
    {
        suma += arr[i].x * arr[i].x;
    }

    return suma;
}

double calcularsumaXY(xyc *arr, int n)
{
    double suma = 0;
    // double sumaX = 0;
    // double sumaY = 0;

    for (int i = 0; i < n; i++)
    {
        suma += arr[i].x * arr[i].y;
    }

    return suma;
}

double pendiente(double promedioX, double promedioY, double sumaX2, double sumaXY, int cantidadDatosEnClase)
{
    double numerador;
    double denominador;
    double m;

    numerador = sumaXY - (cantidadDatosEnClase * promedioX * promedioY);
    denominador = sumaX2 - (cantidadDatosEnClase * (promedioX * promedioX));
    m = numerador / denominador;

    return m;
}

double ordenadaAlOrigen(double promedioX, double promedioY, double pendiente)
{
    double b = promedioY - (pendiente * promedioX);
    return b;
}

double puntoRectaDistancia(double pendiente0, double ordenada0, double pendiente1, double ordenada1, xy *arrPuntos, xyc *arrClase0, xyc *arrClase1, dc **arrPorPuntosMin, xyc *puntoRecta, int cantidadPuntos, int linasConDatos)
{
    double numerador;
    double denominador;
    double distancia;
    double B = -1;

    printf("\n");

    int mitad = linasConDatos / 2;

    int a = 0;
    for (int i = 0; i < mitad; i++) // CLASE 0
    {
        for (int j = 0; j < cantidadPuntos; j++)
        {
            numerador = fabs(pendiente0 * arrPuntos[j].x + B * arrPuntos[j].y + ordenada0);
            denominador = sqrt(pendiente0 * pendiente0 + B * B);
            if (denominador == 0.0)
            {
                return -1.0;
            }
            distancia = numerador / denominador;

            arrPorPuntosMin[j][i].d = distancia;
            arrPorPuntosMin[j][i].c = arrClase0[i].c;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase0[i].x, arrClase0[i].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntosMin[j][i].d, arrPorPuntosMin[j][i].c);
        }
        a++;
    }

    int b = -1;
    for (int i = mitad; i < linasConDatos; i++) // CLASE 1
    {
        b++;
        for (int j = 0; j < cantidadPuntos; j++)
        {
            numerador = fabs(pendiente1 * arrPuntos[j].x + B * arrPuntos[j].y + ordenada1);
            denominador = sqrt(pendiente1 * pendiente1 + B * B);
            if (denominador == 0.0)
            {
                return -1.0;
            }
            distancia = numerador / denominador;

            arrPorPuntosMin[j][i].d = distancia;
            arrPorPuntosMin[j][i].c = arrClase1[b].c;

            // printf("(%lf, %lf) y (%lf, %lf): (%lf, %d)\n", arrClase1[b].x, arrClase1[b].y, arrPuntos[j].x, arrPuntos[j].y, arrPorPuntosMin[j][i].d, arrPorPuntosMin[j][i].c);
        }
        a++;
    }

    printf("\n");

    // Ordenar por distancia para cada punto
    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < linasConDatos - 1; j++)
        {
            for (int k = 0; k < linasConDatos - j - 1; k++)
            {
                if (arrPorPuntosMin[i][k].d > arrPorPuntosMin[i][k + 1].d)
                {
                    dc temp = arrPorPuntosMin[i][k];
                    arrPorPuntosMin[i][k] = arrPorPuntosMin[i][k + 1];
                    arrPorPuntosMin[i][k + 1] = temp;
                }
            }
        }
    }

    int k = 1; // número de vecinos
    for (int i = 0; i < cantidadPuntos; i++)
    {
        for (int j = 0; j < k; j++)
        {
            puntoRecta[i].x = arrPuntos[i].x;
            puntoRecta[i].y = arrPuntos[i].y;
            puntoRecta[i].c = arrPorPuntosMin[i][j].c;
        }
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        printf("\tPunto: (%lf, %lf) Clase: %d\n", puntoRecta[i].x, puntoRecta[i].y, puntoRecta[i].c);
    }

    return 0.0;
}

double puntoRectaDistancia2(double A, double C, xy *puntoCalcularDistancia)
{
    double numerador;
    double denominador;
    double distancia;
    double B = -1;

    printf("\n");

    numerador = fabs(A * puntoCalcularDistancia[0].x + B * puntoCalcularDistancia[0].y + C);
    denominador = sqrt(A * A + B * B);
    if (denominador == 0.0)
    {
        return -1.0;
    }
    distancia = numerador / denominador;

    return distancia;
}

int compararArreglos(xyc *arrComparacion, xyc *arr1, int cantidadPuntos)
{
    int correctas = 0;
    for (int i = 0; i < cantidadPuntos; i++)
    {
        if (arrComparacion[i].c == arr1[i].c)
        {
            correctas++;
        }
    }
    return correctas;
}

int accuracy(int correctas, int cantidadPuntos)
{
    float a = (float)correctas / cantidadPuntos;
    float porcentaje = a * 100;
    return porcentaje;
}

int main()
{
    FILE *archivoDatosClases;
    archivoDatosClases = fopen("datos_clases.txt", "r");

    if (!archivoDatosClases) // Mensaje de error al abirir el archivo
    {
        printf("El archivo 'datos_clases.txt' no existe o la dirección es incorrecta");
        return 1;
    }

    int cantidadDatosEnClase;
    for (int i = 0; i < 1; i++) // Cantidad de datos en cada clase
    {
        fscanf(archivoDatosClases, "%d", &cantidadDatosEnClase);
        printf("\n\tEvalucaicón de %d coordenadas por cada clase (0 y 1)\n\n", cantidadDatosEnClase);
    }

    char linea[256];
    int linasConDatos = 0;
    while (fgets(linea, 256, archivoDatosClases) != NULL) // Cantidad total de datos
    {
        if (!lineaVacia(linea))
        {
            linasConDatos++;
        }
    }
    rewind(archivoDatosClases);

    xyc arrClase0[cantidadDatosEnClase];
    xyc arrClase1[cantidadDatosEnClase];

    char enteroAcadena[256];
    sprintf(enteroAcadena, "%d", cantidadDatosEnClase);
    size_t posicionC1 = strlen(enteroAcadena); // Calcular el fin de la primera línea

    // printf("\n\tClase 0:\n\n");
    fseek(archivoDatosClases, posicionC1, SEEK_SET);    // Dirigir puntero al inicio de la clase 0
    for (int i = 0; i <= cantidadDatosEnClase - 1; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoDatosClases, "%lf %lf %d", &arrClase0[i].x, &arrClase0[i].y, &arrClase0[i].c)) == 3)
        {
            // printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase0[i].x, arrClase0[i].y, arrClase0[i].c);
        }
    }

    long posicionC2 = ftell(archivoDatosClases); // Calcular el fin de la primera clase
    // printf("\n\n\tClase 1:\n\n");

    fseek(archivoDatosClases, posicionC2, SEEK_SET);    // Dirigir puntero al inicio de la clase 1
    for (int i = 0; i <= cantidadDatosEnClase - 1; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoDatosClases, "%lf %lf %d", &arrClase1[i].x, &arrClase1[i].y, &arrClase1[i].c)) == 3)
        {
            // printf("\t%d \t(%f, %f) \tClase: %d \n", i, arrClase1[i].x, arrClase1[i].y, arrClase1[i].c);
        }
    }

    fclose(archivoDatosClases);
    //------------------------------------------------------------------------------------------

    double promedioX0 = calcularPromedioX(arrClase0, cantidadDatosEnClase);
    double promedioY0 = calcularPromedioY(arrClase0, cantidadDatosEnClase);
    double sumaX20 = calcularsumaXX(arrClase0, cantidadDatosEnClase);
    double sumaXY0 = calcularsumaXY(arrClase0, cantidadDatosEnClase);
    double pendiente0 = pendiente(promedioX0, promedioY0, sumaX20, sumaXY0, cantidadDatosEnClase);
    double ordenada0 = ordenadaAlOrigen(promedioX0, promedioY0, pendiente0);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    double promedioX1 = calcularPromedioX(arrClase1, cantidadDatosEnClase);
    double promedioY1 = calcularPromedioY(arrClase1, cantidadDatosEnClase);
    double sumaX21 = calcularsumaXX(arrClase1, cantidadDatosEnClase);
    double sumaXY1 = calcularsumaXY(arrClase1, cantidadDatosEnClase);
    double pendiente1 = pendiente(promedioX1, promedioY1, sumaX21, sumaXY1, cantidadDatosEnClase);
    double ordenada1 = ordenadaAlOrigen(promedioX1, promedioY1, pendiente1);

    //------------------------------------------------------------------------------------------------------------

    FILE *archivoPuntos;
    archivoPuntos = fopen("puntos.txt", "r");

    if (!archivoPuntos) // Mensaje de error al abirir el archivo
    {
        printf("El archivo 'puntos.txt' no existe o la dirección es incorrecta");
        return 1;
    }

    char coordenadas[256];
    int cantidadPuntos = 0;
    while (fgets(coordenadas, 256, archivoPuntos) != NULL) // Cantidad total de datos
    {
        if (!lineaVacia(coordenadas))
        {
            cantidadPuntos++;
        }
    }

    xy arrPuntos[cantidadPuntos];

    rewind(archivoPuntos);

    for (int i = 0; i <= cantidadPuntos; i++) // Guadar los datos en el arr
    {
        if ((fscanf(archivoPuntos, "%lf %lf", &arrPuntos[i].x, &arrPuntos[i].y)) == 2)
        {
            // printf("\t%d \t(%f, %f) \n", i, arrPuntos[i].x, arrPuntos[i].y);
        }
    }
    //-------------------------------------------------------------------------------------
    dc **arrPorPuntos;
    arrPorPuntos = (dc **)malloc(cantidadPuntos * sizeof(dc *));

    if (arrPorPuntos == NULL)
    {
        printf("No se eligió un número de puntos a evaluar");
        return 1;
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        arrPorPuntos[i] = (dc *)malloc(linasConDatos * sizeof(dc));
        if (arrPorPuntos[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arrPorPuntos[j]);
            }
            free(arrPorPuntos);
            return 1;
        }
    }

    //------------------------------------------------------------------------------------

    dc **arrPorPuntosMin;
    arrPorPuntosMin = (dc **)malloc(cantidadPuntos * sizeof(dc *));

    if (arrPorPuntosMin == NULL)
    {
        printf("No se eligió un número de puntos a evaluar");
        return 1;
    }

    for (int i = 0; i < cantidadPuntos; i++)
    {
        arrPorPuntosMin[i] = (dc *)malloc(linasConDatos * sizeof(dc));
        if (arrPorPuntosMin[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arrPorPuntosMin[j]);
            }
            free(arrPorPuntosMin);
            return 1;
        }
    }
    //-------------------------------------------------------------------------------------
    xyc knn[cantidadPuntos];
    xyc puntoRecta[cantidadPuntos];
    //----------------------------------------------------------------------------------------------------------------
    printf("\tKNN, clasificación de punto por clase:\n");
    calculoKNN(arrPuntos, arrClase0, arrClase1, linasConDatos, cantidadPuntos, arrPorPuntos, cantidadDatosEnClase, knn);
    printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("\n\tDistancia de un punto a la línea de ajuste, clasificación de punto por clase:\n");
    printf("\n\tLínea de ajuste de la clase 0: y=%lf x %lf\n", pendiente0, ordenada0);
    printf("\n\tLínea de ajuste de la clase 1: y=%lf x +%lf\n", pendiente1, ordenada1);
    puntoRectaDistancia(pendiente0, ordenada0, pendiente1, ordenada1, arrPuntos, arrClase0, arrClase1, arrPorPuntosMin, puntoRecta, cantidadPuntos, linasConDatos);
    //----------------------------------------------------------------------------------------------------------------

    FILE *archivoComparacionClases;
    archivoComparacionClases = fopen("puntos_ambiguos_prueba.txt", "r");

    if (!archivoComparacionClases) // Mensaje de error al abirir el archivo
    {
        printf("El archivo 'puntos_ambiguos_prueba.txt' no existe o la dirección es incorrecta");
        return 1;
    }

    xyc arrComparacion[cantidadPuntos];

    for (int i = 0; i <= cantidadPuntos - 1; i++) // Guadar los datos en el arr
    {
        (fscanf(archivoComparacionClases, "%lf %lf %d", &arrComparacion[i].x, &arrComparacion[i].y, &arrComparacion[i].c));
    }

    fclose(archivoComparacionClases);
    //----------------------------------------------------------------------------------------------------------------

    int knnCorrectas = compararArreglos(arrComparacion, knn, cantidadPuntos);
    int puntoRectaCorrectas = compararArreglos(arrComparacion, puntoRecta, cantidadPuntos);

    float accuracyKNN = accuracy(knnCorrectas, cantidadPuntos);
    float accuracyPuntoRecta = accuracy(puntoRectaCorrectas, cantidadPuntos);

    printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("\n\tEl método KNN tiene %.2f %% de Accuracy\n", accuracyKNN);
    printf("\n\tEl método de Distancia de un punto a la línea de ajuste tiene %.2f %% de Accuracy\n", accuracyPuntoRecta);

    //----------------------------------------------------------------------------------------------------------------
    xy dosPuntos[2];
    xy puntoCalcularDistancia[1];

    // limpiar_buffer();
    char buffer[256];
    char *endptr;
    printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("\n\tIngrese los dos puntos de la recta que desea obtener:\n");
    for (int i = 0; i < 2; i++)
    {
        printf("\tPunto: %d\n\tIngrese un numero para x: ", i);

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        dosPuntos[i].x = strtod(buffer, &endptr);

        if ((errno == ERANGE && (dosPuntos[i].x == 0 || dosPuntos[i].x == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }
        //------------------------------------------
        printf("\tIngrese un numero para y: ");

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        dosPuntos[i].y = strtod(buffer, &endptr);

        if ((errno == ERANGE && (dosPuntos[i].y == 0 || dosPuntos[i].y == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }
        printf("\t(%lf, %lf)\n\n", dosPuntos[i].x, dosPuntos[i].y);
    }

    double A = (dosPuntos[1].y - dosPuntos[0].y) / (dosPuntos[1].x - dosPuntos[0].x); // pendiente
    double C = dosPuntos[0].y - A * dosPuntos[0].x;                                   // ordenada al origen

    printf("\n\tEcuación de la recta: y = %.2lfx + %.2lf\n", A, C);

    printf("\n\n\tIngrese el punto del cual desea conocer su distancia a la recta encontrada\n");
    for (int i = 0; i < 1; i++)
    {
        printf("\tPunto: %d\n\tIngrese un numero para x: ", i);

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        puntoCalcularDistancia[i].x = strtod(buffer, &endptr);

        if ((errno == ERANGE && (puntoCalcularDistancia[i].x == 0 || puntoCalcularDistancia[i].x == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }

        //------------------------------------------
        printf("\tIngrese un numero para y: ");

        if (fgets(buffer, 256, stdin) == NULL)
        {
            printf("Error de lectura.\n");
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        errno = 0;
        puntoCalcularDistancia[i].y = strtod(buffer, &endptr);

        if ((errno == ERANGE && (puntoCalcularDistancia[i].y == 0 || puntoCalcularDistancia[i].y == 256)))
        {
            printf("Error: Numero fuera del rango de long.\n");
        }
        else if (endptr == buffer)
        {
            printf("Error: No se encontro ningun numero valido al inicio de la cadena.\n");
        }

        printf("\t(%lf, %lf)\n", puntoCalcularDistancia[i].x, puntoCalcularDistancia[i].y);
    }

    double distanciaPuntoARecta = puntoRectaDistancia2(A, C, puntoCalcularDistancia);
    printf("\n\tLa Distancia del punto (%lf, %lf) a la recta y = %.2lfx + %.2lf es: %lf\n\n", puntoCalcularDistancia[0].x, puntoCalcularDistancia[0].y, A, C, distanciaPuntoARecta);
    //---------------------------------------------------------------------------------
    free(arrPorPuntos);
    free(arrPorPuntosMin);
}
