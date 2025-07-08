#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <set>
#include <iostream>
#include "photomosaicGenerator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMessageBox::information(nullptr, "PhotoMosaicGenerator", "To Start generating a photo mosaic, please select the photo you want to convert");

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Photo", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (fileName.isEmpty()) {
        QMessageBox::warning(nullptr, "No Photo Selected", "You must select a photo to generate a mosaic.");
        return -1;
    }

    QMessageBox::information(nullptr, "PhotoMosaicGenerator", "Now, select all the images you want to use as tiles for the mosaic");
    QStringList tileFiles = QFileDialog::getOpenFileNames(nullptr, "Select Tile Images", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (tileFiles.isEmpty()) {
        QMessageBox::warning(nullptr, "No Tiles Selected", "You must select at least one tile image to generate a mosaic.");
        return -1;
    }

    set<string> stringSet;
    for(const QString &tileFile : tileFiles) {
        stringSet.insert(tileFile.toStdString());
    }

    int width = QInputDialog::getInt(
        nullptr,
        "Tile Width",
        "Enter the width of each tile (default is 32):",
        32,
        1, 
        1000,
        1
    );

    int height = QInputDialog::getInt(
        nullptr,
        "Tile Height",
        "Enter the height of each tile (default is 18):",
        18,
        1, 
        1000,
        1
    );

    cv::Mat result = generate(fileName.toStdString(), stringSet, width, height);
    if (result.empty()) {
        QMessageBox::critical(nullptr, "Error", "An error occurred while generating the mosaic.");
        return -1;
    }
    cv::imshow("Photo Mosaic", result);
    cv::waitKey(0);
    cv::destroyAllWindows();

    QMessageBox::StandardButton saveMosaic = QMessageBox::question(
        nullptr,
        "Save Mosaic",
        "Do you want to save the generated mosaic image?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes
    );
    if(saveMosaic == QMessageBox::No) {
        QMessageBox::information(nullptr, "Success", "The mosaic image was generated successfully but not saved.");
        cv::destroyAllWindows();
        return 0; 
    }
    QString saveFileName = QFileDialog::getSaveFileName(
        nullptr,
        "Guardar imagen",
        "",
        "Imágenes (*.png *.jpg *.jpeg *.bmp)"
    );
    if (saveFileName.isEmpty()) {
        QMessageBox::warning(nullptr, "No Save Location", "You must select a location to save the generated mosaic.");
        return -1;
    }

    bool ok = cv::imwrite(saveFileName.toStdString(), result);
    if(!ok){
        QMessageBox::critical(nullptr, "Error", "Failed to save the mosaic image. Please check the file path and permissions.");
        return -1;
    } else {
        QMessageBox::information(nullptr, "Success", "Mosaic image saved successfully!");
    }


    cv::destroyAllWindows();
    
    return 0;
}