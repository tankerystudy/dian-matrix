######################################################################
# Edit by Tankery, Fri Oct 6 2010
# This is the project file for Dian Matrix library.
######################################################################

# basic configuration
PROJECT      = dian_matrix_lib
TEMPLATE     = lib
DEPENDPATH  += ../../dian_matrix_lib \
                ../../qserialdevice-qserialdevice/src
INCLUDEPATH += ../../dian_matrix_lib \
                ../../qserialdevice-qserialdevice/src/qserialdevice
LIBPATH     += ../qserialdevice/lib

CONFIG      += qt warn_on staticlib
CONFIG(debug, debug|release) {
    TARGET   = dian_matrix_libd
    LIBS    += -lqserialdeviced
} else {
    TARGET   = dian_matrix_lib
    LIBS    += -lqserialdevice
}

# Preprocess definitions
unix {
    DEFINES += _UNIX
} else {
    DEFINES += _WINDOWS
}

# Input
HEADERS     += DataManager.h LEDScrn.h matrixCreator.h \
                serialDownloader.h matrixDlg.h QtDianMatrix.h
FORMS       += MatrixDlg.ui
SOURCES     += DataManager.cpp LEDScrn.cpp matrixCreator.cpp \
                serialDownloader.cpp matrixDlg.cpp QtDianMatrix.cpp


# Output paths
DESTDIR      = ./lib
OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
UI_DIR       = ./ui

