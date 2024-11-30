// Libreria base para flujos de entrada y salida
#include <iostream>
// Libreria para el manejo de mermoria, busqueda, ordenamiento, etc. 
#include <cstdlib>
// Libreria para el manejo de cadenas de texto
#include <cstring>
// Libreria para el manejo de arreglos dinamicos (lista ligada)
#include <vector>
// Libreria para el manejo de flujos de caracteres
#include <sstream>
// Libreria para el manejo de carpetas y ficheros 
#include <filesystem>
// Libreria para leer y escribir archivos de texto
#include <fstream>

////////////////////////////////////////////////////////////////////////////
// LIBRERIAS PARA OPEN CV

//Funciones base (Representacion de matrices, operaciones, etc)
#include <opencv2/core/core.hpp>
//Funciones de interfaz grafica
#include <opencv2/highgui/highgui.hpp>
//Funciones para manipular imagenes en distintos formatos
#include <opencv2/imgcodecs/imgcodecs.hpp>
//Operaciones para el procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>
//Manejo de video
#include <opencv2/video/video.hpp>
//Lectura y escritura de video
#include <opencv2/videoio/videoio.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){

    // Manejo de vídeo en OpenCV
    Mat frame;
    Mat frameMedida;

    VideoCapture video("/dev/video0");
                           //VideoCapture video("/home/video.mp4");
    if(video.isOpened()){
        

        Mat GRIS;
        Mat background;
        Mat adaptive_thresh;
        Mat denoisedImage;
        Mat framefinal;
        

        video >> frameMedida;

        int ancho = frameMedida.cols;
        int alto = frameMedida.rows;

        // Crear una imagen de fondo con un degradado
        int width = ancho, height = alto;
        background = Mat(height, width, CV_8UC3);

        // Rellenar la imagen de fondo con un degradado en el espectro visible
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                float normX = static_cast<float>(x) / width;
                float normY = static_cast<float>(y) / height;

                float angle = normX * 2 * M_PI;
                float radius = normY;

                uchar red = static_cast<uchar>(127.5 * (1 + sin(angle + 0))); 
                uchar green = static_cast<uchar>(127.5 * (1 + sin(angle + 2 * M_PI / 3))); 
                uchar blue = static_cast<uchar>(127.5 * (1 + sin(angle + 4 * M_PI / 3))); 

                background.at<Vec3b>(y, x)[0] = blue;      
                background.at<Vec3b>(y, x)[1] = green;     
                background.at<Vec3b>(y, x)[2] = red;      
            }
        }

        while(3==3){
            video >> frame;

            // Giro de ventana y frame
            flip(frame, frame, 1);

            // Cambio a Gris
            cvtColor(frame, GRIS, COLOR_BGR2GRAY);

            // Umbral adaptativo
            adaptiveThreshold(GRIS, adaptive_thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 2);
            
            // Desenfoque con medianBlur
            medianBlur(adaptive_thresh, denoisedImage, 3);  // 3 es el tamaño del kernel (debe ser impar)
            
            // Aplicar el fondo a los bordes
            framefinal = Mat::zeros(background.size(), background.type());

            // Asegúrate de que la máscara sea binaria
            threshold(denoisedImage, denoisedImage, 1, 255, THRESH_BINARY);

            // Copiar usando la máscara
            background.copyTo(framefinal, denoisedImage);

            imshow("Final", framefinal);

            // Esperar hasta que el usuario presione la tecla 'ESC'
            if(waitKey(23) == 27)
                break;
        }

        video.release();
        destroyAllWindows();
    }

    return 0;
}


//Conversion de bitmap problema